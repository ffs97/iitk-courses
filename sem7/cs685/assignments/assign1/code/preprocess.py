import os
import csv

from utils import paths, clean, get_regions

regions, rev_regions = get_regions()

os.system("unzip data/datagov.zip -d data")

for category, category_paths in paths.iteritems():
    for path, sti in category_paths:
        path = "data/datagov/" + category + "/" + path

        data = []
        with open(path, "rb") as csvfile:
            reader = csv.reader(csvfile, delimiter=",", quotechar="\"")
            for row in reader:
                data.append([clean(field) for field in row])

        with open(path, "wb") as csvfile:
            writer = csv.writer(csvfile, delimiter=",",
                                quotechar="\"", quoting=csv.QUOTE_NONNUMERIC)

            for row in data:
                writer.writerow(row)
