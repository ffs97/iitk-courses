from __future__ import print_function

import sys
import math
import numpy as np
from datetime import datetime
from scipy.sparse import csr_matrix
from sklearn.datasets import load_svmlight_file

import warnings
from scipy.sparse import SparseEfficiencyWarning
warnings.simplefilter('ignore', SparseEfficiencyWarning)


def prettyprint(C_vals):
    print("   " + u"\u2014" * 197)
    print("  |", end="")
    for C in C_vals:
        print(" " * 9 + "C = %10f" % C + " " * 9 + "|", end="")
    print("")
    print("   " + u"\u2014" * 197)


def main():
    datafile = sys.argv[1]
    n_iter = int(sys.argv[2])
    spacing = int(sys.argv[3])
	
    data = load_svmlight_file(datafile)

    X = data[0]
    Y = data[1]

    n, d = X.get_shape()
    
    indices = np.arange(n)
    np.random.shuffle(indices)

    X = csr_matrix(X)
    X = X[indices, :]
    
    Y = 2*(Y - 1.5)
    Y = Y.astype(int)
    Y = csr_matrix(Y)
    Y = Y[:, indices]

    n_Xtr = math.floor(83.0 * n / 100.0)

    Xtr = X[:n_Xtr, :]
    Xtr_t = Xtr.transpose()
    Ytr = Y[:, :n_Xtr]
    Ytr_t = Ytr.transpose()

    Xts = X[n_Xtr:]
    Yts_t = Y[:, n_Xtr:].transpose()

    C_vals = np.array([1, 8, 16, 32, 64, 100]) / float(n)
    c = len(C_vals)
    
    w = csr_matrix((d, c))
    w_bar = csr_matrix((d, c))

    XY = Xtr.multiply(Ytr_t).tocsr()
    
    t_start = datetime.now()

    print("\nObjective Function Values:", end="\n\n")
    prettyprint(C_vals * n)

    for t in range(n_iter):
        r = XY * w
        vals = Ytr_t.multiply(r < 1)
        
        g = Xtr_t * vals
        g = w - g

        eta = C_vals / math.sqrt(t+1)
        
        w_old = w
        w = w - g.multiply(eta)

        w_bar_old = w_bar
        w_bar = (w_bar * t + w) / (t + 1)

        if t % spacing == 0:
            w_norms = np.array(w_old.multiply(w_old).sum(0))[0]
            w_bar_norms = np.array(w_bar_old.multiply(w_bar_old).sum(0))[0]

            r_bar = XY * w_bar_old
            
            print("  |  ", end="  ")
            for i in range(0, c):
                x = r[:, i].toarray()
                x_bar = r_bar[:, i].toarray()

                obj_val = w_norms[i] / 2.0 + np.sum(1 - x[x < 1])
                obj_bar_val = w_bar_norms[i] / 2.0 + np.sum(1 - x_bar[x_bar < 1])
                
                print("%.4f  %.4f" % (obj_val, obj_bar_val), end="    |    ")

            print("")

    print("   " + u"\u2014" * 197, end="\n\n\n")

    pred = (Xts*w).multiply(Yts_t) > 0
    pred = np.sum(pred.toarray(), 0) / float(Xts.get_shape()[0]) * 100

    pred_bar = (Xts*w_bar).multiply(Yts_t) > 0
    pred_bar = np.sum(pred_bar.toarray(), 0) / float(Xts.get_shape()[0]) * 100

    print("Prediction Accuracy:", end="\n\n")
    prettyprint(C_vals * n)

    print("  |  ", end="       ")
    for i in range(0, c):
        print("%.2f%%  %.2f%%" % (pred[i], pred_bar[i]), end="         |         ")
    print("\n   " + u"\u2014" * 197, end="\n\n")


if __name__ == '__main__':
    main()
