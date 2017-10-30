clear;
clc;

% Required if using octave
% pkg install -forge io
% pkg install -forge statistics
pkg load statistics

mu = [4 4];
SIGMA = [1 0.8; 0.8 1];

function ret = p(z, mu, SIGMA)
	ret = mvnpdf(z, mu, SIGMA);
end

function ret = q(z, mu, SIGMA)
	ret = mvnpdf(z, mu, SIGMA);
end

var = 100;

T_samples = 10000;

samples = [];

z = [0 0];
sigma = [var 0; 0 var];

n_samples = 0;
n_total = 0;

while (n_samples < T_samples)
	zt = mvnrnd(z, sigma);
	pr = min(1, (p(zt, mu, SIGMA) * q(z, zt, sigma)) / (p(z, mu, SIGMA) * q(zt, z, sigma)));

	if (pr >= rand)
		n_samples += 1;
		z = zt;
		samples = [samples; [z p(z, mu, SIGMA)]];
	end

	n_total += 1;
end

save samples.mat samples
sprintf("Accept Rate %f", n_samples / n_total)
