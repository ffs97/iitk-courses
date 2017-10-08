/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_triangle.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_TRIANGLE_H
#define LEDA_D3_TRIANGLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600081
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/geo/d3_point.h>
#include <LEDA/geo/d3_plane.h>
#include <LEDA/geo/triangle.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------
// d3_triangles
//------------------------------------------------------------------

class __exportC d3_triangle;

class __exportC d3_triangle_rep : public handle_rep {

friend class __exportC d3_triangle;

static atomic_counter id_counter;

 d3_point a,b,c;

 unsigned long id;

public:

 d3_triangle_rep(const d3_point&,const d3_point&,const d3_point&);
 ~d3_triangle_rep() {}

  friend inline unsigned long ID_Number(const d3_triangle&); 
};


/*{\Manpage {d3_triangle} {} {Triangles in 3d space} {T}}*/

class __exportC d3_triangle : public HANDLE_BASE(d3_triangle_rep) { 
/*{\Mdefinition
An instance of the data type |\Mname| is a triangle in 3\-d space. The triangle is defined
by three points |p1,p2,p3|  (|d3_points|). We call the triangle degenerate, if
the three defining points are collinear.
}*/

 d3_triangle_rep* ptr() const { return (d3_triangle_rep*)PTR; }

public:
/*{\Mtypes 5}*/ 

typedef double  coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef d3_point   point_type;
/*{\Mtypemember the point type (|d3_point|).}*/

typedef d3_triangle float_type;

/*{\Mcreation}*/  

 d3_triangle(const d3_point&  a,const d3_point& b,const d3_point& c);
/*{\Mcreate creates the triangle |(a,b,c)|.}*/

 d3_triangle();
/*{\Mcreate creates the triangle |((0,0,0),(1,0,0),(0,1,0))|.}*/

 d3_triangle(const d3_triangle& t) : HANDLE_BASE(d3_triangle_rep)(t) {}

 ~d3_triangle() {}

 d3_triangle& operator=(const d3_triangle& t)
 { HANDLE_BASE(d3_triangle_rep)::operator=(t); return *this; }

/*{\Moperations 2.5 4}*/

 d3_triangle to_d3_triangle() const
 { return *this; }
 
 d3_triangle to_float() const
 { return *this; }
 
 d3_point point1() const { return ptr()->a; }
/*{\Mop returns |p1|.}*/
 d3_point point2() const { return ptr()->b; }
/*{\Mop returns |p2|.}*/
 d3_point point3() const { return ptr()->c; }
/*{\Mop returns |p3|.}*/

 d3_point operator[](int i) const;
/*{\Marrop returns $p_i$. \precond $0\le i\le 3$.}*/

 d3_plane supporting_plane() const;
/*{\Mop returns the supporting plane of |\Mvar|.}*/

 int index(const d3_point& p) const ;
/*{\Mop returns $1$ if |p==p1|, $2$ if |p==p2|, $3$ if |p==p3| and $0$ otherwise.}*/

 bool is_degenerate() const;
/*{\Mop returns true if |\Mvar| is degenerate and false otherwise.}*/

  d3_triangle reflect(const d3_point& p,const d3_point& q,const d3_point& v) const;
/*{\Mop returns |\Mvar| reflected across the plane through |(p,q,v)|.}*/

  d3_triangle reflect(const d3_point& p) const;
  /*{\Mop returns |\Mvar| reflected across point |p|.}*/
  
  d3_triangle translate(const vector& v) const;
/*{\Mop returns |\Mvar| translated by vector v. \\
       |Precond.: v.dim()==3|.}*/

  triangle project_xy() const 
  { return triangle(point1().project_xy(), point2().project_xy(),
                                           point3().project_xy());}
/*{\Mop  returns |\Mvar| projected into the xy-plane. }*/

  triangle project_yz() const
  { return triangle(point1().project_yz(), point2().project_yz(),
                                           point3().project_yz());}
/*{\Mop  returns |\Mvar| projected into the yz-plane. }*/

  triangle project_xz() const
  { return triangle(point1().project_xz(), point2().project_xz(),
                                           point3().project_xz());}
/*{\Mop  returns |\Mvar| projected into the xz-plane. }*/

       
  d3_triangle translate(double dx,double dy,double dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|.}*/

  d3_triangle  operator+(const vector& v) const { return translate(v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

  d3_triangle  operator-(const vector& v) const { return translate(-v); }
  /*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/

 bool operator==(const d3_triangle& t2) const;

 bool operator!=(const d3_triangle& t2) const
 { return (!((*this)==t2)); }

 friend unsigned long ID_Number(const d3_triangle&);

};

inline unsigned long ID_Number(const d3_triangle& t){ return t.ptr()->id; }
inline const char* leda_tname(const d3_triangle*){ return "d3_triangle"; }

inline ostream& operator<<(ostream& out, const d3_triangle& T)
{ return out << T.point1() << " " << T.point2() << " " << T.point3(); }

inline istream& operator>>(istream& in, d3_triangle& T) 
{ d3_point a,b,c;
  in >> a >> b >> c;
  T=d3_triangle(a,b,c);
  return in; 
 }

#if LEDA_ROOT_INCL_ID == 600081
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
