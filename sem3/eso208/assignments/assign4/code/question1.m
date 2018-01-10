points = importdata('input1.txt', ' ');
n = size(points, 1);

d = input('Please Enter the desired degree of the polynomial: ') + 1;

vals = ones(d, n);

for i=2:d
    for j=1:n
        vals(i, j) = points(j, 1)^(i-1);
    end
end

det = zeros(d, d);

for i=1:d
    for j=1:d
        for k=1:n
            det(i, j) = det(i, j) + vals(i, k)*vals(j, k);
        end
    end
end

y = zeros(d, 1);
for i=1:d
    for j=1:n
        y(i) = y(i) + vals(i, j)*points(j, 2);
    end
end

coeff = inv(det)*y;

out = fopen('output.txt', 'w');
fprintf(out, 'Coefficients: \n');
for i = 1:d
    fprintf(out, '\t%.4f\n', coeff(i));
end

coeff = flipud(coeff);
x = [0:0.01:1];
y = polyval(coeff, x);
plot(x, y);
hold on;
plot(points(:, 1), points(:, 2), 's');

errors = points(:, 2) - polyval(coeff, points(:, 1));
ym = 0;
for i=1:n
    ym = ym + points(i, 2);
end
ym = ym / n;
errors2 = points(:, 2) - ym;
s0 = 0;
s = 0;
for i=1:n
    s0 = s0 + errors2(i)^2;
    s = s + errors(i)^2;
end

fprintf(out, '\nR-Square\n\t%.4f', (1 - s/s0));