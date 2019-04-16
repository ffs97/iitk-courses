import os
import cv2
import argparse
import numpy as np

from glob import glob
from tqdm import tqdm
from skimage import segmentation


parser = argparse.ArgumentParser(description="Unsupervised Segmentation")

parser.add_argument("--data_dir", metavar="DATA PATH",
                    type=str, required=True, help="path to data directory")

parser.add_argument("--min_size", metavar="M", default=200, type=int)
parser.add_argument("--sigma", metavar="S", default=0.8, type=float)
parser.add_argument("--scale", metavar="T", default=250, type=int)


def preprocess_data(args, dir="data/processed"):
    if not os.path.exists(dir):
        os.makedirs(dir)

    img_paths = glob(os.path.join(args.data_dir, "*"))

    with tqdm(img_paths) as bar:
        for img_path in bar:
            img_name = img_path.split("/")[-1]
            img_name = img_name.replace(".jpg", "").replace(".png", "")

            img = cv2.imread(img_path)

            segments = segmentation.felzenszwalb(
                img, scale=args.scale, sigma=args.scale,
                min_size=args.min_size, multichannel=True
            ).reshape(-1)

            superpixels = [np.where(segments == x)[0]
                           for x in np.unique(segments)]

            img = img.transpose((2, 0, 1))

            np.save(os.path.join(dir, img_name + ".npy"), (img, superpixels))


if __name__ == "__main__":
    args = parser.parse_args()
    print(args)

    preprocess_data(args)
