function [errors, x, l0, u0] = bisection(eq)

syms f(x)
f(x) = sym(eq);

% Input prompt for required values
l0 = input('Enter the lower value (l): ');
u0 = input('Enter the upper value (u): ');
maxiters = input('Enter the Maximum Iterations: ');
maxerror = input('Enter the Maximum Error (%): ');

l = l0;
u = u0;
x0 = l;
errors = [];

% Looping through all iterations
for n = 1:maxiters
    x = (l+u)/2;
    y = eval(f(x));

    fl = eval(f(l));
    
    e = (x - x0)/x*100;
    errors = [errors e];
    
    if y*fl == 0
        break
    elseif y*fl > 0
        l = x;
    else
        u = x;
    end
    
    x0 = x;
    
    if (abs(e) < maxerror || y == 0)
        break;
    end
end