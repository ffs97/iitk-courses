/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sort.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SORT_H
#define LEDA_SORT_H
#endif

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE


template<class T>
void quicksort(T* l, T* r)
{ 
  if (*r < *l) leda_swap(*l,*r);

  if (r == l+1) return;

  T* k = l + (r-l)/2;

  if (*k < *l) 
     leda_swap(*l,*k);
  else
     if (*r < *k) leda_swap(*k,*r);

  if (r == l+2) return;

  T* i = l+1;
  T* j = r;
  leda_swap(*i,*k); 

  k = i;

  //const T& s = *i;

  T s = *i;

  for(;;)
  { while (*++i < s);
    while (s < *--j);
    if (i<j) leda_swap(*i,*j);
    else break;
   }

  leda_swap(*k,*j); 

  if (j > l+1) quicksort(l,j-1);
  if (r > j+1) quicksort(j+1,r);
}

template<class T> 
void sort(T* l, T* r) { leda::quicksort(l,r); }




template<class E>
E* partition(E* l, E* r)
{ 
  //assert(r-l >= 3);

  E* k = l + (r-l)/2;

  E* k1 = r-1;

  if (*r < *l) leda_swap(*l,*r);

  if (*k < *l) 
    leda_swap(*l,*k);
  else 
    if (*r < *k) leda_swap(*k,*r);

  if (*k1 < *k)
  { leda_swap(*k,*k1);
    if (*k < *l) leda_swap(*l,*k);
   }

  if (k == l+1) return k;


  E* i = l+1;
  E* j = r-1;

  leda_swap(*i,*k); 
  k = i;

  E s = *i;

  for(;;)
  { while (*++i < s);
    while (s < *--j);
    if (i<j) leda_swap(*i,*j);
    else break;
   }
  leda_swap(*k,*j); 

  return j;
}


template<class T>
void select(T* l, T* r, T* p)
{ 
  //precondition: l <= p <= r

  while (l < r) 
  { 
    T* i = l+(r-l)/2;
    T* j = r;

    if (*j < *i) leda_swap(*i,*j);

    leda_swap(*l,*i);

    i = l;

    T s = *l;

    for(;;)
    { while (*++i < s);
      while (s < *--j);
      if (i<j) leda_swap(*i,*j); else break;
     }
  
    leda_swap(*l,*j);

    //j->mark();
    j->id = 1;

    if (p == j) break;
  
    if (p < j) r = j-1;
    else l = j+1;
  }
}


template<class T>
void select1(T* p)
{ 
  if (p->id) return;

  T* l = p-1;
  T* r = p+1;

  while (!l->id) l--;
  while (!r->id) r++;
  l++;
  r--;

  while (l < r) 
  { 
    T* i = l;
    T* j = r;
    T* k = l+(r-l)/2;
    if (*j < *k) leda_swap(*k,*j);
    leda_swap(*l,*k);

    T s = *l;

    for(;;)
    { while (*++i < s);
      while (s < *--j);
      if (i<j) leda_swap(*i,*j); else break;
     }
  
    leda_swap(*l,*j);

    //j->mark();
    j->id = true;

    if (p == j) break;
  
    if (p < j) r = j-1;
    else l = j+1;
  }
}



LEDA_END_NAMESPACE
