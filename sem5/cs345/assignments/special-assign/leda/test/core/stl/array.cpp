/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  array.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/array.h>

#if defined(LEDA_STD_HEADERS)
using namespace std;
#endif

int main ()
{
  leda::array<int> a1(10);
  leda::array<int>::iterator it = a1.begin();

  int i = 1;
  while ( it != a1.end() ) *it++ = i*i;
  for(it = a1.begin(); it != a1.end(); it++) 
  { cout << (it - a1.begin()) << ": ";
    cout << *it << endl;
   }

  return 0;
}
