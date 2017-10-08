/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  quicksort.t
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_QUICKSORT_T
#define LEDA_QUICKSORT_T

// this file contains quicksort function templates


#include <LEDA/system/basic.h>
#include <LEDA/core/basic_alg.h>
#include <LEDA/core/tuple.h>

#include <LEDA/internal/std/string.h>

LEDA_BEGIN_NAMESPACE


template<class Iterator, class Compare, class E>
__temp_func_inline 
void QUICKSORT_STL0(Iterator l, Iterator r, Compare smaller, E*)
{ if (r <= l) return;
  if (smaller(*r,*l)) leda_swap(*l,*r);
  if (r == l+1) return;
  Iterator k = l + (r-l)/2;
  if (smaller(*k,*l)) 
     leda_swap(*l,*k);
  else
     if (smaller(*r,*k)) leda_swap(*k,*r);
  if (r == l+2) return;
  Iterator i = l+1;
  Iterator j = r;
  leda_swap(*i,*k); 
  k = i;
  const E& s = *i;
  for(;;)
  { while (smaller(*++i,s));
    while (smaller(s,*--j));
    if (i<j) leda_swap(*i,*j);
    else break;
   }
  leda_swap(*k,*j); 
  QUICKSORT_STL0(l,j-1,smaller,(E*)0);
  QUICKSORT_STL0(j+1,r,smaller,(E*)0);
}


template<class Iterator, class Compare>
__temp_func_inline 
void QUICKSORT_STL(Iterator l, Iterator r, Compare smaller)
{ QUICKSORT_STL0(l,r,smaller,value_type(l)); }




template<class E, class SMALLER>
void insertion_sort_old(E* l, E* r, const SMALLER& smaller)
{
  E* p=l;
  while (p < r && !smaller(*(p+1),*p)) p++;
  if (p >= r) return;


  // find minimum and move it to front

  E* min=l;
  while (++p <= r)
      if (smaller(*p,*min)) min = p;

  leda_swap(*min,*l);

  if (r == l+1) return;

  for(E* run=l+2; run <= r; run++)
  { E* min = run;
    while (smaller(*run,*(min-1))) min--;
    if (run != min)
    { E tmp = *run;
      for(E* p=run; p > min; p--) *p = *(p-1);
      *min = tmp;
     }
   }
}


template<class E, class SMALLER>
void insertion_sort(E* l, E* r, const SMALLER& smaller)
{ for(E* p=l+1; p <= r; p++)
  { E x = *p;
    E* t = p;
    for(E* s=p-1; s >= l && smaller(x,*s); t--, s--) *t = *s;
    *t = x;
   }
}



template<class E, class SMALLER>
void quicksort_d(E* l, E* r, const SMALLER& smaller,int d)
{ if (r <= l+d) return;
  if (smaller(*r,*l)) swap(*l,*r);
  if (r == l+1) return;
  E* k = l + (r-l)/2;
  if (smaller(*k,*l)) 
     swap(*l,*k);
  else
     if (smaller(*r,*k)) swap(*k,*r);
  if (r == l+2) return;
  E* i = l+1;
  E* j = r;
  swap(*i,*k);
  k = i;
  const E& s = *i;
  for(;;)
  { while (smaller(*++i,s));
    while (smaller(s,*--j));
    if (i >= j) break;
    swap(*i,*j);
   }
  swap(*k,*j);
  if (l < j-1) quicksort_d(l,j-1,smaller,d);
  if (r > j+1) quicksort_d(j+1,r,smaller,d);
}


template<class E, class SMALLER>
void quicksort_i(E* l, E* r, const SMALLER& smaller,int d)
{ 
  if (r <= l+d) 
  { //insertion_sort(l,r,smaller); 

    for(E* p=l+1; p <= r; p++)
    { E x = *p;
      E* t = p;
      for(E* s=p-1; s >= l && smaller(x,*s); t--, s--) *t = *s;
      *t = x;
     }
/*
    while (++l <= r)
    { E x = *l;
      E* t = l;
      for(E* s=l-1; smaller(x,*s); t--, s--) *t = *s;
      *t = x;
     }
*/
    return;
   }

  if (smaller(*r,*l)) swap(*l,*r);
  //if (r == l+1) return;

  E* k = l + (r-l)/2;
  if (smaller(*k,*l)) 
     swap(*l,*k);
  else
     if (smaller(*r,*k)) swap(*k,*r);
  //if (r == l+2) return;

  E* i = l+1;
  E* j = r;
  swap(*i,*k);
  k = i;
  const E& s = *i;
  for(;;)
  { while (smaller(*++i,s));
    while (smaller(s,*--j));
    if (i >= j) break;
    swap(*i,*j);
   }
  swap(*k,*j);
  if (l < j-1) quicksort_i(l,j-1,smaller,d);
  if (r > j+1) quicksort_i(j+1,r,smaller,d);
}




template<class E, class SMALLER>
void quicksort(E* l, E* r, const SMALLER& smaller)
{ if (r <= l) return;
  if (smaller(*r,*l)) swap(*l,*r);
  if (r == l+1) return;
  E* k = l + (r-l)/2;
  if (smaller(*k,*l)) 
     swap(*l,*k);
  else
     if (smaller(*r,*k)) swap(*k,*r);
  if (r == l+2) return;
  E* i = l+1;
  E* j = r;
  swap(*i,*k);
  k = i;
  //const E& s = *i;
  E s = *i;
  for(;;)
  { while (smaller(*++i,s));
    while (smaller(s,*--j));
    if (i >= j) break;
    swap(*i,*j);
   }
  swap(*k,*j);
  if (l < j-1) quicksort(l,j-1,smaller);
  if (r > j+1) quicksort(j+1,r,smaller);
}



// quicksort with exact and approximative smaller

template<class E, class SMALLER_E, class SMALLER_A>
void quicksort(E* l, E* r, const SMALLER_E& smaller_E, 
                           const SMALLER_A& smaller_A)
{ if (r <= l) return;

  if (smaller_E(*r,*l)) swap(*l,*r);
  if (r == l+1) return;

  E* k = l + (r-l)/2;
  if (smaller_E(*k,*l)) 
     swap(*l,*k);
  else
     if (smaller_E(*r,*k)) swap(*k,*r);
  if (r == l+2) return;

  E* i = l+1;
  E* j = r;
  swap(*i,*k);
  k = i;

  const E& s = *i;

  for(;;)
  { while (smaller_A(*++i,s));
    while (smaller_A(s,*--j));
    if (i >= j) break;
    swap(*i,*j);
   }

  swap(*k,*j);

  quicksort(l,j-1,smaller_E,smaller_A);
  quicksort(j+1,r,smaller_E,smaller_A);

  if ((l < j && smaller_E(*j,*(j-1))) || (r > j && smaller_E(*(j+1),*j))) 
  { 
    // exact insertion sort
    // precondition:  both (*l, ..., *(j-1)) and (*(j+1),...,*r) are sorted

    // insertion sort loop starts at position j
    while (j <= r)
    { E x = *j;
      E* t = j;
      for(E* s=j-1; s >= l && smaller_E(x,*s); t--, s--) *t = *s;
      *t = x;
      j++;
     }

/*
    // sentinel: assign minimum(*l,*j,*(j+1)) to *l
    if (smaller_E(*j,*l)) leda_swap(*l,*j);
    if (smaller_E(*(j+1),*l)) leda_swap(*l,*(j+1));

    // insertion sort loop (starts at position j !)
    while (j <= r)
    { E* q = j;
      while (smaller_E(*j,*(q-1))) q--;
      if (q != j) 
      { E tmp = *j;
        for(E* p=j; p > q; p--) *p = *(p-1);
        *q = tmp;
       }
      j++;
     }
*/

  }
}



class QuickSort {
public:
 template<class E, class SMALLER>
 void operator()(E* l, E* r, const SMALLER& smaller) const 
 { quicksort(l,r,smaller); }
};

class MergeSort {
public:
 template<class E, class SMALLER>
 void operator()(E* l, E* r, const SMALLER& smaller) const 
 { mergesort(l,r,smaller); }
};



template<class tuple, class CMP>
class tuple_smaller {
const CMP& cmp;
public:
tuple_smaller(const CMP& _cmp) : cmp(_cmp) {}
bool operator()(const tuple& x, const tuple& y) const 
{ return  cmp(x.first(),y.first()); }
};



template <class T>
void QUICKSORT_C(T* l, T* r)
{ leda_smaller<T> smaller;
  quicksort(l,r,smaller);
 }



// merge sort

template<class E, class SMALLER>
void MERGE(E* l, E* m, E* r, E* tmp, const SMALLER& smaller)
{ 

  E* q = m+1;

/*
  while (smaller(*l,*q)) l++;
  while (smaller(*m,*r)) r--;
*/

  int n = r-l+1;
  E* p = l;

  for(int i=0; i<n; i++)
  { if (q > r || (p <= m && smaller(*p,*q))) 
       tmp[i] = *p++;
    else
       tmp[i] = *q++;
   }
  for(int i=n-1; i>=0; i--) *--q = tmp[i];

}

template<class E, class Cmp>
void MERGE_SORT1(E* l, E* r, E* tmp, Cmp smaller)
{ 
  if (l >= r) return;
  E* m = l + (r-l)/2;
  E* a = l + (m-l)/2;
  E* b = m + (r-m)/2;
  quicksort(l,a,smaller);
  quicksort(a+1,m,smaller);
  quicksort(m+1,b,smaller);
  quicksort(b+1,r,smaller);
  MERGE(l,a,m,tmp,smaller);
  MERGE(m+1,b,r,tmp,smaller);
  MERGE(l,m,r,tmp,smaller);
}


template<class E, class Cmp>
void MERGE_SORT(E* l, E* r, E* tmp, Cmp smaller)
{ 
  if (l >= r) return;

/*
  if (l > r-100000) 
  { quicksort(l,r,smaller);
    return;
   }
*/

/*
  if (r == l+1)
  { if (smaller(*r,*l)) swap(*l,*r);
    return;
   }
*/

  E* m = l + (r-l)/2;

/*
  if (r == l+2) 
  { if (smaller(*r,*l)) swap(*l,*r);
    if (smaller(*m,*l))
      swap(*l,*m);
    else
      if (smaller(*r,*m)) swap(*m,*r);
    return;
   }
*/

  
  //MERGE_SORT(l,m,tmp,smaller);
  //MERGE_SORT(m+1,r,tmp,smaller);
  quicksort(l,m,smaller);
  quicksort(m+1,r,smaller);
  MERGE(l,m,r,tmp,smaller);
}

template<class E, class SMALLER>
void mergesort(E* l, E* r, const SMALLER& smaller)
{ int n = r-l+1;
  if (n > 1)
  { E* tmp = new E[n];
    MERGE_SORT(l,r,tmp,smaller);
    delete[] tmp;
  }
}

template<class E, class SMALLER>
void mergesort1(E* l, E* r, const SMALLER& smaller)
{ int n = r-l+1;
  if (n > 1)
  { E* tmp = new E[n];
    MERGE_SORT1(l,r,tmp,smaller);
    delete[] tmp;
  }
}


LEDA_END_NAMESPACE

#endif
