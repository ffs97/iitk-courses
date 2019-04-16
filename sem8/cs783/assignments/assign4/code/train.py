import os
import cv2
import argparse
import numpy as np
import matplotlib as mpl

from glob import glob
from tqdm import tqdm
from scipy.stats import mode
from skimage import io, segmentation
from classifiers import LinearCNNClassifier, AutoEncoderClassifier

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F

from torch.autograd import Variable
from torch.utils.data import Dataset

mpl.use("TKAgg")
mpl.rc_file_defaults()

use_cuda = torch.cuda.is_available()

parser = argparse.ArgumentParser(description="Unsupervised Segmentation")

parser.add_argument("--collective", action="store_true",
                    default=False, help="collective training flag")

parser.add_argument("--data_dir", metavar="DATA PATH",
                    type=str, default="", help="path to data directory")

parser.add_argument("--model", metavar="MODEL",
                    type=str, required=True, help="model to use")

parser.add_argument("--n_channels", metavar="L_1, L_2 ... L_m", nargs="+",
                    type=int, default=[100, 100], help="number of channels in each cnn layer")
parser.add_argument("--n_classes", metavar="K", default=100,
                    type=int, help="number of classes")
parser.add_argument("--min_size", metavar="MS", default=200, type=int)
parser.add_argument("--sigma", metavar="SS", default=0.8, type=float)
parser.add_argument("--scale", metavar="TT", default=250, type=int)

parser.add_argument("--visualize", action="store_true",
                    default=False, help="visualization flag")
parser.add_argument("--visualize_iters", metavar="V", default=50,
                    type=int, help="visualization iterations")
parser.add_argument("--test_img", metavar="IMAGE PATH",
                    default="", type=str, help="test image path")

parser.add_argument("--lr", metavar="LR", default=0.02,
                    type=float, help="learning rate")
parser.add_argument("--momentum", metavar="MM", default=0.9,
                    type=float, help="learning momentum")
parser.add_argument("--num_epochs", metavar="N", default=1000,
                    type=int, help="number of iterations")
parser.add_argument("--batch_size", metavar="BS", default=5,
                    type=int, help="batch size")


def visualize(img, img_shape, model, out_dir, n_iter):
    label_colours = np.random.randint(255, size=(model.n_classes, 3))

    if args.model == "autoencoder":
        y, x_recon = model(img, True)
    else:
        y = model(img)

    y = y[0].permute(1, 2, 0).contiguous()
    y = y.view(-1, model.n_classes)

    _, y_ = torch.max(y, dim=1)
    y_ = y_.data.cpu().numpy()

    vimg = np.array([label_colours[c] for c in y_])
    vimg = vimg.reshape(img_shape).astype(np.uint8)

    io.imsave("%s/segments-iter-%d.png" % (out_dir, n_iter), vimg)

    if args.model == "autoencoder":
        x_recon = x_recon[0].permute((1, 2, 0)).data.cpu().numpy()
        x_recon = (x_recon * 255).astype(np.uint8)

        io.imsave("%s/recon-iter-%d.png" % (out_dir, n_iter), x_recon)


def collective_train(model, dataloader, args, out_dir="data/output/collective"):
    optimizer = optim.SGD(
        model.parameters(), lr=args.lr, momentum=args.momentum
    )

    if args.visualize:

        if args.test_img == "" or not os.path.exists(args.test_img):
            print("Invalid test image file")
            exit()

        test_img = io.imread(args.test_img)

        img_shape = test_img.shape

        test_img = torch.from_numpy(
            test_img.transpose((2, 0, 1))[np.newaxis, :]
        ).type(torch.FloatTensor) / 255
        if use_cuda:
            test_img = test_img.cuda()

        test_img = Variable(test_img)

        if not os.path.exists(out_dir):
            os.makedirs(out_dir)

    model.train()

    beta = 1.0
    with tqdm(range(args.num_epochs)) as bar:
        for n_iter in bar:
            running_loss = 0.0

            for imgs, superpixels in dataloader:
                if use_cuda:
                    imgs = imgs.cuda()
                imgs = Variable(imgs)

                running_loss += model.step(
                    imgs, superpixels, optimizer, beta, use_cuda
                )

            beta = max(0, beta - 0.1)

            bar.set_postfix({
                "loss": running_loss
            })

            if args.visualize and n_iter % args.visualize_iters == 0:
                model.eval()
                visualize(test_img, img_shape, model, out_dir,
                          n_iter // args.visualize_iters)
                model.train()


def selective_train(model, args, out_dir="data/output/selective"):
    optimizer = optim.SGD(
        model.parameters(), lr=args.lr, momentum=args.momentum
    )

    if args.test_img == "" or not os.path.exists(args.test_img):
        print("Invalid image file")
        exit()

    img = io.imread(args.test_img)

    img_shape = img.shape

    segments = segmentation.felzenszwalb(
        img, scale=args.scale, sigma=args.sigma,
        min_size=args.min_size, multichannel=True
    )

    if args.visualize:
        io.imsave(out_dir + "/segments.png",
                  segmentation.mark_boundaries(img, segments))

    segments = segments.reshape(-1)

    superpixels = [[np.where(semgents == x)[0]
                    for x in np.unique(semgents)]]

    img = torch.from_numpy(
        img.transpose((2, 0, 1))[np.newaxis, :]
    ).type(torch.FloatTensor) / 255
    if use_cuda:
        img = img.cuda()

    img = Variable(img)

    if args.visualize and not os.path.exists(out_dir):
        os.makedirs(out_dir)

    model.train()

    beta = 1.0
    with tqdm(range(args.num_epochs)) as bar:
        for n_iter in bar:
            loss = model.step(
                img, superpixels, optimizer, beta, use_cuda
            )

            beta = max(0, beta - 0.1)

            bar.set_postfix({
                "loss": loss
            })

            if args.visualize and n_iter % args.visualize_iters == 0:
                model.eval()
                visualize(img, img_shape, model, out_dir,
                          n_iter // args.visualize_iters)
                model.train()


def main(args):
    if args.collective:
        if args.data_dir == "" or not os.path.exists(args.data_dir):
            print("Invalid data directory")
            exit()

        class ImageDataset(Dataset):
            def __init__(self, root_dir):
                root_dir = os.path.join(root_dir, "*.npy")

                data_paths = glob(root_dir)

                self.data = [np.load(data_path) for data_path in data_paths]
                self.len = len(data_paths)

            def __getitem__(self, index):
                return self.data[index]

            def __len__(self):
                return self.len

        def my_collate(batch):
            imgs, superpixels = zip(*batch)

            imgs = np.array(imgs)
            imgs = torch.from_numpy(imgs).type(torch.FloatTensor)

            return imgs, superpixels

        dataloader = torch.utils.data.DataLoader(
            ImageDataset(args.data_dir), batch_size=args.batch_size,
            shuffle=True, num_workers=4, collate_fn=my_collate
        )

    if args.model == "autoencoder":
        model = AutoEncoderClassifier(3, args.n_channels, args.n_classes)
    elif args.model == "cnn":
        model = LinearCNNClassifier(3, args.n_channels, args.n_classes)
    if use_cuda:
        model.cuda()

    if args.collective:
        collective_train(model, dataloader, args)
    else:
        selective_train(model, args)


if __name__ == "__main__":
    args = parser.parse_args()
    print(args)

    main(args)
