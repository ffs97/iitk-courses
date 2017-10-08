/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_point.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_RAT_POINT_H
#define LEDA_RAT_POINT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600119
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/rational.h>
#include <LEDA/numbers/rat_vector.h>

#include <LEDA/geo/point.h>
#include <LEDA/core/array.h>

#include <LEDA/internal/handle_types.h>

/*
#include <assert.h>
*/

LEDA_BEGIN_NAMESPACE

class __exportC rat_point;
class __exportC rat_segment;


//------------------------------------------------------------------------------
// rat_points
//------------------------------------------------------------------------------

class __exportC rat_point_rep  : public handle_rep {

friend class __exportC rat_point;
friend class __exportC rat_segment;

static atomic_counter id_counter;

   
   integer x;
   integer y;
   integer w;

   double  xd;
   double  yd;
   double  wd;

   unsigned long id;

public:

   rat_point_rep();
   rat_point_rep(const rational&, const rational&);
   rat_point_rep(integer, integer);
   rat_point_rep(integer, integer, integer);

  ~rat_point_rep() {}


friend __exportF int side_of_circle(const rat_point&, const rat_point&, 
                                                      const rat_point&,
                                                      const rat_point&);

friend __exportF int cmp_signed_dist(const rat_point&, const rat_point&, 
                                                       const rat_point&,
                                                       const rat_point&);

friend inline unsigned long ID_Number(const rat_point& p);


};



/*{\Manpage {rat_point} {} {Rational Points} }*/

class __exportC rat_point  : public HANDLE_BASE(rat_point_rep) {

/*{\Mdefinition
An instance of data type |rat_point| is a point with rational coordinates 
in the two-dimensional plane. A point with cartesian coordinates $(a,b)$ is represented by homogeneous
coordinates $(x,y,w)$ of arbitrary length integers (see \ref{Integers of 
Arbitrary Length}) such that $a = x/w$ and $b = y/w$ and $w > 0$. 
}*/


 friend class __exportC rat_segment;

rat_point_rep* ptr() const { return (rat_point_rep*)PTR; } 

public:

static int orient_count;
static int exact_orient_count;
static int error_orient_count;

static int soc_count;
static int exact_soc_count;
static int error_soc_count;

static int cmp_count;
static int exact_cmp_count;
static int error_cmp_count;

static int use_filter;
static int float_computation_only;

static int default_precision;

static double x_abs_max;
static double y_abs_max;
static double w_abs_max;

static void update_abs_bounds(double,double,double);
   

// static eps for side_of_circle test
static double soc_eps_static;

// static eps for orientation tests
static double ori_eps_static;


static bool   use_static_filter;


/*{\Mtypes 5}*/ 

typedef  rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef  rat_point point_type;
/*{\Mtypemember the point type (|rat_point|).}*/  

typedef  point     float_type;
/*{\Mtypemember the corresponding floating-point type (|point|).}*/  




/*{\Mcreation p}*/

rat_point() { PTR = new rat_point_rep; }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
initialized to the point $(0,0)$.}*/

 rat_point(const rational& a, const rational& b)       
{ PTR = new rat_point_rep(a,b); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
initialized to the point $(a,b)$.}*/



 rat_point(integer a, integer b)  { PTR = new rat_point_rep(a,b); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
            initialized to the point $(a,b)$.}*/

 rat_point(int a, int b)  
 { PTR = new rat_point_rep(integer(a),integer(b)); }


 rat_point(integer x, integer y, integer w) { PTR = new rat_point_rep(x,y,w); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|
            initialized to the point with homogeneous coordinates $(x,y,w)$ if $w > 0$
            and to point $(-x,-y,-w)$ if $w < 0$.\\
            \precond $w \not= 0$.}*/

 rat_point(int a, int b, int c)  
 { PTR = new rat_point_rep(integer(a),integer(b),integer(c)); }


 rat_point(const rat_vector& v) 
{ PTR = new rat_point_rep(v.hcoord(0),v.hcoord(1),v.hcoord(2)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the point  $(v[0],v[1])$.\\
            \precond: |v.dim() = 2|. }*/

 rat_point(const point& p1, int prec = rat_point::default_precision);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized
to the point with homogeneous coordinates $(\lfloor P*x \rfloor,
\lfloor P*y \rfloor, P)$, where $p_1 = (x,y)$ and $P = 2^{|prec|}$. If
|prec| is non-positive, the conversion is without loss of precision,
i.e., |P| is chosen as a sufficiently large power of two such that
$P * x$ and $P * y$ are |integers|.  }*/


rat_point(double x, double y, int prec = rat_point::default_precision);
/*{\Mcreate  see constructor above with $p = (x,y)$. }*/


 rat_point(const rat_point& p) : HANDLE_BASE(rat_point_rep)(p) {}
~rat_point() {}

 rat_point& operator=(const rat_point& p) 
 { HANDLE_BASE(rat_point_rep)::operator=(p); return *this; }

/*{\Moperations 2 3.5 }*/

point to_point() const { return point(xcoordD(),ycoordD()); }

point to_float() const { return to_point(); }
/*{\Mop  returns a floating point approximation of |\Mvar|.}*/

rat_vector  to_vector() const { return rat_vector(ptr()->x,ptr()->y,ptr()->w); }
/*{\Mop     returns the vector extending from the origin to |\Mvar|.}*/

void    normalize() const;
/*{\Mop simplifies the homogenous representation by dividing all
        coordinates by $gcd(X,Y,W)$. }*/


integer X() const { return ptr()->x; }
/*{\Mop     returns the first homogeneous coordinate of |\Mvar|.}*/

integer Y() const { return ptr()->y; }
/*{\Mop     returns the second homogeneous coordinate of |\Mvar|.}*/

integer W() const { return ptr()->w; }
/*{\Mop     returns the third homogeneous coordinate of |\Mvar|.}*/


double XD() const { return ptr()->xd; }
/*{\Mop     returns a floating point approximation of |\Mvar.X()|. }*/

double YD() const { return ptr()->yd; }
/*{\Mop     returns a floating point approximation of |\Mvar.Y()|. }*/

double WD() const { return ptr()->wd; }
/*{\Mop     returns a floating point approximation of |\Mvar.W()|. }*/


rational xcoord()  const { return rational(X(),W());}
/*{\Mop     returns the $x$-coordinate of |\Mvar|.}*/

rational ycoord()  const { return rational(Y(),W());}
/*{\Mop     returns the $y$-coordinate of |\Mvar|.}*/


double   xcoordD() const { return double_quotient(X(),W()); }
/*{\Mop     returns a floating point approximation of |\Mvar.xcoord()|. }*/

double   ycoordD() const { return double_quotient(Y(),W()); }
/*{\Mop     returns a floating point approximation of |\Mvar.ycoord()|. }*/




rat_point rotate90(const rat_point& q, int i = 1) const;
/*{\Mopl    returns |\Mvar| rotated by $i\times 90$ degrees about $q$. 
            If $i > 0$ the rotation is counter-clockwise otherwise it
            is clockwise. }*/

rat_point rotate90(int i = 1) const;
/*{\Mop     returns |\Mvar| rotated by $i\times 90$ degrees about the origin.
            If $i > 0$ the rotation is counter-clockwise otherwise it
            is clockwise. }*/


rat_point reflect(const rat_point& p, const rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.\\
            \precond $p \not= q$.}*/

rat_point reflect(const rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected across point $q$.}*/

rat_point translate(const rational& dx, const rational& dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

rat_point translate(integer dx, integer dy, integer dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/

rat_point translate(const rat_vector& v) const
{ return translate(v.hcoord(0),v.hcoord(1),v.hcoord(2)); }
/*{\Mop     returns $|\Mvar| + v$, i.e., |\Mvar| translated by vector 
            $v$.\\
            \precond $v$.dim() = 2.}*/

rat_point operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

rat_point operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/

rational sqr_dist(const rat_point& q) const;
/*{\Mopl    returns the squared distance between |\Mvar| and $q$. }*/

int      cmp_dist(const rat_point& q, const rat_point& r) const;
/*{\Mopl    returns $|compare|(\Mvar.|sqr_dist|(q),\Mvar.|sqr_dist|(r))$. }*/

rational xdist(const rat_point& q) const;
/*{\Mopl    returns the horizontal distance between |\Mvar| and $q$. }*/

rational ydist(const rat_point& q) const;
/*{\Mopl    returns the vertical distance between |\Mvar| and $q$. }*/


int orientation(const rat_point& q, const rat_point& r) const;
/*{\Mop     returns $|orientation|(\Mvar,q,r)$ (see below). }*/

int orientation(const rat_point&, const rat_point&, double,double,double,
                                                    double,double,double) const;

rational area(const rat_point& q, const rat_point& r) const;
/*{\Mop     returns $|area|(\Mvar,q,r)$ (see below). }*/



friend unsigned long ID_Number(const rat_point& p) { return p.ptr()->id; }


friend bool operator==(const rat_point& p, const rat_point& q)
{ return (identical(p,q)) || rat_point::cmp_xy(p,q) == 0; }

friend bool operator!=(const rat_point& p, const rat_point& q)
{ return (!identical(p,q)) && rat_point::cmp_xy(p,q) != 0; }

rat_vector operator-(const rat_point& q)  const
{ return to_vector() - q.to_vector(); }
/*{\Mbinop  returns the difference vector of the coordinates.}*/



friend __exportF ostream& operator<<(ostream& O, const rat_point& p) ;
friend __exportF istream& operator>>(istream& I, rat_point& p) ;


/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/


/*{\Moptions nextwarning=no}*/


friend __exportF int cmp_signed_dist(const rat_point& a, const rat_point& b,
                                                         const rat_point& c,
                                                         const rat_point& d);
/*{\Mfuncl compares (signed) distances of $c$ and $d$ to the straight line
           passing through $a$ and $b$ (directed from $a$ to $b$). Returns
           $+1$ ($-1$) if $c$ has larger (smaller) distance than $d$ and
           $0$ if distances are equal. }*/


static int  cmp_xy(const rat_point&, const rat_point&);
static int  cmp_xy_F(const rat_point&, const rat_point&);
static int  cmp_yx(const rat_point&, const rat_point&);
static int  cmp_x(const rat_point&, const rat_point&);
static int  cmp_y(const rat_point&, const rat_point&);
static int  cmp(const rat_point& a, const rat_point& b) { return cmp_xy(a,b); }

static int  cmp_distances_F(const rat_point& p1, const rat_point& p2, const rat_point& p3, const rat_point& p4);


static void print_statistics();
static void clear_statistics();

// Functions for setting static error bounds for the side_of_circle and 
// orientation predicates. They all return the previous static bound.
//
// The default value for all static error bounds is MAXDOUBLE.
//
// The static error bounds for the side_of_circle and orientation predicates
// will be used if rat_point::use_static_filter == true  (default). 

static double set_static_soc_error_bound(double bound);
static double set_static_soc_error_bound(double xm, double ym, double wm);

// Sets the static error bound for the side_of_circle predicate.
// xm,ym, and wm must be upper bounds for the absolute x,y,and w
// (floating-point) coordinates of all input points.


static double set_static_orientation_error_bound(double bound);
static double set_static_orientation_error_bound(double xm,double ym,double wm);

// sets the static error bound for the orientation predicate.
// xm,ym,wm  as above.

};

inline int orientation(const rat_point& a, const rat_point& b, 
                                           const rat_point& c)
{ return a.orientation(b,c); }
/*{\Mfuncl computes the orientation of points $a$, $b$, $c$ as the 
           sign of the determinant\\
           \[ \left\Lvert \begin{array}{ccc} a_x & a_y & a_w\\
                                        b_x & b_y & b_w\\
                                        c_x & c_y & c_w
                       \end{array} \right\Lvert \]\\
           i.e., it returns 
           $+1$ if point $c$ lies left of the directed line through 
           $a$ and $b$, $0$ if $a$,$b$, and $c$ are collinear, and 
           $-1$ otherwise. }*/



/*{\Moptions nextwarning=no}*/
extern __exportF int cmp_distances(const rat_point& p1, const rat_point& p2,
                                   const rat_point& p3, const rat_point& p4);
/*{\Mfuncl compares the distances |(p1,p2)| and |(p3,p4)|.
Returns $+1$ ($-1$) if distance |(p1,p2)| is larger (smaller) than
distance |(p3,p4)|, otherwise $0$.}*/ 


inline rat_point center(const rat_point& a, const rat_point& b)
{ return rat_point(a.X()*b.W()+b.X()*a.W(), 
                   a.Y()*b.W()+b.Y()*a.W(),2*a.W()*b.W()); } 

inline rat_point midpoint(const rat_point& a, const rat_point& b)
{ return center(a,b); }
/*{\Mfuncl returns the midpoint of $a$ and $b$. }*/



inline rational area(const rat_point& a, const rat_point& b, const rat_point& c)
{ return a.area(b,c); }
/*{\Mfuncl computes the signed area of the triangle determined by $a$,$b$,$c$,
           positive if $orientation(a,b,c) > 0$ and negative otherwise. }*/


inline bool collinear(const rat_point& a,const rat_point& b,const rat_point& c)
{ return orientation(a,b,c) == 0; }
/*{\Mfuncl  returns true if points $a$, $b$, $c$ are collinear, i.e.,
           $orientation(a,b,c) = 0$, and false otherwise. }*/


inline bool right_turn_F(const rat_point& a,const rat_point& b,const rat_point& c)
{ return (a.xcoordD()-b.xcoordD()) * (a.ycoordD()-c.ycoordD()) <
         (a.ycoordD()-b.ycoordD()) * (a.xcoordD()-c.xcoordD()); }

inline bool right_turn(const rat_point& a,const rat_point& b,const rat_point& c)
{ return orientation(a,b,c) < 0; }
/*{\Mfuncl  returns true if points $a$, $b$, $c$ form a righ turn, i.e.,
           $orientation(a,b,c) < 0$, and false otherwise. }*/


inline bool left_turn(const rat_point& a,const rat_point& b,const rat_point& c)
{ return orientation(a,b,c) > 0; }
/*{\Mfuncl  returns true if points $a$, $b$, $c$ form a left turn, i.e.,
           $orientation(a,b,c) > 0$, and false otherwise. }*/


extern __exportF int side_of_halfspace(const rat_point& a, 
                                       const rat_point& b, 
                                       const rat_point& c);
/*{\Mfuncl returns the sign of the scalar product $(b-a)\cdot(c-a)$. If 
$b \not=a$ this amounts to: Let $h$ be the open halfspace orthogonal to the
vector $b - a$, containing $b$, and having $a$ in its boundary. 
Returns $+1$ if $c$ is contained in
$h$, returns $0$ is $c$ lies on the the boundary of $h$, and returns $-1$ is
$c$ is contained in the interior of the complement of $h$.}*/



extern __exportF int side_of_circle_old(const rat_point& a, const rat_point& b, 
                                                        const rat_point& c,
                                                        const rat_point& d);

extern __exportF int side_of_circle(const rat_point& a, const rat_point& b, 
                                                        const rat_point& c,
                                                        const rat_point& d);
/*{\Mfuncl returns $+1$ if point $d$ lies left of the directed circle through
           points $a$, $b$, and $c$, $0$ if $a$,$b$,$c$,and $d$ are
           cocircular, and $-1$ otherwise. }*/


inline int side_of_circle_F(const rat_point& a, const rat_point& b, 
                                                const rat_point& c, 
                                                const rat_point& d)
{ int save_flag = rat_point::float_computation_only; 
  rat_point::float_computation_only = 1; 
  int soc = side_of_circle(a,b,c,d);
  rat_point::float_computation_only = save_flag; 
  return soc;
}



/*{\Moptions nextwarning=no}*/
inline
bool incircle(const rat_point& a, const rat_point& b, const rat_point& c,
                                                      const rat_point& d)
{ return (orientation(a,b,c) * side_of_circle(a,b,c,d)) > 0; }
inline
bool inside_circle(const rat_point& a, const rat_point& b, const rat_point& c,
                                                      const rat_point& d)
{ return (orientation(a,b,c) * side_of_circle(a,b,c,d)) > 0; }

/*{\Mfuncl returns $true$ if point $d$ lies in the interior of the circle
           through points $a$, $b$, and $c$, and $false$ otherwise. }*/

/*{\Moptions nextwarning=no}*/
inline
bool outcircle(const rat_point& a, const rat_point& b, const rat_point& c,
                                                      const rat_point& d)
{ return (orientation(a,b,c) * side_of_circle(a,b,c,d)) < 0; }
inline
bool outside_circle(const rat_point& a, const rat_point& b, const rat_point& c,
                                                      const rat_point& d)
{ return (orientation(a,b,c) * side_of_circle(a,b,c,d)) < 0; }
/*{\Mfuncl returns $true$ if point $d$ lies outside of the circle
           through points $a$, $b$, and $c$, and $false$ otherwise. }*/

inline
bool on_circle(const rat_point& a, const rat_point& b, const rat_point& c,
                                                      const rat_point& d)
{ return side_of_circle(a,b,c,d) == 0; }
/*{\Mfuncl returns $true$ if points $a$, $b$, $c$, and $d$ are cocircular. }*/

inline
bool cocircular(const rat_point& a, const rat_point& b, const rat_point& c,
                                                      const rat_point& d)
{ return side_of_circle(a,b,c,d) == 0; }
/*{\Mfuncl returns $true$ if points $a$, $b$, $c$, and $d$ are cocircular. }*/

extern __exportF int compare_by_angle(const rat_point& a, const rat_point& b, 
                                      const rat_point& c, const rat_point& d);
/*{\Mfuncl compares vectors |b-a| and |d-c| by angle (more efficient
           than calling |vector::compare_by_angle(b-a,d-x)| on rat\_vectors).
}*/						       

extern __exportF bool affinely_independent(const array<rat_point>& A);
/*{\Mfuncl decides whether the points in $A$ are affinely independent. }*/


extern __exportF bool contained_in_simplex(const array<rat_point>& A,
                                           const rat_point& p);
/*{\Mfuncl determines whether $p$ is contained in the simplex spanned
           by the points in |A|. |A| may consist of up to $3$ 
           points.\\
           \precond The points in |A| are affinely independent. }*/
  // precond : A contains points of different coordinates 
  //           0 < A.size() < 4

extern __exportF bool contained_in_affine_hull(const array<rat_point>& A,
                                               const rat_point& p);
/*{\Mfuncl determines whether $p$ is contained in the affine hull
           of the points in $A$.}*/





COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const rat_point& a, const rat_point& b)
{  return (identical(a,b)) ? 0 : rat_point::cmp_xy(a,b); }

inline int compare_F(const rat_point& a, const rat_point& b)
{  return (identical(a,b)) ? 0 : rat_point::cmp_xy_F(a,b); }



inline const char* leda_tname(const rat_point*) { return "rat_point"; }



#if LEDA_ROOT_INCL_ID == 600119
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif


