import os
import cv2
import os.path as path

from glob import glob

req_w = 400
req_h = 350

data_dir = "./data/"

pad_color = [0, 0, 0]

for phase in ["train", "test"]:
    path_ = path.join(data_dir, phase)

    data = list()
    for c in os.listdir(path_):
        for p in glob(path.join(path_, c, "*.jpg")):
            im = cv2.imread(p)

            h, w = im.shape[:2]

            scale = min(req_w / w, req_h / h)

            h = int(h * scale)
            w = int(w * scale)

            im = cv2.resize(im, (w, h))

            delta_w = req_w - w
            delta_h = req_h - h
            top, bottom = delta_h // 2, delta_h - (delta_h // 2)
            left, right = delta_w // 2, delta_w - (delta_w // 2)

            im = cv2.copyMakeBorder(
                im, top, bottom, left, right,
                cv2.BORDER_CONSTANT, value=pad_color
            )

            cv2.imwrite(p, im)
