import re
import csv
import numpy as np

paths = {
    "Demography": [("child-sex-ratio-0-6-years.csv", 1), ("decadal-growth-rate.csv", 1), ("sex-ratio.csv", 1)],
    "Economy": [("gross-domestic-product-gdp-constant-price.csv", 0), ("gross-domestic-product-gdp-current-price.csv", 0), ("state-wise-net-domestic-product-ndp-constant-price.csv", 0), ("state-wise-net-domestic-product-ndp-current-price.csv", 0)],
    "Education":  [("drop-out-rate.csv", 0), ("gross-enrolment-ratio-higher-education.csv", 1), ("gross-enrolment-ratio-schools.csv", 0), ("literacy-rate-7-years.csv", 1), ("percentage-schools-boys-toilet.csv", 0), ("percentage-schools-computers.csv", 0), ("percentage-schools-drinking-water.csv", 0), ("percentage-schools-electricity.csv", 0), ("percentage-schools-girls-toilet.csv", 0)]
}


remap = {
    "andaman & nicobar islands": "a & n islands",
    "dadra & nagar haveli": "d & n haveli",
    "delhi": "nct of delhi",
    "all india": "india",
    "chhatisgarh": "chhattisgarh",
    "jammu and kashmir": "jammu & kashmir",
    "uttrakhand": "uttarakhand",
    "uttaranchal": "uttarakhand",
    "pondicherry": "puducherry",
    "west bengal1": "west bengal",
    "all_india gdp": "india",
    "all_india ndp": "india"
}


def clean(x):
    """
        Function to clean fields
    """

    global remap

    if type(x) in [float]:
        return x

    x = x.strip().lower()
    x = re.sub(r"\s+", " ", x)

    try:
        x = float(x)
    except ValueError:
        if x in remap:
            x = remap[x]

    return x


def get_regions(path="data/regions.csv"):
    """
        Get regions for all states and UTs in dict(regions) and
        Get states and UTs for all regions in dict(rev_regions)
    """
    regions = dict()
    rev_regions = dict()

    with open(path, "rb") as csvfile:
        reader = csv.reader(csvfile, delimiter=",", quotechar="\"")

        next(reader, None)
        for st, re in reader:
            st = clean(st)
            re = clean(re)

            regions[st] = clean(re)

            if re in rev_regions:
                rev_regions[re].append(st)
            else:
                rev_regions[re] = [st]

    regions["india"] = "india"
    rev_regions["india"] = ["india"]

    return regions, rev_regions


def read_data_csv(path, category, sti, stw=2):
    data = []
    with open(path, "rb") as csvfile:
        reader = csv.reader(csvfile, delimiter=",", quotechar="\"")

        header = next(reader, None)

        for row in reader:
            if row[sti] not in ["na", "nr"]:
                row = [clean(field) for field in row]
                row = row[:stw] + \
                    [field if type(field) ==
                        float else None for field in row[stw:]]

                data.append(row)

    if category == "Economy":
        data = [header] + [["na"] * len(header)] + data
        data = np.array(data).transpose().tolist()

        header = data[:2]
        data = data[2:]

    return header, data


def check_transform_condition(category, path):
    return category == "Education" and path != "Education/literacy-rate-7-years.csv"
