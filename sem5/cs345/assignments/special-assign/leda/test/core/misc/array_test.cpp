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


#include <LEDA/core/array.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int ord(const int& x) { return x%1000; }


int search(const array<int>& A, int x) { return A.binary_search(x); }

int main () 
{

  int i;

  int N = read_int("Number of array entries: "); 

  float T = used_time();


  cout << "allocating    ";
  cout.flush();

  array<int>     A(N);
  array<int>     A1(N);
  array<double>  A2(N);

  for(i=0; i<N; i++) A[i] = rand_int(0,100000);

  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "assignment    ";
  cout.flush();
  A = A1;
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "permute       ";
  cout.flush();
  A.permute();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting(int)  ";
  cout.flush();
  A.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting again ";
  cout.flush();
  A.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "iteration     ";
  cout.flush();
  forall(i,A) {}
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  for(i=0; i<N; i++) A2[i] = A[i];

  cout << "sorting(double)";
  cout.flush();
  A2.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "sorting again ";
  cout.flush();
  A2.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;



  cout << "test sorting  ";
  cout.flush();
  for(i=0; i<N; i++) { 
    if (A[i] != A2[i])  cout << string("%d  != %f\n",A[i],A2[i]);
  }
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "test searching";
  cout.flush();
  for(i=0; i<N; i++) { 
    int j = search(A,A[i]);
    if (A[j] != A[i])  cout << string("%d  != %f\n",A[i],A[j]);
  }
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  
  cout << endl;
  cout << "used memory = " << used_memory()/1024.0 << " kb\n";

  return 0;
}
