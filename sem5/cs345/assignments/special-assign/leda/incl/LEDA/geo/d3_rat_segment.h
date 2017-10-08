/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_rat_segment.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_RAT_SEGMENT_H
#define LEDA_D3_RAT_SEGMENT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600104
#include <LEDA/internal/PREAMBLE.h>
#endif

// rational segments in 3-d space

#include <LEDA/geo/d3_rat_point.h> 
#include <LEDA/geo/rat_segment.h>
#include <LEDA/numbers/rat_vector.h>
#include <LEDA/geo/d3_segment.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_rat_segment;

//------------------------------------------------------------------------------
// d3_rat_segments
//------------------------------------------------------------------------------

class __exportC d3_rat_segment_rep  : public handle_rep {

friend class __exportC d3_rat_segment;

static atomic_counter id_counter;
   
   d3_rat_point a,b;

   unsigned long id;

public:
    
   d3_rat_segment_rep(const d3_rat_point&,const d3_rat_point&);
  ~d3_rat_segment_rep() {}

  friend inline unsigned long ID_Number(const d3_rat_segment&); 
};


/*{\Manpage {d3_rat_segment} {} {Rational Segments in 3D-Space} {s}}*/

class __exportC d3_rat_segment : public HANDLE_BASE(d3_rat_segment_rep) {
/*{\Mdefinition
An instance $s$ of the data type $d3\_rat\_segment$ is a directed straight line
segment in three-dimensional space, i.e.,
a line segment connecting two rational points $p,q \in \real ^3$. $p$ is called the {\em source} or start point and 
$q$ is called the {\em target} or end point of $s$. A segment is called \emph{trivial} if its source is equal to its target.
If |s| is not trivial, we use $line(s)$ to denote the straight line  containing $s$. }*/

 d3_rat_segment_rep* ptr() const { return (d3_rat_segment_rep*)PTR; }

 bool only_one_common(const d3_rat_point& p1, const d3_rat_point& p2) const;

 public:
/*{\Mcreation}*/

  d3_rat_segment(const d3_rat_point& p1,const d3_rat_point& p2);
/*{\Mcreate introduces a variable |S| of type |\Mname|. |S| is initialized to the segment through
    points |p1,p2|.}*/

  d3_rat_segment(const list<d3_rat_point>& L);

  d3_rat_segment(const d3_rat_segment& S) : HANDLE_BASE(d3_rat_segment_rep)(S) {}

  d3_rat_segment();
/*{\Mcreate introduces a variable |S| of type |\Mname|. |S| is initialized to the segment through
    points |(0,0,0,1)| and |(1,0,0,1)|.}*/

  ~d3_rat_segment() {}

  d3_rat_segment& operator=(const d3_rat_segment& s)
  { HANDLE_BASE(d3_rat_segment_rep)::operator=(s); return *this; }

/*{\Moperations 2.5 4}*/

  bool in_xyz(const d3_rat_point& p) const;

  d3_segment to_d3_segment() const;

  d3_segment to_float() const;
/*{\Mop returns a floating point approximation of |\Mvar|.}*/


  bool contains(const d3_rat_point& p) const;
/*{\Mop decides whether |\Mvar| contains |p|.}*/

  d3_rat_point point1() const { return ptr()->a; }
  d3_rat_point point2() const { return ptr()->b; }

  d3_rat_point source() const { return ptr()->a; }
/*{\Mop returns the source point of segment |\Mvar|.}*/
  d3_rat_point target() const { return ptr()->b; }
/*{\Mop returns the target point of segment |\Mvar|.}*/

  d3_rat_point start() const { return ptr()->a; }
  d3_rat_point end() const { return ptr()->b; }

  rational xcoord1() const { d3_rat_point p=point1(); return p.xcoord(); }
/*{\Mop returns the x-coordinate of |\Mvar|.source().}*/
  rational xcoord2() const { d3_rat_point p=point2(); return p.xcoord(); }
/*{\Mop returns the x-coordinate of |\Mvar|.target().}*/
  rational ycoord1() const { d3_rat_point p=point1(); return p.ycoord(); }
/*{\Mop returns the y-coordinate of |\Mvar|.source().}*/
  rational ycoord2() const { d3_rat_point p=point2(); return p.ycoord(); }
/*{\Mop returns the y-coordinate of |\Mvar|.target().}*/
  rational zcoord1() const { d3_rat_point p=point1(); return p.zcoord(); }
/*{\Mop returns the z-coordinate of |\Mvar|.source().}*/
  rational zcoord2() const { d3_rat_point p=point2(); return p.zcoord(); }
/*{\Mop returns the z-coordinate of |\Mvar|.target().}*/
  rational dx() const { return xcoord2()-xcoord1(); }
/*{\Mop returns |xcoord2()-xcoord1()|.}*/
  rational dy() const { return ycoord2()-ycoord1(); }
/*{\Mop returns |ycoord2()-ycoord1()|.}*/
  rational dz() const { return zcoord2()-zcoord1(); }
/*{\Mop returns |zcoord2()-zcoord1()|.}*/

//projections
  rat_segment project_xy() const;
/*{\Mop returns the projection into the xy plane.}*/
  rat_segment project_xz() const;
/*{\Mop returns the projection into the xz plane.}*/
  rat_segment project_yz() const;
/*{\Mop returns the projection into the yz plane.}*/

  d3_rat_segment project(const d3_rat_point& p,const d3_rat_point& q,const d3_rat_point& v) const;
/*{\Mop returns |\Mvar| projected into the plane through |(p,q,v)|.}*/

  d3_rat_segment reflect(const d3_rat_point& p,const d3_rat_point& q,const d3_rat_point& v) const;
/*{\Mop returns |\Mvar| reflected across the plane through |(p,q,v)|.}*/

  d3_rat_segment reflect(const d3_rat_point& p) const;
  /*{\Mop returns |\Mvar| reflected across point |p|.}*/

  d3_rat_segment reverse() const;
/*{\Mop returns |\Mvar| reversed.}*/

  rat_vector to_vector() const { return target()-source(); }
/*{\Mop returns  |S.target()-S.source()|.}*/

  bool intersection(const d3_rat_segment& t) const;
/*{\Mop decides, whether |s| and |t| intersect in a single point.}*/

  bool intersection(const d3_rat_segment& t,d3_rat_point& p) const;
/*{\Mop decides, whether |\Mvar| and |t| intersect. If they intersect
in a single point, the point is assigned to |p| }*/

  bool intersection_of_lines(const d3_rat_segment& t,d3_rat_point& p) const;
/*{\Mop If |line(s)| and |line(t)| intersect in a single point this point
is assigned to |p| and the result is true, otherwise the result is false.}*/

  bool is_trivial() const { return (point1()==point2()); }
  /*{\Mop returns true if |\Mvar| is trivial.}*/

  rational sqr_length() const;
/*{\Mop returns the square of the length of |\Mvar|.}*/

  d3_rat_segment translate(const rat_vector& v) const;
/*{\Mop returns |\Mvar| translated by vector |v|.\\
        |Precond.: v.dim()=3|.}*/

  d3_rat_segment translate(rational dx,rational dy,rational dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|.}*/

  d3_rat_segment translate(integer dx,integer dy,integer dz,integer dw) const;
/*{\Mop returns |\Mvar| translated by vector |(dx/dw,dy/dw,dz/w)|.}*/

  d3_rat_segment  operator+(const rat_vector& v) const { return translate(v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

  d3_rat_segment  operator-(const rat_vector& v) const { return translate(-v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/

  bool operator==(const d3_rat_segment& s2) const
  {
   return (point1()==s2.point1() && point2()==s2.point2());
  }

  bool operator!=(const d3_rat_segment& s2) const
  {
   return (point1()!=s2.point1() || point2()!=s2.point2());
  }

  friend __exportF ostream& operator<<(ostream& out, const d3_rat_segment& s);
  friend __exportF istream& operator>>(istream& in, d3_rat_segment& s); 

  friend unsigned long ID_Number(const d3_rat_segment&);

};


inline unsigned long ID_Number(const d3_rat_segment& s){ return s.ptr()->id; }
inline const char* leda_tname(const d3_rat_segment*){ return "d3_rat_segment"; }

#if LEDA_ROOT_INCL_ID == 600104
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
