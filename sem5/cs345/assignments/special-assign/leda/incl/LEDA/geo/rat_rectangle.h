/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_rectangle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_RECTANGLE_H
#define LEDA_RAT_RECTANGLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600072
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/list.h>

#include <LEDA/numbers/rat_vector.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/rat_segment.h>
#include <LEDA/geo/rat_line.h>
#include <LEDA/geo/rat_circle.h>
#include <LEDA/geo/rat_polygon.h> 
#include <LEDA/geo/rat_gen_polygon.h> 

#include <LEDA/geo/rectangle.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// iso-oriented rectangles with rational coordinates
//
// 03/2002 : difference and intersection test for 2 rat_rectangles added
// 08/2002 : region_of and center added
//
//------------------------------------------------------------------------------

class __exportC rat_rectangle;

class __exportC rat_rectangle_rep : public handle_rep {

  friend class __exportC rat_rectangle;

  static atomic_counter id_counter;

  rat_point  p_ll;    // lower left point
  rat_point  p_lr;    // lower right point
  rat_point  p_ur;    // upper right point
  rat_point  p_ul;    // upper left point

  unsigned long id;

 public:
    

   rat_rectangle_rep();
   rat_rectangle_rep(const rat_point& p1, const rat_point& p2);
   rat_rectangle_rep(const rat_point& p1, rational w, rational h);

   ~rat_rectangle_rep() {}

   friend inline unsigned long ID_Number(const rat_rectangle&);

};


/*{\Manpage {rat_rectangle} { } {Iso-oriented Rational Rectangles} }*/

class __exportC rat_rectangle  : public HANDLE_BASE(rat_rectangle_rep) 
{

/*{\Mdefinition
An instance $r$ of the data type $rectangle$ is an iso-oriented rectangle
in the two-dimensional plane with rational coordinates. }*/

rat_rectangle_rep* ptr() const { return (rat_rectangle_rep*)PTR; }

public:

typedef rational   coord_type;
typedef rat_point  point_type;
typedef rectangle  float_type;

/*{\Mcreation r}*/

 rat_rectangle(const rat_point& p, const rat_point& q);
 /*{\Mcreate 
 introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
 |\Mname| with diagonal corners |p| and |q| }*/

 rat_rectangle(const rat_point& p, rational w, rational h);
 /*{\Mcreate
 introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
 |\Mname| with lower left corner |p|, width |w| and height |h|. }*/

 rat_rectangle(rational x1,rational y1,rational x2,rational y2);
 /*{\Mcreate
 introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the 
 |\Mname| with diagonal corners |(x1,y1)| and |(x2,y2)|. }*/ 

 rat_rectangle(const rectangle& r, int prec = rat_point::default_precision);
 /*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the rectangle obtained by approximating the
            defining points of $r$. }*/

 rat_rectangle();
 
 rat_rectangle(const rat_rectangle& r) : HANDLE_BASE(rat_rectangle_rep)(r) {};

 rat_rectangle& operator=(const rat_rectangle& r) { HANDLE_BASE(rat_rectangle_rep)::operator=(r); return *this; }
 
 ~rat_rectangle() {}
 
/*{\Moperations 2 5.0 }*/

 rectangle to_rectangle() const;

 rectangle to_float() const { return to_rectangle(); }
/*{\Mop    returns a floating point approximation of $R$.}*/


void normalize() const 
{ upper_left().normalize(); 
  upper_right().normalize();  
  lower_left().normalize();  
  lower_right().normalize();  
}
/*{\Mop simplifies the homogenous representation by calling
        |p.normalize()| for every vertex of |\Mvar|. }*/


 rat_point upper_left() const;
/*{\Mop    returns the upper left corner.}*/

 rat_point upper_right() const;
/*{\Mop    returns the upper right corner.}*/

 rat_point lower_left() const;
/*{\Mop    returns the lower left corner.}*/

 rat_point lower_right() const;
/*{\Mop    returns the lower right corner.}*/

 rat_point center() const;
/*{\Mop    returns the center of |\Mvar|.}*/

 list<rat_point> vertices() const;
/*{\Mop    returns the vertices of |\Mvar| in counterclockwise order starting from the lower left point.}*/

 rational xmin() const;
/*{\Mop    returns the minimal x-coordinate of |\Mvar|.}*/

 rational xmax() const;
/*{\Mop    returns the maximal x-coordinate of |\Mvar|.}*/

 rational ymin() const;
/*{\Mop    returns the minimal y-coordinate of |\Mvar|.}*/

 rational ymax() const;
/*{\Mop    returns the maximal y-coordinate of |\Mvar|.}*/

 rational width() const;
/*{\Mop    returns the width of |\Mvar|.}*/

 rational height() const;
/*{\Mop    returns the height of |\Mvar|.}*/

 bool is_degenerate() const;
/*{\Mop    returns true, if |\Mvar| degenerates to a segment or point (the 4 corners are collinear),
           false otherwise.}*/

 bool is_point() const;
/*{\Mop    returns true, if |\Mvar| degenerates to a point.}*/

 bool is_segment() const;
/*{\Mop    returns true, if |\Mvar| degenerates to a segment.}*/

 int  cs_code(const rat_point& p) const;
/*{\Mop    returns the code for Cohen-Sutherland algorithm.}*/

 bool inside(const rat_point& p) const;
/*{\Mop    returns true, if p is inside of |\Mvar|, false otherwise.}*/

 bool inside_or_contains(const rat_point& p) const;
/*{\Mop    returns true, if p is inside of |\Mvar| or on the border, false otherwise.}*/

 bool outside(const rat_point& p) const;
/*{\Mop    returns true, if p is outside of |\Mvar|, false otherwise.}*/

 bool contains(const rat_point& p) const;
/*{\Mop    returns true, if p is on the border of |\Mvar|, false otherwise.}*/

 region_kind region_of(const rat_point& p) const;
/*{\Mop     returns BOUNDED\_REGION if $p$ lies in the 
            bounded region of |\Mvar|, returns ON\_REGION if $p$ lies on 
            |\Mvar|, and returns UNBOUNDED\_REGION if $p$ lies in the 
            unbounded region. }*/

 rat_rectangle include(const rat_point& p) const;
/*{\Mop    returns a new |rat_rectangle| that includes the points of |\Mvar| and p.}*/

 rat_rectangle include(const rat_rectangle& r2) const;
/*{\Mop    returns a new |rat_rectangle| that includes the points of |\Mvar| and r2.}*/

 rat_rectangle translate(rational dx,rational dy) const;
/*{\Mop    returns |\Mvar| translated by |(dx,dy)|.}*/

 rat_rectangle translate(const rat_vector& v) const;
/*{\Mop    returns |\Mvar| translated by |v|.}*/

 rat_rectangle operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop returns |\Mvar| translated by  |v|.}*/

 rat_rectangle operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop returns |\Mvar| translated by vector |-v|.}*/

 rat_point operator[](int i) const;
/*{\Marrop returns the |i-th| vertex of |\Mvar|. Precondition: |(0<i<5)|.}*/

 rat_rectangle rotate90(const rat_point& p, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

 rat_rectangle rotate90(int i=1) const;
/*{\Mop    returns |\Mvar| rotated by an angle of $i\times 90$ degrees 
           about the origin.}*/

 rat_rectangle reflect(const rat_point& p) const;
/*{\Mop    returns |\Mvar| reflected across |p|.}*/

 bool      intersect(rat_point& ps,const rat_segment& s,int c1,int c2) const;

 bool      clip(const rat_segment& t,rat_segment& inter) const;
/*{\Mop    clips |t| on |\Mvar| and returns the result in |inter|.}*/

 bool      clip(const rat_line& l,rat_segment& inter) const;
/*{\Mop    clips |l| on |\Mvar| and returns the result in |inter|.}*/
 
 bool      clip(const rat_ray& ry,rat_segment& inter) const;
/*{\Mop    clips |ry| on |\Mvar| and returns the result in |inter|.}*/

 bool      difference(const rat_rectangle& q, list<rat_rectangle>& L) const;
/*{\Mop    returns |true| iff the difference of \Mvar and |q| is not empty, 
           and |false| otherwise. The difference |L| is returned as a 
           partition into rectangles.}*/

 list<rat_point> intersection(const rat_segment& s) const;
/*{\Mop    returns $\Mvar \cap s$.}*/

 list<rat_point> intersection(const rat_line& l) const;
/*{\Mop    returns $\Mvar \cap l$.}*/

 list<rat_rectangle> intersection(const rat_rectangle& s) const;
/*{\Mop    returns $\Mvar \cap s$.}*/

 bool      do_intersect(const rat_rectangle& b) const;
/*{\Mop    returns |true| iff \Mvar and $b$ intersect, false otherwise.}*/

 rational area() const;
/*{\Mop    returns the area of |\Mvar|.}*/

 bool operator==(const rat_rectangle& s) const
 { if (s.lower_left() == lower_left() && s.upper_right() == upper_right()) return true; else return false; }
 
 bool operator!=(const rat_rectangle& s) const { return !operator==(s); }

 friend inline ostream& operator<<(ostream& out,const rat_rectangle& l);
 friend inline istream& operator>>(istream& in, rat_rectangle& l);  

 friend inline unsigned long ID_Number(const rat_rectangle&);
};

inline ostream& operator<<(ostream& out, const  rat_rectangle& l) 
{ return out << l.lower_left() << l.upper_right(); }

inline istream& operator>>(istream& in, rat_rectangle& l)  
{ rat_point p1,p2; 
  in >> p1;
  in >> p2; 
  l = rat_rectangle(p1,p2); 
  return in; 
}


inline const char* leda_tname(const rat_rectangle*) { return "rat_rectangle"; }

inline unsigned long  ID_Number(const rat_rectangle& r) { return r.ptr()->id; }


// Rational Bounding Boxes (except for circle)
extern __exportF rat_rectangle BoundingBox(const rat_point& p);
extern __exportF rat_rectangle BoundingBox(const rat_segment& s);
extern __exportF rat_rectangle BoundingBox(const rat_ray& r);
extern __exportF rat_rectangle BoundingBox(const rat_line& l);
extern __exportF rat_rectangle BoundingBox(const rat_polygon& P);
extern __exportF rat_rectangle BoundingBox(const rat_gen_polygon& P);

#if LEDA_ROOT_INCL_ID == 600072
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
