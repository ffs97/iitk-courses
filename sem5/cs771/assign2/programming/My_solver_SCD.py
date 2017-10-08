import numpy as np
from scipy.sparse import csr_matrix
import sys
from sklearn.datasets import load_svmlight_file
import random
from datetime import datetime
import math
import matplotlib.pyplot as plt


def predict(xtr, ytr, w): 
	InnerProd= (w*(xtr.transpose()))
	DotProd= np.multiply(ytr.transpose(),InnerProd)
	HingeLoss=1 - DotProd
	NonZeroTerms = (DotProd<1).transpose();
	IndexNonZero = (NonZeroTerms.nonzero())[0]
	HingeLoss= (np.ones(len(IndexNonZero)).transpose())-DotProd[:,IndexNonZero]
	sum1= np.sum(HingeLoss)
	obj_validation=0.5*np.inner(w,w)+sum1


	InnerProd= (w*(xtr.transpose()))
	DotProd= np.multiply(ytr.transpose(),InnerProd)
	corr= (DotProd>0).sum()
	incorr= (DotProd<0).sum()
	
	 
	print("Accuracy =", corr/500); 
	return obj_validation

def main():

	# Get training file name from the command line
	traindatafile = sys.argv[1];
	# For how many iterations do we wish to execute SCD?
	n_iter = int(sys.argv[2]);
	# After how many iterations do we want to timestamp?
	spacing = int(sys.argv[3]);
	
	# The training file is in libSVM format
	tr_data = load_svmlight_file(traindatafile);

	Xtr = tr_data[0]; # Training features in sparse format
	Ytr = tr_data[1]; # Training labels
	
	# We have n data points each in d-dimensions
	n, d = Xtr.get_shape();
	
	# The labels are named 1 and 2 in the data set. Convert them to our standard -1 and 1 labels
	Ytr = 2*(Ytr - 1.5);
	Ytr = Ytr.astype(int);
	
	# Optional: densify the features matrix.
	# Warning: will slow down computations
	Xtr = Xtr.toarray();
	Xtr=np.array(Xtr)
	
	# Initialize model
	# For dual SCD, you will need to maintain d_alpha and w
	# Note: if you have densified the Xt matrix then you can initialize w as a NumPy array
	w = csr_matrix((1, d));
	d_alpha = np.zeros(n);

	arr1=np.asarray(Xtr);#converting to a numpy array
	arr2=np.asarray(Ytr);
	s=np.arange(arr1.shape[0]);
	np.random.shuffle(s);#for shuffling the rows
	arr1=arr1[s];
	arr2=arr2[s];
	X_train=arr1[0:250000];
	#X_train=Xtr[0:300000]
	X_valid=arr1[250000:300000]; 
	Y_train=arr2[0:250000];
	#Y_train=Ytr[0:300000]
	Y_valid=arr2[250000:300000];
	Y_train=np.array(Y_train);
	Y_valid=np.array(Y_valid);
	
	# We will take a timestamp after every "spacing" iterations
        n_log = int(math.ceil(n_iter/spacing))
	time_elapsed = np.zeros(n_log);
	tick_vals = np.zeros(n_log);
	obj_val = np.zeros(n_log);
	
	
	tick = 0;
	
	ttot = 0.0;
	t_start = datetime.now();
	X_sum= (np.multiply(X_train,X_train)).sum(1) # Computes element wise multiplication
	#X_sum=(X_train.multiply(X_train)).sum(1);  
	
	for t in range(n_iter):
		i_rand = random.randint(0,250000-1);
		qii= X_sum[i_rand]
		X_c=X_train[i_rand,:].reshape(54,1)
		numerator=Y_train[i_rand]*w*X_c - csr_matrix(1)
		d_alpha_old = d_alpha[i_rand];
		d_alpha[i_rand] = min(max(d_alpha_old - (numerator/qii),0),1);
		print(d_alpha[i_rand], d_alpha_old, d_alpha[i_rand] - d_alpha_old)
		
		# Update the model - takes only O(d) time!
		w = w + ((d_alpha[i_rand] - d_alpha_old)*Y_train[i_rand])*(X_c.transpose());
		print(w)
		
		# Take a snapshot after every few iterations
		# Take snapshots after every spacing = 5000 or so SCD iterations since they are fast
		if t%spacing == 0:
			# Stop the timer - we want to take a snapshot
			t_now = datetime.now();
			delta = t_now - t_start;
			time_elapsed[tick] = ttot + delta.total_seconds();
			ttot = time_elapsed[tick];
			tick_vals[tick] = tick;

			InnerProd= (w*(X_train.transpose()))
			DotProd= np.multiply(Y_train.transpose(),InnerProd)
			HingeLoss=1 - DotProd;
			NonZeroTerms = (DotProd<1).transpose();
			IndexNonZero = (NonZeroTerms.nonzero())[0]
			Y_relevant= Y_train[IndexNonZero]
			X_relevant= X_train[IndexNonZero,:]
			Sum_relevant=np.matmul(Y_relevant.transpose(),X_relevant)
			HingeLoss= (np.ones(len(IndexNonZero)).transpose())-DotProd[:,IndexNonZero]

			obj_val[tick] =  0.5 * np.inner(w,w) + np.sum(HingeLoss); # Calculate the objective value f(w) for the current model w^t
			print(obj_val[tick])
			tick = tick+1;
			# Start the timer again - training time!
			t_start = datetime.now();


	obj_val1=predict(X_valid,Y_valid,w);
	print("For w model" ,obj_val1);
	plt.plot(obj_val)
	plt.ylabel('objective_w')
	plt.xlabel('no. of iterations')
	plt.show()
			
	w_final = w.toarray();
	np.save("model_SCD.npy", w_final);
		
if __name__ == '__main__':
    main()
