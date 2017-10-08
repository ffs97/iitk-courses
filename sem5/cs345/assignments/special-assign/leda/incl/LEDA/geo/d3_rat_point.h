/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_rat_point.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




#ifndef LEDA_D3_RAT_POINT_H
#define LEDA_D3_RAT_POINT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600079
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/rational.h>
#include <LEDA/numbers/rat_vector.h>
#include <LEDA/geo/d3_point.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_rat_point;


//------------------------------------------------------------------------------
// d3_rat_points
//------------------------------------------------------------------------------

class __exportC d3_rat_point_rep  : public handle_rep {

friend class __exportC d3_rat_point;

static atomic_counter id_counter;
   
   integer x;
   integer y;
   integer z;
   integer w;

   double  xd;
   double  yd;
   double  zd;
   double  wd;

   unsigned long id;

public:

   d3_rat_point_rep();
   d3_rat_point_rep(const rational&, const rational&, const rational&);
   d3_rat_point_rep(integer, integer, integer);
   d3_rat_point_rep(integer, integer, integer,integer);

  ~d3_rat_point_rep() {}

friend __exportF int orientation(const d3_rat_point&, const d3_rat_point&, 
                                 const d3_rat_point&, const d3_rat_point&);

friend inline unsigned long ID_Number(const d3_rat_point& p);

};



/*{\Manpage {d3_rat_point} {} {Rational Points in 3D-Space} }*/

class __exportC d3_rat_point  : public HANDLE_BASE(d3_rat_point_rep) {

/*{\Mdefinition
An instance of data type |d3_rat_point| is a point with rational coordinates 
in the three-dimensional space. A point with cartesian coordinates $(a,b,c)$ 
is represented by homogeneous coordinates $(x,y,z,w)$ of arbitrary length 
integers (see \ref{Integers of Arbitrary Length}) such that $a = x/w$,
$b = y/w$, $c = z/w$ and $w > 0$. 
}*/

 d3_rat_point_rep* ptr() const { return (d3_rat_point_rep*)PTR; }

public:

static int orient_count;
static int exact_orient_count;

static int sos_count;
static int exact_sos_count;

static int cmp_count;
static int exact_cmp_count;

static int default_precision;

static int use_filter;

/*{\Mcreation p}*/

d3_rat_point() { PTR = new d3_rat_point_rep; }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
initialized to the point $(0,0,0)$.}*/

d3_rat_point(const rational& a, const rational& b, const rational& c)       
{ PTR = new d3_rat_point_rep(a,b,c); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
initialized to the point $(a,b,c)$.}*/

/*{\Moptions nextwarning=no }*/

d3_rat_point(int a, int b, int c)  
{ PTR = new d3_rat_point_rep(integer(a),integer(b), integer(c)); }

d3_rat_point(integer a, integer b, integer c)  
{ PTR = new d3_rat_point_rep(a,b,c); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
initialized to the point $(a,b,c)$.}*/

d3_rat_point(integer x, integer y, integer z, integer w) 
{ PTR = new d3_rat_point_rep(x,y,z,w); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
            initialized to the point with homogeneous coordinates $(x,y,z,w)$ 
            if $w > 0$ and to point $(-x,-y,-z,-w)$ if $w < 0$.\\
            \precond $w \not= 0$.}*/

d3_rat_point(const rat_vector& v) 
{ PTR = new d3_rat_point_rep(v.hcoord(0),v.hcoord(1),v.hcoord(2),v.hcoord(3)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the point  $(v[0],v[1],v[2])$.\\
            \precond: |v.dim() = 3|. }*/

// new ...
d3_rat_point(const d3_point& p, int prec = d3_rat_point::default_precision);

 d3_rat_point(const d3_rat_point& p) : HANDLE_BASE(d3_rat_point_rep)(p) {}
~d3_rat_point() {}

 d3_rat_point& operator=(const d3_rat_point& p) 
 { HANDLE_BASE(d3_rat_point_rep)::operator=(p); return *this; }

/*{\Moperations 2.5 3.7 }*/

d3_point to_d3_point() const;

d3_point to_float() const { return to_d3_point(); }
/*{\Mop  returns a floating point approximation of |\Mvar|.}*/

rat_vector to_vector() const;
/*{\Mop     returns the vector extending from the origin to |\Mvar|.}*/


integer X() const { return ptr()->x; }
/*{\Mop     returns the first homogeneous coordinate of |\Mvar|.}*/

integer Y() const { return ptr()->y; }
/*{\Mop     returns the second homogeneous coordinate of |\Mvar|.}*/

integer Z() const { return ptr()->z; }
/*{\Mop     returns the third homogeneous coordinate of |\Mvar|.}*/

integer W() const { return ptr()->w; }
/*{\Mop     returns the fourth homogeneous coordinate of |\Mvar|.}*/



double XD() const { return ptr()->xd; }
/*{\Mop     returns a floating point approximation of |\Mvar.X()|. }*/

double YD() const { return ptr()->yd; }
/*{\Mop     returns a floating point approximation of |\Mvar.Y()|. }*/

double ZD() const { return ptr()->zd; }
/*{\Mop     returns a floating point approximation of |\Mvar.Z()|. }*/

double WD() const { return ptr()->wd; }
/*{\Mop     returns a floating point approximation of |\Mvar.W()|. }*/



rational xcoord()  const { return rational(X(),W()); }
/*{\Mop     returns the $x$-coordinate of |\Mvar|.}*/

rational ycoord()  const { return rational(Y(),W()); }
/*{\Mop     returns the $y$-coordinate of |\Mvar|.}*/

rational zcoord()  const { return rational(Z(),W()); }
/*{\Mop     returns the $z$-coordinate of |\Mvar|.}*/

rational operator[](int i) const;
/*{\Marrop  returns the $i$th cartesian coordinate of |\Mvar|\\
            \precond  $0 \le i \le 2$. }*/


double xcoordD() const { return XD()/WD(); }
/*{\Mop     returns a floating point approximation of |\Mvar.xcoord()|. }*/

double ycoordD() const { return YD()/WD(); }
/*{\Mop     returns a floating point approximation of |\Mvar.ycoord()|. }*/

double zcoordD() const { return ZD()/WD(); }
/*{\Mop     returns a floating point approximation of |\Mvar.zcoord()|. }*/



integer hcoord(int i) const;
/*{\Mop     returns the $i$th homogeneous coordinate of |\Mvar|.\\
            \precond  $0 \le i \le 3$. }*/



rat_point project_xy() const;
/*{\Mop  returns |\Mvar| projected into the xy-plane. }*/

rat_point project_yz() const;
/*{\Mop  returns |\Mvar| projected into the yz-plane. }*/

rat_point project_xz() const;
/*{\Mop  returns |\Mvar| projected into the xz-plane. }*/


d3_rat_point reflect(const d3_rat_point& p, 
                     const d3_rat_point& q,  const d3_rat_point& r) const;
/*{\Mop     returns |\Mvar| reflected  across the plane passing
            through $p$, $q$ and $r$.\\
            \precond $p$, $q$ and $r$ are not collinear.}*/

d3_rat_point reflect(const d3_rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected across point $q$.}*/


d3_rat_point translate(const rational& dx, const rational& dy, 
                                           const rational& dz) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy,dz)$.}*/

d3_rat_point translate(integer dx, integer dy, integer dz, integer dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw,dz/dw)$.}*/

d3_rat_point translate(const rat_vector& v) const
{ return translate(v.hcoord(0),v.hcoord(1),v.hcoord(2),v.hcoord(3)); }
/*{\Mop     returns $|\Mvar| + v$, i.e., |\Mvar| translated by vector $v$\\
            \precond $v$.dim() = 3.}*/

d3_rat_point operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$\\
            \precond $v$.dim() = 3.}*/

d3_rat_point operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$\\
            \precond $v$.dim() = 3.}*/



rational sqr_dist(const d3_rat_point& q) const;
/*{\Mopl    returns the squared distance between |\Mvar| and $q$. }*/

rational xdist(const d3_rat_point& q) const;
/*{\Mopl    returns the x-distance between |\Mvar| and $q$. }*/

rational ydist(const d3_rat_point& q) const;
/*{\Mopl    returns the y-distance between |\Mvar| and $q$. }*/

rational zdist(const d3_rat_point& q) const;
/*{\Mopl    returns the z-distance between |\Mvar| and $q$. }*/


friend unsigned long ID_Number(const d3_rat_point& p) { return p.ptr()->id; }


friend bool operator==(const d3_rat_point& p, const d3_rat_point& q)
{ return (identical(p,q)) || d3_rat_point::cmp_xyz(p,q) == 0; }

friend bool operator!=(const d3_rat_point& p, const d3_rat_point& q)
{ return (!identical(p,q)) && d3_rat_point::cmp_xyz(p,q) != 0; }

rat_vector operator-(const d3_rat_point& q)  const
{ return to_vector() - q.to_vector(); }
/*{\Mbinop  returns the difference vector of the coordinates.}*/


friend __exportF ostream& operator<<(ostream& O, const d3_rat_point& p) ;
/*{\Mbinopfuncl  writes the homogeneous coordinates $(x,y,z,w)$ of |\Mvar| to 
                output stream $O$.}*/

friend __exportF istream& operator>>(istream& I, d3_rat_point& p) ;
/*{\Mbinopfuncl  reads the homogeneous coordinates $(x,y,z,w)$ of |\Mvar| from 
                input stream $I$.}*/


/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/


/*{\Moptions nextwarning=no}*/
friend __exportF int orientation(const d3_rat_point& a, const d3_rat_point& b, 
                                 const d3_rat_point& c, const d3_rat_point& d);
/*{\Mfuncl computes the orientation of points $a$, $b$, $c$ and $d$ as the 
           sign of the determinant\\
           \[ \left\Lvert \begin{array}{cccc} a_w & b_w & c_w &d_w\\
                                              a_x & b_x & c_x &d_x\\
                                              a_y & b_y & c_y &d_y\\
                                              a_z & b_z & c_z &d_z
                       \end{array} \right\Lvert \]\\
           i.e., it returns 
           $+1$ if point $d$ lies left of the directed plane through 
           $a,b,c$, $0$ if $a$,$b$, $c$ and $d$ are coplanar, and 
           $-1$ otherwise. }*/

friend __exportF int orientation_xy(const d3_rat_point& a,
                                    const d3_rat_point& b,
                                    const d3_rat_point& c);
/*{\Mfuncl returns the orientation of the projections of  $a$, $b$ and $c$
into the $xy$-plane.}*/	

friend __exportF int orientation_yz(const d3_rat_point& a, 
                                    const d3_rat_point& b, 
                                    const d3_rat_point& c);
/*{\Mfuncl returns the orientation of the projections of  $a$, $b$ and $c$
into the $yz$-plane.}*/	

friend __exportF int orientation_xz(const d3_rat_point& a, 
                                    const d3_rat_point& b, 
                                    const d3_rat_point& c);
/*{\Mfuncl returns the orientation of the projections of  $a$, $b$ and $c$
into the $xz$-plane.}*/


static int  cmp_xyz_F(const d3_rat_point&, const d3_rat_point&);

static int  cmp_xyz(const d3_rat_point&, const d3_rat_point&);
static int  cmp_x(const d3_rat_point&, const d3_rat_point&);
static int  cmp_y(const d3_rat_point&, const d3_rat_point&);
static int  cmp_z(const d3_rat_point&, const d3_rat_point&);

static int  cmp(const d3_rat_point& a, const d3_rat_point& b) 
{ return cmp_xyz(a,b); }

static void print_statistics();

};


COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const d3_rat_point& a, const d3_rat_point& b)
{  return (identical(a,b))  ? 0 : d3_rat_point::cmp_xyz(a,b); }

inline int compare_F(const d3_rat_point& a, const d3_rat_point& b)
{  return (identical(a,b))  ? 0 : d3_rat_point::cmp_xyz_F(a,b); }



/*{\Moptions nextwarning=no}*/
extern __exportF int cmp_distances(const d3_rat_point& p1, const d3_rat_point& p2,
                                   const d3_rat_point& p3, const d3_rat_point& p4);
/*{\Mfuncl compares the distances |(p1,p2)| and |(p3,p4)|.
Returns $+1$ ($-1$) if distance |(p1,p2)| is larger (smaller) than
distance |(p3,p4)|, otherwise $0$.}*/ 


extern __exportF d3_rat_point center(const d3_rat_point& a, const d3_rat_point& b);

inline d3_rat_point midpoint(const d3_rat_point& a, const d3_rat_point& b)
{ return center(a,b); }
/*{\Mfuncl returns the midpoint of $a$ and $b$. }*/




extern __exportF rational volume(const d3_rat_point& a, const d3_rat_point& b, 
                                                        const d3_rat_point& c,
                                                        const d3_rat_point& d);
/*{\Mfuncl computes the signed volume of the simplex determined by $a$,$b$,$c$,
           and $d$, positive if $orientation(a,b,c,d) > 0$ and negative 
           otherwise. }*/


extern __exportF bool collinear(const d3_rat_point& a,const d3_rat_point& b,
                                               const d3_rat_point& c);
/*{\Mfuncl  returns true if points $a$, $b$, $c$ are collinear, and false 
            otherwise. }*/


inline bool coplanar(const d3_rat_point& a, const d3_rat_point& b, 
                                            const d3_rat_point& c,
                                            const d3_rat_point& d)
{ return orientation(a,b,c,d) == 0; }
/*{\Mfuncl returns true if points $a$, $b$, $c$, $d$ are coplanar and
           false otherwise.}*/



extern __exportF int side_of_sphere(const d3_rat_point& a, const d3_rat_point& b, 
                                                    const d3_rat_point& c,
                                                    const d3_rat_point& d,
                                                    const d3_rat_point& e);
/*{\Mfuncl returns $+1$ ($-1$) if point $e$ lies on the positive (negative)
           side of the oriented sphere through points $a$, $b$, $c$, and $d$, 
           and $0$ if $e$ is contained in this sphere. }*/


extern __exportF int region_of_sphere(const d3_rat_point& a, 
                                      const d3_rat_point& b, 
                                      const d3_rat_point& c, 
                                      const d3_rat_point& d,
                                      const d3_rat_point& x);
/*{\Mfuncl determines whether the point $x$ lies inside ($= +1$), on ($= 0$),
           or outside ($= -1$) the sphere through points $a,b,c,d$, 
           (equivalent to |orientation(a,b,c,d) * side_of_sphere(a,b,c,d,x)|) \\
           \precond $|orientation(A)| \ne 0$}*/


extern __exportF bool contained_in_simplex(const d3_rat_point& a, 
                                           const d3_rat_point& b,
                                           const d3_rat_point& c,
                                           const d3_rat_point& d,
                                           const d3_rat_point& x);
/*{\Mfuncl determines whether $x$ is contained in the simplex spanned
           by the points $a,b,c,d$.\\ 
           \precond $a,b,c,d$ are affinely independent. }*/

extern __exportF bool contained_in_simplex(const array<d3_rat_point>& A, 
                                           const d3_rat_point& x);
/*{\Mfuncl determines whether $x$ is contained in the simplex spanned
           by the points in $A$.\\ \precond $A$ must have size $\le 4$ and
           the points in $A$ must be affinely independent. }*/



extern __exportF bool contained_in_affine_hull(const list<d3_rat_point>& L,
                                               const d3_rat_point& x);
/*{\Mfuncl determines whether $x$ is contained in the affine hull
           of the points in $L$.}*/

extern __exportF bool contained_in_affine_hull(const array<d3_rat_point>& A, 
                                               const d3_rat_point& x);
/*{\Mfuncl determines whether $x$ is contained in the affine hull
           of the points in $A$.}*/


extern __exportF int affine_rank(const array<d3_rat_point>& L);
/*{\Mfuncl computes the affine rank of the points in $L$. }*/

extern __exportF int affine_rank(const array<d3_rat_point>& A);
/*{\Mfuncl computes the affine rank of the points in $A$. }*/


extern __exportF bool affinely_independent(const list<d3_rat_point>& L);
/*{\Mfuncl decides whether the points in $A$ are affinely independent. }*/

extern __exportF bool affinely_independent(const array<d3_rat_point>& A);
/*{\Mfuncl decides whether the points in $A$ are affinely independent. }*/




inline int side_of_sphere(const array<d3_rat_point>& A, const d3_rat_point& x)
{ return side_of_sphere(A[0],A[1],A[2],A[3],x); }

inline int region_of_sphere(const array<d3_rat_point>& A, const d3_rat_point& x)
{ return region_of_sphere(A[0],A[1],A[2],A[3],x); }



/*{\Moptions nextwarning=no}*/
inline bool insphere(const d3_rat_point& a, const d3_rat_point& b, 
                                            const d3_rat_point& c,
                                            const d3_rat_point& d,
                                            const d3_rat_point& e)
{ return region_of_sphere(a,b,c,d,e) > 0; }


inline bool inside_sphere(const d3_rat_point& a, const d3_rat_point& b, 
                                                 const d3_rat_point& c,
                                                 const d3_rat_point& d,
                                                 const d3_rat_point& e)
{ return region_of_sphere(a,b,c,d,e) > 0; }

/*{\Mfuncl returns $true$ if point $e$ lies in the interior of the sphere
           through points $a$, $b$, $c$, and $d$, and $false$ otherwise. }*/


/*{\Moptions nextwarning=no}*/
inline bool outsphere(const d3_rat_point& a, const d3_rat_point& b, 
                                             const d3_rat_point& c,
                                             const d3_rat_point& d,
                                             const d3_rat_point& e)
{ return region_of_sphere(a,b,c,d,e) < 0; }


inline bool outside_sphere(const d3_rat_point& a, const d3_rat_point& b, 
                                                  const d3_rat_point& c,
                                                  const d3_rat_point& d,
                                                  const d3_rat_point& e)
{ return region_of_sphere(a,b,c,d,e) < 0; }
/*{\Mfuncl returns $true$ if point $e$ lies in the exterior of the sphere
           through points $a$, $b$, $c$, and $d$, and $false$ otherwise. }*/


inline bool on_sphere(const d3_rat_point& a, const d3_rat_point& b, 
                                             const d3_rat_point& c,
                                             const d3_rat_point& d,
                                             const d3_rat_point& e)
{ return region_of_sphere(a,b,c,d,e) == 0; }
/*{\Mfuncl returns $true$ if points $a$, $b$, $c$, $d$, and $e$ lie 
           on a common sphere. }*/


extern __exportF d3_rat_point point_on_positive_side(const d3_rat_point& a, 
                                                     const d3_rat_point& b, 
                                                     const d3_rat_point& c);
/*{\Mfuncl returns a point $d$ with $orientation(a,b,c,d) > 0$. }*/




inline const char* leda_tname(const d3_rat_point*) { return "d3_rat_point"; }


#include <LEDA/core/list.h>
#include <LEDA/geo/rat_point.h> 


/*{\Mtext \vspace{1cm} {\bf Point Generators} }*/

extern __exportF d3_rat_point random_d3_rat_point_in_cube(int maxc);
/*{\Mfunc returns a point whose coordinates are random integers in 
$\range{-|maxc|}{|maxc|}$. }*/

extern __exportF void random_d3_rat_points_in_cube(int n, int maxc, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF d3_rat_point random_d3_rat_point_in_square(int maxc);
/*{\Mfunc returns a point whose $x$ and $y$-coordinates are random integers in 
$\range{-|maxc|}{|maxc|}$. The $z$-coordinate is zero. In 2d, this function is
equivalent to |random_rat_point_in_cube|. }*/

extern __exportF void random_d3_rat_points_in_square(int n, int maxc, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF d3_rat_point random_d3_rat_point_in_unit_cube(int D = 16383);
/*{\Mfunc returns a point whose coordinates are random rationals of the form
$i/D$ where $i$ is a random integer in the range $\range{0}{D}$. The default 
value of $D$ is $2^{14} - 1$.}*/

extern __exportF void random_d3_rat_points_in_unit_cube(int n, int D, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_d3_rat_points_in_unit_cube(int n, list<d3_rat_point>& L);
/*{\Mfunc as above, but the default value of $D$ is used. }*/

extern __exportF d3_rat_point random_d3_rat_point_in_ball(int R);
/*{\Mfuncl returns a random point with integer coordinates 
in the ball with radius $R$ centered at the origin.\\
\precond $R \le 2^{14}$.}*/

extern __exportF void random_d3_rat_points_in_ball(int n, int R, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF d3_rat_point random_d3_rat_point_in_unit_ball(int D = 16383);
/*{\Mfuncl returns a point in the unit ball 
whose coordinates are random rationals of the form
$i/D$ where $i$ is a random integer in the range $\range{0}{D}$. The default 
value of $D$ is $2^{14} - 1$.}*/ 

extern __exportF void random_d3_rat_points_in_unit_ball(int n, int D, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_d3_rat_points_in_unit_ball(int n, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/


extern __exportF d3_rat_point random_d3_rat_point_in_disc(int R);
/*{\Mfuncl returns a random point with integer $x$ and $y$-coordinates 
in the disc with radius $R$ centered at the origin. The $z$-coordinate is zero. In 2d this is the same as the function |random_rat_point_in_ball|.\\
\precond $R \le 2^{14}$.}*/

extern __exportF void random_d3_rat_points_in_disc(int n, int R, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/


extern __exportF d3_rat_point random_d3_rat_point_on_circle(int R);
/*{\Mfuncl  returns a random point with integer coordinates that lies
close to the circle with radius $R$ centered at the origin.}*/

extern __exportF void random_d3_rat_points_on_circle(int m, int R, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF d3_rat_point random_d3_rat_point_on_unit_circle(int D = 16383);
/*{\Mfuncl  returns a point close to the unit circle  
whose coordinates are random rationals of the form
$i/D$ where $i$ is a random integer in the range $\range{0}{D}$. The default 
value of $D$ is $2^{14} - 1$.}*/ 

extern __exportF void random_d3_rat_points_on_unit_circle(int m, int D, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_d3_rat_points_on_unit_circle(int m, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/

extern __exportF d3_rat_point random_d3_rat_point_on_sphere(int R);
/*{\Mfuncl returns a point with integer coordinates close to the sphere with
radius $R$ centered at the origin.}*/

extern __exportF void random_d3_rat_points_on_sphere(int m, int R, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF d3_rat_point random_d3_rat_point_on_unit_sphere(int D = 16383);
/*{\Mfuncl  returns a point close to the unit sphere  
whose coordinates are random rationals of the form
$i/D$ where $i$ is a random integer in the range $\range{0}{D}$. The default 
value of $D$ is $2^{14} - 1$. In 2d this function is equivalent to 
|point_on_unit_circle|.}*/ 

extern __exportF void random_d3_rat_points_on_unit_sphere(int m, int D, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_d3_rat_points_on_unit_sphere(int m, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/

extern __exportF d3_rat_point random_d3_rat_point_on_paraboloid(int maxc);
/*{\Mfuncl returns a point $(x,y,z)$  with $x$ and $y$ random integers
in the range $\range{-|maxc|}{|maxc|}$, and 
$z = 0.004*(x*x + y*y) - 1.25*|maxc|$. The function does not make sense in
2d.}*/

extern __exportF void random_d3_rat_points_on_paraboloid(int n, int maxc, list<d3_rat_point>& L);
/*{\Mfuncl returns a list |L| of $n$ points \ldots\ .}*/

extern __exportF void lattice_d3_rat_points(int n, int maxc, list<d3_rat_point>& L);
/*{\Mfuncl returns a list $L$ of approximately $n$ points. The points
have integer coordinates $id/|maxc|$ for an appropriately chosen $d$
and $-|maxc|/d \le i \le |maxc|/d$. }*/

extern __exportF void random_d3_rat_points_on_segment(int n, int maxc, list<d3_rat_point>& L);
/*{\Mfunc generates $n$ points on the diagonal whose coordinates are random
integer in the range from $-|maxc|$ to |maxc|. }*/


// overloaded generators

inline void random_points_in_cube(int n, int maxc, list<d3_rat_point>& L)
{ random_d3_rat_points_in_cube(n,maxc,L); }

inline void random_points_in_unit_cube(int n, list<d3_rat_point>& L)
{ random_d3_rat_points_in_unit_cube(n,L); }

inline void random_points_in_ball(int n, int maxc, list<d3_rat_point>& L)
{ random_d3_rat_points_in_ball(n,maxc,L); }

inline void random_points_in_unit_ball(int n, list<d3_rat_point>& L)
{ random_d3_rat_points_in_unit_ball(n,L); }

inline void random_points_in_square(int n, int maxc, list<d3_rat_point>& L)
{ random_d3_rat_points_in_square(n,maxc,L); }

inline void random_points_on_paraboloid(int n, int maxc, list<d3_rat_point>& L)
{ random_d3_rat_points_on_paraboloid(n,maxc,L); }

inline void lattice_points(int n, int maxc, list<d3_rat_point>& L)
{ lattice_d3_rat_points(n,maxc,L); }

inline void random_points_on_sphere(int n, int maxc, list<d3_rat_point>& L)
{ random_d3_rat_points_on_sphere(n,maxc,L); }

inline void random_points_on_unit_sphere(int n, list<d3_rat_point>& L)
{ random_d3_rat_points_on_unit_sphere(n,L); }

inline void random_points_on_segment(int n, int maxc, list<d3_rat_point>& L)
{ random_d3_rat_points_on_segment(n,maxc,L); }






#if LEDA_ROOT_INCL_ID == 600079
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

