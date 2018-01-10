function linearspline(points, x)

n = size(points(:, 1));
n = n(1) - 1;

mat = zeros(2*n, 2*n);
fun = zeros(2*n, 1);

cnt = 1;
cnt2 = 0;
for i=1:n
    mat(cnt, cnt2*2+1) = 1;
    mat(cnt, cnt2*2+2) = points(i, 1);
    fun(cnt) = points(i, 2);
    cnt = cnt + 1;
    mat(cnt, cnt2*2+1) = 1;
    mat(cnt, cnt2*2+2) = points(i+1, 1);
    fun(cnt) = points(i+1, 2);
    cnt = cnt + 1;
    cnt2 = cnt2 + 1;
end

coeff = inv(mat)*fun;
coeff = flipud(reshape(coeff, [2, n]));

file = fopen('output.txt', 'w');
fprintf(file, 'Linear Spline\n');
for i=1:size(x)
    for j=1:n
        if (x(i) >= points(j, 1) && x(i) <= points(j+1, 1))
            fprintf(file, '%.4f\t%.4f\n', x(i), polyval(coeff(:, j), x(i)));
        end
    end
end
    
y = [];
p1 = [];
for i=1:n
    p = [points(i, 1):0.01:points(i+1,1)];
    p1 = [p1 p];
    y = [y polyval(coeff(:, i), p)];
end
plot(p1, y);

end