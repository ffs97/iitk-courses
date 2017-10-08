/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  array_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_CHECKING_OFF

#include <LEDA/core/map.h>
#include <LEDA/core/array.h>
#include <LEDA/core/d_array.h>
#include <LEDA/core/h_array.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif



int main()
{ 
  int N0[10000];
  int i;
  for(i=0; i<1000; i++) N0[i] = 0;

  array<int>         A0(10000);
  for(i=0; i<10000; i++) A0[i] = 0;

  map<int,int>       M1(0);
  map<int,int>       M2(0,1<<18);
  h_array<int,int>   N1(0);
  h_array<int,int>   N2(0,1<<18);
  d_array<int,int>   N3(0);

  int n = read_int("n = ");

  int* A = new int[n];

  rand_int.set_seed(1234*n);
  for(i=0; i<n; i++)  A[i] = rand_int(1,9999);

  float T = used_time();
  for(i=0; i<n; i++)  N0[A[i]]++;
  cout << string("c++ array:  %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  A0[A[i]]++;
  cout << string("leda array: %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  M1[A[i]]++;
  cout << string("map:        %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  M2[A[i]]++;
  cout << string("map:        %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  N1[A[i]]++;
  cout << string("h_array:    %.2f sec",used_time(T)) << endl; 
//cout << N1.size() <<endl;

  T = used_time();
  for(i=0; i<n; i++)  N2[A[i]]++;
  cout << string("h_array:    %.2f sec",used_time(T)) << endl; 

  T = used_time();
  for(i=0; i<n; i++)  N3[A[i]]++;
  cout << string("d_array:    %.2f sec",used_time(T)) << endl; 
//cout << N3.size() <<endl;

  for(i=1; i<1000; i++)
    if (A0[i] != N0[i] || M1[i] != N0[i] || M2[i] != N0[i] ||
        N1[i] != N0[i] || N2[i] != N0[i] || N3[i] != N0[i])
    { cout << string("%2d: ",i);
      cout << N0[i] << " ";
      cout << A0[i] << " ";
      cout << M1[i] << " ";
      cout << M2[i] << " ";
      cout << N1[i] << " ";
      cout << N2[i] << " ";
      cout << N3[i] << endl;
     }

  return 0;
}

