/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_rat_ray.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_RAT_RAY_H
#define LEDA_D3_RAT_RAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600059
#include <LEDA/internal/PREAMBLE.h>
#endif

// rational rays in 3-d space 

#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/d3_rat_segment.h>
#include <LEDA/geo/d3_ray.h>
#include <LEDA/geo/rat_line.h>
#include <LEDA/geo/rat_ray.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/numbers/rat_vector.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_rat_ray;

//------------------------------------------------------------------------------
// straight rational d3_rays (1999)
//------------------------------------------------------------------------------

class __exportC d3_rat_ray_rep  : public handle_rep {

friend class __exportC d3_rat_ray;

static atomic_counter id_counter;

   d3_rat_segment seg;

   unsigned long id;

public:
    
   d3_rat_ray_rep() {}
   d3_rat_ray_rep(const d3_rat_segment& s);
  ~d3_rat_ray_rep() {}

   friend inline unsigned long ID_Number(const d3_rat_ray&); 
};


/*{\Manpage {d3_rat_ray} {} {Straight Rational Rays in 3D-Space} {r}}*/

class __exportC d3_rat_ray : public HANDLE_BASE(d3_rat_ray_rep) {
/*{\Mdefinition
An instance $r$ of the data type |\Mname| is a directed straight ray
defined by two points with rational coordinates in three-dimensional space.
}*/

 d3_rat_ray_rep* ptr() const { return (d3_rat_ray_rep*)PTR; }

 bool in_xyz(const d3_rat_point& p) const;

 public:
/*{\Mcreation}*/

  d3_rat_ray(const d3_rat_point& p1,const d3_rat_point& p2);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the ray
    starting at point |p1| and going through |p2|.}*/

  d3_rat_ray(const d3_rat_segment& s);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to 
    |ray(s.source(),s.target())| .}*/

  d3_rat_ray(const list<d3_rat_point>& L);

  d3_rat_ray(const d3_rat_ray& r) : HANDLE_BASE(d3_rat_ray_rep)(r) {}

  d3_rat_ray();

  ~d3_rat_ray() {}

  d3_rat_ray& operator=(const d3_rat_ray& r)
  { HANDLE_BASE(d3_rat_ray_rep)::operator=(r); return *this; }

/*{\Moperations 2.5 4}*/

  d3_rat_point source() const;
/*{\Mop returns the source of |\Mvar|.}*/

  d3_rat_point point1() const;
/*{\Mop     returns the source of |\Mvar|.}*/

  d3_rat_point point2() const;
/*{\Mop     returns a point on |\Mvar| different from the source.}*/

  d3_rat_segment seg() const;
/*{\Mop     returns a segment on |\Mvar|.}*/

  d3_ray   to_d3_ray() const { return d3_ray(point1().to_float(),point2().to_float()); }  
  
  d3_ray   to_float() const { return d3_ray(point1().to_float(),point2().to_float()); }


  bool x_equal() const;
  /*{\Xop returns true if all points on |\Mvar| have equal x coordinates.}*/
  bool y_equal() const;
  /*{\Xop returns true if all points on |\Mvar| have equal y coordinates.}*/
  bool z_equal() const;
  /*{\Xop returns true if all points on |\Mvar| have equal z coordinates.}*/
  bool xy_equal() const;
  /*{\Xop returns true if all points on |\Mvar| have equal x and y coordinates.}*/
  bool xz_equal() const;
  /*{\Xop returns true if all points on |\Mvar| have equal x and z coordinates.}*/
  bool yz_equal() const;
  /*{\Xop returns true if all points on |\Mvar| have equal y and z coordinates.}*/

  bool contains(const d3_rat_point& p) const;
/*{\Mop returns true if $p$ lies on |\Mvar|.}*/

  bool contains(const d3_rat_segment& s) const;
/*{\Mop returns true if $s$ lies on |\Mvar|.}*/
//intersections

  bool intersection(const d3_rat_segment& s, d3_rat_point& inter) const;
/*{\Mop if |s| and |\Mvar| intersect in a single point, true is returned and the point
    of intersection is assigned to inter. Otherwise false is returned.}*/
      
  bool intersection(const d3_rat_ray& r, d3_rat_point& inter) const;
/*{\Mop if |r| and |\Mvar| intersect in a single point, true is returned and the point
    of intersection is assigned to inter. Otherwise false is returned.}*/
    
//projections
  bool project_xy(rat_ray& m) const ;
/*{\Mop if the projection of |\Mvar| into the xy plane is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  bool project_xz(rat_ray& m) const ;
/*{\Mop if the projection of |\Mvar| into the xz plane is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  bool project_yz(rat_ray& m) const ;
/*{\Mop if the projection of |\Mvar| into the yz plane is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  bool project(const d3_rat_point& p, const d3_rat_point& q, const d3_rat_point& v, d3_rat_ray& m) const;
/*{\Mop if the projection of |\Mvar| into the plane  through |(p,q,v)| is not a point, the function
  returns true and assignes the projection to |m|. Otherwise false is returned. }*/

  d3_rat_ray reverse() const;
/*{\Mop returns a |rat_ray| starting at |\Mvar|.source() with direction
    -|\Mvar|.to\_vector() .}*/
    
  d3_rat_ray translate(const rat_vector& v) const;
/*{\Mop returns |\Mvar| translated by vector |v|.
        Precond. : |v.dim()| = 3 .}*/

  d3_rat_ray translate(rational dx, rational dy, rational dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|. }*/

  d3_rat_ray  operator+(const rat_vector& v) const { return translate(v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

  d3_rat_ray  operator-(const rat_vector& v) const { return translate(-v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/
  
  d3_rat_ray reflect(const d3_rat_point& p, const d3_rat_point& q, const d3_rat_point& v) const;
/*{\Mop returns |\Mvar| reflected across the plane through |(p,q,v)|.}*/

  d3_rat_ray reflect(const d3_rat_point& p) const;
/*{\Mop returns |\Mvar| reflected across point |p|.}*/  
  
  rat_vector to_vector() const;
/*{\Mop returns |point2()-point1()|.}*/

  friend bool operator==(const d3_rat_ray& r1,const d3_rat_ray& r2)
  {
    return ( r1.seg()==r2.seg() ); // Achtung !!!
  }

  friend bool operator!=(const d3_rat_ray& r1,const d3_rat_ray& r2)
  {
    return ( r1.seg()!=r2.seg() ); // Achtung !!!
  }

  friend unsigned long ID_Number(const d3_rat_ray&);

};



inline unsigned long ID_Number(const d3_rat_ray& l){ return l.ptr()->id; }
inline const char* leda_tname(const d3_rat_ray*){ return "d3_rat_ray"; }


inline ostream& operator<<(ostream& s, const d3_rat_ray& r){
 s << r.seg();
 return s;
}

inline istream& operator>>(istream& in, d3_rat_ray& r)
{
 d3_rat_point a,b;
 in >> a;
 in >> b; 
 
 r=d3_rat_ray(a,b);
 return in;
}

#if LEDA_ROOT_INCL_ID == 600059
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
