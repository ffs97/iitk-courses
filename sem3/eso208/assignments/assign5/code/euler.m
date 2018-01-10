function euler(f, x0, y0, xf, h)

x = [x0:h:xf];
y = [y0];

n = length(x);
for i=1:n-1
    y = [y (y(i)+h*f(x(i), y(i)))];    
end

figure, plot(x, y, '-o'), title('Eulers Method'), grid;
printtofile(x, y, n, 'y_euler' , 'euler.txt');