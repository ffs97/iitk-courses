/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  real_vector.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_REAL_VECTOR_H
#define LEDA_REAL_VECTOR_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600256
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
//  vectors
//------------------------------------------------------------------------------

#include <LEDA/numbers/real.h>
#include <LEDA/numbers/vector.h>

LEDA_BEGIN_NAMESPACE



/*{\Manpage {real_vector} {} {Real-Valued Vectors}}*/

class __exportC real_vector
{

/*{\Mdefinition
An instance of data type |real_vector| is a vector of variables of type |real|.}*/

  friend class __exportC real_matrix;

  real* v;
  int d;

  void check_dimensions(const real_vector&) const;
 
public:

/*{\Mcreation v }*/

real_vector(); 
/*{\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
            the zero-dimensional vector.}*/

explicit 

real_vector(int d); 
/*{\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
            the zero vector of dimension $d$.}*/ 

real_vector(real a, real b);
/*{\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
            the two-dimensional vector $(a,b)$.}*/

real_vector(real a, real b, real c);
/*{\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
            the three-dimensional vector $(a,b,c)$.}*/

real_vector(double a, double b);
/*{\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
            the two-dimensional vector $(a,b)$.}*/

real_vector(double a, double b, double c);
/*{\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
            the three-dimensional vector $(a,b,c)$.}*/

real_vector(const vector& w, int prec = 0);
/* {\Mcreate creates an instance $v$ of type |\Mname|; $v$ is initialized to 
           a copy of $w$. The second argument is for 
           compatibility with |rat_vector|. }*/


  real_vector(const real_vector&);
 ~real_vector(); 
  real_vector& operator=(const real_vector&);


/*{\Moperations 2 5.0}*/

int    dim()    const { return d; }
/*{\Mop       returns the dimension of $v$.}*/ 

  
real& operator[](int i);
/*{\Marrop     returns $i$-th component of $v$.\\
	       \precond $0\le i \le v$.dim()$-$1.}*/
  
real  operator[](int) const;

real  hcoord(int i) const { return (i<d) ? (*this)[i] : real(1); }
/*{\Mop for compatibility with |rat_vector|. }*/

real  coord(int i)  const { return (*this)[i]; }
/*{\Mop for compatibility with |rat_vector|. }*/


real sqr_length() const;
/*{\Mop      returns the square of the Euclidean length of $v$.}*/

real length() const;
/*{\Mop      returns the Euclidean length of $v$.}*/
  
real_vector norm() const { return *this/length(); }
/*{\Mop      returns |\Mvar| normalized. }*/
  
real_vector rotate90(int i=1) const;
/*{\Mopl    returns |\Mvar| by an angle of $i\times 90$ degrees. 
            If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. \precond $v.dim() = 2$}*/

real_vector& operator+=(const real_vector&);
real_vector& operator-=(const real_vector&);
  
real_vector  operator+(const real_vector& v1) const;
/*{\Mbinop     Addition.\\
	       \precond $v$.dim() = $v1$.dim().}*/

real_vector  operator-(const real_vector& v1) const;
/*{\Mbinop     Subtraction.\\
	       \precond $v$.dim() = $v1$.dim().}*/

real  operator*(const real_vector& v1) const;
/*{\Mbinop     Scalar multiplication.\\
               \precond $v$.dim() = $v1$.dim().}*/

real_vector& operator*=(real r);
/*{\Mbinop  multiplies all coordinates by |r|.}*/

real_vector  operator*(real r)        const;
/*{\Mbinop     Componentwise multiplication with real $r$.}*/

real_vector  operator-() const;
real_vector  operator/(real)        const;
real_vector& operator/=(real r);

bool     operator==(const real_vector& w) const;
/*{\Mbinop     Test for equality.}*/

bool     operator!=(const real_vector& w)  const { return !(*this == w); }
/*{\Mbinop     Test for inequality.}*/

friend real_vector operator*(real f, const real_vector& v) { return v *f; }

void print(ostream& O);
/*{\Mop  prints $v$ componentwise to ostream $O$. }*/

void print() { print(cout); }
/*{\Mop  prints $v$ to $cout$. }*/

void read(istream& I);
/*{\Mop  reads $d = |v.dim()|$ numbers from input stream $I$ and writes 
         them into $v[0] \dots v[d-1]$.}*/

void read() { read(cin); }
/*{\Mop  reads $v$ from $cin$. }*/
  
friend __exportF ostream& operator<<(ostream& O, const real_vector& v);
/*{\Mbinopfunc  writes $v$ componentwise to the output stream $O$.}*/

friend __exportF istream& operator>>(istream& I, real_vector& v);
/*{\Mbinopfunc  reads $v$ componentwise from the input stream $I$.}*/

vector    to_vector() const;

vector    to_float() const { return to_vector(); }
/*{\Mop returns a floating point approximation of |\Mvar|. }*/


/*{\Mtext {\bf Additional Operations for vectors in
two and three-dimensional space}}*/

real xcoord()const {return coord(0);}
/*{\Mop     returns the zero-th cartesian coordinate of |\Mvar|.}*/

real ycoord()const {return coord(1);}
/*{\Mop     returns the first cartesian coordinate of |\Mvar|.}*/

real zcoord()const {return coord(2);}
/*{\Mop     returns the second cartesian coordinate of |\Mvar|.}*/

friend __exportF int compare_by_angle(const real_vector& v1, const real_vector& v2);
/*{\Mfunc For a non-zero vector $v$ let $\alpha(v)$ be the angle by which the positive
$x$-axis has to be turned counter-clockwise until it aligns with $v$. The
function compares the angles defined by |v1| and |v2|, respectively. The 
zero-vector precedes all non-zero vectors in the angle-order.}*/


friend __exportF real_vector cross_product(const real_vector& v1, 
                                      const real_vector& v2);
/*{\Mfunc returns the cross product of the three-dimensional vectors |v1| and |v2|.}*/



LEDA_MEMORY(real_vector)

};


inline const char* leda_tname(const real_vector*) { return "real_vector"; }


/*{\Mimplementation
Vectors are implemented by arrays of real numbers. All operations on a vector 
$v$ take $O(v.dim())$ real-number operations, except for dim and $[\ ]$ which take constant
time. The space requirement depends on the size of the representations of the coordinates.
}*/


#if LEDA_ROOT_INCL_ID == 600256
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
