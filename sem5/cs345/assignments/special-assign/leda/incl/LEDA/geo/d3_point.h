/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_point.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_POINT_H
#define LEDA_D3_POINT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600125
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/list.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/geo/point.h>
#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_point;

//------------------------------------------------------------------------------
// d3_points
//------------------------------------------------------------------------------

class __exportC d3_point_rep  : public handle_rep {

friend class __exportC d3_point;

static atomic_counter id_counter;
   
   double x;
   double y;
   double z;

   unsigned long id;

public:
    
   d3_point_rep(double=0, double=0, double=0);
  ~d3_point_rep() {}

friend inline unsigned long ID_Number(const d3_point&);
   
};

/*{\Manpage {d3_point} {} {Points in 3D-Space}}*/

class __exportC d3_point  : public HANDLE_BASE(d3_point_rep) 
{
/*{\Mdefinition
An instance of the data type |\Mname| is a point in the three-dimensional
space $\real ^3$. We use $(x,y,z)$ to denote a point with first (or x-)
coordinate $x$, second (or y-) coordinate $y$, and third (or z-) 
coordinate $z$.}*/

 d3_point_rep* ptr() const { return (d3_point_rep*)PTR; }

public:

/*{\Mcreation p }*/

 d3_point();
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the point $(0,0,0)$.}*/

 d3_point(double x, double y, double z);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized to 
            the point $(x,y,z)$.}*/

 d3_point(vector v);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the point $(v[0],v[1],v[2])$.\\
            \precond |v.dim() = 3|. }*/

// for compatibility with d3_rat_point. 
 d3_point(double x, double y, double z, double w);

 d3_point(const d3_point& p) : HANDLE_BASE(d3_point_rep)(p) {}
~d3_point() {}

d3_point& operator=(const d3_point& p) 
{ HANDLE_BASE(d3_point_rep)::operator=(p); return *this; }


/*{\Moperations 2 4}*/

double  xcoord()  const   { return ptr()->x; }
/*{\Mop     returns the first coordinate of |\Mvar|.}*/

double  ycoord()  const   { return ptr()->y; }
/*{\Mop     returns the second coordinate of |\Mvar|.}*/

double  zcoord()  const   { return ptr()->z; }
/*{\Mop     returns the third coordinate of |\Mvar|.}*/

vector  to_vector() const { return vector(xcoord(),ycoord(),zcoord()); }
/*{\Mop     returns the vector $\vec{xyz}$.}*/

d3_point to_d3_point() const { return *this; }
/*{\Xop  for compatibility with |d3_rat_point|. }*/

d3_point to_float() const { return *this; }
/*{\Xop  for compatibility with |d3_rat_point|. }*/

double  X()   const   { return xcoord(); }
double  XD()  const   { return xcoord(); }
/*{\Xop  for compatibility with |d3_rat_point|. }*/

double  Y()   const   { return ycoord(); }
double  YD()  const   { return ycoord(); }
/*{\Xop  for compatibility with |d3_rat_point|. }*/

double  Z()   const   { return zcoord(); }
double  ZD()  const   { return zcoord(); }
/*{\Xop  for compatibility with |d3_rat_point|. }*/

double  W()   const   { return 1; }
double  WD()  const   { return 1; }
/*{\Xop  for compatibility with |d3_rat_point|. }*/

int     dim() const { return 3; }
/*{\Xop  returns 3.}*/


point project_xy() const;
/*{\Mop  returns |\Mvar| projected into the xy-plane. }*/

point project_yz() const;
/*{\Mop  returns |\Mvar| projected into the yz-plane. }*/

point project_xz() const;
/*{\Mop  returns |\Mvar| projected into the xz-plane. }*/


double  sqr_dist(const d3_point& q) const;
/*{\Mop     returns the square of the Euclidean distance between |\Mvar|
            and $q$.}*/

double xdist(const d3_point& q) const;
/*{\Mopl    returns the x-distance between |\Mvar| and $q$. }*/

double ydist(const d3_point& q) const;
/*{\Mopl    returns the y-distance between |\Mvar| and $q$. }*/

double zdist(const d3_point& q) const;
/*{\Mopl    returns the z-distance between |\Mvar| and $q$. }*/


double  distance(const d3_point& q) const;
/*{\Mop     returns the Euclidean distance between |\Mvar| and $q$.}*/


//double  angle(const d3_point& q, const d3_point& r) const;
//d3_point   translate_by_angle(double alpha, double d) const;


d3_point translate(double dx, double dy, double dz) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy,dz)$.}*/


d3_point translate(const vector& v) const;
/*{\Mop     returns \Mvar$+v$, i.e., |\Mvar| translated by vector 
	    $v$.\\
	    \precond $v$.dim() = 3.}*/

d3_point operator+(const vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

d3_point operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/


d3_point reflect(const d3_point& q, const d3_point& r, const d3_point& s) const;
/*{\Mop     returns |\Mvar| reflected  across the plane passing
            through $q$, $r$ and $s$.}*/

d3_point reflect(const d3_point& q) const;
/*{\Mop     returns |\Mvar| reflected across point $q$. }*/


bool operator==(const d3_point& q) const;
bool operator!=(const d3_point& q)  const { return !operator==(q);}

vector operator-(const d3_point& q)  const
{ return vector(xcoord()-q.xcoord(),
                ycoord()-q.ycoord(),
                zcoord()-q.zcoord()); }
/*{\Mbinop  returns the difference vector of the coordinates.}*/



static int  cmp_xyz(const d3_point&, const d3_point&);
static int  cmp_x(const d3_point&, const d3_point&);
static int  cmp_y(const d3_point&, const d3_point&);
static int  cmp_z(const d3_point&, const d3_point&);

friend __exportF ostream& operator<<(ostream& O, const d3_point& p) ;
/*{\Mbinopfunc  writes |\Mvar| to output stream $O$.}*/

friend __exportF istream& operator>>(istream& I, d3_point& p) ;
/*{\Mbinopfunc  reads the coordinates of |\Mvar| (three $double$ numbers)
	        from input stream $I$.}*/

friend inline unsigned long ID_Number(const d3_point&);

};

COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const d3_point& a, const d3_point& b)
{ return d3_point::cmp_xyz(a,b); }



// geometric primitives

/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

inline int cmp_distances(const d3_point& p1, const d3_point& p2, 
                         const d3_point& p3, const d3_point& p4)
{ return compare(p1.sqr_dist(p2),p3.sqr_dist(p4)); }
/*{\Mfuncl compares the distances |(p1,p2)| and |(p3,p4)|.
Returns $+1$ ($-1$) if distance |(p1,p2)| is larger (smaller) than
distance |(p3,p4)|, otherwise $0$.}*/


extern __exportF d3_point center(const d3_point& a, const d3_point& b);
/*{\Mfuncl returns the center of $a$ and $b$, i.e. $a +\vec{ab}/2$. }*/

inline d3_point midpoint(const d3_point& a, const d3_point& b) 
{ return center(a,b); }
/*{\Mfuncl returns the center of $a$ and $b$. }*/



inline unsigned long ID_Number(const d3_point& p) { return p.ptr()->id; }

/*{\Moptions nextwarning=no}*/
extern __exportF int orientation(const d3_point& a, const d3_point& b, 
                                             const d3_point& c,
                                             const d3_point& d);
/*{\Mfuncl computes the orientation of points $a$, $b$, $c$, and $d$ as
           the sign of the determinant\\
           \[ \left\Lvert \begin{array}{cccc} 1   & 1   &  1   & 1\\
                                              a_x & b_x &  c_x & d_x\\
                                              a_y & b_y &  c_y & d_y\\
                                              a_z & b_z &  c_z & d_z
                       \end{array} \right\Lvert \] 
   }*/


extern __exportF int orientation_xy(const d3_point& a, const d3_point& b, 
                                                const d3_point& c);
/*{\Mfuncl returns the orientation of the projections of  $a$, $b$ and $c$
into the $xy$-plane.}*/					
						
extern __exportF int orientation_yz(const d3_point& a, const d3_point& b, 
                                                const d3_point& c);
/*{\Mfuncl returns the orientation of the projections of  $a$, $b$ and $c$
into the $yz$-plane.}*/						
						
extern __exportF int orientation_xz(const d3_point& a, const d3_point& b, 
                                                const d3_point& c);
/*{\Mfuncl returns the orientation of the projections of  $a$, $b$ and $c$
into the $xz$-plane.}*/

extern __exportF double volume(const d3_point& a, const d3_point& b, 
                                                  const d3_point& c,
                                                  const d3_point& d);
/*{\Mfuncl computes the signed volume of the simplex determined by $a$,$b$,
           $c$, and $d$,
           positive if $orientation(a,b,c,d) > 0$ and negative otherwise. }*/


extern __exportF bool collinear(const d3_point& a, const d3_point& b, 
                                            const d3_point& c);
/*{\Mfuncl returns true if points $a$, $b$, $c$ are collinear and
           false otherwise.}*/

inline bool coplanar(const d3_point& a, const d3_point& b, const d3_point& c,
                                                           const d3_point& d)
{ return orientation(a,b,c,d) == 0; }
/*{\Mfuncl returns true if points $a$, $b$, $c$, $d$ are coplanar and
           false otherwise.}*/


extern __exportF int side_of_sphere(const d3_point& a, const d3_point& b, 
                                                       const d3_point& c,
                                                       const d3_point& d,
                                                       const d3_point& x);
/*{\Mfuncl returns $+1$ ($-1$) if point $x$ lies on the positive (negative)
           side of the oriented sphere through points $a$, $b$, $c$, and $d$, 
           and $0$ if $x$ is contained in this sphere. }*/


extern __exportF int region_of_sphere(const d3_point& a, const d3_point& b, 
                                                         const d3_point& c, 
                                                         const d3_point& d,
                                                         const d3_point& x);
/*{\Mfuncl determines whether the point $x$ lies inside ($= +1$), on ($= 0$),
           or outside ($= -1$) the sphere through points $a,b,c,d$, 
           (equivalent to |orientation(a,b,c,d) * side_of_sphere(a,b,c,d,x)|) \\
           \precond $|orientation(A)| \ne 0$}*/


extern __exportF bool contained_in_simplex(const d3_point& a, const d3_point& b,
                                                              const d3_point& c,
                                                              const d3_point& d,
                                                              const d3_point& x);
/*{\Mfuncl determines whether $x$ is contained in the simplex spanned
           by the points $a,b,c,d$.\\ 
           \precond $a,b,c,d$ are affinely independent. }*/

extern __exportF bool contained_in_simplex(const array<d3_point>& A, 
                                           const d3_point& x);
/*{\Mfuncl determines whether $x$ is contained in the simplex spanned
           by the points in $A$.\\ \precond $A$ must have size $\le 4$ and
           the points in $A$ must be affinely independent. }*/


extern __exportF bool contained_in_affine_hull(const list<d3_point>& L, 
                                               const d3_point& x);
/*{\Mfuncl determines whether $x$ is contained in the affine hull
           of the points in $L$.}*/

extern __exportF bool contained_in_affine_hull(const array<d3_point>& A, 
                                               const d3_point& x);
/*{\Mfuncl determines whether $x$ is contained in the affine hull
           of the points in $A$.}*/



extern __exportF int affine_rank(const array<d3_point>& L);
/*{\Mfuncl computes the affine rank of the points in $L$. }*/

extern __exportF int affine_rank(const array<d3_point>& A);
/*{\Mfuncl computes the affine rank of the points in $A$. }*/

extern __exportF bool affinely_independent(const list<d3_point>& L);
/*{\Mfuncl decides whether the points in $A$ are affinely independent. }*/

extern __exportF bool affinely_independent(const array<d3_point>& A);
/*{\Mfuncl decides whether the points in $A$ are affinely independent. }*/






inline int side_of_sphere(const array<d3_point>& A, const d3_point& x)
{ return side_of_sphere(A[0],A[1],A[2],A[3],x); }

inline int region_of_sphere(const array<d3_point>& A, const d3_point& x)
{ return region_of_sphere(A[0],A[1],A[2],A[3],x); }








/*{\Moptions nextwarning=no}*/
inline bool insphere(const d3_point& a, const d3_point& b, const d3_point& c, 
                                                           const d3_point& d,
                                                           const d3_point& e)
{ return region_of_sphere(a,b,c,d,e) > 0; }


inline bool inside_sphere(const d3_point& a, const d3_point& b, 
                                                 const d3_point& c,
                                                 const d3_point& d,
                                                 const d3_point& e)
{ return insphere(a,b,c,d,e); }
/*{\Mfuncl returns $true$ if point $e$ lies in the interior of the sphere
           through points $a$, $b$, $c$, and $d$, and $false$ otherwise. }*/


/*{\Moptions nextwarning=no}*/


/*{\Moptions nextwarning=no}*/
 inline bool outsphere(const d3_point& a, const d3_point& b, const d3_point& c, 
                                                             const d3_point& d,
                                                             const d3_point& e)
{ return region_of_sphere(a,b,c,d,e) < 0; }


inline bool outside_sphere(const d3_point& a, const d3_point& b, 
                                              const d3_point& c,
                                              const d3_point& d,
                                              const d3_point& e)
{ return outsphere(a,b,c,d,e); }
/*{\Mfuncl returns $true$ if point $e$ lies in the exterior of the sphere
           through points $a$, $b$, $c$, and $d$, and $false$ otherwise. }*/



inline bool on_sphere(const d3_point& a, const d3_point& b, const d3_point& c, 
                                                            const d3_point& d,
                                                            const d3_point& e)
{ return region_of_sphere(a,b,c,d,e) == 0; }
/*{\Mfuncl returns $true$ if $a$, $b$, $c$, $d$, and $e$ lie on a common
           sphere. }*/


extern __exportF d3_point point_on_positive_side(const d3_point& a, const d3_point& b, 
                                                             const d3_point& c);
/*{\Mfuncl returns a point $d$ with $orientation(a,b,c,d) > 0$. }*/


inline const char* leda_tname(const d3_point*) { return "d3_point"; }


#if LEDA_ROOT_INCL_ID == 600125
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

