/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>
#include <LEDA/float_geo_alg.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


void ori_fcn(GeoWin& gw, list<point>& L)
{
 /* if (L.size() == 3) {
    cout << L << "\n";
    point p1=L.head(), p2= L[L.succ(L.first())], p3=L.tail();
    cout << orientation(p1,p2,p3) << "\n";
  } */
  polygon Pl = CONVEX_HULL_POLY(L);
  window& win = gw.get_window();
  color cold= win.set_color(blue);
  win << Pl;
  win.set_color(cold);
}

int main() 
{
  list<point> LP;

  GeoWin gw;
  gw.add_help_text("GeoWin");
  
  list<point> L;
  
  GeoEditScene<list<point> >* sc = gw.new_scene(L);
  gw.add_buffer_fcn(sc, ori_fcn, "Convex Hull");

  gw.edit();
  return 0;
}

