import os
import csv
import math
import pprint
import numpy as np
import pandas as pd
import statsmodels.graphics.gofplots as gofplots

from matplotlib import pyplot as plt

from utils import paths, clean, get_regions, read_data_csv, check_transform_condition

regions, rev_regions = get_regions()


def get_hit_miss(data, st):
    global regions

    distances = [(ss, np.linalg.norm(data[st] - data[ss]))
                 for ss in data.keys() if ss != st]
    distances.sort(key=lambda x: x[1])

    hit = None
    miss = None

    region = regions[st]
    for dist in distances:
        if hit is not None and miss is not None:
            break
        if hit is None and regions[dist[0]] == region:
            hit = dist[0]
        elif miss is None and regions[dist[0]] != region:
            miss = dist[0]

    return hit, miss


def get_top_features(data, m=1000):
    cols = list(data.columns.values)

    retval = dict()

    conds = [
        lambda x: True,
        lambda x: col.split("|")[0].strip() == "Demography",
        lambda x: col.split("|")[0].strip() == "Education",
        lambda x: col.split("|")[0].strip() == "Economy"
    ]
    labels = ["Overall", "Demography", "Education", "Economy"]

    polities = list(data.index.values)
    polities.remove("india")

    for i in range(len(conds)):
        cols_ = [col for col in cols if conds[i](col)]

        data_dict = dict()
        data_matrix = data[cols_].values
        for j, polity in enumerate(polities):
            data_dict[polity] = data_matrix[j]

        scores = np.zeros(len(cols_))

        for _ in range(m):
            n = np.random.randint(0, len(polities))
            hit, miss = get_hit_miss(data_dict, polities[n])
            scores -= data_dict[hit] - data_dict[miss]

        scores = [(cols_[j], scores[j]) for j in range(0, len(cols_))]
        scores.sort(key=lambda x: x[1], reverse=True)

        retval[labels[i]] = [scores[0][0], scores[1][0]]

    return retval


data = pd.read_table("data/features.csv", sep=",", index_col=0)
normalized_data = (data - data.mean()) / (data.max() - data.min())

top_features = get_top_features(normalized_data)

pprint.pprint(top_features, indent=4)

if not os.path.exists("plots"):
    os.makedirs("plots")

# for label in top_features:
#     features = top_features[label]
#     plt.scatter(data[features[0]], data[features[1]], s=7)
#     plt.xlabel(features[0].title())
#     plt.ylabel(features[1].title())
#     plt.title("Top Features (" + label + ")")
#     plt.savefig("plots/scatter-plot-" + label.lower() + ".png")
#     plt.clf()

#     plot_feature1 = gofplots.ProbPlot(data[features[0]])
#     plot_feature2 = gofplots.ProbPlot(data[features[1]])
#     fig = gofplots.qqplot_2samples(
#         plot_feature1, plot_feature2, line="r", xlabel=features[0], ylabel=features[1])

#     dots = fig.findobj(lambda x: hasattr(
#         x, 'get_color') and x.get_color() == 'b')
#     [d.set_ms(3) for d in dots]

#     plt.title("Probability Plot (" + label + ")")
#     plt.savefig("plots/pp-plot-" + label.lower() + ".png")
#     plt.clf()


def intuitive_partion(data, clip=True):
    if len(data) == 0:
        return data

    data = np.array(data, dtype=float)

    if clip:
        m_ = np.quantile(data, 0.05)
        M_ = np.quantile(data, 0.95)
    else:
        m_ = data.min()
        M_ = data.max()

    m, M = m_, M_

    Mm = max(abs(M), abs(m))

    if m == M:
        return [data.tolist()]

    count = 0
    while Mm >= 10 or 1 > Mm:
        if abs(Mm) >= 10:
            count += 1
            M /= 10.0
            m /= 10.0
        if abs(Mm) < 1:
            count -= 1
            M *= 10.0
            m *= 10.0

        Mm = max(abs(M), abs(m))

    M, m = int(math.ceil(M)), int(math.floor(m))
    d = M - m

    if d > 10:
        d = math.ceil(d / 2)

    if d % 3 == 0:
        ranges = [m + i * d / 3 for i in range(4)]
    elif d == 7:
        ranges = [m, m + 2, m + 5, m + 7]
    elif d % 5 == 0 or d == 1:
        ranges = [m + i * d / 5.0 for i in range(6)]
    elif d % 2 == 0 or d == 1:
        ranges = [m + i * d / 4.0 for i in range(5)]

    ranges = [-np.inf] + ranges + [np.inf]

    ranges = np.array(ranges, dtype=float) * 10**count

    partitions = []
    for i in range(0, len(ranges) - 1):
        data_ = data[data < ranges[i+1]]
        data_ = data_[data_ >= ranges[i]]

        if len(data_) > 0:
            partitions.append(list(data_))

    return partitions


for label in top_features:
    features = top_features[label]

    for k in range(2):
        partitioned_feature_data = intuitive_partion(data[features[k]])
        print "Category:" + label, features[k]
        for i, partition in enumerate(partitioned_feature_data):
            partitioned_feature_data[i] = intuitive_partion(partition)

        pprint.pprint(partitioned_feature_data)
