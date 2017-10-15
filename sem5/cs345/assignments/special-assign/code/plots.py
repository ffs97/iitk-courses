import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

with open("welzl.dat", "r") as f:
    welzl_data = np.array(
        [[float(i) for i in x.split()] for x in f.readlines()]
    )

with open("brute.dat", "r") as f:
    brute_data = np.array(
        [[float(i) for i in x.split()] for x in f.readlines()]
    )

sns.set_style("white")

welzl_times = welzl_data[:, 1:]*1000
welzl_means = np.mean(welzl_times, 1)
welzl_points = welzl_data[:, 0].astype(int)
welzl_error = [welzl_means - np.min(welzl_times, 1), np.max(welzl_times, 1) - welzl_means]

welzl_sol = np.sum(welzl_points*welzl_means) * 1000.0 / np.sum(welzl_points**2)

print "m = ", welzl_sol

welzl_high = np.sum((welzl_times - welzl_points[:, None] * welzl_sol / 1000.0) > 0, 1)
welzl_std = np.std(welzl_times, 1)

for i in range(0, len(welzl_points)):
    print int(welzl_points[i]), "\t&\t", welzl_means[i], "\t&\t", welzl_std[i], "\t&\t", welzl_high[i], "\t\\\\"

plt.plot([0, welzl_points[-1]], [0, welzl_points[-1]*welzl_sol / 1000.0], '--', c="orange")
plt.scatter(welzl_points, welzl_means, s=7)
plt.errorbar(welzl_points, welzl_means, yerr=welzl_error, capsize=3, ecolor="green")

plt.title("Welzl Algorithm: N vs Time")
plt.ylabel("Time per 100 iterations (ms)")
plt.xlabel("Number of Points")
plt.savefig("../plots/plot-welzl.pdf")

plt.clf()

welzl_classes = list(welzl_points) * welzl_times.shape[1]
welzl_times_linear = welzl_times.transpose().reshape(welzl_points.shape[0] * welzl_times.shape[1])

sns.violinplot(welzl_classes, welzl_times_linear)
plt.title("Welzl Algorithm: N vs Time (Violin Plot)")
plt.ylabel("Time per 100 iterations (ms)")
plt.xlabel("Number of Points")
plt.savefig("../plots/plot-welzl-violin.pdf")

plt.clf()

brute_points = brute_data[:, 0].astype(int)
brute_times = brute_data[:, 1]

plt.scatter(brute_points, brute_times, s=7)
plt.plot(brute_points, brute_times)
pts = np.arange(0, 1250, 10)
plt.plot(pts, (pts / 100.0)**4 * np.sum(brute_data[4:, 1] * (brute_data[4:, 0] / 100.0)**4) / np.sum((brute_data[4:, 0] / 100.0)**8), '--', c="orange")

plt.title("Brute Force Algorithm: N vs Time")
plt.ylabel("Time per iteration (s)")
plt.xlabel("Number of Points")
plt.savefig("../plots/plot-brute.pdf")
