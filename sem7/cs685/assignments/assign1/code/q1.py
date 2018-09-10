import os
import csv
import numpy as np

from utils import paths, clean, get_regions, read_data_csv, check_transform_condition

regions, rev_regions = get_regions()

# Adding missing values

polity_data = dict([(polity, dict()) for polity in regions.keys()])
for category in paths:
    if not os.path.exists("data/datagov-clean/" + category):
        os.makedirs("data/datagov-clean/" + category)

    for path, sti in paths[category]:
        path = category + "/" + path

        header, data = read_data_csv("data/datagov/" + path, category, sti)

        required_non_sti = [row[1-sti] for row in data] if category == "Education" \
            and path != "Education/literacy-rate-7-years.csv" else ["na"]

        required = list()
        for rns in list(set(required_non_sti)):
            required += [(polity, rns) for polity in regions]
        required = set(required)

        region_data = dict([(region, list()) for region in rev_regions])
        for row in data:
            region = regions[row[sti]]
            region_data[region].append(row[2:])

            toremove = (
                row[sti], row[1-sti]
                if check_transform_condition(category, path) else "na"
            )
            required.remove(toremove)

        for (st, nst) in required:
            data.append([None] * len(data[0]))
            data[-1][sti] = st
            data[-1][1-sti] = nst

        for region, dat in region_data.iteritems():
            dat = region_data[region]
            dat = np.array(dat, dtype=float)
            dat = np.nanmean(dat, axis=0)
            dat[np.isnan(dat)] = 0

            region_data[region] = np.array(dat, dtype=float)

        for row in data:
            region = regions[row[sti]]
            for i in range(2, len(row)):
                if row[i] == None:
                    row[i] = float(region_data[region][i - 2])

        header_ = header
        if type(header[0]) == list:
            header_ = [header[0][i] + " -- " + header[1][i]
                       for i in range(len(header[0]))]

        for row in data:
            if check_transform_condition(category, path):
                for i, field in enumerate(row[2:]):
                    col = category + " | " + header_[i+2] + " -- " + row[1-sti]
                    polity_data[row[sti]][col] = clean(field)
            else:
                for i, field in enumerate(row[2:]):
                    col = category + " | " + header_[i+2]
                    polity_data[row[sti]][col] = clean(field)

        if category == "Economy":
            data = header + data
            data = map(None, *data)
            header = data[0]
            data = [list(row) for row in data[2:]]

        with open("data/datagov-clean/" + path, "wb") as csvfile:
            writer = csv.writer(csvfile, delimiter=",",
                                quotechar="\"", quoting=csv.QUOTE_NONNUMERIC)

            writer.writerow(header)
            for row in data:
                writer.writerow(row)


cols = ["polity"] + polity_data["india"].keys()
with open("data/features.csv", 'w') as csvfile:
    writer = csv.DictWriter(csvfile, fieldnames=cols)

    writer.writeheader()
    for polity in polity_data:
        polity_data[polity]["polity"] = polity
        writer.writerow(polity_data[polity])
