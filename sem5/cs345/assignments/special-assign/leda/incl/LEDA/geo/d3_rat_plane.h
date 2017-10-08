/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_rat_plane.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_RAT_PLANE_H
#define LEDA_D3_RAT_PLANE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600076
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/numbers/rat_vector.h>
#include <LEDA/geo/d3_rat_point.h>
#include <LEDA/geo/d3_plane.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_rat_plane;

//------------------------------------------------------------------------------
// d3_rat_planes
//------------------------------------------------------------------------------

class __exportC d3_rat_plane_rep  : public handle_rep {

friend class __exportC d3_rat_plane;
friend __exportF int side_of(const d3_rat_plane&, const d3_rat_point&);

static atomic_counter id_counter;
   
   d3_rat_point a;
   d3_rat_point b;
   d3_rat_point c;
   
   // plane: Ax + Bx + Cx + D = 0

   integer     nx; // = A
   integer     ny; // = B
   integer     nz; // = C
   
   integer     nd; // = D
   
   // double approx. 
   double     nx_d; 
   double     ny_d;
   double     nz_d; 
   double     nd_d;   

   unsigned long id;

public:
    
 d3_rat_plane_rep(const d3_rat_point&, const d3_rat_point&, const d3_rat_point&,
                  const integer&, const integer&, const integer&);
~d3_rat_plane_rep() {}

friend inline unsigned long ID_Number(const d3_rat_plane&);
   
};

/*{\Manpage {d3_rat_plane} {} {Rational Planes}}*/

class __exportC d3_rat_plane  : public HANDLE_BASE(d3_rat_plane_rep)
{
/*{\Mdefinition
An instance $P$ of the data type |\Mname| is an oriented rational plane in the 
three-dimensional space $\real ^3$. It can be defined by a tripel ($a$,$b$,$c$)
of non-collinear rational points or a single rational point $a$ and a 
normal vector $v$. }*/

 d3_rat_plane_rep* ptr() const { return (d3_rat_plane_rep*)PTR; }

public:

/*{\Mcreation p }*/

 d3_rat_plane();
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the trivial plane. }*/

 d3_rat_plane(const d3_rat_point& a, const d3_rat_point& b, const d3_rat_point& c);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized to 
            the plane through $(a,b,c)$.\\
            \precond $a$, $b$, and $c$ are
            not collinear. }*/

 d3_rat_plane(const d3_rat_point& a, const rat_vector& v);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the plane that contains $a$ with normal vector $v$.\\
            \precond |v.dim()| $= 3$ and |v.length() > 0|. }*/

 d3_rat_plane(const d3_rat_point& a, const d3_rat_point& b);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname| initialized 
            to the plane that contains $a$ with normal vector $b-a$.}*/

 d3_rat_plane(const d3_rat_plane& p) : HANDLE_BASE(d3_rat_plane_rep)(p) {}
~d3_rat_plane() {}

d3_rat_plane& operator=(const d3_rat_plane& p) 
{ HANDLE_BASE(d3_rat_plane_rep)::operator=(p); return *this; }


/*{\Moperations 2.5 3.7}*/


d3_rat_point point1() const { return ptr()->a; }
/*{\Mop     returns the first point of |\Mvar|.}*/

d3_rat_point point2() const { return ptr()->b; }
/*{\Mop     returns the second point of |\Mvar|.}*/

d3_rat_point point3() const { return ptr()->c; }
/*{\Mop     returns the third point of |\Mvar|.}*/


// A*x + B*y + C*z + D = 0;

integer   A() const 
/*{\Mop  returns the $A$ parameter of the plane equation.}*/
{ return ptr()->nx; }

integer   B() const 
/*{\Mop  returns the $B$ parameter of the plane equation.}*/
{ return ptr()->ny; }

integer   C() const 
/*{\Mop  returns the $C$ parameter of the plane equation.}*/
{ return ptr()->nz; }

integer   D() const 
/*{\Mop  returns the $D$ parameter of the plane equation.}*/ 
{ return ptr()->nd; }


rat_vector  normal() const 
{ return rat_vector(ptr()->nx,ptr()->ny,ptr()->nz,1); }
/*{\Mop     returns a normal vector of |\Mvar|.}*/


d3_plane to_d3_plane() const;

d3_plane to_float() const { return to_d3_plane(); }
/*{\Mop  returns a floating point approximation of |\Mvar|. }*/


rational sqr_dist(const d3_rat_point& q) const;
/*{\Mop     returns the square of the Euclidean distance between |\Mvar|
            and $q$.}*/


int cmp_distances(const d3_rat_point& p1, const d3_rat_point& p2) const;

rat_vector normal_project(const d3_rat_point& q) const;
/*{\Mop     returns the vector pointing from |q| to its projection
            on |\Mvar| along the normal direction.}*/

int intersection(const d3_rat_point p1, const d3_rat_point p2, 
                                              d3_rat_point& q) const;
/*{\Mop     if the line |l| through |p1| and |p2| intersects |\Mvar| in
            a single point this point is assigned to |q| and the result is $1$,
            if |l| and |\Mvar| do not intersect the result is $0$, and if |l| 
            is contained in |\Mvar| the result is $2$. }*/

int intersection(const d3_rat_plane& Q, d3_rat_point& i1, 
                                        d3_rat_point& i2) const;
/*{\Mop     if |\Mvar| and plane |Q| intersect in a line $L$ then $(i1,i2)$ 
            are assigned two different points on $L$ and the result is $1$,
            if |\Mvar| and |Q| do not intersect the result is $0$, and if
            $\Mvar = Q$ the result is $2$. }*/ 

d3_rat_plane translate(const rational& dx, const rational& dy, 
                                           const rational& dz) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy,dz)$.}*/


d3_rat_plane translate(integer dx, integer dy, integer dz, integer dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw,dz/dw)$.}*/


d3_rat_plane translate(const rat_vector& v) const;
/*{\Mop     returns \Mvar$+v$, i.e., |\Mvar| translated by vector $v$.\\
	    \precond $v$.dim() = 3.}*/

d3_rat_plane operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/


d3_rat_plane reflect(const d3_rat_plane& Q) const;
/*{\Mop     returns |\Mvar| reflected  across plane $Q$.}*/

d3_rat_plane reflect(const d3_rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected across point $q$. }*/

d3_rat_point reflect_point(const d3_rat_point& q) const;
/*{\Mop     returns |q| reflected across plane |\Mvar|.}*/

int side_of(const d3_rat_point& q) const;
/*{\Mop computes the side of |\Mvar| on which |q| lies. }*/

bool contains(const d3_rat_point& q) const { return side_of(q) == 0;}
/*{\Mop     returns true if point |q| lies on plane |\Mvar|, i.e.,
            (|\Mvar.side_of(q) == 0|), and false otherwise .}*/

bool parallel(const d3_rat_plane& Q) const;
/*{\Mop     returns true if planes |\Mvar| and |Q| are parallel, and 
            false otherwise. }*/

bool operator==(const d3_rat_plane& Q) const;
bool operator!=(const d3_rat_plane& Q) const { return !operator==(Q);}


friend __exportF ostream& operator<<(ostream& O, const d3_rat_plane& p) ;
/*{\Mbinopfuncl  writes |\Mvar| to output stream $O$.}*/

friend __exportF istream& operator>>(istream& I, d3_rat_plane& p);
/*{\Mbinopfuncl  reads |\Mvar| from input stream $I$.}*/


friend unsigned long ID_Number(const d3_rat_plane&);

};

/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

inline int orientation(const d3_rat_plane& p, const d3_rat_point& q) 
{ return p.side_of(q); }
/*{\Mfuncl   computes the orientation of |p.sideof(q)|.}*/



inline unsigned long ID_Number(const d3_rat_plane& p) { return p.ptr()->id; }
inline const char* leda_tname(const d3_rat_plane*) { return "d3_rat_plane"; }


#if LEDA_ROOT_INCL_ID == 600076
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

