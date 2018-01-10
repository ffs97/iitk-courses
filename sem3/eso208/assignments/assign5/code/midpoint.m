function midpoint(f, x0, y0, xf, h)

x = [x0:h:xf];
y = [y0];

n = length(x);
for i=1:n-1
    y1 = y(i) + 0.5*h*f(x(i), y(i));
    y = [y (y(i) + h*f(x(i) + h/2, y1))];
end

figure, plot(x, y, '-o'), title('Midpoint Method'), grid;
printtofile(x, y, n, 'y_midpoint' , 'midpoint.txt');