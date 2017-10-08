/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_sphere.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_SPHERE_H
#define LEDA_D3_SPHERE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600127
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/geo/d3_point.h>
#include <LEDA/geo/d3_plane.h>

LEDA_BEGIN_NAMESPACE


class __exportC d3_sphere;

//------------------------------------------------------------------------------
// d3_spheres
//------------------------------------------------------------------------------

class __exportC d3_sphere_rep  : public handle_rep {

friend class __exportC d3_sphere;

static atomic_counter id_counter;
   
   d3_point a,b,c,d;

   unsigned long id;

public:
    
   d3_sphere_rep(const d3_point&,const d3_point&,const d3_point&,const d3_point&);
  ~d3_sphere_rep() {}

  friend inline unsigned long ID_Number(const d3_sphere&); 
};


/*{\Manpage {d3_sphere} {} {Spheres in 3D-Space} {S}}*/

class __exportC d3_sphere : public HANDLE_BASE(d3_sphere_rep) {

/*{\Mdefinition
An instance of the data type |\Mname| is an oriented sphere in 3\-d space. 
The sphere is defined by four points |p1,p2,p3,p4| (|d3_points|).
}*/

 d3_sphere_rep* ptr() const { return (d3_sphere_rep*)PTR; }

 public:

/*{\Mcreation}*/

  d3_sphere(const d3_point& p1, const d3_point& p2, const d3_point& p3,
                                                    const d3_point& p4);

/*{\Mcreate introduces a variable |S| of type |\Mname|. |S| is initialized 
            to the sphere through points |p1,p2,p3,p4|.}*/

  d3_sphere(const d3_sphere& S) : HANDLE_BASE(d3_sphere_rep)(S) {}

  d3_sphere();

  ~d3_sphere() {}

  d3_sphere& operator=(const d3_sphere& s)
  { HANDLE_BASE(d3_sphere_rep)::operator=(s); return *this; }

/*{\Moperations 2.5 4}*/

  d3_sphere to_d3_sphere() const { return *this; }
  d3_sphere to_float()     const { return *this; }


  bool contains(const d3_point& p) const;
/*{\Mop returns true, if p is on the sphere, false otherwise.}*/

  bool inside(const d3_point& p) const;
/*{\Mop returns true, if p is inside the sphere, false otherwise.}*/

  bool outside(const d3_point& p) const;
/*{\Mop returns true, if p is outside the sphere, false otherwise.}*/
 

  d3_point point1() const { return ptr()->a; }
/*{\Mop returns |p1|.}*/

  d3_point point2() const { return ptr()->b; }
/*{\Mop returns |p2|.}*/

  d3_point point3() const { return ptr()->c; }
/*{\Mop returns |p3|.}*/

  d3_point point4() const { return ptr()->d; }
/*{\Mop returns |p4|.}*/

  bool is_degenerate() const { return coplanar(point1(),point2(),point3(),point4()); }
/*{\Mop returns true, if the 4 defining points are coplanar.}*/

  d3_point center() const;
/*{\Mop returns the center of the sphere.}*/

  double sqr_radius() const;
/*{\Mop returns the square of the radius.}*/

  double radius() const;
/*{\Mop returns the radius.}*/

  double surface() const;
/*{\Mop returns the size of the surface.}*/

  double volume() const;
/*{\Mop returns the volume of the sphere.}*/

  d3_sphere translate(const vector& v) const;
/*{\Mop returns |\Mvar| translated by vector |v|. }*/


  d3_sphere translate(double dx,double dy,double dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|. }*/

  friend bool operator==(const d3_sphere& s1,const d3_sphere& s2)
  { return (s1.point1() == s2.point1() && s1.point2() == s2.point2() && 
            s1.point3() == s2.point3() && s1.point4() == s2.point4());
  }

  friend bool operator!=(const d3_sphere& s1,const d3_sphere& s2)
  { return (s1.point1() != s2.point1() || s1.point2() != s2.point2() || 
            s1.point3() != s2.point3() || s1.point4() != s2.point4());
  }

  friend unsigned long ID_Number(const d3_sphere&);
};

inline unsigned long ID_Number(const d3_sphere& s) { return s.ptr()->id; }
inline const char* leda_tname(const d3_sphere*)    { return "d3_sphere"; }


inline ostream& operator<<(ostream& s, const d3_sphere& T)
{ s << T.point1() << " ";
  s << T.point2() << " ";
  s << T.point3() << " ";
  s << T.point4();
  return s;
}

inline istream& operator>>(istream& in, d3_sphere& T)
{ d3_point a,b,c,d;
  in >> a >> b >> c >> d;
  T = d3_sphere(a,b,c,d);
  return in;
}


#if LEDA_ROOT_INCL_ID == 600127
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

