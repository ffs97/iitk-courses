import pandas as pd
import matplotlib as mpl

mpl.rc_file_defaults()

import matplotlib.pyplot as plt


data = pd.read_csv("data/collated.csv")

data["incorrect"] = (data.response != "ba") & (data.response != "pa")
data = data[["video", "type", "gender", "incorrect"]]
data.type = (data.type == "fu")
data.type = data.type.apply(lambda x: ("Full" if x else "Brief"))

grouped = data.groupby(["video", "type", "gender"], as_index=False).mean()

grouped = grouped[grouped.video != "ba"]
grouped.index = grouped.type + " /" + grouped.video + "/"

grouped = grouped[["incorrect", "gender"]]

grouped.pivot(index=grouped.index, columns="gender").plot.bar(color=["navy", "orange"])
plt.legend(labels=["females", "males"])
plt.ylabel("Percent Visually Influenced Responses")
plt.xlabel("Stimulus Type")
plt.savefig("plot.png")
