/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  triangle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_TRIANGLE_H
#define LEDA_TRIANGLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600128
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/geo/point.h>
#include <LEDA/geo/line.h>
#include <LEDA/geo/geo_global_enums.h>

LEDA_BEGIN_NAMESPACE

class __exportC triangle;

//------------------------------------------------------------------------------
// triangles
//------------------------------------------------------------------------------


class __exportC triangle_rep : public handle_rep {

static atomic_counter id_counter;

friend class __exportC triangle;
   
   point a;
   point b;
   point c;

   unsigned long id;

public:
   
   triangle_rep(const point&, const point&, const point&);
  ~triangle_rep() {}

   friend inline unsigned long ID_Number(const triangle&);

};

/*{\Manpage {triangle} {} {Triangles}}*/

class __exportC triangle  : public HANDLE_BASE(triangle_rep) 
{
/*{\Mdefinition
    An instance $t$ of the data type $triangle$ is an oriented triangle
    in the two-dimensional plane. A triangle splits the plane into one
    bounded and one unbounded region. If the triangle is positively
    oriented, the bounded region is to the left of it, if it is negatively
    oriented, the unbounded region is to the left of it.
    A triangle $t$ is called degenerate, if the 3 vertices of $t$ are collinear.
}*/

triangle_rep* ptr() const { return (triangle_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef double  coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef point   point_type;
/*{\Mtypemember the point type (|point|).}*/

typedef triangle float_type;


/*{\Mcreation t}*/

triangle();                 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the empty triangle.}*/


triangle(const point& p, const point& q, const point& r); 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the triangle $[p,q,r]$. }*/


triangle(double x1, double y1, double x2, double y2, double x3, double y3) ;
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the triangle $[(x1,y1),(x2,y2),(x3,y3)]$.}*/ 


// for compatibility with rat_triangle:
 triangle(const triangle& t, int) : HANDLE_BASE(triangle_rep)(t) {} 

 triangle(const triangle& t) : HANDLE_BASE(triangle_rep)(t) {}     
~triangle() {}
 triangle& operator=(const triangle& t) 
 { HANDLE_BASE(triangle_rep)::operator=(t); return *this;}


/*{\Moperations 2 3.5}*/

triangle  to_float() const { return *this; }
triangle  to_triangle() const { return *this; }
/*{\Xop  for compatibility with rat_triangle. }*/


void    normalize() const {}
/*{\Xop  for compatibility with |rat_triangle|. }*/



/*{\Moptions nextwarning=no}*/
point point1()    const      { return ptr()->a; }
/*{\Mop       returns the first vertex of triangle |\Mvar|.}*/

point point2()    const      { return ptr()->b; }
/*{\Mop       returns the second vertex of triangle |\Mvar|.}*/

point point3()    const      { return ptr()->c; }
/*{\Mop       returns the third vertex of triangle |\Mvar|.}*/

point operator[](int i) const;
/*{\Marrop returns the $i$-th vertex of |\Mvar|. \precond $1\le i\le 3$.}*/

int orientation() const;
/*{\Mop       returns the orientation of |\Mvar|.}*/

double area() const;
/*{\Mop       returns the signed area of |\Mvar| (positive, if $orientation(a,b,c)>0$,
              negative otherwise).}*/

bool is_degenerate() const;
/*{\Mopl    returns true if the vertices of |\Mvar| are collinear. }*/

int side_of(const point& p) const;
/*{\Mopl    returns $+1$ if $p$ lies to the left of |\Mvar|, $0$ if $p$ lies on |\Mvar|
            and $-1$ if $p$ lies to the right of |\Mvar|.}*/

region_kind region_of(const point& p) const;
/*{\Mopl    returns $BOUNDED\_REGION$ if $p$ lies in the bounded region of |\Mvar|, 
            $ON\_REGION$ if $p$ lies on |\Mvar| and $UNBOUNDED\_REGION$ if $p$ lies in
	    the unbounded region.}*/

bool   inside(const point& p) const;
/*{\Mopl    returns true, if $p$ lies to the left of |\Mvar|.}*/

bool   outside(const point& p) const;
/*{\Mopl    returns true, if $p$ lies to the right of |\Mvar|.}*/

bool   on_boundary(const point& p) const;
/*{\Mopl    decides whether $p$ lies on the boundary of |\Mvar|.}*/

bool   contains(const point& p) const;
/*{\Mopl    decides whether |\Mvar| contains $p$. }*/


bool   intersection(const line& l) const;
/*{\Mopl    decides whether the bounded region or the boundary of |\Mvar| and $l$ intersect. }*/

bool   intersection(const segment& s) const;
/*{\Mopl    decides whether the bounded region or the boundary of |\Mvar| and $s$ intersect. }*/


triangle translate(double dx, double dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

triangle translate(const vector& v) const;
/*{\Mop     returns $t+v$, i.e.,  |\Mvar| translated by vector $v$.\\
	    \precond $v$.dim() = 2.}*/ 

triangle operator+(const vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

triangle operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/

triangle rotate(const point& q, double a) const;
/*{\Mopl    returns |\Mvar| rotated about point $q$ by angle $a$.}*/

triangle rotate(double alpha) const;
/*{\Mop     returns $t$.rotate($t$.point1(), $alpha$).}*/

triangle rotate90(const point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

triangle rotate90(int i=1) const;
/*{\Mop     returns $t$.rotate90($t$.source(),i).}*/


triangle reflect(const point& p, const point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

triangle reflect(const point& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/


triangle reverse() const { return triangle(point3(),point2(),point1()); }
/*{\Mop     returns |\Mvar| reversed.}*/

bool operator==(const triangle& t2) const;

bool operator!=(const triangle& t2) const
{ return (!((*this)==t2)); }


friend inline unsigned long ID_Number(const triangle&);

};

inline unsigned long ID_Number(const triangle& t) { return t.ptr()->id; }

inline const char* leda_tname(const triangle*) { return "triangle"; }


inline ostream& operator<<(ostream& out, const triangle& T)
{ return out << T.point1() << " " << T.point2() << " " << T.point3(); }

inline istream& operator>>(istream& in, triangle& T) 
{ point a,b,c;
  in >> a >> b >> c;
  T = triangle(a,b,c);
  return in; 
}

#if LEDA_ROOT_INCL_ID == 600128
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

