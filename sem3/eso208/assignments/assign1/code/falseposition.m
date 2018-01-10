function [errors, x, x0, u] = falseposition(eq)

syms f(x)
f(x) = sym(eq);

% Input prompt for required values
x0 = input('Enter the first value (x0): ');
u = input('Enter the upper value (u): ');
maxiters = input('Enter the Maximum Iterations: ');
maxerror = input('Enter the Maximum Error (%): ');

xp = x0;
fu = eval(f(u));
y = eval(f(x0));

errors = [];

% Looping through all iterations
for n = 1:maxiters
    x = u - fu * (u - xp) / (fu - y);
    y = eval(f(x));
    
    e = (x - xp)/x*100;
    errors = [errors e];
    
    if (abs(e) < maxerror || y == 0)
        break;
    end
    
	if (y * fu > 0)
		fu = y;
		u = x;
		x = xp;
		y = eval(f(x));
	else
		xp = x;
	end
end