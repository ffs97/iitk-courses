/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  line.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_LINE_H
#define LEDA_LINE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600087
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/geo/ray.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// straight lines
//------------------------------------------------------------------------------

class __exportC line;

class __exportC line_rep : public handle_rep {

friend class __exportC line;
friend class __exportC circle;

  segment  seg; 

public:
   
  line_rep() {}
  line_rep(const segment& s) : seg(s) {}

 ~line_rep() {}

friend inline int cmp_slopes(const line&, const line&);
friend inline int orientation(const line&, const point&);

};
   
/*{\Manpage {line} {} {Straight Lines}}*/

class __exportC line   : public HANDLE_BASE(line_rep) 
{

friend class __exportC circle;


/*{\Mdefinition
An instance $l$ of the data type $line$ is a directed straight line
in the two-dimensional plane. The angle between a right oriented horizontal
line and $l$ is called the direction of $l$.}*/

line_rep* ptr() const { return (line_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef double  coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef point   point_type;
/*{\Mtypemember the point type (|point|).}*/

typedef line   float_type;



/*{\Mcreation l }*/

 line(const point& p, const point& q);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line passing through points $p$ and $q$ directed form $p$ to $q$.}*/


 line(const segment& s);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line supporting segment $s$.}*/

 line(const ray& r);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line supporting ray $r$.}*/

 line(const point& p, const vector& v);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line passing through points $p$ and $p+v$.}*/

 line(const point& p, double alpha);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line passing through point $p$ with direction $alpha$.}*/

 line();
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line passing through the origin with direction 0.}*/


 line(const line& l1, int) : HANDLE_BASE(line_rep)(l1) {};
  /*{ \Mcreate 
introduces a variable |\Mvar| of type |\Mname|. 
|\Mvar| is initialized to a copy of $l_1$. 
The second argument is for compatibility with |rat_line|.}*/

 line(const line& l) : HANDLE_BASE(line_rep)(l) {};
 line& operator=(const line& l) { HANDLE_BASE(line_rep)::operator=(l); return *this; }
~line() {}



/*{\Moperations 2 5.0 }*/

point   point1() const { return ptr()->seg.source(); }
/*{\Mop     returns a point on |\Mvar|.}*/

point   point2() const { return ptr()->seg.target(); }
/*{\Mop     returns a second point on |\Mvar|.}*/


segment seg()   const  { return ptr()->seg; }
/*{\Mop     returns a segment on |\Mvar|.}*/




line   to_float() const { return *this; }
line   to_line() const { return *this; }
/*{\Xop  for compatibility with rat_line. }*/


void    normalize() const {}
/*{\Xop  for compatibility with |rat_segment|. }*/



double angle(const line& g) const { return ptr()->seg.angle(g.ptr()->seg); }
/*{\Mop     returns the angle between |\Mvar| and $g$, i.e., 
	    $g$.direction() $-$ |\Mvar|.direction().}*/

double direction() const { return ptr()->seg.angle(); }
/*{\Mop     returns the direction of |\Mvar|.}*/

double angle() const     { return ptr()->seg.angle();     }
/*{\Mop     returns |\Mvar|.direction().}*/

bool is_vertical() const    { return ptr()->seg.is_vertical();  }
/*{\Mop     returns true iff |\Mvar| is vertical.}*/

bool is_horizontal() const  { return ptr()->seg.is_horizontal();}
/*{\Mop     returns true iff |\Mvar| is horizontal.}*/


double sqr_dist(const point& q) const;
/*{\Mop     returns the square of the distance between |\Mvar| and |q|.}*/

double distance(const point& q) const;
/*{\Mop     returns the distance between |\Mvar| and |q|.}*/

int orientation(const point& p) const { return ptr()->seg.orientation(p); }
/*{\Mop     returns $orientation(\Mvar.point1(),\Mvar.point2(),p)$.}*/


double slope() const     { return ptr()->seg.slope();     }
/*{\Mop     returns the slope of |\Mvar|.\\
	    \precond  |\Mvar|  is not vertical.}*/

double y_proj(double x) const  { return ptr()->seg.y_proj(x); };
/*{\Mop     returns $p$.ycoord(), where $p \in l$ with $p$.xcoord() = $x$.\\
            \precond |\Mvar| is not vertical.}*/

double x_proj(double y) const  { return ptr()->seg.x_proj(y); };
/*{\Mop     returns $p$.xcoord(), where $p \in l$ with $p$.ycoord() = $y$.\\
            \precond |\Mvar| is not horizontal.}*/

double y_abs() const { return ptr()->seg.y_proj(0); }
/*{\Mop     returns the y-abscissa of |\Mvar| (|\Mvar|.y\_proj(0)).\\
	    \precond  |\Mvar|  is not vertical.}*/

bool intersection(const line& g, point& p) const;
/*{\Mopl    if |\Mvar| and $g$ intersect in a single point this point
            is assigned to $p$ and the result is true, otherwise 
            the result is false. }*/

bool intersection(const segment& s, point& inter) const;
/*{\Mopl    if |\Mvar| and $s$ intersect in a single point this point
            is assigned to $p$ and the result is true, otherwise 
            the result is false. }*/
	    
bool intersection(const segment& s) const;
/*{\Mopl    returns |true|, if |\Mvar| and $s$ intersect, |false| otherwise.}*/

line translate_by_angle(double a, double d) const 
{ return ptr()->seg.translate_by_angle(a,d); }
/*{\Mopl     returns |\Mvar| translated in direction $a$ by distance $d$.}*/

line translate(double dx, double dy) const 
{ return ptr()->seg.translate(dx,dy); }
/*{\Mopl     returns |\Mvar| translated by vector $(dx,dy)$.}*/

line translate(const vector& v)  const 
{ return ptr()->seg.translate(v); }
/*{\Mopl     returns |\Mvar| translated by vector $v$.\\
	    \precond $v$.dim() = 2.}*/ 

line operator+(const vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

line operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/


line rotate(const point& q, double a) const
{ return ptr()->seg.rotate(q,a); }
/*{\Mopl    returns |\Mvar| rotated about point $q$ by angle $a$.}*/

line rotate(double a) const  
{ return rotate(point(0,0),a);}
/*{\Mop     returns $l$.rotate($point(0,0),\ a$). }*/

line rotate90(const point& q, int i=1) const
{ return ptr()->seg.rotate90(q,i); }
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/


line reflect(const point& p, const point& q) const
{ return ptr()->seg.reflect(p,q); }
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/


line reverse() const { return ptr()->seg.reverse(); }
/*{\Mop     returns |\Mvar| reversed.}*/


segment perpendicular(const point& p) const;
/*{\Mop    returns the segment perpendicular to |\Mvar| with source $p$.
           and target on |\Mvar|.}*/

point dual() const;
/*{\Mop    returns the point dual to |\Mvar|.\\
           \precond |\Mvar| is not vertical.}*/

inline int side_of(const point& p) const { return orientation(p); }
/*{\Mop     computes orientation($a$, $b$, $p$), where $a \not= b$
            and $a$ and $b$ appear in this order on line $l$.}*/


bool contains(const point& p) const;
/*{\Mop     returns true if $p$ lies on |\Mvar|.}*/

bool contains(const segment&) const;

bool clip(point p, point q, segment& s) const;
/*{\Mop clips $l$ at the rectangle $R$ defined by $p$ and $q$.
Returns true if the intersection of $R$ and |\Mvar| is non-empty
and returns false otherwise. If the intersection is non-empty the 
intersection is assigned to $s$; It is guaranteed that the source node of
$s$ is no larger than its target node. }*/


bool operator==(const line& g) const { return contains(g.ptr()->seg); }

bool operator!=(const line& g) const { return !contains(g.ptr()->seg); }


/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/


friend int orientation(const line& l, const point& p);
/*{\Mfunc      computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on line $l$. }*/

friend int cmp_slopes(const line& l1, const line& l2);
/*{\Mfunc      returns compare(slope($l_1$), slope($l_2$)).}*/

friend __exportF istream& operator>>(istream& in, line& l);  

};

inline ostream& operator<<(ostream& out, const line& l) 
{ return out << l.seg(); }

inline  int orientation(const line& l, const point& p)
{ return l.orientation(p); }

inline  int cmp_slopes(const line& l1, const line& l2)
{ return cmp_slopes(l1.seg(),l2.seg()); }

inline bool parallel(const line& l1, const line& l2)
{ return cmp_slopes(l1,l2) == 0; }


extern __exportF line p_bisector(const point& p, const point& q);


inline const char* leda_tname(const line*) { return "line"; }

#if LEDA_ROOT_INCL_ID == 600087
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
