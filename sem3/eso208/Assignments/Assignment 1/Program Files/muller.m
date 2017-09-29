function [x, x0, x2] = muller(eq)
    syms f(x)
    f(x) = sym(eq);

	% Input prompt for required values
    x0 = input('Enter a value x0: ');
    x1 = input('Enter a value x1: ');
    x2 = input('Enter a value x2: ');
    maxiters = input('Enter the maximum iterations you want: ');
    maxerror = input('Enter the maximum error you want: ');

    xp = [x2 x1 x0];
    yp = eval(f(xp));

    for n = 1:maxiters
        w = divdiff(xp(1), xp(2), yp(1), yp(2)) + divdiff(xp(1), xp(3), yp(1), yp(3)) - divdiff(xp(2), xp(3), yp(2), yp(3));
        t = sqrt(w^2 - 4*yp(1)*divdiff(xp(1), xp(3), divdiff(xp(1), xp(2), yp(1), yp(2)), divdiff(xp(2), xp(3), yp(2), yp(3))));
        if (w < 0)
            t = w - t;
        else
            t = w + t;
        end

        x = xp(1) - 2*yp(1)/t;

        e = (x - xp(1))/x*100;

        xp = [x xp(1) xp(2)];
        yp = [eval(f(xp(1))) yp(1) yp(2)];

        if (abs(e) < maxerror || yp(1) == 0)
            break;
        end
    end
end

function diff = divdiff(x1, x2, f1, f2)
    diff = (f1 - f2) / (x1 - x2);
end