/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  real_matrix.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_REAL_MATRIX_H
#define LEDA_REAL_MATRIX_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600243
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
//  matrices
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/numbers/real_vector.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {real_matrix} {}  {Real-Valued Matrices} }*/

class __exportC real_matrix {

/*{\Mdefinition
An instance of the data type |\Mname| is a matrix of variables of type |real|.  
}*/

  real_vector** v;
  int  d1;
  int  d2;

  void     flip_rows(int,int);
  void     check_dimensions(const real_matrix&) const; 
  real&  elem(int i, int j) const { return v[i]->v[j]; }
  real** triang(const real_matrix&, int&) const;
    
public:

 typedef real        value_type;
 typedef real_vector vector_type;

/*{\Mcreation M }*/

 real_matrix(int n=0, int m=0);

/*{\Mcreate creates an instance $M$ of type |\Mname|, $M$ is initialized to 
           the $n \times m$ - zero matrix. }*/

  real_matrix(int n, int m, real* D);
/*{\Mcreate creates the $n\times m$ matrix $M$ with $M(i,j) = D[i*m +j]$
            for $0 \le i \le n-1 $ and $0 \le j \le m-1$. \precond $D$ 
            points to an array of at least $n*m$ numbers of type |real|. }*/

  real_matrix(const real_matrix&);
  real_matrix(const real_vector&);


  real_matrix& operator=(const real_matrix&);

 ~real_matrix();

  LEDA_MEMORY(real_matrix)


/*{\Moperations 2 4 }*/

int     dim1()  const  {  return d1; }
/*{\Mop  returns $n$, the number of rows of $M$. }*/

int     dim2()  const  {  return d2; }
/*{\Mop  returns $m$, the number of columns of $M$. }*/


real_vector& row(int i) const;
/*{\Mop  returns the $i$-th row of $M$ (an $m$-vector).\\
	 \precond  $0 \le i \le n-1$. }*/

real_vector  col(int i) const;
/*{\Mop  returns the $i$-th column of $M$ (an $n$-vector).\\
	 \precond  $0 \le i \le m-1$. }*/

real_matrix  trans() const;
/*{\Mop  returns  $M^T$ ($m\times n$ - matrix). }*/


real_matrix  inv()   const;
/*{\Mop  returns the inverse matrix of $M$.\\
	 \precond  |\Mvar| is quadratic and |\Mvar|.det() $\neq 0$. }*/

real  det()   const;
/*{\Mop  returns the determinant of $M$.\\
	 \precond  |\Mvar| is quadratic. }*/

/*{\Moptions nextwarning=no}*/
real_matrix solve(const real_matrix&) const;
real_vector solve(const real_vector& b) const { return real_vector(solve(real_matrix(b))); }
/*{\Mop  returns vector $x$ with $M\cdot x = b$.\\
	 \precond $M$.dim1() == $M$.dim2() = =$b$.dim()
	 and $M$.det() $\neq$ 0. }*/


operator real_vector() const; 

real_vector& operator[](int i)    const { return row(i); }

real& operator()(int i, int j);
/*{\Mfunop returns $M_{i,j}$. \\
	  \precond $0\le i\le n-1$ and $0\le j\le m-1$.}*/

real  operator()(int,int) const;

int     operator==(const real_matrix&)    const;
int     operator!=(const real_matrix& x)  const { return !(*this == x); }

real_matrix operator+(const real_matrix& M1) const;
/*{\Mbinop Addition. \\
 	   \precond $M$.dim1() == $M1$.dim1() and $M$.dim2() == $M1$.dim2().}*/

real_matrix operator-(const real_matrix& M1) const;
/*{\Mbinop Subtraction. \\
	   \precond $M$.dim1() == $M1$.dim1() and
	   $M$.dim2() == $M1$.dim2().}*/

real_matrix operator-() const; // unary

real_matrix& operator-=(const real_matrix&);
real_matrix& operator+=(const real_matrix&);

real_matrix operator*(const real_matrix& M1) const;
/*{\Mbinop Multiplication. \\
	   \precond $M$.dim2() == $M1$.dim1().}*/

real_vector operator*(const real_vector& vec) const 
{ return real_vector(*this * real_matrix(vec)); }
/*{\Mbinop  Multiplication with vector.\\
	    \precond $M$.dim2() == $vec$.dim(). }*/

real_matrix operator*(real x) const;
/*{\Mbinop Multiplication with real x.}*/

void print(ostream& O);
/*{\Mop  prints $M$ row by row to ostream $O$. }*/

void print() { print(cout); }
/*{\Mop  prints $M$ $cout$. }*/

void read(istream& I);
/*{\Mop  reads $|M.dim1()| \times |M.dim2()|$ numbers from input stream
         $I$ and writes them row by row into matrix $M$. }*/

void read() { read(cin); }
/*{\Mop  prints $M$ from $cin$. }*/

friend __exportF ostream& operator<<(ostream& O, const real_matrix& M);
/*{\Mbinopfunc  writes matrix $M$ row by row to the output stream $O$. }*/

friend __exportF istream& operator>>(istream& I, real_matrix& M);
/*{\Mbinopfunc  reads a matrix row by row from the input stream $I$
                and assigns it to $M$. }*/

/*{\Mimplementation
Data type |\Mname| is implemented by two-dimensional arrays of real numbers. 
Operations det, solve, and inv take time $O(n^3)$ operations on reals, dim1, 
dim2, row, and col take constant time, all other operations perform $O(nm)$
operations on reals.  
The space requirement is $O(nm)$ plus the space for the $nm$ entries of type
|real|.
}*/

};


inline const char* leda_tname(const real_matrix*) { return "real_matrix"; }


#if LEDA_ROOT_INCL_ID == 600243
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
