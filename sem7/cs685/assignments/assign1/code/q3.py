import os
import csv
import numpy as np
import pandas as pd

from utils import paths, clean, get_regions, read_data_csv, check_transform_condition


data = pd.read_csv("data/features.csv", sep=',')
corr_data = data.corr()

cols = list(data.columns.values)[1:]

corrs = []
for i, col1 in enumerate(cols):
    for col2 in cols[i+1:]:
        c1 = map(lambda x: x.strip(), col1.split("|"))
        c2 = map(lambda x: x.strip(), col2.split("|"))

        corrs.append((c1[1], c2[1], c1[0], c2[0],
                      float(corr_data[col1][col2])))

corrs.sort(key=lambda x: abs(x[4]), reverse=True)

ccorrs = [corr for corr in corrs if corr[2] != corr[3]]
num = len([True for corr in ccorrs if abs(corr[4]) > 0.3]) * \
    100.0 / len(ccorrs)

print "Cross Category:"
print "\n\n".join(["\tFeature A  : %s\n\tFeature B  : %s\n\tCorrelation: %.5f" % (
    col1, col2, corr) for col1, col2, cat1, cat2, corr in corrs if cat1 != cat2][:10])
print

corrs = [corr for corr in corrs if corr[2] == corr[3]]

for category in ["Economy", "Education", "Demography"]:
    print category + ":"
    print "\n\n".join(["\tFeature A  : %s\n\tFeature B  : %s\n\tCorrelation: %.5f" % (
        col1, col2, corr) for col1, col2, cat1, cat2, corr in corrs if cat1 == category][:10])
    print

print "There is less correlation between cross category features"
print "Only %f%% tuples have a correlation over 0.3" % (num)
