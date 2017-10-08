/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sort_t.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


/*
#define LEDA_STL_ITERATORS
*/

#if defined(__GNUC__)
#define vector stl_vector
#include<vector>
#include<algorithm>
#undef vector
#endif

#include <LEDA/core/array.h>
#include <LEDA/core/random.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::endl;
#endif



class tuple
{

public:

  int a,b;

  tuple() : a(0),b(0) {}

  friend int compare(const tuple &t1, const tuple &t2)
  { int d = t1.a - t2.a;
    return (d == 0) ? (t1.b - t2.b) : d;
   }

  friend int compare1(const void *t1, const void *t2)
  { int d = ((tuple*)t1)->a - ((tuple*)t2)->a;
    return (d == 0) ? (((tuple*)t1)->b - ((tuple*)t2)->b) : d;
   }

  friend bool operator<(const tuple& t1, const tuple& t2)
  {  return (t1.a < t2.a || (t1.a == t2.a && t1.b < t2.b)); }


  friend ostream& operator<<(ostream &s, const tuple& t){ return s; }
  friend istream& operator>>(istream &s, tuple& t)      { return s; }


};


class tuple_cmp {
public:
bool operator()(const tuple& t1, const tuple& t2) { return t1 < t2; }
};



main()
{
  int N = read_int("N = ");

  random_source S(0,10*N);

  cout << endl;

  cout << "Constructing Arrays " << flush;

  array<tuple> A(N);

  tuple *B = new tuple[N];
  tuple *C = new tuple[N];

  int i,j,k;
  for(i=0;i<A.size();i++)
  { S >> j;
    C[i].a = A[i].a= B[i].a= j;
    S >> k;
    C[i].b = A[i].b = B[i].b = k;
   }

  cout << "done" << endl;
  cout << endl;

  float t;

  cout << "LEDA_SORT:   " << flush;
  t = used_time();
  A.sort();
  cout << string("%.2f sec", used_time(t)) << endl;

  cout << "LEDA_C_SORT: " << flush;
  t = used_time();
  QUICKSORT_C(C,C+N-1);
  cout << string("%.2f sec", used_time(t)) << endl;

#if defined(__GNUC__)
  stl_vector<tuple> V(B,B+N);
  cout << "STL_SORT1:   " << flush;
  t = used_time();
  sort(V.begin(),V.end());
  cout << string("%.2f sec", used_time(t)) << endl;

  stl_vector<tuple> V1(B,B+N);
  tuple_cmp tcmp;
  cout << "STL_SORT3:   " << flush;
  t = used_time();
  QUICKSORT_STL(V1.begin(),V1.end()-1,tcmp);
  cout << string("%.2f sec", used_time(t)) << endl;
#endif

  cout << "qsort:       " << flush;
  t = used_time();
  qsort(B,N,sizeof(tuple),compare1);
  cout << string("%.2f sec", used_time(t)) << endl;

  
  // check sorting

  for(i=0;i<A.size();i++)
  { int j = A[i].a;
    int k = A[i].b;
    if (j != C[i].a || j != B[i].a || k != C[i].b || k != B[i].b )
    { cerr << A[i].a << " " << B[i].a << " " << C[i].a << endl;
      cerr << endl;
     }
   }
  return 0;
}




