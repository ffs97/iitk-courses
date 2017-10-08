/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_vector.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_VECTOR_H
#define LEDA_RAT_VECTOR_H 


#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600250
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/array.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/rational.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/numbers/geo_rep.h> 
#include <LEDA/numbers/vector.h> 

LEDA_BEGIN_NAMESPACE

/*{\Manpage {rat_vector} {} {Rational Vectors} {v}}*/

class __exportC rat_vector:public HANDLE_BASE(geo_rep){

/*{\Mdefinition

An instance of data type |\Mname| is a vector of rational
numbers. A $d$-dimensional vector $r = (r_0, \ldots ,r_{d-1})$ is
represented in homogeneous coordinates $(h_0, \ldots ,h_d)$, where $r_i
= h_i/h_d$ and the $h_i$'s are of type |integer|. We call the $r_i$'s
the cartesian coordinates of the vector. The homogenizing coordinate
$h_d$ is positive.

This data type is meant for use in computational geometry. It realizes
free vectors as opposed to position vectors (type |rat_point|). The
main difference between position vectors and free vectors is their
behavior under affine transformations, e.g., free vectors are
invariant under translations.

|rat_vector| is an item type.}*/


geo_rep* ptr() const { return (geo_rep*)PTR; }


rat_vector(const HANDLE_BASE(geo_rep)&p):HANDLE_BASE(geo_rep)(p){}

friend class __exportC rat_point;

void print(ostream&)const;
void read(istream&);

public:

/*{\Mcreation 6}*/

explicit

rat_vector(int d=2);
/*{\Mcreate introduces a variable |\Mvar| of type |rat_vector| initialized
            to the zero vector of dimension $d$.}*/

rat_vector(integer a,integer b,integer D);
/*{\Mcreate introduces a variable  |\Mvar| of type |rat_vector|
            initialized to the two-dimensional vector with homogeneous
            representation $(a,b,D)$ if $D$ is positive and representation
            $(-a,-b,-D)$ if $D$ is negative.\\
            \precond $D$ is non-zero.}*/

rat_vector(rational x, rational y);
/*{\Mcreate introduces a variable  |\Mvar| of type |rat_vector|
            initialized to the two-dimensional vector with homogeneous
            representation $(a,b,D)$, where $x = a/D$ and $y = b/D$.}*/

rat_vector(integer a, integer b, integer c,integer D);
/*{\Mcreate introduces a variable  |\Mvar| of type |rat_vector|
            initialized to the three-dimensional vector with homogeneous
            representation $(a,b,c,D)$ if $D$ is positive and representation
            $(-a,-b,-c,-D)$ if $D$ is negative.\\
            \precond $D$ is non-zero.}*/

rat_vector(rational x, rational y, rational z);
/*{\Mcreate introduces a variable  |\Mvar| of type |rat_vector|
            initialized to the three-dimensional vector with homogeneous
            representation $(a,b,c,D)$, where $x = a/D$, $y = b/D$
and $z = c/D$.}*/


rat_vector(const array<rational>& A);
/*{\Mcreate introduces a variable  |\Mvar| of type |rat_vector|
            initialized to the d-dimensional vector with homogeneous
            coordinates $(\pm c_0,\ldots,\pm c_{d-1},\pm D)$,
            where $d = A.size()$ and $A[i] = c_i/D$, for
            $i = 0, \ldots , d-1$.}*/


rat_vector(integer a,integer b) { PTR = new geo_rep(a,b,1); }
/*{\Mcreate introduces a variable  |\Mvar| of type |rat_vector|
            initialized to the two-dimensional vector with homogeneous
            representation $(a,b,1)$.}*/

rat_vector(const integer_vector&c,integer D);
/*{\Mcreate introduces a variable |\Mvar| of type |rat_vector|
            initialized to the vector with homogeneous
            coordinates $(\pm c_0,\ldots,\pm c_{d-1},\pm D)$,
            where $d$ is the dimension of $c$ and the sign
            chosen is the sign of $D$.\\
            \precond $D$ is non-zero.}*/

rat_vector(const integer_vector&c);
/*{\Mcreate introduces a variable |\Mvar| of type |rat_vector|
            initialized to the direction with homogeneous coordinate
            vector $\pm c$, where the sign chosen is the sign of the
            last component of $c$. \\
            \precond The last component of $c$ is non-zero.}*/

 rat_vector(const vector& w, int prec);
/*{\Mcreate introduces a variable |\Mvar| of type |rat_vector| initialized to 
            $(\lfloor P*w_0 \rfloor,\ldots,\lfloor P*w_{d-1}\rfloor,P)$,
            where $d$ is the dimension of $w$ and $P = 2^{\mathit prec}$.}*/


 rat_vector(const rat_vector&p):HANDLE_BASE(geo_rep)(p){}
~rat_vector(){}

rat_vector&operator= (const rat_vector&p)
{HANDLE_BASE(geo_rep)::operator= (p);return*this;}


/*{\Moperations 3 5.0}*/
/*{\Mtext {\bf 3.1 Initialization, Access and Conversions}}*/

static rat_vector d2(integer a,integer b, integer D);
/*{\Mstatic returns a |rat_vector| of dimension $2$ initialized to a
            vector with homogeneous representation $(a,b,D)$ if $D$
            is positive and representation $(-a,-b,-D)$ if $D$ is
            negative.\\
            \precond $D$ is non-zero.}*/

static rat_vector d3(integer a,integer b,integer c, integer D);
/*{\Mstatic returns a |rat_vector| of dimension $3$ initialized to a
            vector with homogeneous representation $(a,b,c,D)$ if $D$
            is positive and representation $(-a,-b,-c,-D)$ if $D$ is
            negative.\\
            \precond $D$ is non-zero.}*/

static rat_vector unit(int i,int d=2);
/*{\Mstatic  returns a |rat_vector| of dimension $d$ initialized
            to the $i$-th unit vector.\\
            \precond $0 \le i < d$.}*/

static rat_vector zero(int d=2);
/*{\Mstatic  returns the zero vector in d-dimensional space.}*/

int dim() const { return ptr()->dim; }
/*{\Mop     returns the dimension of |\Mvar|.}*/

integer   hcoord(int i) const { return ptr()->v[i];}
/*{\Mop     returns the $i$-th homogeneous coordinate of |\Mvar|.}*/

rational  coord(int i) const  { return rational(hcoord(i),hcoord(dim())); }
/*{\Mop     returns the $i$-th cartesian coordinate of |\Mvar|.}*/

rational  operator[](int i) const { return coord(i); }
/*{\Marrop  returns the $i$-th cartesian coordinate of |\Mvar|.}*/


rational  sqr_length() const;
/*{\Mop     returns the square of the length of |\Mvar|. }*/

vector    to_vector() const;

vector    to_float() const { return to_vector(); }
/*{\Mop     returns a floating point approximation of |\Mvar|. }*/


/*{\Mtext {\bf Additional Operations for vectors in
two and three-dimensional space}}*/

rational xcoord()const {return coord(0);}
/*{\Mop     returns the zero-th cartesian coordinate of |\Mvar|.}*/

rational ycoord()const {return coord(1);}
/*{\Mop     returns the first cartesian coordinate of |\Mvar|.}*/

rational zcoord()const {return coord(2);}
/*{\Mop     returns the second cartesian coordinate of |\Mvar|.}*/

integer X() const {return hcoord(0);}
/*{\Mop     returns the zero-th homogeneous coordinate of |\Mvar|.}*/

integer Y() const {return hcoord(1);}
/*{\Mop     returns the first homogeneous coordinate of |\Mvar|.}*/

integer Z() const {return hcoord(2);}
/*{\Mop     returns the second homogeneous coordinate of |\Mvar|.}*/

integer W() const {return hcoord(ptr()->dim);}
/*{\Mop     returns the homogenizing coordinate of |\Mvar|.}*/


rat_vector rotate90(int i=1) const;
/*{\Mopl    returns |\Mvar| by an angle of $i\times 90$ degrees. 
            If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise.  \precond |\Mvar.dim() == 2|.}*/

friend __exportF int compare_by_angle(const rat_vector& v1, 
                                      const rat_vector& v2);
/*{\Mfunc For a non-zero vector $v$ let $\alpha(v)$ be the angle by which the positive
$x$-axis has to be turned counter-clockwise until it aligns with $v$. The
function compares the angles defined by |v1| and |v2|, respectively. The 
zero-vector precedes all non-zero vectors in the angle-order.}*/

friend __exportF rat_vector cross_product(const rat_vector& v1, 
                                          const rat_vector& v2);
/*{\Mfunc returns the cross product of the three-dimensional vectors |v1| and |v2|.}*/



/*{\Mtext {\bf 3.2 Tests}}*/

int cmp(const rat_vector&x,const rat_vector&y)const
{ return(identical(x,y) ? 0 : x.ptr()->cmp_rat_coords(x.ptr(),y.ptr())); }


bool operator==(const rat_vector&w)const
{return cmp(*this,w)==0;}
/*{\Mbinop     Test for equality.}*/

bool operator!=(const rat_vector&w)const
{return cmp(*this,w)!=0;}
/*{\Mbinop     Test for inequality.}*/

/*{\Mtext {\bf 3.3 Arithmetical Operators}}*/

rat_vector scale(integer m,integer n)const;
void  self_scale(integer m,integer n);

friend __exportF rat_vector operator*(int n,const rat_vector&v);

friend __exportF rat_vector operator*(integer n,const rat_vector&v);
/*{\Mbinopfunc   multiplies all cartesian coordinates by |n|.   }*/

friend __exportF rat_vector operator*(rational r,const rat_vector&v);
/*{\Mbinopfunc    multiplies all cartesian coordinates by |r|.   }*/

rat_vector& operator*= (integer n);
/*{\Mbinop  multiplies all cartesian coordinates by |n|.   }*/
rat_vector& operator*= (int n);

rat_vector& operator*= (rational r);
/*{\Mbinop  multiplies all cartesian coordinates by |r|.   }*/

friend __exportF rat_vector operator/(const rat_vector&v,int n);

friend __exportF rat_vector operator/(const rat_vector&v,integer n);
/*{\Mbinopfunc    divides all cartesian coordinates by $n$.}*/

friend __exportF rat_vector operator/(const rat_vector&v,rational r);
/*{\Mbinopfunc    divides all cartesian coordinates by $r$.}*/

rat_vector&operator/= (integer n);
/*{\Mbinop  divides all cartesian coordinates by |n|.   }*/
rat_vector&operator/= (int n);

rat_vector&operator/= (rational r);
/*{\Mbinop  divides all cartesian coordinates by |r|.   }*/

friend __exportF rational operator*(const rat_vector v,const rat_vector&w);
/*{\Mbinopfunc  scalar product, i.e., $\sum_{0 \le i < d} v_i w_i$,
                where $v_i$ and $w_i$ are the cartesian coordinates
                of $v$ and $w$ respectively. }*/

friend __exportF rat_vector operator+(const rat_vector&v,const rat_vector&w);
/*{\Mbinopfunc   adds cartesian coordinates.   }*/

rat_vector& operator+= (const rat_vector&w);
/*{\Mbinop       addition plus assignment.   }*/

friend __exportF rat_vector operator-(const rat_vector&v,const rat_vector&w);
/*{\Mbinopfunc    subtracts cartesian coordinates. }*/

rat_vector& operator-= (const rat_vector&w);
/*{\Mbinop       subtraction plus assignment. }*/

rat_vector operator-() const;
/*{\Munop    returns -|\Mvar|.}*/


/*{\Mtext {\bf 3.4 Input and Output}}*/

friend ostream&operator<<(ostream&O,const rat_vector&v)
{v.print(O);return O;}
/*{\Mbinopfunc  writes $v$'s homogeneous coordinates
                component\-wise to the output stream $O$.}*/

friend istream&operator>>(istream&I,rat_vector&v)
{v.read(I);return I;}
/*{\Mbinopfunc  reads $v$'s homogeneous coordinates
                componentwise from the input stream $I$.
                The operator uses the current dimension of |v|.}*/


};

COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const rat_vector&p,const rat_vector&q)
{return p.cmp(p,q);}



/*{\Mtext {\bf 3.5 Linear Hull, Dependence and Rank}}*/

extern __exportF bool contained_in_linear_hull(const array<rat_vector>&A,
const rat_vector&x);
/*{\Mfuncl determines whether $x$ is contained in the linear hull
           of the vectors in $A$.}*/

extern __exportF int linear_rank(const array<rat_vector>&A);
/*{\Mfuncl computes the linear rank of the vectors in $A$.}*/

extern __exportF bool linearly_independent(const array<rat_vector>&A);
/*{\Mfuncl decides whether the vectors in $A$ are linearly independent.}*/

extern __exportF array<rat_vector> linear_base(const array<rat_vector>&A);
/*{\Mfuncl computes a basis of the linear space spanned by the vectors
           in $A$.}*/


/*{\Mimplementation
Vectors are implemented by arrays of integers as an item type.  All
operations like creation, initialization, tests, vector arithmetic,
input and output on an vector $v$ take time $O(|v.dim()|)$. |dim()|,
coordinate access and conversions take constant time.  The operations
for linear hull, rank and independence have the cubic costs of the
used matrix operations.  The space requirement is $O(|v.dim()|)$.  }*/


#if LEDA_ROOT_INCL_ID == 600250
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif



