/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  my_point.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//#define GEOWIN_USE_NAMESPACE
//#define GEOWIN_NAMESPACE_NAME CGAL

#include<LEDA/graphics/window.h>
#include<LEDA/graphics/ps_file.h>
#include <LEDA/graphics/geowin.h>
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

const char* leda_tname(My_Point* p) {  return "My_Point"; }

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

GEOWIN_BEGIN_NAMESPACE

bool geowin_IntersectsBox(const My_Point& obj, double x1,double y1,double x2, double y2,bool f)
{
 if (obj.x < x2 && obj.x > x1 && obj.y < y2 && obj.y > y1) return true;
 else return false;
}

void geowin_BoundingBox(const My_Point& obj, double& x1, double& x2,double& y1, double& y2)
{ x1=obj.x; y1=obj.y; x2=obj.x; y2=obj.y; }

void geowin_Translate(My_Point& obj, double dx, double dy)
{
 obj.x = obj.x + dx;
 obj.y = obj.y + dy;
}

void geowin_Rotate(My_Point& obj, double dx, double dy,double a)
{
}

//}

GEOWIN_END_NAMESPACE

#undef deque
#undef list
#include <deque>
#include <list>

//#define CONTAINER std::list<My_Point>
//#define CONTAINER std::deque<My_Point>
#define MYCONTAINER leda_list<My_Point>

//using namespace CGAL;
//namespace CGAL {

GEOWIN_BEGIN_NAMESPACE

void geowin_generate_objects(GeoWin& gw, MYCONTAINER& L)
{
}

string geowin_info_fcn(const MYCONTAINER& L)
{
  string s;
  return s;
}

GEOWIN_END_NAMESPACE

#include <LEDA/graphics/geowin_init.h>

void dummy_d3_f(const MYCONTAINER&, d3_window&, GRAPH<d3_point,int>&)
{
}

//}

int main()
{
//  geowin_init_default_type((CONTAINER*)0,string("MyPointlist"), dummy_d3_f) ;
  geowin_init_default_type((MYCONTAINER*)0,string("MyPointlist")) ;

  GeoWin gw;

  gw.edit();

  return 1;
}
