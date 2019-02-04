from __future__ import print_function
from __future__ import division
import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import torchvision
from torchvision import datasets, models, transforms
import matplotlib.pyplot as plt
import time
import os
import copy
print("PyTorch Version: ", torch.__version__)
print("Torchvision Version: ", torchvision.__version__)


######################################################################
# Inputs
# ------
#
# Here are all of the parameters to change for the run. We will use the
# *hymenoptera_data* dataset which can be downloaded
# `here <https://download.pytorch.org/tutorial/hymenoptera_data.zip>`__.
# This dataset contains two classes, **bees** and **ants**, and is
# structured such that we can use the
# `ImageFolder <https://pytorch.org/docs/stable/torchvision/datasets.html#torchvision.datasets.ImageFolder>`__
# dataset, rather than writing our own custom dataset. Download the data
# and set the ``data_dir`` input to the root directory of the dataset. The
# ``model_name`` input is the name of the model you wish to use and must
# be selected from this list:
#
# ::
#
#    [resnet, alexnet, vgg, squeezenet, densenet, inception]
#
# The other inputs are as follows: ``num_classes`` is the number of
# classes in the dataset, ``batch_size`` is the batch size used for
# training and may be adjusted according to the capability of your
# machine, ``num_epochs`` is the number of training epochs we want to run,
# and ``feature_extract`` is a boolean that defines if we are finetuning
# or feature extracting. If ``feature_extract = False``, the model is
# finetuned and all model parameters are updated. If
# ``feature_extract = True``, only the last layer parameters are updated,
# the others remain fixed.
#

# Top level data directory. Here we assume the format of the directory conforms
#   to the ImageFolder structure
data_dir = "./data/assign_data"

# Models to choose from [resnet, alexnet, vgg, squeezenet, densenet, inception]
model_name = "resnet"

# Number of classes in the dataset
num_classes = 16

# Batch size for training (change depending on how much memory you have)
batch_size = 10

# Number of epochs to train for
num_epochs = 15

# Flag for feature extracting. When False, we finetune the whole model,
#   when True we only update the reshaped layer params
feature_extract = True


######################################################################
# Helper Functions
# ----------------
#

def train_model(model, dataloaders, criterion, optimizer, num_epochs=25, is_inception=False):
    since = time.time()

    val_acc_history = []

    best_model_wts = copy.deepcopy(model.state_dict())
    best_acc = 0.0

    for epoch in range(num_epochs):
        print('Epoch {}/{}'.format(epoch, num_epochs - 1))
        print('-' * 10)

        # Each epoch has a training and validation phase
        for phase in ['train', 'val']:
            if phase == 'train':
                model.train()  # Set model to training mode
            else:
                model.eval()   # Set model to evaluate mode

            running_loss = 0.0
            running_corrects = 0

            # Iterate over data.
            for inputs, labels in dataloaders[phase]:
                inputs = inputs.to(device)
                labels = labels.to(device)

                # zero the parameter gradients
                optimizer.zero_grad()

                # forward
                # track history if only in train
                with torch.set_grad_enabled(phase == 'train'):
                    # Get model outputs and calculate loss
                    # Special case for inception because in training it has an auxiliary output. In train
                    #   mode we calculate the loss by summing the final output and the auxiliary output
                    #   but in testing we only consider the final output.
                    if is_inception and phase == 'train':
                        # From https://discuss.pytorch.org/t/how-to-optimize-inception-model-with-auxiliary-classifiers/7958
                        outputs, aux_outputs = model(inputs)
                        loss1 = criterion(outputs, labels)
                        loss2 = criterion(aux_outputs, labels)
                        loss = loss1 + 0.4*loss2
                    else:
                        outputs = model(inputs)
                        loss = criterion(outputs, labels)

                    _, preds = torch.max(outputs, 1)

                    # backward + optimize only if in training phase
                    if phase == 'train':
                        loss.backward()
                        optimizer.step()

                # statistics
                running_loss += loss.item() * inputs.size(0)
                running_corrects += torch.sum(preds == labels.data)

            epoch_loss = running_loss / len(dataloaders[phase].dataset)
            epoch_acc = running_corrects.double(
            ) / len(dataloaders[phase].dataset)

            print('{} Loss: {:.4f} Acc: {:.4f}'.format(
                phase, epoch_loss, epoch_acc))

            # deep copy the model
            if phase == 'val' and epoch_acc > best_acc:
                best_acc = epoch_acc
                best_model_wts = copy.deepcopy(model.state_dict())
            if phase == 'val':
                val_acc_history.append(epoch_acc)

        print()

    time_elapsed = time.time() - since
    print('Training complete in {:.0f}m {:.0f}s'.format(
        time_elapsed // 60, time_elapsed % 60))
    print('Best val Acc: {:4f}'.format(best_acc))

    # load best model weights
    model.load_state_dict(best_model_wts)
    return model, val_acc_history


######################################################################
# Set Model Parameters’ .requires_grad attribute
# -----------------------------------
#

def set_parameter_requires_grad(model, feature_extracting):
    if feature_extracting:
        for param in model.parameters():
            param.requires_grad = False


######################################################################
# Initialize and Reshape the Networks
# -----------------------------------
#
# Resnet
# ~~~~~~
#
#    (fc): Linear(in_features=512, out_features=1000, bias=True)
#
# Thus, we must reinitialize ``model.fc`` to be a Linear layer with 512
# input features and 2 output features with:
#
#    model.fc = nn.Linear(512, num_classes)
#
# Alexnet
# ~~~~~~~
#
#    (classifier): Sequential(
#        ...
#        (6): Linear(in_features=4096, out_features=1000, bias=True)
#     )
#
# To use the model with our dataset we reinitialize this layer as
#
#    model.classifier[6] = nn.Linear(4096,num_classes)
#
# VGG
# ~~~
#
#    (classifier): Sequential(
#        ...
#        (6): Linear(in_features=4096, out_features=1000, bias=True)
#     )
#
# Therefore, we use the same technique to modify the output layer
#
#    model.classifier[6] = nn.Linear(4096,num_classes)
#
# Squeezenet
# ~~~~~~~~~~
#
#    (classifier): Sequential(
#        (0): Dropout(p=0.5)
#        (1): Conv2d(512, 1000, kernel_size=(1, 1), stride=(1, 1))
#        (2): ReLU(inplace)
#        (3): AvgPool2d(kernel_size=13, stride=1, padding=0)
#     )
#
# To modify the network, we reinitialize the Conv2d layer to have an
# output feature map of depth 2 as
#
#    model.classifier[1] = nn.Conv2d(512, num_classes, kernel_size=(1,1), stride=(1,1))
#
# Densenet
# ~~~~~~~~
#
#    (classifier): Linear(in_features=1024, out_features=1000, bias=True)
#
# To reshape the network, we reinitialize the classifier’s linear layer as
#
#    model.classifier = nn.Linear(1024, num_classes)
#
# Inception v3
# ~~~~~~~~~~~~
#
#    (AuxLogits): InceptionAux(
#        ...
#        (fc): Linear(in_features=768, out_features=1000, bias=True)
#     )
#     ...
#    (fc): Linear(in_features=2048, out_features=1000, bias=True)
#
# To finetune this model we must reshape both layers. This is accomplished
# with the following
#
#    model.AuxLogits.fc = nn.Linear(768, num_classes)
#    model.fc = nn.Linear(2048, num_classes)
#

def initialize_model(model_name, num_classes, feature_extract, use_pretrained=True):
    # Initialize these variables which will be set in this if statement. Each of these
    #   variables is model specific.
    model_ft = None
    input_size = 0

    if model_name == "resnet":
        """ Resnet18
        """
        model_ft = models.resnet18(pretrained=use_pretrained)
        set_parameter_requires_grad(model_ft, feature_extract)
        num_ftrs = model_ft.fc.in_features
        model_ft.fc = nn.Linear(num_ftrs, num_classes)
        input_size = 224

    elif model_name == "alexnet":
        """ Alexnet
        """
        model_ft = models.alexnet(pretrained=use_pretrained)
        set_parameter_requires_grad(model_ft, feature_extract)
        num_ftrs = model_ft.classifier[6].in_features
        model_ft.classifier[6] = nn.Linear(num_ftrs, num_classes)
        input_size = 224

    elif model_name == "vgg":
        """ VGG11_bn
        """
        model_ft = models.vgg11_bn(pretrained=use_pretrained)
        set_parameter_requires_grad(model_ft, feature_extract)
        num_ftrs = model_ft.classifier[6].in_features
        model_ft.classifier[6] = nn.Linear(num_ftrs, num_classes)
        input_size = 224

    elif model_name == "squeezenet":
        """ Squeezenet
        """
        model_ft = models.squeezenet1_0(pretrained=use_pretrained)
        set_parameter_requires_grad(model_ft, feature_extract)
        model_ft.classifier[1] = nn.Conv2d(
            512, num_classes, kernel_size=(1, 1), stride=(1, 1))
        model_ft.num_classes = num_classes
        input_size = 224

    elif model_name == "densenet":
        """ Densenet
        """
        model_ft = models.densenet121(pretrained=use_pretrained)
        set_parameter_requires_grad(model_ft, feature_extract)
        num_ftrs = model_ft.classifier.in_features
        model_ft.classifier = nn.Linear(num_ftrs, num_classes)
        input_size = 224

    elif model_name == "inception":
        """ Inception v3
        Be careful, expects (299,299) sized images and has auxiliary output
        """
        model_ft = models.inception_v3(pretrained=use_pretrained)
        set_parameter_requires_grad(model_ft, feature_extract)
        # Handle the auxilary net
        num_ftrs = model_ft.AuxLogits.fc.in_features
        model_ft.AuxLogits.fc = nn.Linear(num_ftrs, num_classes)
        # Handle the primary net
        num_ftrs = model_ft.fc.in_features
        model_ft.fc = nn.Linear(num_ftrs, num_classes)
        input_size = 299

    else:
        print("Invalid model name, exiting...")

    return model_ft, input_size


# Initialize the model for this run
model_ft, input_size = initialize_model(
    model_name, num_classes, feature_extract, use_pretrained=True)

# Print the model we just instantiated
print(model_ft)


######################################################################
# Load Data
# ---------
#

# Data augmentation and normalization for training
# Just normalization for validation
data_transforms = {
    'train': transforms.Compose([
        transforms.RandomResizedCrop(input_size),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
    'val': transforms.Compose([
        transforms.Resize(input_size),
        transforms.CenterCrop(input_size),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
    'test': transforms.Compose([
        transforms.Resize(input_size),
        transforms.CenterCrop(input_size),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
}

print("Initializing Datasets and Dataloaders...")

# Create training and validation datasets
image_datasets = {x: datasets.ImageFolder(os.path.join(
    data_dir, x), data_transforms[x]) for x in ['train', 'val', 'test']}

# Create training and validation dataloaders
dataloaders_dict = {x: torch.utils.data.DataLoader(
    image_datasets[x], batch_size=batch_size, shuffle=True, num_workers=4) for x in ['train', 'val', 'test']}

# Detect if we have a GPU available
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")


######################################################################
# Create the Optimizer
# --------------------

# Send the model to GPU
model_ft = model_ft.to(device)

params_to_update = model_ft.parameters()
print("Params to learn:")
if feature_extract:
    params_to_update = []
    for name, param in model_ft.named_parameters():
        if param.requires_grad == True:
            params_to_update.append(param)
            print("\t", name)
else:
    for name, param in model_ft.named_parameters():
        if param.requires_grad == True:
            print("\t", name)

# Observe that all parameters are being optimized
optimizer_ft = optim.SGD(params_to_update, lr=0.001, momentum=0.9)


######################################################################
# Run Training and Validation Step
# --------------------------------

# Setup the loss fxn
criterion = nn.CrossEntropyLoss()

# Train and evaluate
model_ft, hist = train_model(model_ft, dataloaders_dict, criterion, optimizer_ft,
                             num_epochs=num_epochs, is_inception=(model_name == "inception"))


for inputs, labels in dataloaders_dict["test"]:
    inputs = inputs.to(device)
    labels = labels.to(device)

    with torch.set_grad_enabled(False):
        outputs = model_ft(inputs)

        _, preds = torch.max(outputs, 1)

        print(preds)

# Plot the training curves of validation accuracy vs. number
#  of training epochs for the transfer learning method and
#  the model trained from scratch
hist = [h.cpu().numpy() for h in hist]

plt.title("Validation Accuracy vs. Number of Training Epochs")
plt.xlabel("Training Epochs")
plt.ylabel("Validation Accuracy")
plt.plot(range(1, num_epochs+1), hist)
plt.ylim((0, 1.))
plt.xticks(np.arange(1, num_epochs+1, 1.0))
plt.legend()
plt.show()
