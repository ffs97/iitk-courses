method = input('Please select one of the methods: \n(1) Direct Power Method \n(2) Inverse Power Method \n(3) Shifted Power Method \n(4) QR Method \n');

filedata = textread('matrix.txt', '%f');

tmp = importdata('matrix.txt', ' ');
n = tmp(1);

count = n*n+1;
A = reshape(tmp(2:n*n+1), [n, n]);
A = A';
count = count+1;

iters = tmp(count);
count = count+1;

maxerror = tmp(count);
count = count+1;

switch (method)
    case 1
        directpower(A, n, iters, maxerror);
    case 2
        inversepower(inv(A), n, iters, maxerror);
    case 3
        shift = tmp(count);
        shiftedpower(inv(A - shift*eye(n)), n, iters, maxerror, shift);
    case 4
        qrdec(A, n, iters, maxerror);
end