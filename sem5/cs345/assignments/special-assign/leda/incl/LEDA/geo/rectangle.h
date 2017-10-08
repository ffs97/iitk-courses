/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rectangle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RECTANGLE_H
#define LEDA_RECTANGLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600069
#include <LEDA/internal/PREAMBLE.h>
#endif



#include <LEDA/core/list.h>

#include <LEDA/numbers/vector.h>
#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/geo/line.h>
#include <LEDA/geo/circle.h>
#include <LEDA/geo/polygon.h>
#include <LEDA/geo/gen_polygon.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// iso-oriented rectangles
//
// 03/2002 : difference and intersection test for 2 rectangles added
// 08/2002 : region_of, center added
//
//------------------------------------------------------------------------------

class __exportC rectangle;

class __exportC rectangle_rep : public handle_rep {

  friend class __exportC rectangle;

  static atomic_counter id_counter;

  point  p_ll;    // lower left point
  point  p_lr;    // lower right point
  point  p_ur;    // upper right point
  point  p_ul;    // upper left point

  unsigned long id;

 public:
   

   rectangle_rep();
   rectangle_rep(const point& p1, const point& p2);
   rectangle_rep(const point& p1, double w, double h);

   ~rectangle_rep() {}

   friend inline unsigned long ID_Number(const rectangle&);

};

/*{\Manpage {rectangle} { } {Iso-oriented Rectangles} }*/

class __exportC rectangle  : public HANDLE_BASE(rectangle_rep) 
{

/*{\Mdefinition
An instance $r$ of the data type $rectangle$ is an iso-oriented rectangle
in the two-dimensional plane. }*/

rectangle_rep* ptr() const { return (rectangle_rep*)PTR; }

point get_out_point(const ray& r) const;

public:

typedef double    coord_type;
typedef point     point_type;
typedef rectangle float_type;

/*{\Mcreation r}*/

 rectangle(const point& p, const point& q);
 /*{\Mcreate 
 introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
 |\Mname| with diagonal corners |p| and |q| }*/

 rectangle(const point& p, double w, double h);
 /*{\Mcreate
 introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
 |\Mname| with lower left corner |p|, width |w| and height |h|. }*/

 rectangle(double x1,double y1,double x2,double y2);
 /*{\Mcreate
 introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
 |\Mname| with diagonal corners |(x1,y1)| and |(x2,y2)|. }*/ 

 rectangle();
 
 rectangle(const rectangle& r) : HANDLE_BASE(rectangle_rep)(r) {};

 rectangle& operator=(const rectangle& r) { HANDLE_BASE(rectangle_rep)::operator=(r); return *this; }
 
 ~rectangle() {}
 
/*{\Moperations 2 5.0 }*/

rectangle to_float() const { return *this; }
rectangle to_rectangle() const { return *this; }
/*{\Xop for compatibility with |rat_rectangle|. }*/

void    normalize() const {}
/*{\Xop  for compatibility with |rat_rectangle|. }*/


 point upper_left() const;
/*{\Mop    returns the upper left corner.}*/

 point upper_right() const;
/*{\Mop    returns the upper right corner.}*/

 point lower_left() const;
/*{\Mop    returns the lower left corner.}*/

 point lower_right() const;
/*{\Mop    returns the lower right corner.}*/

 point center() const;
/*{\Mop    returns the center of |\Mvar|.}*/

 list<point> vertices() const;
/*{\Mop    returns the vertices of |\Mvar| in counterclockwise order starting from the lower left point.}*/

 double xmin() const;
/*{\Mop    returns the minimal x-coordinate of |\Mvar|.}*/

 double xmax() const;
/*{\Mop    returns the maximal x-coordinate of |\Mvar|.}*/

 double ymin() const;
/*{\Mop    returns the minimal y-coordinate of |\Mvar|.}*/

 double ymax() const;
/*{\Mop    returns the maximal y-coordinate of |\Mvar|.}*/

 double width() const;
/*{\Mop    returns the width of |\Mvar|.}*/

 double height() const;
/*{\Mop    returns the height of |\Mvar|.}*/

 bool is_degenerate() const;
/*{\Mop    returns true, if |\Mvar| degenerates to a segment or point (the 4 corners are collinear),
           false otherwise.}*/

 bool is_point() const;
/*{\Mop    returns true, if |\Mvar| degenerates to a point.}*/

 bool is_segment() const;
/*{\Mop    returns true, if |\Mvar| degenerates to a segment.}*/

 int  cs_code(const point& p) const;
/*{\Mop    returns the code for Cohen-Sutherland algorithm.}*/

 bool inside(const point& p) const;
/*{\Mop    returns true, if p is inside of |\Mvar|, false otherwise.}*/

 bool outside(const point& p) const;
/*{\Mop    returns true, if p is outside of |\Mvar|, false otherwise.}*/

 bool inside_or_contains(const point& p) const;
/*{\Mop    returns true, if p is inside of |\Mvar| or on the border, false otherwise.}*/

 bool contains(const point& p) const; 
/*{\Mop    returns true, if p is on the border of |\Mvar|, false otherwise.}*/

 region_kind region_of(const point& p) const;
/*{\Mop     returns BOUNDED\_REGION if $p$ lies in the 
            bounded region of |\Mvar|, returns ON\_REGION if $p$ lies on 
            |\Mvar|, and returns UNBOUNDED\_REGION if $p$ lies in the 
            unbounded region. }*/

 rectangle include(const point& p) const;
/*{\Mop    returns a new rectangle that includes the points of |\Mvar| and p.}*/

 rectangle include(const rectangle& r2) const;
/*{\Mop    returns a new rectangle that includes the points of |\Mvar| and r2.}*/

 rectangle translate(double dx,double dy) const;
/*{\Mop    returns a new rectangle that is the translation of |\Mvar| by |(dx,dy)|.}*/

 rectangle translate(const vector& v) const;
/*{\Mop    returns a new rectangle that is the translation of |\Mvar| by |v|.}*/

 rectangle operator+(const vector& v) const { return translate(v); }
/*{\Mbinop returns |\Mvar| translated by  |v|.}*/

 rectangle operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop returns |\Mvar| translated by |-v|.}*/

 point operator[](int i) const;
/*{\Marrop returns the |i-th| vertex of |\Mvar|. Precondition: |(0<i<5)|.}*/

 rectangle rotate90(const point& p, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $p$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

 rectangle rotate90(int i=1) const;
/*{\Mop    returns |\Mvar| rotated by an angle of $i\times 90$ degrees 
           about the origin.}*/

 rectangle reflect(const point& p) const;
/*{\Mop    returns |\Mvar| reflected across |p| .}*/

 list<point> intersection(const segment& s) const;
/*{\Mop    returns $\Mvar \cap s$ .}*/

 bool      intersect(point& ps,const segment& s,int c1,int c2) const;

 bool      clip(const segment& t,segment& inter) const;
/*{\Mop    clips |t| on |\Mvar| and returns the result in |inter|.}*/

 bool      clip(const segment& t,segment& inter,int c1,int c2) const;

 bool      clip(const line& l,segment& inter) const;
/*{\Mop    clips |l| on |\Mvar| and returns the result in |inter|.}*/

 bool      clip(const ray& ry,segment& inter) const;
/*{\Mop    clips |ry| on |\Mvar| and returns the result in |inter|.}*/

 bool      difference(const rectangle& q, list<rectangle>& L) const;
/*{\Mop    returns |true| iff the difference of \Mvar and |q| is not empty, 
           and |false| otherwise. The difference |L| is returned as a 
           partition into rectangles.}*/

 list<point> intersection(const line& l) const;
/*{\Mop    returns $\Mvar \cap l$.}*/

 list<rectangle> intersection(const rectangle& s) const;
/*{\Mop    returns $\Mvar \cap s$.}*/

 bool      do_intersect(const rectangle& b) const;
/*{\Mop    returns |true| iff \Mvar and $b$ intersect, false otherwise.}*/

 double area() const;
/*{\Mop    returns the area of |\Mvar|.}*/

 bool operator==(const rectangle& s) const 
 { if (s.lower_left() == lower_left() && s.upper_right() == upper_right()) return true; else return false; }

 bool operator!=(const rectangle& s) const  { return !operator==(s); }

 friend inline ostream& operator<<(ostream& out, const rectangle& r);
 friend inline istream& operator>>(istream& in, rectangle& r);  

 friend inline unsigned long ID_Number(const rectangle&);
};

inline ostream& operator<<(ostream& out, const rectangle& r) 
{ return out << r.lower_left() << r.upper_right(); }

inline istream& operator>>(istream& in, rectangle& r)  
{ point p1,p2; 
  in >> p1;
  in >> p2; 
  r = rectangle(p1,p2); 
  return in; 
}

inline const char* leda_tname(const rectangle*) { return "rectangle"; }

inline unsigned long  ID_Number(const rectangle& r) { return r.ptr()->id; }

// BoundingBox functions

extern __exportF rectangle BoundingBox(const point& p);
extern __exportF rectangle BoundingBox(const segment& s);
extern __exportF rectangle BoundingBox(const ray& r);
extern __exportF rectangle BoundingBox(const line& l);
extern __exportF rectangle BoundingBox(const circle& c);
extern __exportF rectangle BoundingBox(const polygon& P);
extern __exportF rectangle BoundingBox(const gen_polygon& P);

#if LEDA_ROOT_INCL_ID == 600069
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
