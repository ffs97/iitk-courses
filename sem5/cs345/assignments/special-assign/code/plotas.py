import numpy as np

with open("dat.txt", "r") as f:
    dat = np.array([[float(i) for i in x.split()] for x in f.readlines()])

points = dat[:, 0].astype(int)
times = dat[:, 1]

expected_times = points * np.sum(points*times) / np.sum(points**2)