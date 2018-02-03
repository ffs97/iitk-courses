import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import math

mu = 0
variance = 1
sigma = math.sqrt(variance)
x = np.linspace(- 3, 3, 100)
plt.plot(x, mlab.normpdf(x, mu, sigma), label="x | $\sigma^2$ (Gaussian)")
plt.plot(x, np.exp(-np.abs(x)) / 2, label="x | $\gamma$ (Laplace)")
ax = plt.gca()
ax.legend()
plt.show()
