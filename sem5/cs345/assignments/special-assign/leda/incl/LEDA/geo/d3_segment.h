/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_segment.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_SEGMENT_H
#define LEDA_D3_SEGMENT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600075
#include <LEDA/internal/PREAMBLE.h>
#endif

// segments in 3-d space

#include <LEDA/geo/d3_point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/numbers/vector.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_segment;

//------------------------------------------------------------------------------
// d3_segments
//------------------------------------------------------------------------------

class __exportC d3_segment_rep  : public handle_rep {

friend class __exportC d3_segment;

static atomic_counter id_counter;
   
   d3_point a,b;

   unsigned long id;

public:
    
   d3_segment_rep(const d3_point&,const d3_point&);
  ~d3_segment_rep() {}

  friend inline unsigned long ID_Number(const d3_segment&); 
};


/*{\Manpage {d3_segment} {} {Segments in 3D-Space} {s}}*/

class __exportC d3_segment : public HANDLE_BASE(d3_segment_rep) {
/*{\Mdefinition
    An instance $s$ of the data type $d3\_segment$ is a directed straight line
    segment in three-dimensional space, i.e., a straight line segment $[p,q]$
    connecting two points $p,q \in \real ^3$. $p$ is called the {\em source} or 
    start point and $q$ is called the {\em target} or end point of $s$. The 
    length of $s$ is the Euclidean distance between $p$ and $q$. A segment is called
    \emph{trivial} if its source is equal to its target. If |s| is not trivial, we use
    $line(s)$ to denote the straight line  containing $s$. }*/

 d3_segment_rep* ptr() const { return (d3_segment_rep*)PTR; }

 bool only_one_common(const d3_point& p1, const d3_point& p2) const;

 public:
/*{\Mcreation}*/

  d3_segment(const d3_point& p1,const d3_point& p2);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the segment 
    from |p1| to |p2|.}*/

  d3_segment(const list<d3_point>& L);

  d3_segment(const d3_segment& S) : HANDLE_BASE(d3_segment_rep)(S) {}

  d3_segment();
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the segment 
    from |(0,0,0)| to |(1,0,0)|.}*/

  ~d3_segment() {}

  d3_segment& operator=(const d3_segment& s)
  { HANDLE_BASE(d3_segment_rep)::operator=(s); return *this; }

/*{\Moperations 2.5 4}*/

  bool in_xyz(const d3_point& p) const;

  d3_segment to_d3_segment() const;
  d3_segment to_float()      const;

  bool contains(const d3_point& p) const;
/*{\Mop decides whether |\Mvar| contains |p|.}*/

  d3_point point1() const { return ptr()->a; }
  d3_point point2() const { return ptr()->b; }

  d3_point source() const { return ptr()->a; }
/*{\Mop returns the source point of segment |\Mvar|.}*/
  d3_point target() const { return ptr()->b; }
/*{\Mop returns the target point of segment |\Mvar|.}*/

  d3_point start() const { return ptr()->a; }
  d3_point end() const { return ptr()->b; }

  double xcoord1() const { d3_point p=point1(); return p.xcoord(); }
/*{\Mop returns the x-coordinate of |\Mvar|.source().}*/
  double xcoord2() const { d3_point p=point2(); return p.xcoord(); }
/*{\Mop returns the x-coordinate of |\Mvar|.target().}*/
  double ycoord1() const { d3_point p=point1(); return p.ycoord(); }
/*{\Mop returns the y-coordinate of |\Mvar|.source().}*/
  double ycoord2() const { d3_point p=point2(); return p.ycoord(); }
/*{\Mop returns the y-coordinate of |\Mvar|.target().}*/
  double zcoord1() const { d3_point p=point1(); return p.zcoord(); }
/*{\Mop returns the z-coordinate of |\Mvar|.source().}*/
  double zcoord2() const { d3_point p=point2(); return p.zcoord(); }
/*{\Mop returns the z-coordinate of |\Mvar|.target().}*/
  double dx() const { return xcoord2()-xcoord1(); }
/*{\Mop returns |xcoord2()-xcoord1()|.}*/
  double dy() const { return ycoord2()-ycoord1(); }
/*{\Mop returns |ycoord2()-ycoord1()|.}*/
  double dz() const { return zcoord2()-zcoord1(); }
/*{\Mop returns |zcoord2()-zcoord1()|.}*/

//projections
  segment project_xy() const ;
/*{\Mop returns the projection into the xy plane.}*/
  segment project_xz() const ;
/*{\Mop returns the projection into the xz plane.}*/
  segment project_yz() const ;
/*{\Mop returns the projection into the yz plane.}*/

  d3_segment project(const d3_point& p,const d3_point& q,const d3_point& v) const;
/*{\Mop returns |\Mvar| projected into the plane through |(p,q,v)|.}*/

  d3_segment reflect(const d3_point& p,const d3_point& q,const d3_point& v) const;
/*{\Mop returns |\Mvar| reflected across the plane through |(p,q,v)|.}*/

  d3_segment reflect(const d3_point& p) const;
  /*{\Mop returns |\Mvar| reflected across point |p|.}*/

  d3_segment reverse() const;
/*{\Mop     returns |\Mvar| reversed.}*/

  vector to_vector() const { return vector(dx(),dy(),dz()); }
/*{\Mop returns  |s.target()-s.source()|.}*/

  bool project_2d_intersection(const d3_segment& t) const;

  bool intersection(const d3_segment& t) const;
/*{\Mop decides, whether |s| and |t| intersect in a single point.}*/

  bool intersection(const d3_segment& t,d3_point& p) const;
/*{\Mop decides, whether |\Mvar| and |t| intersect in a single point. 
If they intersect in a single point, the point is assigned to |p| and the 
result is true, otherwise the result is false }*/

  bool intersection_of_lines(const d3_segment& t,d3_point& p) const;
/*{\Mop If |line(s)| and |line(t)| intersect in a single point this point
is assigned to |p| and the result is true, otherwise the result is false.}*/

  bool is_trivial() const { return (point1()==point2()); }
/*{\Mop returns true if |\Mvar| is trivial.}*/

  double sqr_length() const;
/*{\Mop returns the square of the length of |\Mvar|.}*/

  double length() const;
/*{\Mop returns the length of |\Mvar|.}*/

  d3_segment translate(const vector& v) const;
/*{\Mop returns |\Mvar| translated by vector v. \\
       |Precond.: v.dim()=3|.}*/
  d3_segment translate(double dx,double dy,double dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|.}*/

  d3_segment  operator+(const vector& v) const { return translate(v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

  d3_segment  operator-(const vector& v) const { return translate(-v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/

  bool operator==(const d3_segment& s2) const
  {
   return (point1()==s2.point1() && point2()==s2.point2());
  }

  bool operator!=(const d3_segment& s2) const
  {
   return (point1()!=s2.point1() || point2()!=s2.point2());
  }

  friend __exportF ostream& operator<<(ostream& out, const d3_segment& s);
  friend __exportF istream& operator>>(istream& in, d3_segment& s); 

  friend unsigned long ID_Number(const d3_segment&);

};


inline unsigned long ID_Number(const d3_segment& s){ return s.ptr()->id; }
inline const char* leda_tname(const d3_segment*){ return "d3_segment"; }


#if LEDA_ROOT_INCL_ID == 600075
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
