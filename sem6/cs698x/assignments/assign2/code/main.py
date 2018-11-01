import os
import math

import numpy as np
import matplotlib as mpl

from scipy.special import gamma
from matplotlib import pyplot as plt

mpl.rc_file_defaults()


def lgamma(x):
    g = np.zeros(x.shape)
    for i, el in enumerate(x):
        el = el - 1
        g[i] = math.sqrt(abs(el)) * (el ** el) * math.exp(-el)
    return math.sqrt(2 * math.pi) * g

a = np.arange(1.1, 5, 0.1)

if not os.path.exists("plots"):
    os.makedirs("plots")
    
plt.plot(a, lgamma(a), 'b--', label="Approximate")
plt.plot(a, gamma(a), 'r.', label="Actual")
plt.title("Comparision Plot: Laplace Approximation of Gamma Function")
plt.legend()
plt.savefig("plots/q3.png")
plt.show()
