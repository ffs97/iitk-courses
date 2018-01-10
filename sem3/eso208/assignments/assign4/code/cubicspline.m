function mat = cubicspline(points)

n = size(points(:, 1));
n = n(1) - 1;

mat = zeros(4*n, 4*n+1);
fun = zeros(4*n, 1);

cnt = 1;
cnt2 = 0;
for i=1:n
    mat(cnt, cnt2*4+1) = 1;
    mat(cnt, cnt2*4+2) = points(i, 1);
    mat(cnt, cnt2*4+3) = points(i, 1)^2;
    mat(cnt, cnt2*4+4) = points(i, 1)^3;
    mat(cnt, 4*n+1) = points(i, 2);
    cnt = cnt + 1;
    mat(cnt, cnt2*4+1) = 1;
    mat(cnt, cnt2*4+2) = points(i+1, 1);
    mat(cnt, cnt2*4+3) = points(i+1, 1)^2;
    mat(cnt, cnt2*4+4) = points(i+1, 1)^3;
    mat(cnt, 4*n+1) = points(i+1, 2);
    cnt = cnt + 1;
    cnt2 = cnt2 + 1;
end

for i=1:n-1
    mat(cnt, (i-1)*4 + 2) = 1;
    mat(cnt, i*4 + 2) = -1;
    mat(cnt, (i-1)*4 + 3) = 2*points(i+1);
    mat(cnt, i*4 + 3) = -2*points(i+1);
    mat(cnt, (i-1)*4 + 4) = 3*points(i+1)^2;
    mat(cnt, i*4 + 4) = -3*points(i+1)^2;
    cnt = cnt + 1;
end
for i=1:n-1
    mat(cnt, (i-1)*4 + 3) = 2;
    mat(cnt, i*4 + 3) = -2;
    mat(cnt, (i-1)*4 + 4) = 6*points(i+1);
    mat(cnt, i*4 + 4) = -6*points(i+1);
    cnt = cnt + 1;
end
end