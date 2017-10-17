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


def validate(X, Y, n_iter=5000, spacing=100):
    n, d = X.get_shape()
    
    indices = np.arange(n)
    np.random.shuffle(indices)

    X = csr_matrix(X)
    X = X[indices, :]
    
    Y = 2*(Y - 1.5)
    Y = Y.astype(int)
    Y = csr_matrix(Y)
    Y = Y[:, indices]

    n_Xtr = math.floor(80.0 * n / 100.0)

    Xtr = X[:n_Xtr, :]
    Xtr_t = Xtr.transpose()
    Ytr = Y[:, :n_Xtr]
    Ytr_t = Ytr.transpose()

    Xts = X[n_Xtr:]
    Yts_t = Y[:, n_Xtr:].transpose()

    C_vals = np.array([1, 8, 16, 32, 64, 100]) / float(n_Xtr)
    c = len(C_vals)
    
    w = csr_matrix((d, c))
    w_bar = csr_matrix((d, c))

    XY = Xtr.multiply(Ytr_t).tocsr()
    
    t_start = datetime.now()

    print("\nObjective Function Values:", end="\n\n")
    prettyprint(C_vals * n_Xtr)

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
    prettyprint(C_vals * n_Xtr)

    print("  |  ", end="       ")
    for i in range(0, c):
        print("%.2f%%  %.2f%%" % (pred[i], pred_bar[i]), end="         |         ")
    print("\n   " + u"\u2014" * 197, end="\n\n")


def main():
    traindatafile = sys.argv[1]
    n_iter = int(sys.argv[2])
    spacing = int(sys.argv[3])
	
    tr_data = load_svmlight_file(traindatafile)

    Xtr = tr_data[0]
    Ytr = tr_data[1]
	
    ## Held out validation to find out a decent value for C (used in eta)
    # validate(Xtr, Ytr)
    ## Achieved value of C = 32 with h(n) = C / n
    # return

    n, d = Xtr.get_shape()
    
    Ytr = 2*(Ytr - 1.5)
    Ytr = Ytr.astype(int)
    Ytr = csr_matrix(Ytr)
    Ytr_t = Ytr.transpose(None, True)
    
    Xtr = csr_matrix(Xtr)

    w = csr_matrix((d, 1))
    w_bar = csr_matrix((d, 1))
    
    # n_log = int(math.ceil(float(n_iter)/spacing)) + 1
    
    # time_elapsed = np.zeros(n_log)
    # tick_vals = np.zeros(n_log)
    # obj_val = np.zeros(n_log)
    
    tick = 0
    
    ttot = 0.0
    t_start = datetime.now()

    XY = Xtr.multiply(Ytr_t).tocsr()
    C = 32.0 / n

    for t in range(n_iter):
        r = XY * w
        indices = (r < 1).nonzero()[0]

        g = XY[indices, :].sum(0)
        g = w - g.transpose()
       
        eta = C / math.sqrt(t + 1)

        w_old = w

        w = w - eta * g
        w = csr_matrix(w)

        #w_bar = (w_bar * t + w) / (t + 1)
        
        # if t % spacing == 0:
            # t_now = datetime.now()
            # delta = t_now - t_start
            # time_elapsed[tick] = ttot + delta.total_seconds()
            # ttot = time_elapsed[tick]
            # tick_vals[tick] = tick

            # r = r.toarray()
            # obj_val[tick] = w_old.multiply(w_old).sum() / 2.0 + np.sum(1 - r[indices])
            # print("%14f\t%14f" % (ttot, obj_val[tick]))

            # tick = tick+1
            # t_start = datetime.now()
    
    # t_now = datetime.now()
    # delta = t_now - t_start
    # time_elapsed[tick] = ttot + delta.total_seconds()
    # ttot = time_elapsed[tick]
    # tick_vals[tick] = tick

    # r = XY * w
    # r = r.toarray()
    # obj_val[tick] = w.multiply(w).sum() / 2.0 + np.sum(1 - r[r < 1])
    # print("%14f\t%14f" % (ttot, obj_val[tick]))

    w_final = w.toarray().reshape(d)
    np.save("model_GD.npy", w_final)

    # data = np.array([tick_vals*n*d*spacing, time_elapsed, obj_val]).transpose()
    # np.save("gd.npy", data)

    # r = np.sum(XY * w, 1)
    # print("Accuracy: ", np.sum(r > 0) * 100.0 / n)


if __name__ == '__main__':
    main()
