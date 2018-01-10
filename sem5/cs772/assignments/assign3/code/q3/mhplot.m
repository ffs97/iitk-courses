pkg load statistics

load samples1.mat

x1 = 0:0.1:8;
x2 = 0:0.1:8;

[X1 X2] = meshgrid(x1, x2);

mu = [4 4];
sigma = [1 0.8; 0.8 1];

F = mvnpdf([X1(:) X2(:)], mu, sigma);

set (0, "defaultaxesfontname", "Helvetica") % this is the line to add BEFORE plotting
hold on;
f = figure();
plot(samples(:, 1), samples(:, 2), 'ro', 'MarkerSize', 1);
title('MH Sampling Samples with Variance = 0.01');

load samples2.mat

x1 = 0:0.1:8;
x2 = 0:0.1:8;

[X1 X2] = meshgrid(x1, x2);

mu = [4 4];
sigma = [1 0.8; 0.8 1];

F = mvnpdf([X1(:) X2(:)], mu, sigma);

f = figure()
plot(samples(:, 1), samples(:, 2), 'ro', 'MarkerSize', 1);
title('MH Sampling Samples with Variance = 1')

load samples3.mat

x1 = 0:0.1:8;
x2 = 0:0.1:8;

[X1 X2] = meshgrid(x1, x2);

mu = [4 4];
sigma = [1 0.8; 0.8 1];

F = mvnpdf([X1(:) X2(:)], mu, sigma);

f = figure()
plot(samples(:, 1), samples(:, 2), 'ro', 'MarkerSize', 1);
title('MH Sampling Samples with Variance = 100')
