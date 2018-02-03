import sys
import numpy as np

import matplotlib as mpl
from matplotlib import pyplot as plt
from matplotlib import rcParams

rcParams['axes.titlepad'] = 20
rcParams['axes.titlesize'] = "medium"
rcParams['axes.edgecolor'] = "red"
rcParams['axes.spines.right'] = rcParams['axes.spines.top'] = False
rcParams['xtick.labelsize'] = "small"
rcParams['xtick.major.pad'] = 10
rcParams['ytick.labelsize'] = "small"
rcParams['ytick.major.pad'] = 10
rcParams['axes.formatter.use_mathtext'] = True
rcParams['axes.labelpad'] = 10
rcParams['font.sans-serif'][0] = 'Ubuntu Mono'
rcParams['figure.dpi'] = 240.0


def get_data():
    in_file = "data.txt"
    if len(sys.argv) > 1:
        in_file = sys.argv[1]

    with open(in_file, "r") as f:
        X = list()
        y = list()

        for line in f:
            t = [float(x) for x in line.split(",")]
            X.append([t[0], t[1]])
            y.append(t[2])

    return np.array(X), np.array(y)


def get_posterior_paramters(X, y, lmbda, beta):
    var = np.linalg.inv(
        beta * np.transpose(X).dot(X) + lmbda * np.eye(X.shape[1])
    )
    mu = beta * var.dot(np.transpose(X).dot(y))

    return mu, var


def main():
    beta, lmbda = 25, 2

    X_full, y_full = get_data()

    k = 500
    x_linspace = np.transpose([np.ones(k), np.linspace(-1.0, 1.0, num=k)])

    for i, n in enumerate([1, 2, 5, 10, 20]):
        X = X_full[:n]
        y = y_full[:n]

        mu, var = get_posterior_paramters(X, y, lmbda, beta)

        print "N =", n
        print "\tMean:"
        for coord in mu:
            if coord < 0:
                print "\t\t", "%.3f" % coord
            else:
                print "\t\t", "%.4f" % coord

        print "\tCovariance:"
        for row in var:
            print "\t\t",
            for col in row:
                if col < 0:
                    print "%.3f" % col, "\t",
                else:
                    print "%.4f" % col, "\t",
            print ""

        pts = np.random.multivariate_normal(mu, var, 10000)

        plt.scatter(pts[:, 0], pts[:, 1], s=1, c="blue", marker=".")
        plt.scatter(mu[0], mu[1], s=100, c="red", marker="+", label="mean")
        plt.title("Sampling $w$ from posterior $w | y, X$")
        plt.legend()
        plt.savefig(
            "../includes/plots/q4b/posterior-samples-%s.png" % str(i + 1), pad_inches=0.1
        )
        plt.clf()

        pts = np.random.multivariate_normal(mu, var, 10)
        plt.scatter(X[:, 1], y, label="Data Points", s=10)
        for w in pts:
            plt.plot(x_linspace[:, 1], x_linspace.dot(w))
        plt.legend()
        plt.savefig(
            "../includes/plots/q4c/predictive-models-%s.png" % str(i + 1), pad_inches=0.1
        )
        plt.clf()


if __name__ == "__main__":
    main()
