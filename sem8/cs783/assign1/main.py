import os
import cv2
import time
import pickle
import numpy as np

from glob import glob

start = time.time()

## Parameters

data_dir = "./data/"
output_dir = "./output/"

preprocess = False

maj_score_num = 3
score_threshold = 0
distance_threshold = 0.4

num_neighbours = 2
matcher = "brute-force"

if matcher == "flann":
    index_params = {
        "algorithm": 1,
        "trees": 5
    }
    search_params = {
        "checks": 50
    }
    def get_matcher():
        return cv2.FlannBasedMatcher(index_params, search_params)
    
elif matcher == "brute-force":
    def get_matcher():
        return cv2.BFMatcher()

## Data paths

train_dir = os.path.join(data_dir, "train")
test_dir = os.path.join(data_dir, "test")

classes = [
    dir for dir in os.listdir(train_dir)
    if os.path.isdir(os.path.join(train_dir, dir))
]

## Preprocessing sift features for train files

if not os.path.exists("data/train/features.pkl"):
    preprocess = True
    
if preprocess:
    print("Computing sift features for train data")

    train_files = {
        class_: glob(os.path.join(train_dir, class_ + "/*.jpg"))
        for class_ in classes
    }
    
    features = dict()
    for class_, files in train_files.items():
        sift = cv2.xfeatures2d.SIFT_create()

        features[class_] = dict()
        for file in files:
            img = cv2.imread(file)[50:300,225:375,:]

            features[class_][file] = sift.detectAndCompute(img, None)[1]

    with open("data/train/features.pkl", "wb") as f:
        pickle.dump(file=f, obj=features)
else:
    with open("data/train/features.pkl", "rb") as f:
        features = pickle.load(file=f)

sift = cv2.xfeatures2d.SIFT_create()
test_files = glob(os.path.join(test_dir, "*.jpg"))

if not os.path.exists("output"):
    os.makedirs("output")

print("Computing matches for test data")
for ts_file in test_files:
    print("\t", ts_file)
    scores = dict()
    
    img = cv2.imread(ts_file)
    _, ts_ft = sift.detectAndCompute(img, None)
    
    for class_, tr_fts in features.items():
        scores[class_] = dict()
        
        matcher = get_matcher()
        
        for tr_file, tr_ft in tr_fts.items():
            matches = matcher.knnMatch(tr_ft, ts_ft, k=num_neighbours)
            
            tr_name = tr_file.split("/")[-1]
            scores[class_][tr_name] = max(sum([
                1 if (m.distance < distance_threshold * n.distance) else 0
                 for m, n in matches
            ]) - score_threshold, 0)
    
    scores_cw = {
        class_: sum(sorted(
            list(scores[class_].values()), reverse=True
        )[:maj_score_num])
        for class_ in classes
    }
    
    ranked_classes = sorted(
        [class_ for class_ in classes],
        key=lambda x: -scores_cw[x]
    )
    
    ranked_files = []
    for class_ in ranked_classes:
        ranked_files.extend([
            ("%s_%s" % (class_, file), score) 
            for file, score in scores[class_].items()
        ])
        
    ranked_files = [
        file for file, _ in sorted(ranked_files, key=lambda x: -x[1])
    ]
    
    out_file = ts_file.split("/")[-1].replace(".jpg", ".txt")
    with open(os.path.join(output_dir, out_file), "w") as f:
        f.write("\n".join(ranked_files))

print("Time Taken:", time.time())
