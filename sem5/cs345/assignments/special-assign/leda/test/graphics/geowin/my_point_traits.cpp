/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  my_point_traits.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include<LEDA/graphics/ps_file.h>
#include<ctype.h>

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;
#endif

using namespace leda;

//using namespace CGAL;

class My_Point{
public:
 double x,y;

 My_Point(){ x=0; y=0; }
 My_Point(double xw, double yw) { x=xw; y=yw; }
};

ostream& operator<<(ostream& out, const My_Point& p)
{ out << "(" << p.x << "," << p.y << ")";
  return out;
}

istream& operator>>(istream& in, My_Point& p) 
{
  double x,y; 
  char c;

  do in.get(c); while (in && isspace(c));

  if (!in) return in;

  if (c != '(') in.putback(c);

  in >> x;

  do in.get(c); while (isspace(c));
  if (c != ',') in.putback(c);

  in >> y; 

  do in.get(c); while (c == ' ');
  if (c != ')') in.putback(c);

  p.x=x; p.y=y;
  return in; 
} 

ps_file& operator<<(ps_file& F,const My_Point& o) { return F; }  // only a dummy

//const char* leda_tname(My_Point* p) {  return "My_Point"; }

leda_window& operator << (leda_window& w, const My_Point& obj){ w << point(obj.x,obj.y); return w; }

leda_window& operator >> (leda_window& w, My_Point& obj)
{
  point p1;
  if( w >> p1 ) {
    obj.x = p1.xcoord();
    obj.y = p1.ycoord();
  }
  return w;
}

//namespace CGAL {

bool IBox(const My_Point& obj, double x1,double y1,double x2, double y2,bool f)
{
 if (obj.x < x2 && obj.x > x1 && obj.y < y2 && obj.y > y1) return true;
 else return false;
}

void BBox(const My_Point& obj, double& x1, double& x2,double& y1, double& y2)
{ x1=obj.x; y1=obj.y; x2=obj.x; y2=obj.y; }

void Translate(My_Point& obj, double dx, double dy)
{
 obj.x = obj.x + dx;
 obj.y = obj.y + dy;
}

void Rotate(My_Point& obj, double dx, double dy,double a)
{
}

//}


#include <LEDA/graphics/geowin.h>
#include <LEDA/graphics/geowin_init.h>
#undef list
#include <list>

//using namespace CGAL;
//namespace CGAL {

template<class CONT>
class geo_scene_traits {
 string description;
 
 public:
 
 typedef CONT                       CONTAINER;
 typedef typename CONT::value_type  MYTYPE;
 
 string get_name() { return description; }

 string (*geowin_info_fcn)(const CONTAINER& L);
 bool (*geowin_IntersectsBox)(const MYTYPE& obj, double x1,double y1,double x2, double y2,bool f);
 void (*geowin_BoundingBox)(const MYTYPE& obj, double& x1, double& x2,double& y1, double& y2);
 void (*geowin_Translate)(MYTYPE& obj, double dx, double dy);
 void (*geowin_Rotate)(MYTYPE& obj, double dx, double dy,double a);
 void (*geowin_generate_objects)(GeoWin& gw, CONTAINER& L);
 
 geo_scene_traits( string (*f1)(const CONTAINER& ), \
   bool (*f2)(const MYTYPE&, double, double, double, double,bool ), \
   void (*f3)(const MYTYPE&, double&, double&, double&, double& ), \
   void (*f4)(MYTYPE&, double, double), void (*f5)(MYTYPE&, double, double, double), \
   void (*f6)(GeoWin&, CONTAINER&), string scene_type_name)
 {
   description = scene_type_name;
   geowin_info_fcn = f1;
   geowin_IntersectsBox = f2;
   geowin_BoundingBox = f3;
   geowin_Translate = f4;
   geowin_Rotate = f5;
   geowin_generate_objects = f6;
 }
};

void generate_objects(GeoWin& gw, std::list<My_Point>& L)
{
}

string info_fcn(const std::list<My_Point>& L)
{
  string s;
  return s;
}

//}

int main()
{
  geo_scene_traits<std::list<My_Point> > TR(info_fcn, IBox, BBox, 
             Translate, Rotate, generate_objects, string("MyPointlist"));
  
  geowin_init_default_type(TR);
  
  GeoWin gw;

  gw.edit();

  return 1;
}
