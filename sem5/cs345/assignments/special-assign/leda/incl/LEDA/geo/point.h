/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  point.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_POINT_H
#define LEDA_POINT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600095
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/list.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

class __exportC point;
class __exportC segment;

//------------------------------------------------------------------------------
// points
//------------------------------------------------------------------------------

class __exportC point_rep  : public handle_rep {

friend class __exportC point;
friend class __exportC segment;
friend class __exportC line;
friend class __exportC circle;

static atomic_counter id_counter;
   
   unsigned long id;

   double x;
   double y;

public:
    
   point_rep(double=0, double=0);
  ~point_rep() {}

friend inline unsigned long ID_Number(const point&);
   
};

/*{\Manpage {point} {} {Points}}*/

class __exportC point  : public HANDLE_BASE(point_rep) 
{
/*{\Mdefinition
An instance of the data type $point$ is a point in the two-dimensional
plane $\real ^2$. We use $(x,y)$ to denote a point with first (or x-)
coordinate $x$ and second (or y-) coordinate $y$.}*/

friend class __exportC segment;
friend class __exportC line;
friend class __exportC circle;


point_rep* ptr() const { return (point_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef double coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef point  point_type;
/*{\Mtypemember the point type (|point|).}*/  

typedef point  float_type;



/*{\Mcreation p }*/

 point();
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the point $(0,0)$.}*/

 point(double x, double y);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized to 
            the point $(x,y)$.}*/

// for compatibility with rat_point. 
 point(double x, double y, double w);

 point(vector v);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the point $(v[0],v[1])$.\\
            \precond: |v.dim() = 2|. }*/

 point(const point& p, int prec);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
to the point with coordinates 
$(\lfloor P*x \rfloor/P, \lfloor P*x \rfloor/P)$, 
where $p = (x,y)$ and $P = 2^{|prec|}$. If |prec| is 
non-positive, the new point has coordinates $x$ and $y$.
}*/

 point(const point& p) : HANDLE_BASE(point_rep)(p) {}

~point() {}

point& operator=(const point& p) 
{ HANDLE_BASE(point_rep)::operator=(p); return *this; }


/*{\Moperations 2 4}*/

double  xcoord()  const   { return ptr()->x; }
/*{\Mop     returns the first coordinate of |\Mvar|.}*/

double  ycoord()  const   { return ptr()->y; }
/*{\Mop     returns the second coordinate of |\Mvar|.}*/

vector  to_vector() const { return vector(xcoord(),ycoord()); }
/*{\Mop     returns the vector $\vec{xy}$.}*/

point   to_float() const { return *this; }
point   to_point() const { return *this; }
/*{\Xop  for compatibility with |rat_point|. }*/

void    normalize() const {}
/*{\Xop  for compatibility with |rat_point|. }*/


double  xcoordD()  const   { return ptr()->x; }
/*{\Xop  for compatibility with |rat_point|. }*/

double  ycoordD()  const   { return ptr()->y; }
/*{\Xop  for compatibility with |rat_point|. }*/


double  X()  const   { return ptr()->x; }
/*{\Xop  for compatibility with |rat_point|, same as |xcoord()|. }*/

double  Y()  const   { return ptr()->y; }
/*{\Xop  for compatibility with |rat_point|, same as |ycoord()|. }*/

double  W()  const   { return 1; }
/*{\Xop  for compatibility with |rat_point|, returns the constant 1. }*/


double  XD()  const   { return ptr()->x; }
/*{\Xop  for compatibility with |rat_point|, same as |xcoord()|. }*/

double  YD()  const   { return ptr()->y; }
/*{\Xop  for compatibility with |rat_point|, same as |ycoord()|. }*/

double  WD()  const   { return 1; }
/*{\Xop  for compatibility with |rat_point|, returns the constant 1. }*/


int     dim() const { return 2; }
/*{\Xop  returns 2.}*/

int orientation(const point& q, const point& r) const
{ double d1 = (xcoord() - q.xcoord()) * (ycoord() - r.ycoord());
  double d2 = (ycoord() - q.ycoord()) * (xcoord() - r.xcoord()); 
  if (d1 == d2) return 0; else return (d1 > d2) ? +1 : -1;
}
/*{\Mop     returns $|orientation|(\Mvar,q,r)$ (see below). }*/


double area(const point& q, const point& r) const
{ return ((xcoord()-q.xcoord()) * (ycoord()-r.ycoord()) -
          (ycoord()-q.ycoord()) * (xcoord()-r.xcoord()))/2; }
/*{\Mop     returns $|area|(\Mvar,q,r)$ (see below). }*/


double  sqr_dist(const point& q) const;
/*{\Mop     returns the square of the Euclidean distance between |\Mvar|
            and $q$.}*/

int      cmp_dist(const point& q, const point& r) const;
/*{\Mopl    returns $|compare|(\Mvar.|sqr_dist|(q),\Mvar.|sqr_dist|(r))$. }*/

double xdist(const point& q) const;
/*{\Mopl    returns the horizontal distance between |\Mvar| and $q$. }*/

double ydist(const point& q) const;
/*{\Mopl    returns the vertical distance between |\Mvar| and $q$. }*/


double  distance(const point& q) const;
/*{\Mop     returns the Euclidean distance between |\Mvar| and $q$.}*/

double  distance() const { return distance(point(0,0)); }
/*{\Mop     returns the Euclidean distance between |\Mvar| and $(0,0)$.}*/


double  angle(const point& q, const point& r) const;
/*{\Mop     returns the angle between $\vec{\Mvar q}$ and $\vec{\Mvar r}$.}*/


point   translate_by_angle(double alpha, double d) const;
/*{\Mopl    returns |\Mvar| translated in direction $alpha$ by distance $d$. 
            The  direction is given by its angle with a right oriented 
            horizontal ray.}*/

point   translate(double dx, double dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/


point   translate(const vector& v) const;
/*{\Mop     returns \Mvar$+v$, i.e., |\Mvar| translated by vector 
            $v$.\\
            \precond $v$.dim() = 2.}*/

point operator+(const vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

point operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/



point   rotate(const point& q, double a) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by angle $a$.}*/

point   rotate(double a) const;
/*{\Mop     returns |\Mvar|.rotate($point(0,0),\ a$). }*/

point   rotate90(const point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

point   rotate90(int i=1) const;
/*{\Mop     returns |\Mvar|.rotate90($point(0,0),i$). }*/


point reflect(const point& q, const point& r) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $q$ and $r$.}*/

point reflect(const point& q) const;
/*{\Mop     returns |\Mvar| reflected across point $q$. }*/


bool operator==(const point& q) const;
bool operator!=(const point& q)  const { return !operator==(q);}

vector operator-(const point& q)  const
{ return vector(xcoord()-q.xcoord(),ycoord()-q.ycoord()); }
/*{\Mbinop  returns the difference vector of the coordinates.}*/


static int  cmp_xy(const point&, const point&);
static int  cmp_yx(const point&, const point&);

static int  cmp_x(const point&, const point&);
static int  cmp_y(const point&, const point&);

friend __exportF istream& operator>>(istream& I, point& p) ;

friend inline unsigned long ID_Number(const point&);

};

inline ostream& operator<<(ostream& out, const point& p)
{ return out << "(" << p.xcoord() << "," << p.ycoord() << ")";} 


COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const point& a, const point& b)
{ //int r = compare(a.xcoord(),b.xcoord());
  //return (r!=0) ? r : compare(a.ycoord(),b.ycoord());
  if (a.xcoord() < b.xcoord()) return -1;
  else if (a.xcoord() > b.xcoord()) return 1;
       else if (a.ycoord() < b.ycoord()) return -1;
            else if (a.ycoord() > b.ycoord()) return 1;
                 else return 0;
 }



// geometric primitives

/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

inline int cmp_distances(const point& p1, const point& p2, 
                         const point& p3, const point& p4)
{ return compare(p1.sqr_dist(p2),p3.sqr_dist(p4)); }
/*{\Mfuncl compares the distances |(p1,p2)| and |(p3,p4)|.
Returns $+1$ ($-1$) if distance |(p1,p2)| is larger (smaller) than
distance |(p3,p4)|, otherwise $0$.}*/

inline point center(const point& a, const point& b)
{ return point((a.xcoord()+b.xcoord())/2,(a.ycoord()+b.ycoord())/2); } 
/*{\Mfuncl returns the center of $a$ and $b$, i.e. $a +\vec{ab}/2$. }*/

inline point midpoint(const point& a, const point& b) { return center(a,b); }
/*{\Mfuncl returns the center of $a$ and $b$. }*/



inline unsigned long ID_Number(const point& p) { return p.ptr()->id; }


inline int orientation(const point& a, const point& b, const point& c)
{ return a.orientation(b,c); }
/*{\Mfuncl computes the orientation of points $a$, $b$, and $c$ as
           the sign of the determinant\\

           \[ \left\Lvert \begin{array}{ccc} a_x & a_y & 1\\
                                        b_x & b_y & 1\\
                                        c_x & c_y & 1
                       \end{array} \right\Lvert \] \\

           i.e., it returns
           $+1$ if point $c$ lies left of the directed line through
           $a$ and $b$, $0$ if $a$,$b$, and $c$ are collinear, and 
           $-1$ otherwise. }*/


inline int cmp_signed_dist(const point& a, const point& b, const point& c,
                                                           const point& d)
{ double d1 = (a.xcoord() - b.xcoord()) * (d.ycoord() - c.ycoord());
  double d2 = (a.ycoord() - b.ycoord()) * (d.xcoord() - c.xcoord()); 
  if (d1 == d2) return 0; else return (d1 > d2) ? +1 : -1;
}
/*{\Mfuncl compares (signed) distances of $c$ and $d$ to the straight line
           passing through $a$ and $b$ (directed from $a$ to $b$). Returns
           $+1$ ($-1$) if $c$ has larger (smaller) distance than $d$ and
           $0$ if distances are equal. }*/


inline double area(const point& a, const point& b, const point& c)
{ return a.area(b,c);}
/*{\Mfuncl computes the signed area of the triangle determined by $a$,$b$,$c$,
           positive if $orientation(a,b,c) > 0$ and negative otherwise. }*/


inline bool collinear(const point& a, const point& b, const point& c)
{ return (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()) ==
         (a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()); }
/*{\Mfuncl returns |true| if points $a$, $b$, $c$ are collinear, i.e.,
           $orientation(a,b,c) = 0$, and |false| otherwise. }*/


inline bool right_turn(const point& a, const point& b, const point& c)
{ return (a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()) <
         (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()); }
/*{\Mfuncl returns |true| if points $a$, $b$, $c$ form a righ turn, i.e.,
           $orientation(a,b,c) < 0$, and |false| otherwise. }*/


inline bool left_turn(const point& a, const point& b, const point& c)
{ return (a.xcoord()-b.xcoord()) * (a.ycoord()-c.ycoord()) >
         (a.ycoord()-b.ycoord()) * (a.xcoord()-c.xcoord()); }
/*{\Mfuncl returns |true| if points $a$, $b$, $c$ form a left turn, i.e.,
           $orientation(a,b,c) > 0$, and |false| otherwise. }*/


extern __exportF int side_of_halfspace(const point& a, 
                                       const point& b, 
                                       const point& c);
/*{\Mfuncl returns the sign of the scalar product $(b-a)\cdot(c-a)$. If 
$b \not=a$ this amounts to: Let $h$ be the open halfspace orthogonal to the
vector $b - a$, containing $b$, and having $a$ in its boundary. 
Returns $+1$ if $c$ is contained in
$h$, returns $0$ is $c$ lies on the the boundary of $h$, and returns $-1$ is
$c$ is contained in the interior of the complement of $h$.}*/

extern __exportF int side_of_circle(const point& a, const point& b, 
                                    const point& c, const point& d);
/*{\Mfuncl returns $+1$ if point $d$ lies left of the directed circle through
           points $a$, $b$, and $c$, $0$ if $a$,$b$,$c$,and $d$ are
           cocircular, and $-1$ otherwise. }*/


inline bool inside_circle(const point& a, const point& b, const point& c,
                                                     const point& d)
{ return (orientation(a,b,c) * side_of_circle(a,b,c,d)) > 0; }
/*{\Mfuncl returns |true| if point $d$ lies in the interior of the circle
           through points $a$, $b$, and $c$, and |false| otherwise. }*/


inline bool outside_circle(const point& a, const point& b, const point& c,
                                                      const point& d)
{ return (orientation(a,b,c) * side_of_circle(a,b,c,d)) < 0; }
/*{\Mfuncl returns |true| if point $d$ lies outside of the circle
           through points $a$, $b$, and $c$, and |false| otherwise. }*/


inline bool on_circle(const point& a, const point& b, const point& c,
                                                       const point& d)
{ return side_of_circle(a,b,c,d) == 0; }
/*{\Mfuncl returns |true| if points $a$, $b$, $c$, and $d$ are cocircular. }*/



inline bool incircle(const point& a, const point& b, const point& c,
                                                     const point& d)
{ return inside_circle(a,b,c,d); }

inline bool outcircle(const point& a, const point& b, const point& c,
                                                      const point& d)
{ return outside_circle(a,b,c,d); }


inline bool cocircular(const point& a, const point& b, const point& c,
                                                       const point& d)
{ return side_of_circle(a,b,c,d) == 0; }
/*{\Mfuncl returns |true| if points $a$, $b$, $c$, and $d$ are cocircular. }*/

extern __exportF int compare_by_angle(const point& a, const point& b, 
                                      const point& c, const point& d);
/*{\Mfuncl compares vectors |b-a| and |d-c| by angle (more efficient
           than calling |compare_by_angle(b-a,d-x)| on vectors).
}*/						       


extern __exportF bool affinely_independent(const array<point>& A);
/*{\Mfuncl decides whether the points in $A$ are affinely independent. }*/


extern __exportF bool contained_in_simplex(const array<point>& A,
                                           const point& p);
/*{\Mfuncl determines whether $p$ is contained in the simplex spanned
           by the points in |A|. |A| may consist of up to $3$ 
           points.\\
           \precond The points in |A| are affinely independent. }*/
  // precond : A contains points of different coordinates 
  //           0 < A.size() < 4

extern __exportF bool contained_in_affine_hull(const array<point>& A,
                                               const point& p);
/*{\Mfuncl determines whether $p$ is contained in the affine hull
           of the points in $A$.}*/





inline const char* leda_tname(const point*) { return "point"; }



#if LEDA_ROOT_INCL_ID == 600095
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

