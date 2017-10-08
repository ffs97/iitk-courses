/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_line.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_LINE_H
#define LEDA_D3_LINE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600062
#include <LEDA/internal/PREAMBLE.h>
#endif

// lines in 3-d space 

#include <LEDA/geo/point.h>
#include <LEDA/geo/line.h>
#include <LEDA/geo/point.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/geo/d3_segment.h>

LEDA_BEGIN_NAMESPACE


class __exportC d3_line;

//------------------------------------------------------------------------------
// d3_lines (1999)
//------------------------------------------------------------------------------

class __exportC d3_line_rep  : public handle_rep {

friend class __exportC d3_line;

static atomic_counter id_counter;

   d3_segment s;

   unsigned long id;

public:
    
   d3_line_rep(const d3_point&,const d3_point&);
   d3_line_rep(const d3_segment&);
  ~d3_line_rep() {}

  friend inline unsigned long ID_Number(const d3_line&); 
};


/*{\Manpage {d3_line} {} {Straight Lines in 3D-Space} {l}}*/

class __exportC d3_line : public HANDLE_BASE(d3_line_rep) {
/*{\Mdefinition
An instance $l$ of the data type |\Mname| is a directed straight line
in  three-dimensional space.
}*/

 d3_line_rep* ptr() const { return (d3_line_rep*)PTR; }

 bool project_2d_intersection(const d3_line& t, point&) const;
 bool project_2d_intersection_segment(const d3_segment& t) const;

 public:
/*{\Mcreation}*/

  d3_line(const d3_point& p1,const d3_point& p2);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the line through
    points |p1,p2|.\\
    |Precondition: p1 != p2|.}*/

  d3_line(const d3_segment& s);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the line supporting
    segment |s|.\\
    |Precondition: s is not trivial|.}*/

  d3_line(const list<d3_point>& L);

  d3_line(const d3_line& l) : HANDLE_BASE(d3_line_rep)(l) {}

  d3_line();
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the line through
    points |(0,0,0)| and |(1,0,0)|.}*/

  ~d3_line() {}

  d3_line& operator=(const d3_line& l)
  { HANDLE_BASE(d3_line_rep)::operator=(l); return *this; }

/*{\Moperations 2.5 4}*/

  bool get_para(double& a,double& k, double& b, double& h) const;

  bool contains(const d3_point& p) const;
/*{\Mop returns true if $p$ lies on |\Mvar|.}*/

  d3_point point1() const;
/*{\Mop     returns a point on |\Mvar|.}*/

  d3_point point2() const;
/*{\Mop     returns a second point on |\Mvar|.}*/

  d3_segment seg() const;
/*{\Mop     returns a non-trivial segment on |\Mvar| with the same direction.}*/

  d3_line   to_d3_line() const { return *this; }  
  d3_line   to_float()   const { return *this; }

  bool x_equal() const;
  bool y_equal() const;
  bool z_equal() const;
  bool xy_equal() const;
  bool xz_equal() const;
  bool yz_equal() const;
  
//projections
  bool project_xy(line& m) const ;
/*{\Mop if the projection of |\Mvar| into the xy plane is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  bool project_xz(line& m) const ;
/*{\Mop if the projection of |\Mvar| into the xz plane is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  bool project_yz(line& m) const ;
/*{\Mop if the projection of |\Mvar| into the yz plane is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  bool project(const d3_point& p, const d3_point& q, const d3_point& v, d3_line& m) const;
/*{\Mop if the projection of |\Mvar| into the plane  through |(p,q,v)| is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  d3_line translate(double dx,double dy,double dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|.}*/

  d3_line translate(const vector& v) const;
/*{\Mop returns |\Mvar| translated by |v|.\\
       |Precond.: v.dim()=3|.}*/

  d3_line  operator+(const vector& v) const { return translate(v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

  d3_line  operator-(const vector& v) const { return translate(-v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/

  d3_line reflect(const d3_point& p, const d3_point& q, const d3_point& v) const;
/*{\Mop returns |\Mvar| reflected across the plane through |(p,q,v)|.}*/

  d3_line reflect(const d3_point& p) const;
  /*{\Mop returns |\Mvar| reflected across point |p|.}*/

  d3_line reverse() const;
/*{\Mop     returns |\Mvar| reversed.}*/

  vector to_vector() const;
/*{\Mop returns |point2()-point1()|.}*/

  bool intersection(const d3_segment& s) const;
/*{\Mop decides, whether |\Mvar| and |s| intersect in a single point.}*/

  bool intersection(const d3_segment& s,d3_point& p) const;
/*{\Mop decides, whether |\Mvar| and |s| intersect in a single point. If so, the point
    of intersection is assigned to |p|.}*/

  bool intersection(const d3_line& m) const;
/*{\Mop decides, whether |\Mvar| and |m| intersect.}*/

  bool intersection(const d3_line& m,d3_point& p) const;
/*{\Mop decides, whether |\Mvar| and |m| intersect in a single point. If so, the point
    of intersection is assigned to |p|.}*/

  double sqr_dist(const d3_point& p) const;
/*{\Mop returns the square of the distance between |\Mvar| and |p|.}*/

  double distance(const d3_point& p) const;
/*{\Mop returns the distance between |\Mvar| and |p|.}*/

  bool operator==(const d3_line& l2) const
  {
    return ( collinear(point1(),point2(),l2.point1()) 
	     && collinear(point1(),point2(),l2.point2()) ); //direction?
  }

  bool operator!=(const d3_line& l2) const
  {
    return (! ( collinear(point1(),point2(),l2.point1()) 
	     && collinear(point1(),point2(),l2.point2()) ) ); //direction? 
  }

  friend __exportF ostream& operator<<(ostream& out, const d3_line& l);
  friend __exportF istream& operator>>(istream& in, d3_line& l); 

  friend unsigned long ID_Number(const d3_line&);

};


inline unsigned long ID_Number(const d3_line& l){ return l.ptr()->id; }
inline const char* leda_tname(const d3_line*){ return "d3_line"; }

#if LEDA_ROOT_INCL_ID == 600062
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
