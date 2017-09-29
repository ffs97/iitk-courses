function rk45(f, x0, y0, xf)

hmax = input('Please Enter the Maximum Interval Size (hmax) : ');
alpha = input('Please Enter Constant for Refining h (Alpha) : ');
tol = input('Please Enter the Error Tolerance (tol) : ');

x = x0;
y = y0;

xt = x0;
yt = y0;

while (xt < xf)
    flag = 1;
    h = hmax;
    while (1)
        k1 = f(xt, yt);
        k2 = f(xt + 0.2*h, yt + 0.2*h*k1);
        k3 = f(xt + 0.3*h, yt + h*(0.075*k1 + 0.225*k2));
        k4 = f(xt + 0.6*h, yt + h*(0.3*k1 - 0.9*k2 + 1.2*k3));
        k5 = f(xt + h, yt + h*(-(11/54)*k1 + 2.5*k2 - (70/27)*k3 + (35/27)*k4));
        k6 = f(xt + 0.875*h, yt + h*((1631/55296)*k1 + (175/512)*k2 + (575/13824)*k3 + (44275/110592)*k4 + (253/4096)*k5));

        y4 = yt + h*((37/378)*k1 + (250/621)*k3 + (125/594)*k4 + (512/1771)*k6);
        y5 = yt + h*((2825/27648)*k1 + (18575/48384)*k3 + (13525/55296)*k4 + (277/14336)*k5 + (1/4)*k6);

        err = abs(y5 - y4);
        if (err < tol && flag == 0)
            break;
        elseif (err > tol)
            flag = 0;
        end
        h = min(hmax, h*(tol/err)^alpha);
    end
    
    if (xt + h > xf)
        h = xf - xt;
        k1 = f(xt, yt);
        k2 = f(xt + 0.2*h, yt + 0.2*h*k1);
        k3 = f(xt + 0.3*h, yt + h*(0.075*k1 + 0.225*k2));
        k4 = f(xt + 0.6*h, yt + h*(0.3*k1 - 0.9*k2 + 1.2*k3));
        k5 = f(xt + h, yt + h*(-(11/54)*k1 + 2.5*k2 - (70/27)*k3 + (35/27)*k4));
        k6 = f(xt + 0.875*h, yt + h*((1631/55296)*k1 + (175/512)*k2 + (575/13824)*k3 + (44275/110592)*k4 + (253/4096)*k5));
        y5 = yt + h*((2825/27648)*k1 + (18575/48384)*k3 + (13525/55296)*k4 + (277/14336)*k5 + (1/4)*k6);
    end
    
    xt = xt + h;
    yt = y5;
    x = [x xt];
    y = [y yt];
end

figure, plot(x, y, '-o'), title('Runge-Kutte-Fehlberg Method'), grid;
printtofile(x, y, length(x), 'y_RK45' , 'rk45.txt');