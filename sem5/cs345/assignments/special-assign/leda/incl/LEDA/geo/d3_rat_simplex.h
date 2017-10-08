/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_rat_simplex.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_RAT_SIMPLEX_H
#define LEDA_D3_RAT_SIMPLEX_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600090
#include <LEDA/internal/PREAMBLE.h>
#endif

// simplices using points with rational coordinates

#include <LEDA/geo/d3_rat_point.h>
#include <LEDA/geo/d3_rat_plane.h>
#include <LEDA/geo/d3_rat_sphere.h>
#include <LEDA/geo/d3_simplex.h>

LEDA_BEGIN_NAMESPACE

class __exportC d3_rat_simplex;


//------------------------------------------------------------------
// d3_rat_simplices
//------------------------------------------------------------------

class __exportC d3_rat_simplex_rep : public handle_rep {

friend class __exportC d3_rat_simplex;

static atomic_counter id_counter;

 d3_rat_point a,b,c,d;

 unsigned long id;

public:

 d3_rat_simplex_rep(const d3_rat_point&,const d3_rat_point&,const d3_rat_point&,const d3_rat_point&);
 ~d3_rat_simplex_rep() {}

  friend inline unsigned long ID_Number(const d3_rat_simplex&); 
};


/*{\Manpage {d3_rat_simplex} {} {Rational Simplices} {S}}*/

class __exportC d3_rat_simplex : public HANDLE_BASE(d3_rat_simplex_rep) { 
/*{\Mdefinition
An instance of the data type |\Mname| is a simplex in 3\-d space. The simplex is defined
by four points |p1,p2,p3,p4| with rational coordinates (|d3_rat_points|).
We call the simplex degenerate, if the four defining points are coplanar.
}*/

 d3_rat_simplex_rep* ptr() const { return (d3_rat_simplex_rep*)PTR; }

public:
/*{\Mtypes 5}*/ 

typedef rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef d3_rat_point   point_type;
/*{\Mtypemember the point type (|d3_rat_point|).}*/

typedef d3_simplex float_type;

/*{\Mcreation}*/  

 d3_rat_simplex(const d3_rat_point& a, const d3_rat_point& b,
                const d3_rat_point& c, const d3_rat_point& d);
/*{\Mcreate creates the simplex |(a,b,c,d)|.}*/

 d3_rat_simplex();
/*{\Mcreate creates the simplex |((0,0,0),(1,0,0),(0,1,0),(0,0,1) )|.}*/

 d3_rat_simplex(const d3_rat_simplex& S) : HANDLE_BASE(d3_rat_simplex_rep)(S) {}

 ~d3_rat_simplex() {}

 d3_rat_simplex& operator=(const d3_rat_simplex& s)
 { HANDLE_BASE(d3_rat_simplex_rep)::operator=(s); return *this; }

/*{\Moperations 2.5 4}*/

 d3_simplex to_d3_simplex() const;
/*{\Mop returns a floating point approximation of |\Mvar|.}*/
 
 d3_simplex to_float() const;
 
 d3_rat_point point1() const { return ptr()->a; }
/*{\Mop returns |p1|.}*/
 d3_rat_point point2() const { return ptr()->b; }
/*{\Mop returns |p2|.}*/
 d3_rat_point point3() const { return ptr()->c; }
/*{\Mop returns |p3|.}*/
 d3_rat_point point4() const { return ptr()->d; }
/*{\Mop returns |p4|.}*/

 d3_rat_point operator[](int i) const;
/*{\Marrop returns |pi|. \precond |i>0| and |i<5|.}*/

 int index(const d3_rat_point& p) const;
/*{\Mop returns 1 if |p==p1|, 2 if |p==p2|, 3 if |p==p3|, 4 if |p==p4|, 0 otherwise.}*/

 bool is_degenerate() const;
/*{\Mop returns true if |\Mvar| is degenerate and false otherwise.}*/

 d3_rat_sphere circumscribing_sphere() const;
/*{\Mop returns a |d3_rat_sphere| through |(p1,p2,p3,p4)| (precondition: the |d3_rat_simplex| is not degenerate).}*/

 bool in_simplex(const d3_rat_point& p) const;
/*{\Mop returns true, if |p| is contained in the simplex.}*/

 bool insphere(const d3_rat_point& p) const;
/*{\Mop returns true, if |p| lies in the interior of the sphere through |p1,p2,p3,p4|.}*/

 rational vol() const;
/*{\Mop returns the signed volume of the simplex.}*/

 d3_rat_simplex reflect(const d3_rat_point& p,const d3_rat_point& q,const d3_rat_point& v) const;
/*{\Mop returns |\Mvar| reflected across the plane through |(p,q,v)|.}*/

 d3_rat_simplex reflect(const d3_rat_point& p) const;
/*{\Mop returns |\Mvar| reflected across point |p|.}*/
  
 d3_rat_simplex translate(const rat_vector& v) const;
/*{\Mop returns |\Mvar| translated by vector |v|.\\
        |Precond.: v.dim()=3|.}*/

 d3_rat_simplex translate(rational dx,rational dy,rational dz) const;
/*{\Mop returns |\Mvar| translated by vector |(dx,dy,dz)|.}*/

 d3_rat_simplex translate(integer dx,integer dy,integer dz,integer dw) const;
/*{\Mop returns |\Mvar| translated by vector |(dx/dw,dy/dw,dz/w)|.}*/

 d3_rat_simplex  operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop   returns |\Mvar| translated by vector $v$.}*/

 d3_rat_simplex  operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop   returns |\Mvar| translated by vector $-v$.}*/


 friend bool operator==(const d3_rat_simplex& s1,const d3_rat_simplex& s2)
 {  
   return (s1.point1()==s2.point1() && s1.point2()==s2.point2() && s1.point3()==s2.point3() && s1.point4()==s2.point4());
 }

 friend bool operator!=(const d3_rat_simplex& s1,const d3_rat_simplex& s2)
 {
   return (s1.point1()!=s2.point1() || s1.point2()!=s2.point2() || s1.point3()!=s2.point3() || s1.point4()!=s2.point4());
 }

 friend unsigned long ID_Number(const d3_rat_simplex&);

};

inline unsigned long ID_Number(const d3_rat_simplex& s){ return s.ptr()->id; }
inline const char* leda_tname(const d3_rat_simplex*){ return "d3_rat_simplex"; }


inline ostream& operator<<(ostream& s, const d3_rat_simplex& T)
{
 d3_rat_point a,b,c,d;
 a=T.point1(); b=T.point2(); c=T.point3(); d=T.point4();
 s << a;
 s << b;
 s << c;
 s << d;
 return s;
}

inline istream& operator>>(istream& in, d3_rat_simplex& T) 
{ 
 d3_rat_point a,b,c,d;
 in >> a;
 in >> b; 
 in >> c;
 in >> d;

 T=d3_rat_simplex(a,b,c,d);

 return in; 
}

#if LEDA_ROOT_INCL_ID == 600090
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif

