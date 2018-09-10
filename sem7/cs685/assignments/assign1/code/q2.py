import os
import csv
import numpy as np

from utils import paths, clean, read_data_csv, check_transform_condition


def compare(polity, polity_data):
    x, y = polity_data[polity], polity_data["india"]

    x = np.array(x, dtype="float")
    y = np.array(y, dtype="float")

    return np.linalg.norm(x - y)


def get_comparison(polity_data):
    comparison = [(polity, compare(polity, polity_data))
                  for polity in polity_data.keys() if polity != "india"]
    comparison.sort(key=lambda x: x[1])
    return [polity for polity, _ in comparison]


_, data = read_data_csv("data/features.csv", "", 0, stw=1)

polity_data = dict()
for row in data:
    polity_data[row[0]] = row[1:]


# Without Normalization
comparison = get_comparison(polity_data)
print "\n", "Closest 5 States/Union Territories(Without Normalization) -"
print "\t", ", ".join([state.title() for state in comparison[:5]]), "\n"

polities = list()
polities_arr = list()
for polity, polity_arr in polity_data.iteritems():
    polities.append(polity)
    polities_arr.append(polity_arr)

polities_arr = np.array(polities_arr, dtype="float")
polities_arr = polities_arr / polities_arr.max(axis=0)
polities_arr = polities_arr.tolist()

for polity, polity_arr in zip(polities, polities_arr):
    polity_data[polity] = polity_arr

# With Normalization
comparison = get_comparison(polity_data)
print "Closest 5 States/Union Territories (With Normalization) -"
print "\t", ", ".join([state.title() for state in comparison[:5]]), "\n"
