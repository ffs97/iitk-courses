/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ray.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAY_H
#define LEDA_RAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600056
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// straight rays
//------------------------------------------------------------------------------

class __exportC ray;

class __exportC ray_rep : public handle_rep {

friend class __exportC ray;
friend class __exportC line;
friend class __exportC circle;

  segment  seg; 

public:
   
  ray_rep() {}
  ray_rep(const segment& s) : seg(s) {}

 ~ray_rep() {}

friend inline int cmp_slopes(const ray&, const ray&);
friend inline int orientation(const ray&, const point&);

};
   
/*{\Manpage {ray} {} {Straight Rays} {r}}*/

class __exportC ray   : public HANDLE_BASE(ray_rep) 
{

friend class __exportC line;
friend class __exportC circle;

/*{\Mdefinition
An instance $r$ of the data type $ray$ is a directed straight ray
in the two-dimensional plane. The angle between a right oriented horizontal
ray and $r$ is called the direction of $r$.}*/

ray_rep* ptr() const { return (ray_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef double  coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef point   point_type;
/*{\Mtypemember the point type (|point|).}*/

typedef ray    float_type;


/*{\Mcreation r }*/

 ray(const point& p, const point& q);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
ray starting at point $p$ and passing through point $q$. }*/


 ray(const segment& s);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to 
$ray(s.source(),s.target())$. }*/

 ray(const point& p, const vector& v);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to
$ray(p,p+v)$.}*/

 ray(const point& p, double alpha);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
ray starting at point $p$ with direction $alpha$.}*/

 ray();
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
ray starting at the origin with direction 0.}*/


 ray(const ray& r1, int) : HANDLE_BASE(ray_rep)(r1) {};
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to a
copy of $r_1$. The second argument is for compatibility with |rat_ray|.}*/

 
 ray(const ray& r) : HANDLE_BASE(ray_rep)(r) {};
 ray& operator=(const ray& r) { HANDLE_BASE(ray_rep)::operator=(r); return *this; }
~ray() {}



/*{\Moperations 2 5.0 }*/

ray  to_float() const { return *this; }
ray  to_ray() const { return *this; }
/*{\Xop  for compatibility with rat_ray. }*/


void    normalize() const {}
/*{\Xop  for compatibility with |rat_segment|. }*/


point  source() const   { return ptr()->seg.source(); }
/*{\Mop     returns the source of |\Mvar|.}*/

point  point1() const   { return ptr()->seg.source(); }
/*{\Mop     returns the source of |\Mvar|.}*/

point  point2() const   { return ptr()->seg.target(); }
/*{\Mop     returns a point on |\Mvar| different from |\Mvar.source()|.}*/

double direction() const     { return ptr()->seg.angle(); }
/*{\Mop     returns the direction of |\Mvar|.}*/

double angle(const ray& s) const { return ptr()->seg.angle(s.ptr()->seg); }
/*{\Mop     returns the angle between |\Mvar| and $s$, i.e., 
	    $s$.direction() $-$ |\Mvar|.direction().}*/

bool is_vertical() const    { return ptr()->seg.is_vertical();  }
/*{\Mop     returns true iff |\Mvar| is vertical.}*/

bool is_horizontal() const  { return ptr()->seg.is_horizontal();}
/*{\Mop     returns true iff |\Mvar| is horizontal.}*/


double slope() const     { return ptr()->seg.slope();     }
/*{\Mop     returns the slope of the straight line underlying |\Mvar|.\\
	    \precond  |\Mvar|  is not vertical.}*/

segment seg()  const     { return ptr()->seg; }


bool intersection(const ray& s, point& inter) const;
/*{\Mopl    if $r$ and $s$ intersect in a single point this point 
            is assigned to $inter$ and the result is |true|, otherwise 
            the result is |false|.}*/

bool intersection(const segment& s, point& inter) const;
/*{\Mopl    if $r$ and $s$ intersect in a single point this point 
            is assigned to $inter$ and the result is |true|, otherwise 
            the result is |false|.}*/

ray translate_by_angle(double a, double d) const 
{ return ptr()->seg.translate_by_angle(a,d); }
/*{\Mopl     returns |\Mvar| translated in direction $a$ by distance $d$.}*/

ray translate(double dx, double dy) const 
{ return ptr()->seg.translate(dx,dy); }
/*{\Mopl     returns |\Mvar| translated by vector $(dx,dy)$.}*/

ray translate(const vector& v)  const 
{ return ptr()->seg.translate(v); }
/*{\Mopl     returns |\Mvar| translated by vector $v$\\
	    \precond $v$.dim() = 2.}*/ 

ray  operator+(const vector& v) const { return translate(v); }
/*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

ray  operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/


ray  rotate(const point& q, double a) const
{ return ptr()->seg.rotate(q,a); }
/*{\Mopl    returns |\Mvar| rotated about point $q$ by angle $a$.}*/

ray  rotate(double a) const  
{ return rotate(point(0,0),a);}
/*{\Mop     returns $r$.rotate($point(0,0),\ a$). }*/

ray  rotate90(const point& q, int i=1) const
{ return ptr()->seg.rotate90(q,i); }
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/


ray  reflect(const point& p, const point& q) const
{ return ptr()->seg.reflect(p,q); }
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

ray  reflect(const point& p) const
{ return ptr()->seg.reflect(p); }
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/


ray reverse() const { return ptr()->seg.reverse(); }
/*{\Mop     returns |\Mvar| reversed.}*/


bool contains(const point&) const;
/*{\Mopl    decides whether |\Mvar| contains $p$. }*/

bool contains(const segment&) const;
/*{\Mopl    decides whether |\Mvar| contains $s$. }*/


bool operator==(const ray& s) const;
bool operator!=(const ray& s) const { return !operator==(s); }



/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/


friend int orientation(const ray& r, const point& p);
/*{\Mfunc      computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on ray $r$. }*/

friend int cmp_slopes(const ray& r1, const ray& r2);
/*{\Mfunc      returns compare(slope($r_1$), slope($r_2$)) where
              $slope(r_i)$ denotes the slope of the straight line
              underlying $r_i$. }*/


friend __exportF istream& operator>>(istream& in, ray& r);  

};

inline ostream& operator<<(ostream& out, const ray& r) 
{ return out << r.seg(); }

inline  int orientation(const ray& r, const point& p)
{ return orientation(r.ptr()->seg,p); }

inline  int cmp_slopes(const ray& r1, const ray& r2)
{ return cmp_slopes(r1.ptr()->seg,r2.ptr()->seg); }

inline bool parallel(const ray& r1, const ray& r2)
{ return cmp_slopes(r1,r2) == 0; }



inline const char* leda_tname(const ray*) { return "ray"; }

#if LEDA_ROOT_INCL_ID == 600056
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
