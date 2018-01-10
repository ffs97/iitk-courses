clear;
method = input('Please select the method to run:\n(1) Linear Spline\n(2) Quadratic Spline\n(3) Natural Cubic Spline\n(4) Not-a-Knot Cubic Spline\n(5) Periodic Cubic Spline\n(6) Clamped Cubic Spline\n');

n = 1;
data = [];
fid=fopen('input2.txt');
while ~feof(fid)
  line = fgetl(fid);
  if any(line)
    data = [data; sscanf(line,'%f')'];
  else
      x{n} = data;
    data = [];
    n = n+1;
  end
end
if any(data)
  x{n} = data;
end
fclose(fid);

points = table2array(x(1));
eval = table2array(x(2));

if (method == 1)
    linearspline(points, eval);
elseif (method == 2)
    quadraticspline(points, eval);
else
    file = fopen('output.txt', 'w');
    
    n = size(points(:, 1));
    n = n(1) - 1;
    mat = cubicspline(points);
    title = '';
    switch (method)
        case 3
            title = 'Natural';
            mat(4*n-1, 3) = 2;
            mat(4*n-1, 4) = 6*points(1, 1);
            mat(4*n, 4*n-1) = 2;
            mat(4*n, 4*n) = 6*points(n+1, 1);
            fprintf(file, 'Natural Spline\n\n');
        case 4
            title = 'Not-a-Knot';
            mat(4*n-1, 4) = 1;
            mat(4*n-1, 8) = -1;
            mat(4*n, 4*n-4) = 1;
            mat(4*n, 4*n) = -1;
            fprintf(file, 'Not-a-Knot Spline\n\n');
        case 5
            title = 'Periodic';
            mat(4*n-1, 2) = 1;
            mat(4*n-1, 3) = 2*points(1, 1);
            mat(4*n-1, 4) = 3*points(1, 1)^2;
            mat(4*n-1, 4*n-2) = -1;
            mat(4*n-1, 4*n-1) = -2*points(n+1, 1);
            mat(4*n-1, 4*n) = -3*points(n+1, 1)^2;
            mat(4*n, 3) = 2;
            mat(4*n, 4) = 6*points(1, 1);
            mat(4*n, 4*n-1) = -2;
            mat(4*n, 4*n) = -6*points(n+1, 1);
            fprintf(file, 'Periodic Spline\n\n');
        case 6
            slopes = table2array(x(3));
            mat(4*n-1, 2) = 1;
            mat(4*n-1, 3) = 2*points(1, 1);
            mat(4*n-1, 4) = 3*points(1, 1)^2;
            mat(4*n-1, 4*n+1) = slopes(1);
            mat(4*n, 4*n-2) = 1;
            mat(4*n, 4*n-1) = 2*points(n+1, 1);
            mat(4*n, 4*n) = 3*points(n+1, 1)^2;
            mat(4*n, 4*n+1) = slopes(2);
            fprintf(file, 'Clamped Spline\n\n');
            
    end
    coeff = inv(mat(:, 1:4*n))*mat(:, 4*n+1);
    coeff = flipud(reshape(coeff, [4, n]));
    for i=1:size(eval)
        for j=1:n
            if (eval(i) >= points(j, 1) && eval(i) <= points(j+1, 1))
                fprintf(file, '%.4f\t%.4f\n', eval(i), polyval(coeff(:, j), eval(i)));
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