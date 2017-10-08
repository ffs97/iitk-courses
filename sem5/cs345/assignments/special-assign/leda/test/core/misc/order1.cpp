/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  order1.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/list.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


/*

// redefine the default linear order

LEDA_BEGIN_NAMESPACE
int compare(const int& x, const int& y) { return leda_default_compare(y,x); }
LEDA_END_NAMESPACE
*/


int main () 
{
  list<int>     L;

  int N = read_int("N = "); 

  for (int i = 0; i < N; i++) L.append(rand_int(1,100));

  L.sort();
  L.print();

  return 0;
}
