function rk4(f, x0, y0, xf, h)

x = [x0:h:xf];
y = [y0];

n = length(x);
for i=1:n-1
    k1 = f(x(i), y(i));
    k2 = f(x(i) + 0.5*h, y(i) + 0.5*h*k1);
    k3 = f(x(i) + 0.5*h, y(i) + 0.5*h*k2);
    k4 = f(x(i) + h, y(i) + h*k3);
    
    y = [y (y(i) + h*(k1 + 2*k2 + 2*k3 + k4)/6)];
end

figure, plot(x, y, '-o'), title('Order 4 Runge-Kutta Method'), grid;
printtofile(x, y, n, 'y_RK4' , 'rk4.txt');