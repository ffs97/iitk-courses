from __future__ import print_function
from __future__ import division

import os
import sys
import copy
import torch
import pickle
import hashlib
import argparse
import numpy as np
import torchvision
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F

from tqdm import tqdm
from torchvision import datasets, models, transforms


# Parser

parser = argparse.ArgumentParser(
    description="Training file for DMVAE and DVMOE"
)

parser.add_argument("--model_name", type=str, default="default",
                    help="Name of the model")

parser.add_argument("--data_dir", type=str, default="./data",
                    help="Path for the data directory")

parser.add_argument("--num_fine_classes", type=int, default=36,
                    help="Number of classes for fine classification")
parser.add_argument("--num_coarse_classes", type=int, default=5,
                    help="Number of classes for coarse classification")

parser.add_argument("--batch_size", type=int, default=10,
                    help="Batch Size for training")

parser.add_argument("--feature_extract", action="store_false", default=True,
                    help="Whether to use fix pretrained resnet or not")
parser.add_argument("--one_hot_tune", action="store_true", default=False,
                    help="Whether to train using one-hot or not")
parser.add_argument("--tune_full", action="store_true", default=False,
                    help="Whether to finetune feature extractor (resnet) or not")

parser.add_argument("--use_big", action="store_true", default=False,
                    help="Whether to use larger classifiers or not")
parser.add_argument("--use_bcnn", action="store_true", default=False,
                    help="Whether to use bilinear extractor or not")


parser.add_argument("--fix_training", action="store_true", default=False,
                    help="Whether to fix model or not")

parser.add_argument("--load_trained", action="store_true", default=False,
                    help="Whether to load trained model or not")
parser.add_argument("--save_trained", action="store_true", default=False,
                    help="Whether to save trained model or not")

parser.add_argument("--gpu_device", type=int, default=0,
                    help="GPU Device to use")

args = parser.parse_args()


# Parameters

data_dir = args.data_dir

num_fine_classes = args.num_fine_classes
num_coarse_classes = args.num_coarse_classes

batch_size = args.batch_size

feature_extract = args.feature_extract
one_hot_tune = args.one_hot_tune
tune_full = args.tune_full

use_big = args.use_big
use_bcnn = args.use_bcnn

training = not args.fix_training
load_trained = args.load_trained
save_trained = args.save_trained

gpu_device = args.gpu_device

model_name = args.model_name

if use_big:
    coarse_fc_layer_sizes = [2048]
    fine_fc_layer_sizes = [2048, 1024, 512]
else:
    coarse_fc_layer_sizes = []
    fine_fc_layer_sizes = [250]

if use_bcnn:
    num_epochs = {
        "coarse": 50,
        "fine": 0,
        "tune": 200
    }
else:
    num_epochs = {
        "coarse": 25,
        "fine": 100,
        "tune": 150
    }


# Helper Functions


def train_model(coarse_model, fine_model, dataloaders, criterion, optimizer, num_epochs=25, training="coarse"):
    num_epochs = num_epochs[training]

    if num_epochs == 0:
        return

    fine_model.train()
    coarse_model.train()

    epoch_len = len(dataloaders["train"].dataset)

    best_model_fine = copy.deepcopy(fine_model.state_dict())
    best_model_coarse = copy.deepcopy(coarse_model.state_dict())

    best_acc = 0.0

    with tqdm(range(num_epochs)) as bar:
        for _ in bar:
            postfix = {}

            running_loss = 0.0

            running_corrects_fine = 0
            running_corrects_coarse = 0

            for (inputs, fine_labels), _ in dataloaders["train"]:
                inputs = inputs.to(device)

                coarse_labels, coarse_labels_onehot = get_coarse_labels(
                    fine_labels.numpy()
                )

                fine_labels = fine_labels.to(device)
                coarse_labels = coarse_labels.to(device)
                coarse_labels_onehot = coarse_labels_onehot.to(device)

                optimizer.zero_grad()

                coarse_outputs = coarse_model(inputs)

                fine_outputs = None
                if training == "fine":
                    fine_outputs = fine_model(
                        inputs, coarse_labels_onehot
                    )
                elif training == "tune":
                    if one_hot_tune:
                        pred_onehot = get_onehot(
                            torch.argmax(coarse_outputs, dim=1)
                        )
                        fine_outputs = fine_model(
                            inputs, pred_onehot, softmax=False
                        )
                    else:
                        fine_outputs = fine_model(
                            inputs, coarse_outputs, softmax=True
                        )

                if training == "tune":
                    loss = criterion(coarse_outputs, coarse_labels) + \
                        criterion(fine_outputs, fine_labels)
                elif training == "fine":
                    loss = criterion(fine_outputs, fine_labels)
                elif training == "coarse":
                    loss = criterion(coarse_outputs, coarse_labels)

                loss.backward()
                optimizer.step()

                running_loss += loss.item() * inputs.size(0)

                if training != "fine":
                    _, coarse_preds = torch.max(coarse_outputs, 1)

                    running_corrects_coarse += torch.sum(
                        coarse_preds == coarse_labels.data
                    )

                if training != "coarse":
                    _, fine_preds = torch.max(fine_outputs, 1)

                    running_corrects_fine += torch.sum(
                        fine_preds == fine_labels.data
                    )

            postfix["loss"] = "%.4f" % (running_loss / epoch_len)

            if training != "fine":
                epoch_acc = running_corrects_coarse.double() / epoch_len
                postfix["coarse_acc"] = "%.4f" % epoch_acc

            if training != "coarse":
                epoch_acc = running_corrects_fine.double() / epoch_len
                postfix["fine_acc"] = "%.4f" % epoch_acc

            # test_acc_coarse, _, test_acc_fine = evaluate(dataloaders["test"])

            # postfix["test_fine_acc"] = test_acc_fine
            # postfix["test_coarse_acc"] = test_acc_coarse

            bar.set_postfix(postfix)

            if epoch_acc > best_acc:
                best_acc = epoch_acc

                best_model_fine = copy.deepcopy(fine_model.state_dict())
                best_model_coarse = copy.deepcopy(coarse_model.state_dict())

    print()
    print("Best Acc: %.4f" % best_acc)
    print()

    fine_model.load_state_dict(best_model_fine)
    coarse_model.load_state_dict(best_model_coarse)

    return coarse_model, fine_model


def evaluate(data, saving_predictions=False):
    fine_model.eval()
    coarse_model.eval()

    running_corrects_fine = 0
    running_corrects_coarse = 0
    running_corrects_fcoarse = 0

    epoch_len = len(data.dataset)

    lines = []
    for (inputs, fine_labels), (paths, _) in data:
        inputs = inputs.to(device)

        coarse_labels, coarse_labels_onehot = get_coarse_labels(
            fine_labels.numpy()
        )

        fine_labels = fine_labels.to(device)
        coarse_labels = coarse_labels.to(device)

        coarse_outputs = coarse_model(inputs)
        # fine_outputs = fine_model(inputs, coarse_outputs, softmax=True)
        fine_outputs = fine_model(inputs, coarse_labels_onehot.to(device))

        fine_preds = torch.argmax(fine_outputs, 1)
        coarse_preds = torch.argmax(coarse_outputs, 1)
        fcoarse_preds, _ = get_coarse_labels(fine_preds.cpu().numpy())

        running_corrects_fine += torch.sum(
            fine_preds == fine_labels.data
        )
        running_corrects_coarse += torch.sum(
            coarse_preds == coarse_labels.data
        )
        running_corrects_fcoarse += torch.sum(
            fcoarse_preds.to(device) == coarse_labels.data
        )

        fine_preds = fine_preds.cpu().numpy()
        coarse_preds = coarse_preds.cpu().numpy()

        for i in range(len(fine_preds)):
            lines.append(" ".join([
                paths[i].split("/")[-1],
                coarse_class_mappings[coarse_preds[i]],
                fine_class_mappings[fine_preds[i]]
            ]) + "\n")

        if not os.path.exists("predictions"):
            os.makedirs("predictions")

        with open("predictions/%s.txt" % model_name, "w") as f:
            f.writelines(lines)

    fine_acc = running_corrects_fine.double().cpu().numpy() / epoch_len
    coarse_acc = running_corrects_coarse.double().cpu().numpy() / epoch_len
    fcoarse_acc = running_corrects_fcoarse.double().cpu().numpy() / epoch_len

    return coarse_acc, fcoarse_acc, fine_acc


def set_parameter_requires_grad(model, feature_extract):
    if feature_extract:
        for param in model.parameters():
            param.requires_grad = False


def initialize_model(use_pretrained=True):
    if use_bcnn:
        class FeaturesModel(nn.Module):
            def __init__(self):
                nn.Module.__init__(self)

                self.network = models.resnet18(pretrained=True)
                self.network = nn.Sequential(
                    *list(self.network.children())[:-1]
                )

                self.relu = nn.ReLU(inplace=False)

                self.features_len = 512
                self.input_size = 448

                self.fc = nn.Linear(
                    512 * 512, self.features_len
                )

            def forward(self, inputs):
                N = inputs.size()[0]
                outputs = self.network(inputs)

                outputs = self.relu(outputs)

                outputs = torch.reshape(outputs, (N, 512, 8 * 8))
                outputs = torch.bmm(
                    outputs, torch.transpose(outputs, 1, 2)
                ) / (8 * 8)
                outputs = torch.reshape(outputs, (N, 512 * 512))

                # X = torch.sign(X) * torch.sqrt(torch.abs(X) + 1e-5)
                outputs = torch.sqrt(outputs + 1e-5)
                outputs = F.normalize(outputs)

                outputs = self.fc(outputs)
                outputs = F.relu(outputs)

                return outputs

    else:
        class FeaturesModel(nn.Module):
            def __init__(self):
                nn.Module.__init__(self)

                self.network = models.resnet34(pretrained=use_pretrained)
                self.features_len = self.network.fc.in_features

                self.network.fc = nn.ReLU()

                set_parameter_requires_grad(self.network, feature_extract)

                self.input_size = 224

            def forward(self, inputs):
                outputs = self.network(inputs)

                return outputs

    class CoarseModel(nn.Module):
        def __init__(self, features_model):
            nn.Module.__init__(self)

            self.features_model = features_model

            self.fc1 = nn.Linear(
                features_model.features_len, num_coarse_classes
            )

            coarse_fc_layer_sizes_ = [features_model.features_len]
            coarse_fc_layer_sizes_ += coarse_fc_layer_sizes

            network = list()
            for i in range(len(coarse_fc_layer_sizes_) - 1):
                network.append(
                    nn.Linear(coarse_fc_layer_sizes_[
                              i], coarse_fc_layer_sizes_[i + 1])
                )
                network.append(nn.ReLU())

            network.append(
                nn.Linear(coarse_fc_layer_sizes_[-1], num_coarse_classes)
            )

            self.network_coarse = nn.Sequential(*network)

        def forward(self, inputs):
            outputs = self.features_model(inputs)
            outputs = self.network_coarse(outputs)

            return outputs

    class FineModel(nn.Module):
        def __init__(self, features_model):
            nn.Module.__init__(self)

            self.features_model = features_model

            fine_fc_layer_sizes_ = [
                features_model.features_len + num_coarse_classes
            ]
            fine_fc_layer_sizes_ += fine_fc_layer_sizes

            network = list()
            for i in range(len(fine_fc_layer_sizes_) - 1):
                network.append(
                    nn.Linear(fine_fc_layer_sizes_[
                              i], fine_fc_layer_sizes_[i + 1])
                )
                network.append(nn.ReLU())

            network.append(
                nn.Linear(fine_fc_layer_sizes_[-1], num_fine_classes)
            )

            self.network = nn.Sequential(*network)

        def forward(self, inputs, coarse_outputs, softmax=False):
            if softmax:
                coarse_outputs = F.softmax(coarse_outputs, dim=1)

            outputs = self.features_model(inputs)
            outputs = torch.cat([outputs, coarse_outputs], dim=1)
            outputs = self.network(outputs)

            return outputs

    features_model = FeaturesModel()
    coarse_model = CoarseModel(features_model)
    fine_model = FineModel(features_model)

    return features_model, coarse_model, fine_model


def get_coarse_labels(fine_labels):
    splits = [7, 11, 8, 5, 5]

    coarse_labels = list()
    coarse_labels_onehot = list()
    for label in fine_labels:
        for i, v in enumerate(splits):
            if v > label:
                coarse_labels.append(i)

                coarse_labels_onehot.append([0] * num_coarse_classes)
                coarse_labels_onehot[-1][i] = 1

                break

            else:
                label -= v

    return torch.tensor(coarse_labels, dtype=torch.long), torch.tensor(coarse_labels_onehot, dtype=torch.float32)


def get_onehot(labels):
    labels = labels.cpu().numpy()
    ans = torch.from_numpy(np.eye(5)[labels]).to(device)
    return ans


def save_model():
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    torch.save(features_model.state_dict(), save_dir + "features_model.pt")
    torch.save(coarse_model.state_dict(), save_dir + "coarse_model.pt")
    torch.save(fine_model.state_dict(), save_dir + "fine_model.pt")

    print("Models Saved")
    print()


# Initiliazing and Loading Models
features_model, coarse_model, fine_model = initialize_model(
    use_pretrained=True
)
print("Model Initialized")
print()

save_dir = "saved-models/%s/" % model_name

if load_trained:
    no_models = True

    if os.path.isfile(save_dir + "features_model.pt"):
        try:
            features_model.load_state_dict(
                torch.load(save_dir + "features_model.pt")
            )

            print("Features Model Loaded")

            no_models = False
        except:
            pass

    if os.path.isfile(save_dir + "coarse_model.pt"):
        try:
            coarse_model.load_state_dict(
                torch.load(save_dir + "coarse_model.pt")
            )
            print("Coarse Classification Model Loaded")

            no_models = False
        except:
            pass

    if os.path.isfile(save_dir + "fine_model.pt"):
        try:
            fine_model.load_state_dict(
                torch.load(save_dir + "fine_model.pt")
            )
            print("Fine Classification Model Loaded")

            no_models = False
        except:
            pass

    if no_models:
        print("No models to load")

    print()


# Loading Data

data_transforms = {
    "train": transforms.Compose([
        transforms.RandomResizedCrop(features_model.input_size),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
    "test": transforms.Compose([
        transforms.Resize(features_model.input_size),
        transforms.CenterCrop(features_model.input_size),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
}


class ImageFolder(datasets.ImageFolder):
    def __getitem__(self, index):
        item = datasets.ImageFolder.__getitem__(self, index)
        return item, self.imgs[index]


image_datasets = {
    x: ImageFolder(
        os.path.join(data_dir, x), data_transforms[x]
    ) for x in ["train", "test"]
}

dataloaders_dict = {
    x: torch.utils.data.DataLoader(
        image_datasets[x], batch_size=batch_size,
        shuffle=(x == "train"), num_workers=4
    ) for x in ["train", "test"]
}

fine_classes = sorted(os.listdir(os.path.join(data_dir, "train")))
coarse_classes = sorted(list(set([x.split("@")[0] for x in fine_classes])))

fine_class_mappings = {
    i: x for i, x in enumerate(fine_classes)
}
coarse_class_mappings = {
    i: x for i, x in enumerate(coarse_classes)
}


# Create the Optimizer

device = torch.device("cuda:%d" % (gpu_device)
                      if torch.cuda.is_available() else "cpu")

fine_model = fine_model.to(device)
coarse_model = coarse_model.to(device)

params_to_update_ = list(set(
    list(coarse_model.named_parameters()) +
    list(fine_model.named_parameters())
))
params_to_update = []

print("Params to learn:")
if feature_extract:
    for name, param in params_to_update_:
        if param.requires_grad == True:
            params_to_update.append(param)
            print("\t", name)
else:
    for name, param in params_to_update_:
        if param.requires_grad == True:
            print("\t", name)
        params_to_update.append(param)
print()

optimizer_ft = optim.SGD(params_to_update, lr=0.001, momentum=0.9)

criterion = nn.CrossEntropyLoss()


# Training

if training:
    print("Training Models")

    models = train_model(
        coarse_model, fine_model, dataloaders_dict, criterion,
        optimizer_ft, num_epochs=num_epochs, training="coarse"
    )
    if models is not None:
        coarse_model, fine_model = models
        save_trained and save_model()

    models = train_model(
        coarse_model, fine_model, dataloaders_dict, criterion,
        optimizer_ft, num_epochs=num_epochs, training="fine"
    )
    if models is not None:
        coarse_model, fine_model = models
        save_trained and save_model()

    set_parameter_requires_grad(features_model.network, not tune_full)
    models = train_model(
        coarse_model, fine_model, dataloaders_dict, criterion,
        optimizer_ft, num_epochs=num_epochs, training="tune"
    )
    if models is not None:
        coarse_model, fine_model = models
        save_trained and save_model()

    print("Training Complete")
    print()


# Computing Hash

if os.path.exists(save_dir):
    hash_md5 = hashlib.md5()

    with open(save_dir + "features_model.pt", "rb") as f:
        hash_md5.update(f.read())

    with open(save_dir + "coarse_model.pt", "rb") as f:
        hash_md5.update(f.read())

    with open(save_dir + "fine_model.pt", "rb") as f:
        hash_md5.update(f.read())

    print(hash_md5.hexdigest())


# Evaluation on Train Data

coarse_acc, fcoarse_acc, fine_acc = evaluate(dataloaders_dict["test"])
print(
    "Coarse Acc: %.4f, FCoarse Acc: %.4f, Fine Acc: %.4f" %
    (coarse_acc, fcoarse_acc, fine_acc)
)
