import os
import cv2
import os.path as path

from glob import glob

from includes.config import pad_color, data_dir, req_w, req_h


course_classes = sorted(os.listdir(data_dir))

fine_classes = {
    course_class: sorted(os.listdir(path.join(data_dir, course_class)))
    for course_class in course_classes
}

data = list()
for c in course_classes:
    for f in fine_classes[c]:
        for p in glob(path.join(data_dir, c, f, "*.jpg")):
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
