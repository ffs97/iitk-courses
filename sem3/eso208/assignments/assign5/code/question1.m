clear
clc

str = input('Please Enter the function to be integrated in x (e.g. 13*(x-x^2)*exp(-1.5*x)):\n', 's');
f = inline(str, 'x');

a = input('Please enter the lower limit of integration: ');
b = input('Please enter the upper limit of integration: ');

err = input('Please enter the maximum allowed error: ');

global points;
points = [];

x = a:1:b;
figure
ezplot(f, x), title('Adaptive Quadrature (Simpson 1/3 Rule)'), grid, hold on;

integral = adaptive(f, a, b, err, simpson(f, a, b));
points = unique(points);

fprintf('\nI = %.4f\n', integral);
fprintf('n = %d\n', length(points)-1);