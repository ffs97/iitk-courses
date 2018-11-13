import time
import numpy as np
import pandas as pd
import scipy.stats as stats

from functools import partial
from sklearn.neighbors import KNeighborsClassifier


def get_fns():
    def log_fn(l, x):
        return np.log(x + l)

    def pow_fn(l, x):
        return np.power(x, l)

    log_fns = [partial(log_fn, +l) for l in np.linspace(0.1, 1.0, num=10)]
    ppw_fns = [partial(pow_fn, +l) for l in np.linspace(1.1, 3.0, num=20)]
    npw_fns = [partial(pow_fn, -l) for l in np.linspace(1.1, 3.0, num=20)]

    return log_fns + ppw_fns + npw_fns


fns = get_fns()

data = pd.read_csv("data/data.csv")
data["c"] = data["class"].astype("category")
del data["class"]


tranformations = [list(), list(), list()]
for fn in fns:
    tranformations[0].append(fn(data["x1"]))
    tranformations[1].append(fn(data["x2"]))
    tranformations[2].append(fn(data["x3"]))

tranformations = np.array(tranformations)

means = list()
for k in range(10):
    indices = data["c"] == k
    means.append(np.mean(tranformations[:, :, indices], axis=-1))

means = np.array(means)

n_neighbours = 3


# def neg_acc(f1, f2, f3, data):
#     d1 = tranformations[0][f1]
#     d2 = tranformations[1][f2]
#     d3 = tranformations[2][f3]

#     dd = (
#         np.square(d1 - d1[:, None]) +
#         np.square(d2 - d2[:, None]) +
#         np.square(d3 - d3[:, None])
#     ) + np.eye(len(data)) * 100000.0

#     indices = (np.argpartition(dd, n_neighbours)[:, :n_neighbours]).flatten()
#     classes = np.array(data["c"][indices])
#     classes = classes.reshape((len(data), n_neighbours))

#     classes = stats.mode(classes, axis=-1)[0][:, 0]

#     return - np.sum(classes == data["c"])


def neg_acc(f1, f2, f3, data):
    d1 = tranformations[0][f1]
    d2 = tranformations[1][f2]
    d3 = tranformations[2][f3]

    d1 = (d1 - np.min(d1)) / (np.max(d1) - np.min(d1))
    d2 = (d2 - np.min(d2)) / (np.max(d2) - np.min(d2))
    d3 = (d3 - np.min(d3)) / (np.max(d3) - np.min(d3))

    m1 = means[:, 0, f1]
    m2 = means[:, 1, f2]
    m3 = means[:, 2, f3]

    d1 = np.square(d1[:, None].repeat(10, axis=-1) - m1)
    d2 = np.square(d2[:, None].repeat(10, axis=-1) - m2)
    d3 = np.square(d3[:, None].repeat(10, axis=-1) - m3)

    d = d1 + d2 + d3

    classes = np.argpartition(d, 1, axis=-1)[:, 0]

    return - np.sum(classes == data["c"])


min_error = 0
f_indices = None
for i in range(0, 50):
    for j in range(0, 50):
        for k in range(0, 50):
            error = neg_acc(i, j, k, data)

            if error < min_error:
                min_error = error
                f_indices = [i, j, k]

                print f_indices, min_error

print f_indices, min_error
