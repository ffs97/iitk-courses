import os
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib as mpl
import matplotlib.pyplot as plt

from optparse import OptionParser
from mpl_toolkits.mplot3d import Axes3D
from scipy.stats import gamma, beta, describe


mpl.rc_file_defaults()


parser = OptionParser()
parser.add_option(
    "-p", "--plotting", action="store_true", default=False
)
parser.add_option(
    "-t", "--threshold", default=45
)

(options, _) = parser.parse_args()


def get_outliers(data, filter, plotting):
    if plotting:
        for x, r in [("x1", (0, 1)), ("x2", (0, 30)), ("x3", (0, 1))]:
            plt.violinplot(data[x], vert=False)
            plt.xlim(r)
            plt.savefig("plots/violin/%s.png" % x)
            plt.clf()

    if filter:
        data_fl = data[data["class"] == 0]
    else:
        data_fl = data

    pdf = pd.DataFrame({})

    a, b, loc, scale = beta.fit(data_fl["x1"])
    pdf["x1"] = beta.logpdf(data["x1"], a, b, loc=loc, scale=scale)

    a, loc, scale = gamma.fit(data_fl["x2"])
    pdf["x2"] = gamma.logpdf(data["x2"], a, loc=loc, scale=scale)

    a, b, loc, scale = beta.fit(data_fl["x3"])
    pdf["x3"] = beta.logpdf(data["x3"], a, b, loc=loc, scale=scale)

    pdfs = pdf["x1"] + pdf["x2"] + pdf["x3"]

    if plotting:
        sns.boxplot(y=pdfs, x="class", data=data)
        plt.savefig("plots/boxplot.png")
        plt.clf()

    if plotting:
        plt.plot(np.sort(pdfs))
        splits = [40, 45, 50, 60]
        for split in splits:
            split = np.sort(pdfs)[60]
            plt.plot((0, 1000), (split, split), 'k-', lw=0.5)
            split = np.sort(pdfs)[50]
            plt.plot((0, 1000), (split, split), 'k.', lw=0.5)
            split = np.sort(pdfs)[45]
            plt.plot((0, 1000), (split, split), 'k--', lw=0.5)
            split = np.sort(pdfs)[40]
            plt.plot((0, 1000), (split, split), 'k--', lw=0.5)

        plt.savefig("plots/thresholds.png")
        plt.clf()

    outliers = np.argsort(pdfs)

    final = []
    for outlier in outliers:
        if data["class"][outlier] == -1:
            final.append(outlier)

    return np.array(final[:100])


if os.path.exists("data/data.csv"):
    data = pd.read_csv("data/data.csv", header=None)
else:
    raise FileExistsError

if options.plotting:
    if not os.path.exists("plots/violin/"):
        os.makedirs("plots/violin")

data.columns = ["idx", "x1", "x2", "x3", "class"]

outliers_wo_filter = get_outliers(data, False, options.plotting)
outliers_wt_filter = get_outliers(data, True, options.plotting)

outliers = np.intersect1d(outliers_wo_filter, outliers_wt_filter)

options.threshold = min(options.threshold, 80)
options.threshold = max(options.threshold, 40)

data["class"] = 0
data["class"][outliers] = -1

outliers = get_outliers(data, True, options.plotting)

data["class"][outliers[:options.threshold]] = 1

if options.plotting:
    n_vals = data[data["class"] != 1]
    o_vals = data[data["class"] == 1]

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(n_vals["x1"], n_vals["x2"], n_vals["x3"])
    ax.scatter(o_vals["x1"], o_vals["x2"], o_vals["x3"])

    plt.savefig("plots/outliers.png")

outliers_idx = np.array(data[data["class"] == 1]["idx"])

with open("output.csv", "w") as f:
    f.write("\n".join(str(int(idx)) for idx in outliers_idx))
