/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_ARRAY_H
#define LEDA_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600033
#include <LEDA/internal/PREAMBLE.h>
#endif

//------------------------------------------------------------------------------
// array
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/quicksort.h>
#include <LEDA/core/bin_search.h>
#include <LEDA/core/basic_alg.h>

#include <assert.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {array} {E} {One Dimensional Arrays} {A}}*/


template<class E> 

class array {

/*{\Mdefinition
    An instance $A$ of the parameterized data type |\Mname| is a mapping from 
    an interval $I =[a..b]$ of integers, the index set of $A$, to the set of 
    variables of data type $E$, the element type of $A$. $A(i)$ is called the 
    element at position $i$. The array access operator (|A[i]|) checks its 
    precondition ($a\le i\le b$). The check can be turned off by compiling 
    with the flag \verb+-DLEDA_CHECKING_OFF+. }*/


   E*    vec;
   int   sz;
   int   l;
   int   h;


void construct(int low, int high)
{ if (low > high+1) 
    LEDA_EXCEPTION(1,string("array: bad array bounds [%d..%d]",low,high));
  l = low;
  h = high;
  sz = high-low+1;
  vec = (sz > 0) ? LEDA_NEW_VECTOR(E,size_t(sz)) : 0;
  for(int i=0; i<sz; i++) new(vec+i) E;
}

void construct(const array<E>& A)
{ l = A.l;
  h = A.h;
  sz = h-l+1;
  vec = (sz > 0) ? LEDA_NEW_VECTOR(E,size_t(sz)) : 0;
  for(int i=0; i<sz; i++) new(vec+i) E(A.vec[i]);
}

void destruct(E* v, int s)
{ E* stop = v+s;
  for(E* p=v; p<stop; p++) p->~E();
  LEDA_DEL_VECTOR(v);
}


public:

/*{\Mtypes 5}*/ 

typedef E* item;
/*{\Mtypemember the item type.}*/

typedef E value_type;
/*{\Mtypemember the value type.}*/


// additional type definitions ...
typedef value_type*         pointer;
typedef const value_type*   const_pointer;
typedef value_type&         reference;
typedef const value_type&   const_reference;
typedef int                 size_type;
typedef int                 difference_type;

typedef int (*cmp_func)(const E&,const E&);


/*{\Mcreation 3}*/

array(int low, int high) { construct(low,high); }
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname| with 
             index set $[low..high]$. }*/


explicit 

array(int n) { construct(0,n-1); }
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname| with index set $[0..n-1]$. }*/ 


array()  { construct(0,-1); }
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname| with empty index set.}*/


/*{\Mtext \headerline{Special Constructors} }*/

array(int low, const E& x, const E& y)
{ construct(low,low+1);
  vec[0] = x;
  vec[1] = y;
}
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname| with index set 
            $[low, low+1]$ initialized to $[x,y]$. }*/ 

array(int low, const E& x, const E& y, const E& w)
{ construct(low,low+2);
  vec[0] = x;
  vec[1] = y;
  vec[2] = w;
}
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname| with index set 
            $[low, low+2]$ initialized to $[x,y,w]$. }*/ 


array(int low, const E& x, const E& y, const E& z, const E& w) 
{ construct(low,low+3);
  vec[0] = x;
  vec[1] = y;
  vec[2] = z;
  vec[3] = w;
}
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname| with index set 
            $[low, low+3]$ initialized to $[x,y,z,w]$. }*/ 


// copy constructor, destructor, operator=

array(const array<E>& A) 
{ //construct(A.l,A.h);
  //for(int i=0; i<sz; i++) vec[i] = A.vec[i];
  l = A.l;
  h = A.h;
  sz = h-l+1;
  vec = (sz > 0) ? LEDA_NEW_VECTOR(E,sz) : 0;
  for(int i=0; i<sz; i++) new(vec+i) E(A.vec[i]);
}

~array()  { destruct(vec,sz); }


array<E>& operator=(const array<E>& A) 
{ if (this != &A)
  { destruct(vec,sz);
    construct(A);
   }
  return *this;
}


bool operator==(const array<E>& A)  const
{ if (l != A.l || h != A.h) return false; 
  for(int i=0; i<sz; i++)
    if (vec[i] != A.vec[i]) return false;
  return true;
}

/*{\Moperations 1.2 5 }*/

/*{\Mtext \headerline{Basic Operations}}*/

const E& operator[](int i) const { 
#if !defined(LEDA_CHECKING_OFF)
if (i<l || i>h) 
    LEDA_EXCEPTION(1,string("array: index %d out of bounds",i));
#endif
return vec[i-l];
}

E& operator[](int i) {
#if !defined(LEDA_CHECKING_OFF)
if (i<l || i>h)
    LEDA_EXCEPTION(1,string("array: index %d out of bounds",i));
#endif
return vec[i-l]; 
}

/*{\Moptions nextwarning=no}*/
/*
E& operator[](int x);
*/
/*{\Marrop     returns $A(x)$.\\
               \precond $a\le x\le b$.  }*/

const E& get(int x ) const { return operator[](x); }

E& get(int x ) { return operator[](x); }
/*{\Mop        returns $A(x)$.\\
               \precond $a\le x\le b$.  }*/

void set(int x, const E& e) { vec[x] = e; }
/*{\Mop        sets $A(x)=e$.\\
               \precond $a\le x\le b$.  }*/

void swap(int i, int j) { leda_swap(vec[i],vec[j]); }
/*{\Mop        swaps the values of |A[i]| and |A[j]|. }*/

void copy(int x, int y) { vec[x] = vec[y]; }
/*{\Mop        sets $A(x)=A(y)$.\\
               \precond $a\le x\le b$ and $low()\le y \le high()$.  }*/

void copy(int x, const array<E>&B, int y) { vec[x] = B[y]; }
/*{\Mop        sets $A(x)=B(y)$.\\
               \precond $a\le x\le b$ and $B.low()\le y \le B.high()$. }*/


void resize(int low, int high) 
{ if (low==l && high==h) return;
  E*  old_vec = vec;
  int old_sz = sz;
  int old_l = l;
  int old_h = h;
  construct(low,high);
  for(int i=low; i<=high; i++)
  { if (i < old_l || i > old_h) continue;
    vec[i-l] = old_vec[i-old_l];
   }
  if (old_vec) destruct(old_vec,old_sz);
}

/*{\Mop        sets the index set of |\Mvar| to $[a..b]$ such that
               for all $i\in[a..b]$ which are not contained in the old 
               index set $A(i)$ is set to the default value of type |E|. }*/

void resize_x(int n) 
{ 
  assert(l == 0 && n >= sz);

  if (n == sz) return;

  cout << "resize: n = " << n << "  sz = " << sz << endl;
  assert(sz == h+1);
  assert(n > sz);
  E* v = LEDA_NEW_VECTOR(E,n);
  char* p = (char*)v;
  char* q = (char*)vec;
  for(size_t i=0;i<sz*sizeof(E); i++) p[i] = q[i];
  for(int i=sz; i<n; i++) new(v+i) E;
  LEDA_DEL_VECTOR(vec);
  vec = v;
  sz = n;
  h = n-1;
}

void resize(int n)  { resize(0,n-1); }
/*{\Mop        same as |\Mvar.resize(0,n-1)|. }*/


int low()  const { return l;}
/*{\Mop        returns the minimal index $a$ of |\Mvar|. }*/


int high() const { return h;}
/*{\Mop        returns the maximal index $b$ of |\Mvar|. }*/

int size() const { return sz;}
/*{\Mop        returns the size ($b-a+1$) of |\Mvar|. }*/

void init(const E& x) { for(int i=0; i<sz; i++) vec[i] = x; }
/*{\Mop  assigns $x$ to |A[i]| for every $i\in \{\ a \dots b\ \}$. }*/

bool C_style() const { return l==0;}
/*{\Mop        returns |true| if the array has ``C-style'', i.e.,
               the index set is $[0..size-1]$. }*/


void permute() { permute(l,h); }
/*{\Mop        the elements of |\Mvar| are randomly permuted.}*/

void permute(int low, int high)
{ if (low < l || low > h || high < l || high > h || low > high)
    LEDA_EXCEPTION(1,"array::permute illegal range");
  E* last = vec+(high-l);
  for(E* p = vec+(low-l); p < last; p++) 
    leda_swap(*p,*(p+rand_int(1,int(last-p))));
}
/*{\Mop        the elements of \Mvar$[low..high]$ are randomly permuted.}*/



/*{\Mtext \headerline{Sorting and Searching}}*/



template<class SMALLER> 
void sort_smaller(const SMALLER& smaller)
{ if (sz > 1) quicksort(vec,vec+sz-1,smaller); }


void sort(const leda_cmp_base<E>& cmp)
{ if (sz > 1) 
  { leda_smaller_cmp_obj<E,leda_cmp_base<E> > smaller(cmp);
    quicksort(vec,vec+sz-1,smaller);
   }
}

void sort(int (*cmp)(const E&, const E&)) 
{ if (sz > 1)
  { leda_smaller_cmp<E> smaller(cmp);
    quicksort(vec,vec+sz-1,smaller);
   }
}
/*{\Mopl       sorts the elements of |\Mvar|, using function $cmp$
	      to compare two elements, i.e., if $(in_a,\dots,in_b)$
	      and $(out_a,\dots,out_b)$ denote the values of the
	      variables $(A(a),\dots,A(b))$ before and after the
	      call of sort, then $cmp(out_i,out_j) \le 0$ for $i\le j$
	      and there is a permutation $\pi$ of $[a..b]$ such that
	      $out_i=in_{\pi(i)}$ for $a \le i \le b$.}*/

void sort0()
{ if (sz > 1)
  { leda_smaller<E> smaller;
    quicksort(vec,vec+sz-1,smaller);
   }
}

void merge_sort() 
{ if (sz > 1)
  { leda_smaller<E> smaller;
    mergesort(vec,vec+sz-1,smaller);
   }
}

void sort() 
{ if (sz > 1)
  { leda_smaller<E> smaller;
    quicksort(vec,vec+sz-1,smaller);
   }
}
/*{\Mop  sorts the elements of |\Mvar| according to the linear order of the 
         element type $E$. \precond A linear order on $E$ must have been 
         defined by $compare(const E\&, const E\&)$ if $E$ is a user-defined 
         type (see Section \ref{User Defined Parameter Types}).. }*/



template<class SMALLER> 
void sort_smaller(const SMALLER& smaller, int low, int high)
{ if (high > low) quicksort(vec+low-l,vec+high-l,smaller); }


void sort(const leda_cmp_base<E>& cmp, int low, int high)
{ leda_smaller_cmp_obj<E,leda_cmp_base<E> > smaller(cmp);
  quicksort(vec+low-l,vec+high-l,smaller);
}

void sort(int (*cmp)(const E&, const E&), int low, int high) 
{ if (high > low)
  { leda_smaller_cmp<E> smaller(cmp);
    quicksort(vec+low-l,vec+high-l,smaller);
   }
}
/*{\Mopl   sorts sub-array \Mvar$[llow..high]$ using compare function $cmp$.}*/


void sort(int low, int high)
{ if (high > low)
  { leda_smaller<E> smaller;
    quicksort(vec+low-l,vec+high-l,smaller);
   }
}
/*{\Mop  sorts sub-array \Mvar$[low..high]$ using the linear order on $E$. If 
         $E$ is a user-defined type, you have to define the linear order by 
         providing the compare function (see Section \ref{User Defined 
         Parameter Types}).}*/


int unique()
{ if (sz == 0) return l-1;
  int i = 0; 
  for (int j = 0; j < sz; j++) {
   if (vec[j] != vec[i]) vec[++i] = vec[j];
  }
  return l+i; 
}
/*{\Mop  removes duplicates from $A$ by copying the unique elements of $A$
         to $A[A.low()],...,A[h]$ and returns $h$ (|A.low()-1| if |A| is 
         empty). \precond $A$ is sorted increasingly according to the default 
         ordering of type $E$. If $E$ is a user-defined type, you have to 
         define the linear order by providing the compare function (see 
         Section \ref{User Defined Parameter Types}).}*/



int binary_search(const leda_cmp_base<E>& cmp, const E& x) const
{ E* p = BINARY_SEARCH_CMP(vec,vec+sz-1,x,cmp);
  return (cmp(*p,x)==0) ? l+int(p-vec) : l-1;
}

int binary_search(int (*cmp)(const E&,const E&), const E& x) const
{ E* p = BINARY_SEARCH_CMP(vec,vec+sz-1,x,cmp);
  return (cmp(*p,x)==0) ? l+int(p-vec) : l-1;
}
/*{\Mopl      performs a binary search for $x$. Returns an $i$
	      with $A[i] = x$ if $x$ in $A$, $A$.low$()-1$
	      otherwise. Function $cmp$ is used to compare
	      two elements.\\
	      \precond $A$ must be sorted
	      according to $cmp$.}*/

int binary_search(const E& x) const 
{ E* p = BINARY_SEARCH_C(vec,vec+sz-1,x);
  return (compare(*p,x)==0) ? l+int(p-vec) : l-1;
}
/*{\Mop       as above but uses the default linear order on $E$. 
              If $E$ is a user-defined type, you have to define the
              linear order by providing the compare 
              function (see Section \ref{User Defined Parameter Types}). }*/



int binary_locate(const leda_cmp_base<E>& cmp, const E& x) const
{ E* p = BINARY_LOCATE_CMP(vec,vec+sz-1,x,cmp);
  return l+int(p-vec); 
}

int binary_locate(int (*cmp)(const E&,const E&), const E& x) const
{ E* p = BINARY_LOCATE_CMP(vec,vec+sz-1,x,cmp);
  return l+int(p-vec); 
}
/*{\Mopl      Returns the maximal $i$ with $A[i] \le x$ or |A.low()|-1 
              if $x < A[low]$. Function $cmp$ is used to compare elements.\\
	      \precond $A$ must be sorted according to $cmp$.}*/


int binary_locate(const E& x) const 
{ E* p = BINARY_LOCATE_C(vec,vec+sz-1,x);
  return l+int(p-vec); 
}
/*{\Mop   as above but uses the default linear order on $E$. If $E$ is a 
          user-defined type, you have to define the linear order by providing 
          the compare function (see Section \ref{User Defined Parameter Types}).}*/




/*{\Mtext \headerline{Input and Output} }*/

void read(istream& I) { for(int i=0; I && i < sz; i++)  I >> vec[i]; }
/*{\Mop       reads $b-a+1$ objects of type $E$ from the
	      input stream $I$ into the array $A$ using the
	      |operator>>(istream&,E&)|.}*/


void read() { read(cin); }
/*{\Mop       calls $A$.read($cin$) to read $A$ from the
	      standard input stream $cin$.}*/



void read(string s) { cout << s; read(cin); }
/*{\Mop        As above, uses string $s$ as prompt.}*/



void print(ostream& O, char space = ' ') const
{ for (int i=0; i<sz; i++) O << space << vec[i]; }
/*{\Mopl      prints the contents of array $A$ to the output
	      stream $O$ using |operator<<(ostream&,const E&)|
	      to print each element. The elements are separated by 
              character $space$.}*/


void print(char space = ' ') const { print(cout,space); }
/*{\Mop       calls $A$.print($cout$, $space$) to print $A$ on 
	      the standard output stream $cout$.}*/



void print(string s, char space = ' ') const { cout << s; print(cout,space); }
/*{\Mopl        As above, uses string $s$ as header.}*/


/*{\Moptions nextwarning=no }*/
/*
friend ostream& operator<<(ostream& out, const array<E>& A);
*/
/*{\Mbinopfunc  same as |\Mvar.print(out)|; returns |out|.}*/


/*{\Moptions nextwarning=no }*/
/*
friend istream& operator>>(istream& in,  array<E>& A);
*/
/*{\Mbinopfunc  same as |\Mvar.read(in))|; returns |in|.}*/


// iteration

 const E& inf(item it) const { return *it; }

 item first_item() const { return vec; }
 item last_item()  const { return vec+sz-1; }
 item next_item(item p) const { return (p && p!=last_item())  ? p+1 : 0;}
 item pred_item(item p) const { return (p && p!=first_item()) ? p-1 : 0;}

 item stl_next_item(item it) const { return it+1; }
 item stl_pred_item(item it) const { return it-1; }




#if defined(LEDA_STL_ITERATORS)

E&       stl_item_access(item it)       { return *it; }
const E& stl_item_access(item it) const { return *it; }

DEFINE_STL_ITERATORS(array<E>,item,E)

//typedef typename array<E>::iterator iterator;

#if !defined(__BORLANDC__ ) && !defined(_MSC_VER) && !defined(__DMC__)
friend iterator operator+(const iterator& iter, int i) 
{ return iterator(iter.L,(E*)iter.it + i); }

friend iterator operator-(const iterator& iter, int i)
{ return iterator(iter.L,(E*)iter.it - i); }

friend int operator-(const iterator& it1, const iterator& it2)
{ return (E*)it1.it - (E*)it2.it; }
#endif


#endif


};

template<class E>
inline ostream& operator<<(ostream& out, const array<E>& A) 
{ A.print(out); 
  return out;
}

template<class E>
inline istream& operator>>(istream& in,  array<E>& A)
{ A.read(in); return in;}


/*{\Mtext \headerline{Iteration}
STL compatible iterators are provided when compiled with 
\texttt{-DLEDA\_STL\_ITERATORS} (see |LEDAROOT/demo/stl/array.c| 
for an example).
}*/

/*{\Mimplementation
Arrays are implemented by \CC vectors. The access operation takes time
$O(1)$, the sorting is realized by quicksort (time $O(n \log n)$) and
the binary\_search operation takes time $O(\log n)$, where $n = b-a+1$.
The space requirement is $O(n* sizeof(E))$.}*/


// specializations

template<> 
inline void array<int>::sort() { INTSORT(vec,sz); }



#if LEDA_ROOT_INCL_ID == 600033
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
