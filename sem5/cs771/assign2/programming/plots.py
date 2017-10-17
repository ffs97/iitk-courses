import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

gd = np.load("gd.npy")
scd = np.load("scd.npy")[:-1, :]

sns.set_style("white")
sns.set_context("paper")

plt.plot(gd[:, 1], gd[:, 2] / 1000.0, label="GD")
plt.plot(scd[:, 1], scd[:, 2] / 1000.0, label="SCD")
plt.legend()
ax = plt.axes()
plt.title("Clock Time: GD vs SCD")
plt.ylabel("Primal Objective Value (x $10^3$)")
plt.xlabel("Wall-Clock Time (s)")
ax.yaxis.labelpad = 12
ax.xaxis.labelpad = 12
ax.title.set_position([0.5, 1.05])

plt.savefig("q4-plot-clocktime.pdf")

plt.clf()

plt.plot(gd[:10, 0] / 10**9, gd[:10, 2] / 1000.0, label="GD")
plt.plot(scd[:, 0] / 10**9, scd[:, 2] / 1000.0, label="SCD")
plt.legend()
ax = plt.axes()
plt.title("Theoretical Time: GD vs SCD")
plt.ylabel("Primal Objective Value (x $10^3$)")
plt.xlabel("Theoretical Time (x $10^9$)")
ax.yaxis.labelpad = 12
ax.xaxis.labelpad = 12
ax.title.set_position([0.5, 1.05])

plt.savefig("q4-plot-theoreticaltime.pdf")

plt.clf()

plt.plot(gd[:, 0] / 10**9, gd[:, 2] / 1000.0, label="GD")
plt.legend()
ax = plt.axes()
plt.title("Theoretical Time: GD")
plt.ylabel("Primal Objective Value (x $10^3$)")
plt.xlabel("Theoretical Time (x $10^9$)")
ax.yaxis.labelpad = 12
ax.xaxis.labelpad = 12
ax.title.set_position([0.5, 1.05])

plt.savefig("q4-plot-theoreticaltime-GD.pdf")

plt.clf()

plt.plot(scd[:, 0] / 10**7, scd[:, 2] / 1000.0, label="SCD")
plt.legend()
ax = plt.axes()
plt.title("Theoretical Time: SCD")
plt.ylabel("Primal Objective Value (x $10^3$)")
plt.xlabel("Theoretical Time (x $10^7$)")
ax.yaxis.labelpad = 12
ax.xaxis.labelpad = 12
ax.title.set_position([0.5, 1.05])

plt.savefig("q4-plot-theoreticaltime-SCD.pdf")
