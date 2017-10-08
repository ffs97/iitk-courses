/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  array_resize.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/array.h>
#include <LEDA/core/list.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif



int main () 
{
  array<list<string> >  A(3,7);

  A[3].append("1");
  A[4] = A[3];
  A[4].append("2");
  A[5] = A[4];
  A[5].append("3");
  A[6] = A[5];
  A[6].append("4");
  A[7] = A[6];
  A[7].append("6");

  for(int i = A.low(); i<=A.high(); i++)
    cout << string("A[%d] = ",i) << A[i] << endl;
  cout << endl;

  A.resize(1,10);

  for(int i = A.low(); i<=A.high(); i++)
    cout << string("A[%d] = ",i) << A[i] << endl;
  cout << endl;

  A.resize(5,10);

  for(int i = A.low(); i<=A.high(); i++)
    cout << string("A[%d] = ",i) << A[i] << endl;
  cout << endl;

  return 0;
}
