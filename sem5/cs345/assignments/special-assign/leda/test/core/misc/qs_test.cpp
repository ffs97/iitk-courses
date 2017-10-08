/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  qs_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


static int k = 0;

static int count1 = 0;
static int count2 = 0;


bool smaller1(int x, int y) 
{ count1++;
  return x < y; 
}

bool smaller2(int x, int y) 
{ count2++; 
  return x < (y-k); 
}

void INSERTION_SORT(int* l, int* r)
{
  if (l >= r) return;

  int* min=l;
  int* run;

  for (run = l+1; run <= r; run++)
      if (smaller1(*run,*min)) min = run;

  leda_swap(*min,*l);

  if (r == l+1) return;

  for(run=l+2; run <= r; run++)
  { for (min = run-1; smaller1(*run,*min); min--);
    min++;
    if (run != min) 
    { int tmp = *run;
      int* p;
      int* q;
      for(p=run, q = run-1; p > min; p--,q--) *p = *q;
      *min = tmp;
     }
   }
}


void INSERTION_SORT1(int* l, int* m, int* r)
{
  if (l >= r) return;


  // precondition: (*l, ..., *(m-1)) and (*(m+1),...,*r) are sorted

  // swap minimum to *l

  int* p = m+1;
  if (smaller1(*m,*l)) leda_swap(*l,*m);
  if (smaller1(*p,*l)) leda_swap(*l,*p);

  if (r == l+1) return;

  int* run;
  for(run=m; run <= r; run++)
  { int* min;
    for (min = run-1; smaller1(*run,*min); min--);
    min++;
    if (run != min) 
    { int tmp = *run;
      int* p;
      int* q;
      for(p=run, q = run-1; p > min; p--,q--) *p = *q;
      *min = tmp;
     }
   }
}




void QUICKSORT(int* l, int* r)
{ 
  int* i = l;
  int* j = r+1;
  int  s = *l;

  for(;;)
  { while (i < r && smaller2(*++i,s));
    while (j > l && smaller2(s,*--j));
    if (i < j) leda_swap(*i,*j); else break;
   }

  leda_swap(*l,*j);

  if (l < j-1) QUICKSORT(l,j-1);
  if (r > j+1) QUICKSORT(j+1,r);



  if ((j > l && smaller1(s,*(j-1))) || (j < r && smaller1(*(j+1),s))) 
    INSERTION_SORT1(l,j,r);

/*
  i = j;
  while (i > l && smaller1(s,*--i));
  while (j < r && smaller1(*++j,s));

  INSERTION_SORT(i,j);
*/
}




main()
{
  int N = read_int("N = ");

  k = read_int("k = ");

  count1 = 0;
  count2 = 0;


  int* A = new int[N];
  int i;

  list<int> L;

  for(i=1; i<=N; i++)  L.append(i);
  L.permute();

  for(i=0; i<N; i++) A[i] = L.pop();

  QUICKSORT(A,A+N-1);

  //for(i=0; i<N-1; i++)  cout << A[i] << " ";
  //cout << endl;

  int err = 0;

  for(i=0; i<N-1; i++) 
  { //leda_assert(A[i] <= A[i+1],1,"array not sorted"); 
    if (A[i] > A[i+1]) err++;
   }

  cout << endl;
  cout << "err  = " << err << endl;
  cout << "cmp1 = " << count1 << endl;
  cout << "cmp2 = " << count2 << endl;
  cout << endl;

  delete[] A;

  return 0;
}



