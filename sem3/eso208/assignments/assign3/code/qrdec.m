function qrdec(A, n, iters, maxerror)

x0 = zeros(n, 1);
count = 0;

for i=1:iters
    Q = A;
    for x=1:n
        sum = 0;
        v = Q(:, x);
        for y=1:x-1
            p = Q(:, y)' * v;
            sum = sum + Q(:, y)*p(1);
        end
        v = v-sum;
        v = v / norm(v);
        Q(:, x) = v;
    end
    A = Q'*A*Q;
    x = diag(A);
    
    count = count + 1;
    if max(100*abs((x - x0) ./ x)) < maxerror
        break
    end
    x0 = x;
end

out = fopen('qrdec.txt', 'w');
fprintf(out, 'QR Method\n');
printMatrix(x, n, 1, out, 'Eigen Values');
printMatrix([count], 1, 1, out, 'Iterations');