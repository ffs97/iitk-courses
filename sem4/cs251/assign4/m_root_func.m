function root = m_root_func(n, m = 2, d = 4)

  if n == 0
    root = 0;
    return;
  endif
  
  if (nargin > 3 || nargin < 1)
    error('Invalid number of Arguments');
  elseif !(isa(n, 'double') == 1 && isa(m, 'double') == 1 && isa(d, 'double') == 1)
    error('Invalid Input Format');
  elseif (d < 0)
    error('Number of decimal digits cannot be less than 0');
  elseif (m <= 0)
    error('m should be greater than 0');
  elseif (floor(d) != d)
    error('Number of decimal places should be an integer');
  elseif (floor(m) != m)
    error('Exponent should be an integer');
  endif

  format long;
  
  negFlag = 0;
  if n < 0
    n = -n;
    negFlag = 1;
    if mod(m, 2) == 0
      negFlag = 2;
    endif
  endif

  low = 0;
  high = max(1, n);
  
  e = 10^(d+1);
  ldl = 1/e;
  
  lowp = low-1;
  highp = high+1;
  
  while !(lowp == low && highp == high)
    lowp = low;
    highp = high;
    dl = max(floor(e*(high - low)*rand())/e, ldl);
    rand_mid = low + dl;
    
    if (rand_mid^m < n)
      low = rand_mid;
    elseif (rand_mid^m > n)
      high = rand_mid;
    else
      low = rand_mid;
      break;
    endif
    
  endwhile
  
  if negFlag == 1
    low = -low;
  elseif negFlag == 2
    low = floor(e * low * (cos(pi/m) + i*sin(pi/m)))/e;
  endif
  
  e = e / 10;
  root = floor(e * low) / e;

endfunction