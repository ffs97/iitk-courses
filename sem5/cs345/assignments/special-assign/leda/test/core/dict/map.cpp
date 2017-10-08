/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  map.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/map.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int main()
{
  int N      = read_int("# keys = ");
  int i;

  map<int,string> M;

  for(i=0; i< N; i++) M[i] = string("%d",i);

//forall_defined(i,M) cout << i <<endl;

  return 0;
}

