/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  vector.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_VECTOR_H
#define LEDA_VECTOR_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600248
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
//  vectors
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE



/*{\Manpage {vector} {} {Double-Valued Vectors}}*/

class __exportC vector
{

/*{\Mdefinition
An instance of data type |vector| is a vector of variables of type |double|.}*/

  friend class __exportC matrix;

  double* v;
  int d;

  void check_dimensions(const vector&) const;
 
public:

/*{\Mcreation v }*/

vector(); 
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the zero-dimensional vector.}*/

explicit 

vector(int d); 
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the zero vector of dimension $d$.}*/ 

vector(double a, double b);
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the two-dimensional vector $(a,b)$.}*/

vector(double a, double b, double c);
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
            the three-dimensional vector $(a,b,c)$.}*/

  vector(const vector& w, int prec);
/*{\Mcreate creates an instance $v$ of type $vector$; $v$ is initialized to 
           a copy of $w$. The second argument is for 
           compatibility with |rat_vector|. }*/


  vector(const vector&);
 ~vector(); 
  vector& operator=(const vector&);


/*{\Moperations 2 5.0}*/

int    dim()    const { return d; }
/*{\Mop       returns the dimension of $v$.}*/ 

  
double& operator[](int i);
/*{\Marrop     returns $i$-th component of $v$.\\
	       \precond $0\le i \le v$.dim()$-$1.}*/
  
double  operator[](int) const;

double  hcoord(int i) const { return (i<d) ? (*this)[i] : 1; }
/*{\Mop for compatibility with |rat_vector|. }*/

double  coord(int i)  const { return (*this)[i]; }
/*{\Mop for compatibility with |rat_vector|. }*/

vector  to_vector() const { return *this; }

vector  to_float() const { return *this; }

double sqr_length() const;
/*{\Mop      returns the square of the Euclidean length of $v$.}*/

double length() const;
/*{\Mop      returns the Euclidean length of $v$.}*/
  
vector norm() const { return *this/length(); }
/*{\Mop      returns |\Mvar| normalized. }*/
  
double angle(const vector& w) const; 
/*{\Mop     returns the angle between $v$ and $w$.}*/

vector rotate90(int i=1) const;
/*{\Mopl    returns |\Mvar| by an angle of $i\times 90$ degrees. 
            If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. \precond $v.dim() = 2$}*/

vector rotate(double a) const;
/*{\Mop     returns the |\Mvar| rotated counterclockwise by an angle of $a$ (in radian).\\
	    \precond $v.dim() = 2$}*/

vector& operator+=(const vector& v1);
/*{\Mbinop         Addition and assign.\\
                \precond $v$.dim() = $v1$.dim().}*/

vector& operator-=(const vector& v1);
/*{\Mbinop         Subtraction and assign.\\
                \precond $v$.dim() = $v1$.dim().}*/  

vector  operator+(const vector& v1) const;
/*{\Mbinop     Addition.\\
	       \precond $v$.dim() = $v1$.dim().}*/

vector  operator-(const vector& v1) const;
/*{\Mbinop     Subtraction.\\
	       \precond $v$.dim() = $v1$.dim().}*/

double  operator*(const vector& v1) const;
/*{\Mbinop     Scalar multiplication.\\
               \precond $v$.dim() = $v1$.dim().}*/

vector  operator*(double r)        const;
/*{\Mbinop     Componentwise multiplication with double $r$.}*/

vector& operator*=(double r);
/*{\Mbinop  multiplies all coordinates by |r|.}*/

vector  operator-() const;

vector  operator/(double r)        const;
/*{\Mbinop      Componentwise division which double $r$.}*/

vector& operator/=(double r);

bool     operator==(const vector& w) const;
/*{\Mbinop     Test for equality.}*/

bool     operator!=(const vector& w)  const { return !(*this == w); }
/*{\Mbinop     Test for inequality.}*/

friend vector operator*(double f, const vector& v) { return v *f; }

void print(ostream& O);
/*{\Mop  prints $v$ componentwise to ostream $O$. }*/

void print() { print(cout); }
/*{\Mop  prints $v$ to $cout$. }*/

void read(istream& I);
/*{\Mop  reads $d = |v.dim()|$ numbers from input stream $I$ and writes 
         them into $v[0] \dots v[d-1]$.}*/

void read() { read(cin); }
/*{\Mop  reads $v$ from $cin$. }*/
  
friend __exportF ostream& operator<<(ostream& O, const vector& v);
/*{\Mbinopfunc  writes $v$ componentwise to the output stream $O$.}*/

friend __exportF istream& operator>>(istream& I, vector& v);
/*{\Mbinopfunc  reads $v$ componentwise from the input stream $I$.}*/



/*{\Mtext {\bf Additional Operations for vectors in
two and three-dimensional space}}*/

double xcoord()const {return coord(0);}
/*{\Mop     returns the zero-th cartesian coordinate of |\Mvar|.}*/

double ycoord()const {return coord(1);}
/*{\Mop     returns the first cartesian coordinate of |\Mvar|.}*/

double zcoord()const {return coord(2);}
/*{\Mop     returns the second cartesian coordinate of |\Mvar|.}*/

friend __exportF int compare_by_angle(const vector& v1, const vector& v2);
/*{\Mfunc For a non-zero vector $v$ let $\alpha(v)$ be the angle by which the positive
$x$-axis has to be turned counter-clockwise until it aligns with $v$. The
function compares the angles defined by |v1| and |v2|, respectively. The 
zero-vector precedes all non-zero vectors in the angle-order.}*/


friend __exportF vector cross_product(const vector& v1, 
                                      const vector& v2);
/*{\Mfunc returns the cross product of the three-dimensional vectors |v1| and |v2|.}*/



LEDA_MEMORY(vector)

};


inline const char* leda_tname(const vector*) { return "vector"; }


/*{\Mimplementation
Vectors are implemented by arrays of real numbers. All operations on a vector 
$v$ take time $O(v.dim())$, except for dim and $[\ ]$ which take constant
time. The space requirement is $O(v.dim())$.

Be aware that the operations on vectors and matrices incur rounding errors and
hence are not completely reliable. For example, if $M$ is a matrix, $b$ is a
vector, and $x$ is computed by |x = M.solve(b)| it is not necessarily true that
the test |b == M * x| evaluates to true. The types |integer_vector| and |integer_matrix| provide exact linear algebra.
}*/


#if LEDA_ROOT_INCL_ID == 600248
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
