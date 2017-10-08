/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_ray.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_RAT_RAY_H
#define LEDA_RAT_RAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600094
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/rat_segment.h>
#include <LEDA/geo/ray.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// rat_rays
//------------------------------------------------------------------------------

class __exportC rat_ray;

class __exportC rat_ray_rep : public handle_rep {

friend class __exportC rat_ray;
friend class __exportC rat_line;
friend class __exportC circle;

  rat_segment  seg; 

public:
   
  rat_ray_rep() {}
  rat_ray_rep(const rat_segment& s) : seg(s) {}

 ~rat_ray_rep() {}

friend inline int cmp_slopes(const rat_ray&, const rat_ray&);
friend inline int orientation(const rat_ray&, const rat_point&);

};
   
/*{\Manpage {rat_ray} {} {Rational Rays } {r}}*/

class __exportC rat_ray   : public HANDLE_BASE(rat_ray_rep) 
{

friend class __exportC rat_line;
friend class __exportC circle;

/*{\Mdefinition
An instance $r$ of the data type $rat\_ray$ is a directed straight ray defined
by two points with rational coordinates in the two-dimensional plane.
}*/

rat_ray_rep* ptr() const { return (rat_ray_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef rat_point point_type;
/*{\Mtypemember the point type (|rat_point|).}*/

typedef ray       float_type;
/*{\Mtypemember the corresponding floatin-point type (|ray|).}*/


/*{\Mcreation r }*/

 rat_ray(const rat_point& p, const rat_point& q);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
ray starting at point $p$ and passing through point $q$. \\
\precond $p \not= q$.}*/


 rat_ray(const rat_segment& s);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
$(rat\_ray(s.source(),s.target())$.\\
\precond $s$ is nontrivial. }*/

 rat_ray(const rat_point& p, const rat_vector& v);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to
$rat_ray(p,p+v)$.}*/


 rat_ray();
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|.}*/

rat_ray(const ray& r1, int prec = rat_point::default_precision)
{ PTR = new rat_ray_rep(rat_segment(rat_point(r1.source(),prec), 
                                    rat_point(r1.point2(),prec))); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the ray obtained by approximating the two
            defining points of $r_1$. }*/
   


 rat_ray(const rat_ray& r) : HANDLE_BASE(rat_ray_rep)(r) {};
 rat_ray& operator=(const rat_ray& r) { HANDLE_BASE(rat_ray_rep)::operator=(r); return *this; }

~rat_ray() {}



/*{\Moperations 2 5.0 }*/

ray to_ray() const;

ray to_float() const { return to_ray(); }
/*{\Mop     returns a floating point approximation of |\Mvar|. }*/

void normalize() const { ptr()->seg.normalize(); }
/*{\Mop simplifies the homogenous representation by calling
        |point1().normalize()| and |point2().normlize()|. }*/


rat_point  source() const   { return ptr()->seg.source(); }
/*{\Mop     returns the source of |\Mvar|.}*/

rat_point  point1() const   { return ptr()->seg.source(); }
/*{\Mop     returns the source of |\Mvar|.}*/

rat_point  point2() const   { return ptr()->seg.target(); }
/*{\Mop     returns a point on |\Mvar| different from |\Mvar.source()|.}*/


bool is_vertical() const    { return ptr()->seg.is_vertical();  }
/*{\Mop     returns true iff |\Mvar| is vertical.}*/

bool is_horizontal() const  { return ptr()->seg.is_horizontal();}
/*{\Mop     returns true iff |\Mvar| is horizontal.}*/


rat_segment seg()  const     { return ptr()->seg; }


bool intersection(const rat_ray& s, rat_point& inter) const;
/*{\Mopl    returns true if |\Mvar| and $s$ intersect. If so, a point of intersection is returned in |inter|. }*/

bool intersection(const rat_segment& s, rat_point& inter) const;
/*{\Mopl    returns true if |\Mvar| and $s$ intersect. If so, a point of intersection is returned in |inter|. }*/


rat_ray translate(const rational& dx, const rational& dy) const
{ return ptr()->seg.translate(dx,dy); }
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/


rat_ray translate(integer dx, integer dy, integer dw) const
{ return ptr()->seg.translate(dx,dy,dw); }
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/

rat_ray  translate(const rat_vector& v)  const 
{ return ptr()->seg.translate(v); }
/*{\Mop     returns $r+v$, i.e., $r$ translated by vector $v$.\\
            \precond $v$.dim() = 2.}*/ 

rat_ray  operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

rat_ray  operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/


rat_ray  rotate90(const rat_point& q, int i=1) const
{ return ptr()->seg.rotate90(q,i); }
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/


rat_ray  reflect(const rat_point& p, const rat_point& q) const
{ return ptr()->seg.reflect(p,q); }
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.\\
\precond $p \not= q$.}*/

rat_ray  reflect(const rat_point& p)
{ return ptr()->seg.reflect(p); }
/*{\Mop     returns |\Mvar| reflected  across point $p$. }*/

rat_ray reverse() const { return ptr()->seg.reverse(); }
/*{\Mop     returns |\Mvar| reversed.}*/


bool contains(const rat_point& p) const;
/*{\Mopl    decides whether |\Mvar| contains $p$. }*/

bool contains(const rat_segment& s) const;
/*{\Mopl    decides whether |\Mvar| contains $s$. }*/


bool operator==(const rat_ray& g) const;
bool operator!=(const rat_ray& g) const { return !operator==(g); }


/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/


friend int orientation(const rat_ray& r, const rat_point& p);
/*{\Mfunc      computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on ray $r$. }*/

friend int cmp_slopes(const rat_ray& r1, const rat_ray& r2);
/*{\Mfunc      returns compare(slope($r_1$), slope($r_2$)).}*/


friend __exportF ostream& operator<<(ostream& out, const rat_ray& r);
friend __exportF istream& operator>>(istream& in, rat_ray& r);  

};

inline  int orientation(const rat_ray& r, const rat_point& p)
{ return orientation(r.ptr()->seg,p); }

inline  int cmp_slopes(const rat_ray& r1, const rat_ray& r2)
{ return cmp_slopes(r1.ptr()->seg,r2.ptr()->seg); }

inline bool parallel(const rat_ray& r1, const rat_ray& r2)
{ return cmp_slopes(r1,r2) == 0; }



inline const char* leda_tname(const rat_ray*) { return "rat_ray"; }

#if LEDA_ROOT_INCL_ID == 600094
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif



