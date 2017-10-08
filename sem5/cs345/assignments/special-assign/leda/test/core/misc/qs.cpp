#define LEDA_CHECKING_OFF

#include <LEDA/system/basic.h>
#include <LEDA/core/array.h>
#include <LEDA/core/basic_alg.h>
#include <LEDA/core/quicksort.h>

#include <LEDA/core/b_stack.h>

#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
using std::flush;
#endif





template<class E, class SMALLER>
void quicksort1(E* l, E* r, const SMALLER& smaller)
{ 
  if (r <= l) return;
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

  E s = *i;

  for(;;)
  { while (smaller(*++i,s));
    while (smaller(s,*--j));
    if (i >= j) break;
    swap(*i,*j);
   }
  swap(*k,*j);

  quicksort1(l,j-1,smaller);
  quicksort1(j+1,r,smaller);
}



template<class E, class SMALLER>
void quicksort2(E* l, E* r, const SMALLER& smaller)
{  
  // eliminate tail rekursion
  // recursion depth <= log n

  while (r > l)
  { if (smaller(*r,*l)) swap(*l,*r);
    if (r == l+1) break;

    E* k = l + (r-l)/2;
    if (smaller(*k,*l)) 
       swap(*l,*k);
    else
       if (smaller(*r,*k)) swap(*k,*r);
    if (r == l+2) break;
  
    E* i = l+1;
    E* j = r;
    swap(*i,*k);
    k = i;
  
    E s = *i;

    for(;;)
    { while (smaller(*++i,s));
      while (smaller(s,*--j));
      if (i >= j) break;
      swap(*i,*j);
     }
    swap(*k,*j);

    if (j-l < r-j)
      { quicksort2(l,j-1,smaller); l = j+1; }
    else
      { quicksort2(j+1,r,smaller); r = j-1; }
  }
}



template<class E, class SMALLER>
void quicksort3(E* l, E* r, const SMALLER& smaller)
{ 
  if (r <= l) return;
  if (smaller(*r,*l)) swap(*l,*r);
  if (r == l+1) return;

  E* k = l + (r-l)/2;
  if (smaller(*k,*l)) 
     swap(*l,*k);
  else
     if (smaller(*r,*k)) swap(*k,*r);
  if (r == l+2) return;

  E* i = l;
  E* j = r;

  E s = *k;

  for(;;)
  { while (smaller(*++i,s));
    while (smaller(s,*--j));
    if (i >= j) break;
    swap(*i,*j);
    if (k == j) k = i;
    else if (k == i) k = j;
   }

  if (k > j) j = i;
  *k = *j;
  *j = s;

  quicksort3(l,j-1,smaller);
  quicksort3(j+1,r,smaller);
}






#define CONTINUE { l = r+2; P.pop(); continue; }


template<class E, class SMALLER>
void quicksort_non_rec1(E* first, E* last, const SMALLER& smaller)
{ 
  int n = last-first+1;

  if (n <= 1) return;

  b_stack<E*> P(1000);

  P.push(last+1);

  E* l = first;

  while (!P.empty())
  { 
    E* r = P.top()-1;

    if (r <= l) CONTINUE;

    if (smaller(*r,*l)) swap(*l,*r);

    if (r == l+1) CONTINUE;

    E* k = l + (r-l)/2;

    if (smaller(*k,*l))
       swap(*l,*k);
    else
       if (smaller(*r,*k)) swap(*k,*r);

    if (r == l+2) CONTINUE;


    E* i = l+1;

    E s = *k;
    *k = *i;
    *i = s;

    for(;;)
    { while (smaller(*++i,s));
      while (smaller(s,*--r));
      if (i >= r) break;
      swap(*i,*r);
     }

    *(l+1) = *r;
    *r = s;
    P.push(r);
  }
}


/*
#define CONTINUE2 { while (l < r+2) *out++ = *l++; P.pop(); continue; }
*/


template<class E, class SMALLER>
void quicksort_non_rec2(E* first, E* last, const SMALLER& smaller)
{ 
  int n = last-first+1;

  if (n <= 1) return;

  b_stack<E*> P(1000);

  P.push(last+1);
  
  E* l = first;

  while (!P.empty())
  { 
    E* r = P.top()-1;

    if (r <= l) CONTINUE;

    if (smaller(*r,*l)) swap(*l,*r);

    if (r == l+1) CONTINUE;

    E* k = l + (r-l)/2;

    if (smaller(*k,*l))
       swap(*l,*k);
    else
       if (smaller(*r,*k)) swap(*k,*r);

    if (r == l+2) CONTINUE;


    E* i = l;
    E s = *k;

    for(;;)
    { while (smaller(*++i,s));
      while (smaller(s,*--r));
      if (i >= r) break;
      swap(*i,*r);
      if (k == r) k = i;
      else if (k == i) k = r;
     }

    if (k > r) r = i; 
    *k = *r;
    *r = s;
    P.push(r);
  }
}



int main () 
{
  int N = read_int("N = "); 


  array<int> A(N);
  array<int> B(N);
  array<int> C(N);
  array<int> D(N);

  for (int i = 0; i < N; i++) A[i] = i;
  A.permute();

  for (int i = 0; i < N; i++) B[i] = C[i] = D[i] = A[i];

  leda_smaller<int> smaller;

  float T = used_time();

  cout << "quicksort1       " << flush;
  quicksort1(A.first_item(),A.last_item(),smaller);
  cout << string("%5.3f sec",used_time(T)) << endl;

  cout << "quicksort2       " << flush;
  quicksort2(B.first_item(),B.last_item(),smaller);
  cout << string("%5.3f sec",used_time(T)) << endl;

  cout << "quicksort_non_r1 " << flush;
  quicksort_non_rec1(C.first_item(),C.last_item(),smaller);
  cout << string("%5.3f sec",used_time(T)) << endl;

  cout << "quicksort_non_r2 " << flush;
  quicksort_non_rec2(D.first_item(),D.last_item(),smaller);
  cout << string("%5.3f sec",used_time(T)) << endl;


  for (int i = 0; i < N; i++) 
  { assert(A[i] == B[i]);
    assert(B[i] == C[i]);
    assert(C[i] == D[i]);
   }
 
  return 0;

}


