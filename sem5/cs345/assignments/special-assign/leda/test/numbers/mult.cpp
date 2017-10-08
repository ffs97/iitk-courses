/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  mult.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/system/basic.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/core/IO_interface.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
using std::flush;
#endif



int main(int argc, char** argv)
{ 

  LEDA::use_karatsuba = false;

  int n = 25000;

/*
  if (argc > 1) n = atoi(argv[1]);
*/

  if (argc > 1) { 
     if (string(argv[1]) == "-k") LEDA::use_karatsuba = true;
  }

  rand_int.set_seed(n);


  integer a = integer::random(n);

  a =  a | (integer(1) << n);


  float T0 = 0;

  cout << endl;
  cout << endl;

  for(;;)
  {
     cout << string("n = %7d  ",n) << flush;
     float T = used_time();
     a = a*a;
     float T1 = used_time(T);
     cout << string("%6.2f sec   ",T1);
     if (T0 != 0) cout << string("f = %.2f",T1/T0); 
     cout << endl;
     n = 2*n;
     T0 = T1;
  }

return sign(a);
}


