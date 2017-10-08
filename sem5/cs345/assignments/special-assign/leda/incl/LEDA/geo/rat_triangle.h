/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_triangle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_TRIANGLE_H
#define LEDA_RAT_TRIANGLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600093
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/geo/triangle.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/rat_line.h>
#include <LEDA/geo/geo_global_enums.h>

LEDA_BEGIN_NAMESPACE

class __exportC rat_triangle;

//------------------------------------------------------------------------------
// rat_triangles
//------------------------------------------------------------------------------


class __exportC rat_triangle_rep : public handle_rep {

static atomic_counter id_counter;

friend class __exportC rat_triangle;
   
   rat_point a;
   rat_point b;
   rat_point c;

   unsigned long id;

public:
   
   rat_triangle_rep(const rat_point&, const rat_point&, const rat_point&);
  ~rat_triangle_rep() {}

   friend inline unsigned long ID_Number(const rat_triangle&);

};

/*{\Manpage {rat_triangle} {} {Rational Triangles}}*/

class __exportC rat_triangle  : public HANDLE_BASE(rat_triangle_rep) 
{
/*{\Mdefinition
    An instance $t$ of the data type |rat_triangle| is an oriented triangle
    in the two-dimensional plane with rational coordinates. 
    A |rat_triangle| $t$ splits the plane into one
    bounded and one unbounded region. If $t$ is positively
    oriented, the bounded region is to the left of it, if it is negatively
    oriented, the unbounded region is to the left of it.
    $t$ is called degenerate, if the 3 vertices of $t$ are collinear.   
}*/

rat_triangle_rep* ptr() const { return (rat_triangle_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef rat_point   point_type;
/*{\Mtypemember the point type (|rat_point|).}*/

typedef triangle float_type;


/*{\Mcreation t}*/

rat_triangle();                 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the empty triangle.}*/

rat_triangle(const rat_point& p, const rat_point& q, const rat_point& r); 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the triangle $[p,q,r]$. }*/


rat_triangle(const rational& x1, const rational& y1,
             const rational& x2, const rational& y2,
	     const rational& x3, const rational& y3) ;
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the triangle $[(x1,y1),(x2,y2),(x3,y3)]$.}*/ 


rat_triangle(const triangle& t, int prec = rat_point::default_precision) 
{ PTR = new rat_triangle_rep(rat_point(t.point1(),prec), 
                             rat_point(t.point2(),prec), 
                             rat_point(t.point3(),prec)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the triangle obtained by approximating the three
            defining points of $t$. }*/


 rat_triangle(const rat_triangle& t) : HANDLE_BASE(rat_triangle_rep)(t) {}     
~rat_triangle() {}
 rat_triangle& operator=(const rat_triangle& t) 
 { HANDLE_BASE(rat_triangle_rep)::operator=(t); return *this;}


/*{\Moperations 2 3.5}*/

triangle  to_float() const 
{ return triangle(point1().to_float(),point2().to_float(),point3().to_float()); }

triangle  to_triangle() const 
{ return triangle(point1().to_float(),point2().to_float(),point3().to_float()); }


void normalize() const 
{ point1().normalize(); 
  point2().normalize();  
  point3().normalize(); 
}
/*{\Mop simplifies the homogenous representation by calling
        |p.normalize()| for every vertex of |\Mvar|. }*/


/*{\Moptions nextwarning=no}*/
rat_point point1()    const      { return ptr()->a; }
/*{\Mop       returns the first vertex of triangle |\Mvar|.}*/

rat_point point2()    const      { return ptr()->b; }
/*{\Mop       returns the second vertex of triangle |\Mvar|.}*/

rat_point point3()    const      { return ptr()->c; }
/*{\Mop       returns the third vertex of triangle |\Mvar|.}*/

rat_point operator[](int i) const;
/*{\Marrop returns the $i$-th vertex of |\Mvar|. \precond $1\le i\le 3$.}*/

int orientation() const;
/*{\Mop       returns the orientation of |\Mvar|.}*/

rational area() const;
/*{\Mop       returns the signed area of |\Mvar| (positive, if $orientation(a,b,c)>0$,
              negative otherwise).}*/

bool is_degenerate() const;
/*{\Mopl    returns true if the vertices of |\Mvar| are collinear. }*/

int side_of(const rat_point& p) const;
/*{\Mopl    returns $+1$ if $p$ lies to the left of |\Mvar|, $0$ if $p$ lies on |\Mvar|
            and $-1$ if $p$ lies to the right of |\Mvar|.}*/

region_kind region_of(const rat_point& p) const;
/*{\Mopl    returns $BOUNDED\_REGION$ if $p$ lies in the bounded region of |\Mvar|, 
            $ON\_REGION$ if $p$ lies on |\Mvar| and $UNBOUNDED\_REGION$ if $p$ lies in
	    the unbounded region.}*/

bool   inside(const rat_point& p) const;
/*{\Mopl    returns true, if $p$ lies to the left of |\Mvar|.}*/

bool   outside(const rat_point& p) const;
/*{\Mopl    returns true, if $p$ lies to the right of |\Mvar|.}*/

bool   on_boundary(const rat_point& p) const;
/*{\Mopl    decides whether $p$ lies on the boundary of |\Mvar|.}*/

bool   contains(const rat_point& p) const;
/*{\Mopl    decides whether |\Mvar| contains $p$. }*/

bool   intersection(const rat_line& l) const;
/*{\Mopl    decides whether the bounded region or the boundary of |\Mvar| and $l$ intersect. }*/

bool   intersection(const rat_segment& s) const;
/*{\Mopl    decides whether the bounded region or the boundary of |\Mvar| and $s$ intersect. }*/

rat_triangle translate(rational dx, rational dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

rat_triangle translate(const rat_vector& v) const;
/*{\Mop     returns $t+v$, i.e.,  |\Mvar| translated by vector $v$.\\
	    \precond $v$.dim() = 2.}*/ 

rat_triangle operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

rat_triangle operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/


rat_triangle rotate90(const rat_point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

rat_triangle rotate90(int i=1) const;
/*{\Mop     returns $t$.rotate90($t$.source(),i).}*/


rat_triangle reflect(const rat_point& p, const rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

rat_triangle reflect(const rat_point& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/


rat_triangle reverse() const { return rat_triangle(point3(),point2(),point1()); }
/*{\Mop     returns |\Mvar| reversed.}*/

bool operator==(const rat_triangle& t2) const;

bool operator!=(const rat_triangle& t2) const
{ return (!((*this)==t2)); }

friend inline unsigned long ID_Number(const rat_triangle&);

};


inline unsigned long ID_Number(const rat_triangle& t) { return t.ptr()->id; }

inline const char* leda_tname(const rat_triangle*) { return "rat_triangle"; }

inline ostream& operator<<(ostream& out, const rat_triangle& T)
{ return out << T.point1() << " " << T.point2() << " " << T.point3(); }

inline istream& operator>>(istream& in, rat_triangle& T) 
{ rat_point a,b,c;
  in >> a >> b >> c;
  T= rat_triangle(a,b,c);
  return in; 
 }


#if LEDA_ROOT_INCL_ID == 600093
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

