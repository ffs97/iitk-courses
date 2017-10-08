/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  circle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_CIRCLE_H
#define LEDA_CIRCLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600123
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/geo/line.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// circles
//------------------------------------------------------------------------------


class __exportC circle_rep : public handle_rep {

friend class __exportC circle;

  point a; 
  point b; 
  point c; 

  int orient;  // orientation(a,b,c)

  double D1;
  double D2;
  double D3;

  point*  cp; // pointer to center
  double* rp; // pointer to radius

  bool first_side_of;
  
public:

  circle_rep() {}
  circle_rep(const point&, const point&, const point&);
 ~circle_rep();

};


/*{\Manpage {circle} {} {Circles} }*/ 


/*{\Mdefinition
An instance $C$ of the data type |\Mname| is an oriented circle in the 
plane passing through three points $p_1$, $p_2$, $p_3$. The orientation of 
$C$ is equal to the orientation of the three defining points, 
i.e. $orientation(p_1,p_2,p_3)$. 
If \Labs{\{p_1,p_2,p_3\}}$ = 1$ $C$ is the empty circle with center $p_1$. 
If $p_1,p_2,p_3$ are collinear $C$ is a straight line passing through
$p_1$, $p_2$ and $p_3$ in this order and the center of $C$ is undefined. }*/

class __exportC circle   : public HANDLE_BASE(circle_rep) 
{

circle_rep* ptr() const { return (circle_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef double  coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef point   point_type;
/*{\Mtypemember the point type (|point|).}*/

typedef circle float_type;

/*{\Mcreation C }*/ 

circle(const point& a, const point& b, const point& c);
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
            the oriented circle through points $a$, $b$, and $c$. }*/

circle(const point& a, const point& b);
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
            the counter-clockwise oriented circle with center $a$ passing
            through $b$.}*/

explicit

circle(const point& a);
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
            the trivial circle with center $a$. }*/


circle();
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
            the trivial circle with center $(0,0)$.}*/

circle(const point& c, double r);
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
            the circle with center $c$ and radius $r$ with positive (i.e.
            counter-clockwise) orientation.}*/

circle(double x, double y, double r);
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
            the circle with center $(x,y)$ and radius $r$ with positive (i.e.
            counter-clockwise) orientation.}*/


 circle(const circle& c, int) : HANDLE_BASE(circle_rep)(c) {}
/*{\Mcreate introduces a variable $C$ of type |\Mname|. $C$ is initialized to 
    a copy of $c$. The second argument is for compatability with |rat_circle|.}*/

 circle(const circle& c) : HANDLE_BASE(circle_rep)(c) {}
~circle() {}

 circle& operator=(const circle& C) { HANDLE_BASE(circle_rep)::operator=(C); return *this; }


/*{\Moperations 2.2 4.9 }*/

point center()  const;
/*{\Mop  returns the center of |\Mvar|.\\
         \precond The orientation of |\Mvar| is not $0$.}*/

double radius() const;
/*{\Mop  returns the radius of |\Mvar|.\\
         \precond The orientation of |\Mvar| is not $0$.}*/

double sqr_radius() const;
/*{\Mop  returns the squared radius of |\Mvar|.\\
         \precond The orientation of |\Mvar| is not $0$.}*/

point point1() const { return ptr()->a; }
/*{\Mop  returns $p_1$. }*/

point point2() const { return ptr()->b; }
/*{\Mop  returns $p_2$. }*/

point point3() const { return ptr()->c; }
/*{\Mop  returns $p_3$. }*/

circle to_float() const { return *this; }
circle to_circle() const { return *this; }
/*{\Xop for compatibility with |rat_circle|. }*/


void    normalize() const {}
/*{\Xop  for compatibility with |rat_segment|. }*/


point point_on_circle(double alpha, double=0) const;
/*{\Mop  returns a point $p$ on |\Mvar| with angle of |alpha|. 
The second argument is for compatability with |rat_circle|. }*/


bool  is_degenerate() const { return ptr()->orient == 0; }
/*{\Mop returns true if the defining points are collinear.}*/

bool is_trivial() const { return ptr()->a == ptr()->b; }
/*{\Mop returns true if |\Mvar| has radius zero.}*/

bool is_line() const { return is_degenerate() && !is_trivial(); }
/*{\Mop returns true if |\Mvar| is a line.}*/

line to_line() const { return line(point1(), point3()); }
/*{\Mop returns |line(point1(), point3())|.}*/

int orientation()  const { return ptr()->orient; }
/*{\Mop  returns the orientation of |\Mvar|.}*/

int side_of(const point& p) const;
/*{\Mop   returns $-1$, $+1$, or $0$ if $p$ lies right of, left of, or on 
          |\Mvar| respectively. }*/

bool    inside(const point& p) const;
/*{\Mop   returns true iff $p$ lies inside of |\Mvar|.}*/

bool    outside(const point& p) const;
/*{\Mop   returns true iff $p$ lies outside of |\Mvar|.}*/


bool    contains(const point& p) const;
/*{\Mop   returns true iff $p$ lies on |\Mvar|.}*/


circle  translate_by_angle(double a, double d) const;
/*{\Mop    returns |\Mvar| translated in direction $a$ by distance $d$.}*/

circle  translate(double dx, double dy) const;
/*{\Mop    returns |\Mvar| translated by vector $(dx,dy)$.}*/

circle  translate(const vector& v) const; 
/*{\Mop    returns |\Mvar| translated by vector $v$.}*/

circle  operator+(const vector& v) const { return translate(v); }
/*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

circle  operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/

circle  rotate(const point& q, double a) const;
/*{\Mopl    returns |\Mvar| rotated  about point $q$ by angle $a$.}*/

circle  rotate(double a) const;
/*{\Mopl    returns |\Mvar| rotated about the origin by angle $a$.}*/

circle rotate90(const point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

circle reflect(const point& p, const point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

circle reflect(const point& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/

circle reverse() const { return circle(point3(),point2(),point1()); }
/*{\Mop     returns |\Mvar| reversed.}*/

list<point> intersection(const circle& D) const;
/*{\Mop    returns $C \cap D$ as a list of points.}*/

list<point> intersection(const line& l) const;
/*{\Mop  returns $C \cap l$ as a list of (zero, one, or two) points 
         sorted along $l$.}*/

list<point> intersection(const segment& s) const;
/*{\Mop  returns $C \cap s$ as a list of (zero, one, or two) points 
         sorted along $s$.}*/

segment left_tangent(const point& p) const;
/*{\Mop   returns the line segment starting in $p$ tangent 
	  to |\Mvar| and left of segment $[p,C.center()]$.}*/

segment right_tangent(const point& p) const;
/*{\Mop   returns the line segment starting in $p$ tangent 
	  to |\Mvar| and right of segment $[p,C.center()]$.}*/

double  distance(const point& p) const;
/*{\Mop   returns the distance between |\Mvar| and $p$. }*/

double  sqr_dist(const point& p) const;
/*{\Mop   returns the squared distance between |\Mvar| and $p$. }*/

double  distance(const line& l) const;
/*{\Mop    returns the distance between |\Mvar| and $l$. }*/

double  distance(const circle& D) const;
/*{\Mop    returns the distance between |\Mvar| and $D$. }*/


bool operator==(const circle& D) const;
bool operator!=(const circle& D) const { return !operator==(D); };

friend __exportF istream& operator>>(istream& in, circle& c);  


};

__exportF bool radical_axis(const circle& C1, const circle& C2, line& rad_axis);
/*{\Mfuncl if the radical axis for |C1| and |C2| exists, it is assigned to
           |rad_axis| and true is returned; otherwise the result is false.}*/

inline ostream& operator<<(ostream& out, const circle& c) 
{ return out << c.point1() << " " << c.point2() << " " << c.point3();} 

inline const char* leda_tname(const circle*) { return "circle"; }

#if LEDA_ROOT_INCL_ID == 600123
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
