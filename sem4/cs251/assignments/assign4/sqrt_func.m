function root = sqrt_func(n, k = 10^5)
  
  if (nargin > 2 || nargin < 1)
    error('Invalid number of Arguments');
  elseif !(isa(n, 'double') == 1 && isa(k, 'double') == 1)
    error('Invalid Input Format');
  elseif (floor(k) != k)
    error('Number of Points must be an integer');
  elseif (k <= 0)
    error('Number of Points to be generated should be positive');
  endif
  
  format long;
  
  neg = false;
  if (n < 0)
    n = abs(n);
    neg = true;
  endif
  
	points = n*rand(1, k);

	h = ((n+1)/2)^2;
	b = ((n-1)/2)^2;

	count = 0;
	for x = points
		if (x^2 + b < h)
			count++;
		endif
	endfor

	root = count*n/k;
  if neg
    root = root*i;
  endif

endfunction