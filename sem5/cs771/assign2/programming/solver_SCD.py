import sys
import math
import random
import numpy as np
from datetime import datetime
from scipy.sparse import csr_matrix
from sklearn.datasets import load_svmlight_file

import warnings
from scipy.sparse import SparseEfficiencyWarning
warnings.simplefilter('ignore', SparseEfficiencyWarning)


def main():
    traindatafile = sys.argv[1]
    n_iter = int(sys.argv[2])
    spacing = int(sys.argv[3])
	
    tr_data = load_svmlight_file(traindatafile)

    Xtr = tr_data[0]
    Ytr = tr_data[1]
    
    n, d = Xtr.get_shape()
    
    Ytr = 2*(Ytr - 1.5)
    Ytr = Ytr.astype(int)

    Xtr = csr_matrix(Xtr)
    Ytr = csr_matrix(Ytr)
    Ytr_t = Ytr.transpose()

    w = csr_matrix((d, 1))
    d_alpha = np.zeros(n)

    n_log = int(math.ceil(float(n_iter)/spacing)) + 1
    
    time_elapsed = np.zeros(n_log)
    tick_vals = np.zeros(n_log)
    obj_val = np.zeros(n_log)

    tick = 0
    
    ttot = 0.0
    t_start = datetime.now()

    XX = np.array(Xtr.multiply(Xtr).sum(1)).reshape(n)
    XY = Xtr.multiply(Ytr_t).tocsr()

    for t in range(n_iter):	
        i_rand = random.randint(0,n-1)
	
	d_alpha_old = d_alpha[i_rand]
        d_alpha[i_rand] = max(0, min(1, d_alpha_old - ((XY[i_rand, :] * w)[0, 0] - 1) / XX[i_rand]))

        w_old = w
        w = w + (d_alpha[i_rand] - d_alpha_old)*XY[i_rand, :].transpose();
	
	if t % spacing == 0:
            t_now = datetime.now();
            delta = t_now - t_start;
            time_elapsed[tick] = ttot + delta.total_seconds();
            ttot = time_elapsed[tick];
            tick_vals[tick] = tick;

            r = XY * w_old
            indices = (r < 1).nonzero()[0]
            r = r.toarray()

            obj_val[tick] = w_old.multiply(w_old).sum() / 2.0 + np.sum(1 - r[indices])
            print("%11f\t%15f" % (ttot, obj_val[tick]))

            tick = tick + 1;
            t_start = datetime.now();
		
    t_now = datetime.now();
    delta = t_now - t_start;
    time_elapsed[tick] = ttot + delta.total_seconds();
    ttot = time_elapsed[tick];
    tick_vals[tick] = tick;

    r = XY * w
    indices = (r < 1).nonzero()[0]
    r = r.toarray()

    obj_val[tick] = w.multiply(w).sum() / 2.0 + np.sum(1 - r[indices])
    print("%11f\t%15f" % (ttot, obj_val[tick]))

    w_final = w.toarray().reshape(d);
    np.save("model_SCD.npy", w_final);

    data = np.array([tick_vals*d*spacing, time_elapsed, obj_val]).transpose()
    np.save("scd.npy", data)


if __name__ == '__main__':
    main()
