clear;
clc;

load('20newsgroups');
[N, M] = size(X);

a_u = 1;b_u = 1e-6;
a_v = 1;b_v = 1e-6;

K = 20;

U = gamrnd(a_u,1/b_u,N,K);
V = gamrnd(a_v,1/b_v,M,K);

max_iters = 1000;
burnin = 500;

[I, J, vals] = find(X);

num_elem = length(I);
rp = randperm(num_elem);

I_train = I(rp(1:round(0.8*num_elem)));
J_train = J(rp(1:round(0.8*num_elem)));

J_test = J(rp(round(0.8*num_elem)+1:end));
I_test = I(rp(round(0.8*num_elem)+1:end));

vals_train = vals(rp(1:round(0.8*num_elem)));
vals_test = vals(rp(round(0.8*num_elem)+1:end));

[unique_I_train, ~, I_train_C] = unique(I_train);
[unique_J_train, ~, J_train_C] = unique(J_train);

count = 0;
predicted_train_avg = zeros(length(I_train), 1);
predicted_test_avg = zeros(length(I_test), 1);

gibbs_test_errors = [];
gibbs_test_errors_avg = [];

for iters=1:max_iters
	Xk_p = U(I_train, :) .* V(J_train, :);
	Xk_p = bsxfun(@rdivide, Xk_p, sum(Xk_p, 2));

	Xk = mnrnd(vals_train, Xk_p);

	Xnk = [];
	for k=1:K
		Xnk = [Xnk accumarray(I_train_C, Xk(:, k))];
	end

	A_u = zeros([N K]) + a_u;
	A_u(unique_I_train, :) = A_u(unique_I_train, :) + Xnk;
	
	B_u = zeros([N K]) + b_u + sum(V, 1);

	U = gamrnd(A_u, 1 ./ B_u);

	Xmk = [];
	for k=1:K
		Xmk = [Xmk accumarray(J_train_C, Xk(:, k))];
	end

	A_v = zeros([M K]) + a_v;
	A_v(unique_J_train, :) = A_v(unique_J_train, :) + Xmk;
	
	B_v = zeros([M K]) + b_v + sum(U, 1);

	V = gamrnd(A_v, 1./B_v);

	predicted_train = sum(U(I_train, :) .* V(J_train, :), 2);
	predicted_test = sum(U(I_test, :) .* V(J_test, :), 2);

    mae_train = mean(abs(predicted_train - vals_train));
    mae_test = mean(abs(predicted_test - vals_test));

	gibbs_test_errors = [gibbs_test_errors mae_test];

    fprintf('Done with iteration %d, MAE_train = %f, MAE_test = %f\n',iters,mae_train,mae_test);
    
    if iters > burnin
		predicted_train_avg = predicted_train_avg .* count;
		predicted_train_avg = predicted_train_avg + predicted_train;

		predicted_test_avg = predicted_test_avg .* count;
		predicted_test_avg = predicted_test_avg + predicted_test;

		count = count + 1;
		predicted_train_avg = predicted_train_avg ./ count;
		predicted_test_avg = predicted_test_avg ./ count;

	    mae_train_avg = mean(abs(predicted_train - vals_train));
   	 	mae_test_avg = mean(abs(predicted_test - vals_test));

		gibbs_test_errors_avg = [gibbs_test_errors_avg mae_test_avg];
        fprintf('With Posterior Averaging, MAE_train_avg = %f, MAE_test_avg = %f\n',mae_train_avg,mae_test_avg);
    end
end

printtopics(V);

f = figure();
plot(gibbs_test_errors);
xlabel('Number of Iterations');
ylabel('MAE');
title('Reconstruction Error for Gibbs Sampling vs Number of Iterations');
saveas(f, 'gibbs.png');

f = figure();
plot(gibbs_test_errors_avg);
xlabel('Number of Iterations');
ylabel('MAE');
title('Reconstruction Error for Gibbs Sampling with Monte Carlo Sampling vs Number of Iterations');
saveas(f, 'gibbs_mc.png');
