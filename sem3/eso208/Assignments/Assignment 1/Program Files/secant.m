function [errors, x, x0, x1] = secant(eq)

syms f(x)
f(x) = sym(eq);

% Input prompt for required values
x0 = input('Enter the value of x0: ');
x1 = input('Enter the value of x1: ');
maxiters = input('Enter the Maximum Iterations: ');
maxerror = input('Enter the Maximum Error (%): ');

x = x1;
xp = x0;
yp = eval(f(x0));
y = eval(f(x));

errors = [];

% Looping through all iterations
for n = 1:maxiters
    t = x;
    x = x-y*(x-xp)/(y-yp);
    xp = t;
    yp = y;
    y = eval(f(x));
    
    e = (x - xp)/x*100;
    errors = [errors e];
    
    if (abs(e) < maxerror || y == 0)
        break;
    end
end

errors = errors(1:end-1);