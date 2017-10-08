/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  h.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/h_array.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int main()
{
  int N      = read_int("# keys = ");
  int i;

  h_array<int,int> M;

  for(i=0; i< N; i++) M[i] = i;

  for(i=0; i< N; i++)
  { int x = M[i];
    if (x != i) cout << i << ": " << x <<endl;
   }

  return 0;
}

