clear
clc
str = input('Please Enter the Derivative of y (i.e. dy/dx) (e.g. 5*exp(-100*(x-2)^2) - y/2):\n', 's');
f = inline(str, 'x', 'y');

x0 = input('Please Enter Initial Point (x0): ');
y0 = input('Please Enter Initial Function Value (y0): ');
xf = input('Please Enter Final Point (xf): ');
h = input('Please Enter Interval Size (h): ');

method = input('Please Enter the Method you want to use: \n(1) Eulers Method \n(2) Midpoint Method \n(3) Order 4 Runge-Kutta \n(4) Runge-Kutta Fehlberg (RK45) method \n');
switch(method)
    case 1
        euler(f, x0, y0, xf, h);
    case 2
        midpoint(f, x0, y0, xf, h);
    case 3
        rk4(f, x0, y0, xf, h);
    case 4
        rk45(f, x0, y0, xf);
end