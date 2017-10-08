/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  array2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_ARRAY2_H
#define LEDA_ARRAY2_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600042
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/array.h>

//--------------------------------------------------------------------------
// 2 dimensional arrays                                                   
//--------------------------------------------------------------------------


LEDA_BEGIN_NAMESPACE

/*{\Manpage {array2} {E} {Two Dimensional Arrays} }*/


template<class E> 

class array2 {

/*{\Mdefinition
An instance $A$ of the parameterized data type |\Mname| is a mapping from a 
set of pairs $I = [a..b] \times [c..d]$, called the index set of $A$, to the 
set of variables of data type $E$, called the element type of $A$, for two 
fixed intervals of integers $[a..b]$ and $[c..d]$.  $A(i,j)$ is called the 
element at position $(i,j)$.
}*/

  int l1;
  int h1;
  int l2;
  int h2;

  int dim1;
  int dim2;
  int sz;

  E*  vec;


public:

/*{\Mcreation  A }*/

array2(int a, int b, int c, int d) : l1(a), h1(b), l2(c), h2(d)
{ dim1 = b-a+1;
  dim2 = d-c+1;
  sz = dim1*dim2;
  vec = LEDA_NEW_VECTOR(E,sz);
  for(int i=0; i<sz; i++) new(vec+i) E;
 }

/*{\Mcreate      creates an instance |\Mvar| of type |\Mname| with index set 
                 $[a..b]\times [c..d]$.  }*/

array2(int n, int m) : l1(0),h1(n-1),l2(0),h2(m-1),dim1(n),dim2(m),sz(n*m)
{ vec = LEDA_NEW_VECTOR(E,sz);
  for(int i=0; i<sz; i++) new(vec+i) E;
 }
/*{\Mcreate      creates an instance |\Mvar| of type |\Mname| with index set 
                 $[0..n-1]\times [0..m-1]$.  }*/


array2(const array2& A) : l1(A.l1),h1(A.h1),l2(A.l2),h2(A.h2),dim1(A.dim1),dim2(A.dim2), sz(A.sz)
{ vec = LEDA_NEW_VECTOR(E,sz);
  for(int i=0; i<sz; i++) new(vec+i) E(A.vec[i]);
cout << "sz = " << sz <<endl;
}


~array2() 
{ for(int i=0; i<sz; i++) vec[i].~E();
  LEDA_DEL_VECTOR(vec); 
}


array2<E>& operator=(const array2<E>& A) 
{ if (this != &A)
  { // destroy old vector
    for(int i=0; i<sz; i++) vec[i].~E();
    LEDA_DEL_VECTOR(vec); 
    // construct new vector
    l1 = A.l1; h1 = A.h1;
    l2 = A.l2; h2 = A.h2;
    dim1 = A.dim1;
    dim2 = A.dim2;
    sz = A.sz;
    vec = LEDA_NEW_VECTOR(E,sz);
    for(int i=0; i<sz; i++) new(vec+i) E(A.vec[i]);
   }
  return *this;
}



/*{\Moperations 1.5 5 }*/

void init(const E& x)
{ for(int i=0; i<sz; i++) vec[i] = x; }
/*{\Mop       assigns $x$ to each element of $A$. }*/


E& operator()(int i, int j)  {
#if !defined(LEDA_CHECKING_OFF)
  if (i < l1 || i > h1 || j < l2 || j > h2) 
    LEDA_EXCEPTION(99,string("array2[%d,%d]: index out of bounds",i,j));
#endif
  return vec[(i-l1)*dim2+(j-l2)]; 
}

/*{\Mfunop      returns $A(i,j)$.\\
        	\precond $a\le i\le b$ and $c\le j\le d$.}*/

const E& operator()(int i, int j) const {
#if !defined(LEDA_CHECKING_OFF)
  if (i < l1 || i > h1 || j < l2 || j > h2) 
    LEDA_EXCEPTION(99,string("array2[%d,%d]: index out of bounds",i,j));
#endif
  return vec[(i-l1)*dim2+(j-l2)]; 
}


int low1() const { return l1; }
/*{\Mop         returns $a$. }*/

int high1() const {return h1; }
/*{\Mop         returns $b$. }*/

int low2() const {return l2;  }
/*{\Mop         returns $c$. }*/

int high2() const {return h2; }
/*{\Mop         returns $d$. }*/

};

/*{\Mimplementation
Two dimensional arrays are implemented by \CC vectors. All operations
take time $O(1)$, the space requirement is $O(|I|* sizeof(E))$.
}*/


#if LEDA_ROOT_INCL_ID == 600042
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

