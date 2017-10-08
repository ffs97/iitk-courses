/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  map_test.c
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



int map_test(map<int,int>& D, int N, int* A, const char* name)
{ 
  cout << string("%-12s",name);
  cout.flush();

  float T;
  float T0 = T = used_time();

  int sum = 0;

  int i;
  for(i=0; i<N; i++)  D[A[i]] = A[i];
  cout << string("%10.2f",used_time(T));
  cout.flush();

  for(i=0; i<N; i++)  sum += D[A[i]];
  cout << string("%10.2f",used_time(T));
  cout.flush();

  cout << string("%10.2f",used_time(T0));
  cout << endl;

  return sum;
}

int main()
{
  int     N      = read_int("# keys = ");
  int*    Int    = new int[N];
  int*    Int1   = new int[N];



  int i;
  for(i=0; i<N; i++) Int[i] = rand_int(0,100000);
  for(i=0; i<N; i++) Int1[i] = i;

  int k;


  cout << endl;
  cout << "                insert    lookup    total\n";
  cout << endl;

  cout << "Random Integers" << endl;
  cout << endl;

  { map<int,int> M(0);
    map_test(M,N,Int,"map");
    cout << endl;
   }

  for(k=1; k<=1024; k*=2)
  { cout << "k = " << k << endl;
    map<int,int> M(0,k);
    map_test(M,N,Int,"map");
   }
  cout << endl;


  cout << "Sorted Integers" << endl;
  cout << endl;

  { map<int,int> M(0);
    map_test(M,N,Int1,"map");
    cout << endl;
   }

  for(k=1; k<=1024; k*=2)
  { cout << "k = " << k << endl;
    map<int,int> M(0,k);
    map_test(M,N,Int1,"map");
   }
  cout << endl;

  delete[] Int;
  delete[] Int1;

  return 0;
}

