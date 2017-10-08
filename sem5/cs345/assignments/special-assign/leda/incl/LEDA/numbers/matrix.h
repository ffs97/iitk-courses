/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  matrix.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_MATRIX_H
#define LEDA_MATRIX_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600254
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
//  matrices
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/numbers/vector.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {matrix} {}  {Double-Valued Matrices} }*/

class __exportC matrix {

/*{\Mdefinition
An instance of the data type |matrix| is a matrix of variables of type |double|.  
}*/

  vector** v;
  int  d1;
  int  d2;

  void     flip_rows(int,int);
  void     check_dimensions(const matrix&) const; 
  double&  elem(int i, int j) const { return v[i]->v[j]; }
  double** triang(const matrix&, int&) const;
    
public:

 matrix triang1(const matrix&) const;


 typedef double value_type;
 typedef vector vector_type;

/*{\Mcreation M }*/

 matrix(int n=0, int m=0);

/*{\Mcreate creates an instance $M$ of type $matrix$, $M$ is initialized to 
           the $n \times m$ - zero matrix. }*/

  matrix(int n, int m, double* D);
/*{\Mcreate creates the $n\times m$ matrix $M$ with $M(i,j) = D[i*m +j]$
            for $0 \le i \le n-1 $ and $0 \le j \le m-1$. \precond $D$ 
            points to an array of at least $n*m$ numbers of type |double|. }*/

  matrix(const matrix&);
  matrix(const vector&);


  matrix& operator=(const matrix&);

 ~matrix();

  LEDA_MEMORY(matrix)


/*{\Moperations 2 4 }*/

int     dim1()  const  {  return d1; }
/*{\Mop  returns $n$, the number of rows of $M$. }*/

int     dim2()  const  {  return d2; }
/*{\Mop  returns $m$, the number of columns of $M$. }*/


vector& row(int i) const;
/*{\Mop  returns the $i$-th row of $M$ (an $m$-vector).\\
	 \precond  $0 \le i \le n-1$. }*/

vector  col(int i) const;
/*{\Mop  returns the $i$-th column of $M$ (an $n$-vector).\\
	 \precond  $0 \le i \le m-1$. }*/

matrix  trans() const;
/*{\Mop  returns  $M^T$ ($m\times n$ - matrix). }*/


matrix  inv()   const;
/*{\Mop  returns the inverse matrix of $M$.\\
	 \precond  |\Mvar| is quadratic and |\Mvar|.det() $\neq 0$. }*/

double  det()   const;
/*{\Mop  returns the determinant of $M$.\\
	 \precond  $M$ is quadratic. }*/

/*{\Moptions nextwarning=no}*/
matrix solve(const matrix&) const;
vector solve(const vector& b) const { return vector(solve(matrix(b))); }
/*{\Mop  returns vector $x$ with $M\cdot x = b$.\\
	 \precond $M$.dim1() == $M$.dim2() = =$b$.dim()
	 and $M$.det() $\neq$ 0. }*/


operator vector() const; 

vector& operator[](int i)    const { return row(i); }

double& operator()(int i, int j);
/*{\Mfunop returns $M_{i,j}$. \\
	  \precond $0\le i\le n-1$ and $0\le j\le m-1$.}*/

double  operator()(int,int) const;

int     operator==(const matrix&)    const;
int     operator!=(const matrix& x)  const { return !(*this == x); }

matrix operator+(const matrix& M1) const;
/*{\Mbinop Addition. \\
 	   \precond $M$.dim1() == $M1$.dim1() and $M$.dim2() == $M1$.dim2().}*/

matrix operator-(const matrix& M1) const;
/*{\Mbinop Subtraction. \\
	   \precond $M$.dim1() == $M1$.dim1() and
	   $M$.dim2() == $M1$.dim2().}*/

matrix operator-() const; // unary

matrix& operator-=(const matrix&);
matrix& operator+=(const matrix&);

matrix operator*(const matrix& M1) const;
/*{\Mbinop Multiplication. \\
	   \precond $M$.dim2() == $M1$.dim1().}*/

vector operator*(const vector& vec) const 
{ return vector(*this * matrix(vec)); }
/*{\Mbinop  Multiplication with vector.\\
	    \precond $M$.dim2() == $vec$.dim(). }*/

matrix operator*(double x) const;
/*{\Mbinop Multiplication with double x.}*/

void print(ostream& O);
/*{\Mop  prints $M$ row by row to ostream $O$. }*/

void print() { print(cout); }
/*{\Mop  prints $M$ $cout$. }*/

void read(istream& I);
/*{\Mop  reads $|M.dim1()| \times |M.dim2()|$ numbers from input stream
         $I$ and writes them row by row into matrix $M$. }*/

void read() { read(cin); }
/*{\Mop  prints $M$ from $cin$. }*/

friend __exportF ostream& operator<<(ostream& O, const matrix& M);
/*{\Mbinopfunc  writes matrix $M$ row by row to the output stream $O$. }*/

friend __exportF istream& operator>>(istream& I, matrix& M);
/*{\Mbinopfunc  reads a matrix row by row from the input stream $I$
                and assigns it to $M$. }*/

/*{\Mimplementation
Data type $matrix$ is implemented by two-dimensional arrays of double numbers. 
Operations det, solve, and inv take time $O(n^3)$, dim1, dim2, row, and col 
take constant time, all other operations take time $O(nm)$.  
The space requirement is $O(nm)$.

Be aware that the operations on vectors and matrices incur rounding error and
hence are not completely reliable. For example, if $M$ is a matrix, $b$ is a
vector, and $x$ is computed by |x = M.solve(b)| it is not necessarly true that
the test |b == M * b| evaluates to true. The types |integer_vector| and |integer_matrix| provide exact linear algebra.

}*/

};


inline const char* leda_tname(const matrix*) { return "matrix"; }


#if LEDA_ROOT_INCL_ID == 600254
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
