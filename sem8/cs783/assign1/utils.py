import glob
import matplotlib.pyplot as plt
import cv2
import numpy as np
from sklearn.manifold import TSNE

classes_list = glob.glob("./train/*")
classes = dict()
sift = cv2.xfeatures2d.SIFT_create()
test_img = cv2.imread("./sample_test/easy_multi_2.jpg")
test_kp, test_des = sift.detectAndCompute(test_img, None)

scores = [[], [], [], [], [], [], [], [], [], [], [], [], [], [], [], []]

for i,c in enumerate(classes_list):
    classes[i] = c.split('/')[-1]
    files = glob.glob(c+"/*.jpg")

    FLANN_INDEX_KDTREE = 1
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
    search_params = dict(checks = 50)
    flann = cv2.FlannBasedMatcher(index_params, search_params)

    for j,file in enumerate(files):
        img = cv2.imread(file)[50:300,225:375,:]
        kp, des = sift.detectAndCompute(img,None)

        counter = 0
        matches = flann.knnMatch(des,test_des,k=2)
        for m,n in matches:
            if m.distance < 0.5*n.distance:
                counter += 1
        scores[i].append(counter)

for i in range(16):
    print classes[i]
    print sum(scores[i])
    scores[i].sort(reverse=True)
    print scores[i][:10]