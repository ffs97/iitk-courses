/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_line.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_RAT_LINE_H
#define LEDA_RAT_LINE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600050
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/rat_segment.h>
#include <LEDA/geo/rat_ray.h>
#include <LEDA/geo/line.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// straight rat_lines
//------------------------------------------------------------------------------

class __exportC rat_line;

class __exportC rat_line_rep : public handle_rep {

friend class __exportC rat_line;
friend class __exportC rat_circle;

  rat_segment  seg; 

public:
   
  rat_line_rep() {}

  rat_line_rep(const rat_segment& s) : seg(s) {}

 ~rat_line_rep() {}

friend inline int cmp_slopes(const rat_line&l1, const rat_line& l2);
friend inline int orientation(const rat_line& l, const rat_point& p);

};
   
/*{\Manpage {rat_line} {} {Straight Rational Lines}}*/

class __exportC rat_line   : public HANDLE_BASE(rat_line_rep) 
{

friend class __exportC circle;

/*{\Mdefinition
An instance $l$ of the data type $rat\_line$ is a directed straight line
in the two-dimensional plane. 
}*/

rat_line_rep* ptr() const { return (rat_line_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef rat_point point_type;
/*{\Mtypemember the point type (|rat_point|).}*/

typedef line       float_type;
/*{\Mtypemember the corresponding floatin-point type (|line|).}*/



/*{\Mcreation l }*/

 rat_line(const rat_point& p, const rat_point& q);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line passing through points $p$ and $q$ directed form $p$ to $q$.\\
\precond $p \not= q$.}*/


 rat_line(const rat_segment& s);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line supporting segment $s$.\\
\precond $s$ is nontrivial.}*/


rat_line(const rat_point& p, const rat_vector& v);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line passing through points $p$ and $p+v$.\\
\precond $v$ is a nonzero vector.}*/



 rat_line(const rat_ray& r);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
line supporting ray $r$.}*/



 rat_line();
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|.}*/

rat_line(const line& l1, int prec = rat_point::default_precision)
{ PTR = new rat_line_rep(rat_segment(rat_point(l1.point1(),prec), 
                                     rat_point(l1.point2(),prec))); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the line obtained by approximating the two
defining points of $l_1$. }*/
   


 rat_line(const rat_line& l) : HANDLE_BASE(rat_line_rep)(l) {};
 rat_line& operator=(const rat_line& l) 
 { HANDLE_BASE(rat_line_rep)::operator=(l); return *this; }

~rat_line() {}



/*{\Moperations 2 5.0 }*/

line to_line() const;

line to_float() const { return to_line(); }
/*{\Mop  returns a floating point approximation of |\Mvar|.}*/


void normalize() const { ptr()->seg.normalize(); }
/*{\Mop simplifies the homogenous representation by calling
        |point1().normalize()| and |point2().normlize()|. }*/


rat_point   point1() const { return ptr()->seg.source(); }
/*{\Mop     returns a point on |\Mvar|.}*/

rat_point   point2() const { return ptr()->seg.target(); }
/*{\Mop     returns a second point on |\Mvar|.}*/

rat_segment seg()   const  { return ptr()->seg; }
/*{\Mop     returns a segment on |\Mvar|.}*/


bool is_vertical() const    { return ptr()->seg.is_vertical();  }
/*{\Mopl    decides whether |\Mvar| is vertical. }*/

bool is_horizontal() const  { return ptr()->seg.is_horizontal();}
/*{\Mopl    decides whether |\Mvar| is horizontal. }*/

rational slope() const { return ptr()->seg.slope(); }
/*{\Mop     returns the slope of $s$.\\
\precond |\Mvar| is not vertical.}*/

rational  x_proj(rational y) const { return ptr()->seg.x_proj(y); }
/*{\Mop     returns $p$.xcoord(), where $p \in line(|\Mvar|)$ with 
            $p$.ycoord() = $y$.\\
\precond |\Mvar| is not horizontal.}*/

rational  y_proj(rational x) const { return ptr()->seg.y_proj(x); }
/*{\Mop     returns $p$.ycoord(), where $p \in line(|\Mvar|)$ with 
            $p$.xcoord() = $x$.\\
\precond |\Mvar| is not vertical.}*/

rational  y_abs() const { return ptr()->seg.y_abs(); }
/*{\Mop     returns the y-abscissa of $line(\Mvar)$, i.e., |\Mvar|.y\_proj(0).\\
\precond |\Mvar|  is not vertical.}*/


bool intersection(const rat_line& g, rat_point& inter) const;
/*{\Mopl    returns true if $l$ and $g$ intersect. In case of intersection a common point is returned in |inter|.}*/

bool intersection(const rat_segment& s, rat_point& inter) const;
/*{\Mopl    returns true if $l$ and $s$ intersect. In case of intersection a common point is returned in |inter|.}*/

bool intersection(const rat_segment& s) const;
/*{\Mopl    returns |true|, if |\Mvar| and $s$ intersect, |false| otherwise.}*/

rat_line translate(const rational& dx, const rational& dy) const
{ return ptr()->seg.translate(dx,dy); }
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

rat_line translate(integer dx, integer dy, integer dw) const
{ return ptr()->seg.translate(dx,dy,dw); }
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/

rat_line translate(const rat_vector& v)  const 
{ return ptr()->seg.translate(v); }
/*{\Mopl   returns |\Mvar| translated by vector $v$.\\
           \precond $v$.dim() = 2.}*/ 

rat_line  operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

rat_line  operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/



rat_line rotate90(const rat_point& q, int i=1) const
{ return ptr()->seg.rotate90(q,i); }
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/


rat_line reflect(const rat_point& p, const rat_point& q) const
{ return ptr()->seg.reflect(p,q); }
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

rat_line reflect(const rat_point& p) const
{ return ptr()->seg.reflect(p); }
/*{\Mop     returns |\Mvar| reflected across point $p$. }*/


rat_line reverse() const { return ptr()->seg.reverse(); }
/*{\Mop     returns |\Mvar| reversed.}*/


rational sqr_dist(const rat_point& q) const;
/*{\Mop     returns the square of the distance between |\Mvar| and |q|.}*/


rat_segment perpendicular(const rat_point& p) const;
/*{\Mop    returns the segment perpendicular to |\Mvar| with source $p$
           and target on |\Mvar|.}*/

rat_point dual() const;
/*{\Mop    returns the point dual to |\Mvar|.\\
           \precond |\Mvar| is not vertical.}*/


inline int orientation(const rat_point& p) const 
{ return ptr()->seg.orientation(p); }
/*{\Mop     computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on line $l$.}*/

inline int side_of(const rat_point& p) const { return orientation(p); }
/*{\Mop     computes orientation($a$, $b$, $p$), where $a \not= b$
and $a$ and $b$ appear in this order on line $l$.}*/


bool contains(const rat_point& p) const;
/*{\Mop returns true if $p$ lies on |\Mvar|.}*/
bool contains(const rat_segment&) const;

bool clip(rat_point p, rat_point q, rat_segment& s) const;
/*{\Mop clips $l$ at the rectangle $R$ defined by $p$ and $q$.
Returns true if the intersection of $R$ and |\Mvar| is non-empty
and returns false otherwise. If the intersection is non-empty the 
intersection is assigned to $s$; It is guaranteed that the source node of
$s$ is no larger than its target node. }*/


bool operator==(const rat_line& g) const 
{ return ( contains(g.ptr()->seg) && 
          ( point2() - point1() )*( g.point2() - g.point1() ) > 0 ); }
/*{\Mbinop returns true if the |l| and |g| are equal as oriented lines. }*/

bool operator!=(const rat_line& g) const { return !operator==(g); }

friend bool equal_as_sets(const rat_line& l, const rat_line& g) 
{ return l.contains(g.seg()); }
/*{\Mfunc returns true if the |l| and |g| are equal as unoriented lines. }*/





friend __exportF ostream& operator<<(ostream& out, const rat_line& l);
friend __exportF istream& operator>>(istream& in, rat_line& l);  



/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

friend int orientation(const rat_line& l, const rat_point& p)
{ return l.orientation(p); }
/*{\Mfunc      computes orientation($a$, $b$, $p$), where $a \not= b$
               and $a$ and $b$ appear in this order on line $l$.}*/

friend int cmp_slopes(const rat_line& l1, const rat_line& l2)
{ return cmp_slopes(l1.ptr()->seg,l2.ptr()->seg); }
/*{\Mfunc      returns compare(slope($l_1$), slope($l_2$)).}*/


};


inline bool parallel(const rat_line& l1, const rat_line& l2)
{ return cmp_slopes(l1,l2) == 0; }


extern __exportF rat_line p_bisector(const rat_point& p, const rat_point& q);
/*{\Mfunc  returns the perpendicular bisector of |p| and |q|. The bisector has |p| on its left.\\
\precond $p \not= q$.
}*/


inline const char* leda_tname(const rat_line*) { return "rat_line"; }

#if LEDA_ROOT_INCL_ID == 600050
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif



