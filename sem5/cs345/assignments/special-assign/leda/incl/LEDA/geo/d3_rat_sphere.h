/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_rat_sphere.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_RAT_SPHERE_H
#define LEDA_D3_RAT_SPHERE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600049
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/d3_rat_point.h>
#include <LEDA/geo/d3_rat_plane.h>
#include <LEDA/geo/d3_sphere.h>

LEDA_BEGIN_NAMESPACE


class __exportC d3_rat_sphere;

//------------------------------------------------------------------------------
// d3_rat_spheres
//------------------------------------------------------------------------------

class __exportC d3_rat_sphere_rep  : public handle_rep {

friend class __exportC d3_rat_sphere;

static atomic_counter id_counter;
   
   d3_rat_point a,b,c,d;

   unsigned long id;

public:
    
   d3_rat_sphere_rep(const d3_rat_point&,const d3_rat_point&,const d3_rat_point&,const d3_rat_point&);
  ~d3_rat_sphere_rep() {}

  friend inline unsigned long ID_Number(const d3_rat_sphere&); 
};


/*{\Manpage {d3_rat_sphere} {} {Rational Spheres} {S}}*/

class __exportC d3_rat_sphere : public HANDLE_BASE(d3_rat_sphere_rep) {

/*{\Mdefinition
An instance of the data type |\Mname| is an oriented sphere in 3\-d space. 
The sphere is defined by four points |p1,p2,p3,p4| with rational coordinates 
(|d3_rat_points|). }*/

 d3_rat_sphere_rep* ptr() const { return (d3_rat_sphere_rep*)PTR; }

 public:

/*{\Mcreation}*/

  d3_rat_sphere(const d3_rat_point& p1, const d3_rat_point& p2,
                                        const d3_rat_point& p3,
                                        const d3_rat_point& p4);
/*{\Mcreate introduces a variable |S| of type |\Mname|. 
            |S| is initialized to the sphere through points |p1,p2,p3,p4|.}*/

  d3_rat_sphere(const d3_rat_sphere& S) : HANDLE_BASE(d3_rat_sphere_rep)(S) {}

  d3_rat_sphere();

  ~d3_rat_sphere() {}

  d3_rat_sphere& operator=(const d3_rat_sphere& s)
  { HANDLE_BASE(d3_rat_sphere_rep)::operator=(s); return *this; }


/*{\Moperations 2.5 4}*/

  d3_sphere to_d3_sphere() const  
  { return  d3_sphere(point1().to_float(),point2().to_float(),
                      point3().to_float(),point4().to_float()); }

  d3_sphere to_float() const { return  to_d3_sphere(); }
/*{\Mop       returns a floating point approximation of |\Mvar|. }*/

  bool contains(const d3_rat_point& p) const;
/*{\Mop returns true, if p is on the sphere, false otherwise.}*/

  bool inside(const d3_rat_point& p) const;
/*{\Mop returns true, if p is inside the sphere, false otherwise.}*/

  bool outside(const d3_rat_point& p) const;
/*{\Mop returns true, if p is outside the sphere, false otherwise.}*/
 

  d3_rat_point point1() const { return ptr()->a; }
/*{\Mop returns |p1|.}*/

  d3_rat_point point2() const { return ptr()->b; }
/*{\Mop returns |p2|.}*/

  d3_rat_point point3() const { return ptr()->c; }
/*{\Mop returns |p3|.}*/

  d3_rat_point point4() const { return ptr()->d; }
/*{\Mop returns |p4|.}*/

  bool is_degenerate() const { return coplanar(point1(),point2(),point3(),point4()); }
/*{\Mop returns true, if the 4 defining points are coplanar.}*/

  d3_rat_point center() const;
/*{\Mop returns the center of the sphere.}*/

  rational sqr_radius() const;
/*{\Mop returns the square of the radius.}*/

  d3_rat_sphere translate(const rat_vector& v) const;
/*{\Mop translates the sphere by vector v and returns a new |d3_rat_sphere|.}*/

  d3_rat_sphere translate(const rational& r1, const rational& r2, const rational& r3) const;
/*{\Mop translates the sphere by vector (r1,r2,r3) and returns a new |d3_rat_sphere|.}*/

  friend bool operator==(const d3_rat_sphere& s1,const d3_rat_sphere& s2)
  { return (s1.point1()==s2.point1() && s1.point2()==s2.point2() && 
           s1.point3()==s2.point3() && s1.point4()==s2.point4());
   }

  friend bool operator!=(const d3_rat_sphere& s1,const d3_rat_sphere& s2)
  { return (s1.point1()!=s2.point1() || s1.point2()!=s2.point2() || 
            s1.point3()!=s2.point3() || s1.point4()!=s2.point4());
   }

  friend unsigned long ID_Number(const d3_rat_sphere&);

};

inline unsigned long ID_Number(const d3_rat_sphere& s) 
{ return s.ptr()->id; }

inline const char* leda_tname(const d3_rat_sphere*)  
{ return "d3_rat_sphere"; }


inline ostream& operator<<(ostream& s, const d3_rat_sphere& T)
{ s << T.point1() << " ";
  s << T.point2() << " ";
  s << T.point3() << " ";
  s << T.point4();
  return s;
}

inline istream& operator>>(istream& in, d3_rat_sphere& T)
{ d3_rat_point a,b,c,d;
  in >> a >> b >> c >> d;
  T = d3_rat_sphere(a,b,c,d);
  return in;
} 



#if LEDA_ROOT_INCL_ID == 600049
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
