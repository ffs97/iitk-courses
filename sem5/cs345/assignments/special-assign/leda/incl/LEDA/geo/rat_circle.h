/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_circle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_RAT_CIRCLE_H
#define LEDA_RAT_CIRCLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600063
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/rat_line.h>
#include <LEDA/geo/circle.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// rat_circle: circles with rational coordinates
//------------------------------------------------------------------------------

class __exportC rat_circle;

class __exportC rat_circle_rep : public handle_rep {

friend class __exportC rat_circle;

  rat_point  a; 
  rat_point  b; 
  rat_point  c; 

  int orient;    // orientation(a,b,c)

  integer D1;
  integer D2;
  integer D3;

  rat_point* cp;  // pointer to center

  bool    first_side_of;
  
public:

  rat_circle_rep() {}
  rat_circle_rep(const rat_point&, const rat_point&, const rat_point&);
 ~rat_circle_rep() { if (cp) delete cp; }

friend __exportF ostream& operator<<(ostream& out, const rat_circle& c);
friend __exportF istream& operator>>(istream& in, rat_circle& c);  
};


/*{\Manpage {rat_circle} {} {Rational Circles} }*/ 

class __exportC rat_circle   : public HANDLE_BASE(rat_circle_rep) 
{
/*{\Mdefinition An instance $C$ of data type |rat_circle| is an
oriented circle in the plane. A circle is defined by three points
$p_1$, $p_2$, $p_3$ with rational coordinates (|rat_points|). The
orientation of $C$ is equal to the orientation of the three defining
points, i.e., $|orientation|(p_1,p_2,p_3)$. Positive orientation
corresponds to counterclockwise orientation and negative orientation
corresponds to clockwise orientation.

Some triples of points are unsuitable for defining a circle.  A triple
is \emph{admissable} if $\Labs{\{p_1,p_2,p_3\}} \not= 2$. Assume now
that $p_1$, $p_2$, $p_3$ are admissable.  If $\Labs{\{p_1,p_2,p_3\}} =
1$ they define the circle with center $p_1$ and radius zero. If $p_1$,
$p_2$, and $p_3$ are collinear $C$ is a straight line passing through
$p_1$, $p_2$ and $p_3$ in this order and the center of $C$ is
undefined.  If $p_1$, $p_2$, and $p_3$ are not collinear, $C$ is the
circle passing through them.  }*/


rat_circle_rep* ptr() const { return (rat_circle_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef rat_point point_type;
/*{\Mtypemember the point type (|rat_point|).}*/

typedef circle    float_type;
/*{\Mtypemember the corresponding floatin-point type (|circle|).}*/




/*{\Mcreation C }*/ 


rat_circle(const rat_point& a, const rat_point& b, const rat_point& c);
/*{\Mcreate introduces a variable $C$ of type $rat\_circle$. $C$ is 
            initialized to the circle through points $a$, $b$, and $c$.\\
\precond $a$, $b$, and $c$ are admissable.}*/

rat_circle(const rat_point& a, const rat_point& b);
/*{\Mcreate introduces a variable $C$ of type $circle$. $C$ is initialized to 
            the counter-clockwise oriented circle with center $a$  passing
            through $b$.}*/


rat_circle(const rat_point& a);
/*{\Mcreate introduces a variable $C$ of type $circle$. $C$ is initialized to 
            the trivial circle with center $a$. }*/

rat_circle();
/*{\Mcreate introduces a variable $C$ of type $rat\_circle$. 
            $C$ is initialized to the trivial circle centered at $(0,0)$.}*/

rat_circle(const circle& c, int prec = rat_point::default_precision)
{ PTR = new rat_circle_rep(rat_point(c.point1(),prec), 
                           rat_point(c.point2(),prec),
                           rat_point(c.point3(),prec)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the circle obtained by approximating three 
defining points of $c$. }*/
   


 rat_circle(const rat_circle& c) : HANDLE_BASE(rat_circle_rep)(c) {}
~rat_circle() {}

 rat_circle& operator=(const rat_circle& C) { HANDLE_BASE(rat_circle_rep)::operator=(C); return *this; }


/*{\Moperations 2.2 4.9 }*/

circle to_circle() const;

circle to_float() const { return to_circle(); }
/*{\Mop  returns a floating point approximation of |\Mvar|.}*/


void normalize() const 
{ point1().normalize(); 
  point2().normalize(); 
  point3().normalize(); 
}
/*{\Mop simplifies the homogenous representation by normalizing
        $p_1$, $p_2$, and $p_3$. }*/

int orientation()  const { return ptr()->orient; }
/*{\Mop  returns the orientation of |\Mvar|.}*/

rat_point center()  const;
/*{\Mop  returns the center of |\Mvar|.\\
         \precond |\Mvar| has a center, i.e., is not a line. }*/

rat_point point1() const { return ptr()->a; }
/*{\Mop  returns $p_1$. }*/

rat_point point2() const { return ptr()->b; }
/*{\Mop  returns $p_2$. }*/

rat_point point3() const { return ptr()->c; }
/*{\Mop  returns $p_3$. }*/

rational sqr_radius() const;
/*{\Mop  returns the square of the radius of |\Mvar|. }*/



rat_point point_on_circle(double alpha, double epsilon) const;
/*{\Mop  returns  a point $p$ on |\Mvar| such that the angle of $p$ 
         differs from alpha by at most |epsilon|.}*/


bool  is_degenerate() const { return ptr()->orient == 0; }
/*{\Mop returns true if the defining points are collinear.}*/

bool is_trivial() const { return ptr()->a == ptr()->b; }
/*{\Mop returns true if |\Mvar| has radius zero.}*/

bool is_line() const { return ptr()->orient == 0 && !is_trivial(); }
/*{\Mop returns true if |\Mvar| is a line.}*/

rat_line to_line() const { return rat_line(point1(), point3()); }
/*{\Mop returns |line(point1(), point3())|.}*/


int side_of(const rat_point& p) const;
/*{\Mop   returns $-1$, $+1$, or $0$ if $p$ lies right of, left of, or on 
          |\Mvar| respectively. }*/

bool    inside(const rat_point& p) const;
/*{\Mop   returns true iff $p$ lies inside of |\Mvar|.}*/

bool    outside(const rat_point& p) const;
/*{\Mop   returns true iff $p$ lies outside of |\Mvar|.}*/

bool    contains(const rat_point& p) const;
/*{\Mop   returns true iff $p$ lies on |\Mvar|.}*/


rat_circle translate(const rational& dx, const rational& dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

rat_circle translate(integer dx, integer dy, integer dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/

rat_circle translate(const rat_vector& v) const; 
/*{\Mop    returns |\Mvar| translated by vector $v$.}*/

rat_circle operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

rat_circle operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/


rat_circle rotate90(const rat_point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated by $i\times 90$ degrees about $q$. 
            If $i > 0$ the rotation is counter-clockwise otherwise it
            is clockwise. }*/
 

rat_circle reflect(const rat_point& p, const rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

rat_circle reflect(const rat_point& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/

rat_circle reverse() const { return rat_circle(point3(),point2(),point1()); }
/*{\Mop     returns |\Mvar| reversed.}*/


bool operator==(const rat_circle& D) const;
/*{\Mbinop returns true if |\Mvar| and |D| are equal as oriented circles.}*/

bool operator!=(const rat_circle& D) const { return !operator==(D); };

friend __exportF bool equal_as_sets(const rat_circle& C1,const rat_circle& C2);
/*{\Mfuncl returns true if |C1| and |C2| are equal as unoriented circles.}*/

friend __exportF bool radical_axis(const rat_circle& C1, const rat_circle& C2, rat_line& rad_axis);
/*{\Mfuncl if the radical axis for |C1| and |C2| exists, it is assigned to
           |rad_axis| and true is returned; otherwise the result is false.}*/

friend __exportF ostream& operator<<(ostream& out, const rat_circle& c);
/*{\Mbinopfunc writes the three defining points.}*/

friend __exportF istream& operator>>(istream& in, rat_circle& c);  
/*{\Mbinopfunc reads three points and assigns the circle defined by them to $c$. }*/
};



inline const char* leda_tname(const rat_circle*) { return "rat_circle"; }

#if LEDA_ROOT_INCL_ID == 600063
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
