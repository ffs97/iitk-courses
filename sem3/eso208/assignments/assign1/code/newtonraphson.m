function [errors, x, l, u] = newtonraphson(eq)

syms f(x)
f(x) = sym(eq);

% Input prompt for required values
x0 = input('Enter the starting value (x0): ');
maxiters = input('Enter the Maximum Iterations: ');
maxerror = input('Enter the Maximum Error (%): ');

x = x0;
xp = x0;
y = eval(f(x0));

errors = [];

% Looping through all iterations
for n = 1:maxiters
    x = x - y/eval(f1(x));
    y = eval(f(x));
    
    e = (x - xp)/x*100;
    errors = [errors e];
    
    if (abs(e) < maxerror || y == 0)
        break;
    end
    xp = x;
end

l = x0-0.5;
u = x0+0.5;