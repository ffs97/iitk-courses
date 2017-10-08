/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_crust.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/rat_window.h>
#include<LEDA/rat_geo_alg.h>
#include<LEDA/geowin.h>

using namespace leda;

typedef rat_point   POINT;
typedef rat_polygon POLYGON;


geowin_update<list<POINT>, list<POLYGON> >          hull(CONVEX_HULL_POLY);

geowin_graph_update<list<POINT>, GRAPH<POINT,int> > crust(CRUST);

int main()
{
  GeoWin gw("Crust Demo");
  
  list<POINT> L;
  geo_scene sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);
  
  
  geo_scene sc1 = gw.new_scene(crust, sc_input, "Crust"); 
  gw.set_color(sc1,blue);
  gw.set_visible(sc1,true);

  geo_scene sc2 = gw.new_scene(hull,sc_input, "CONVEX HULL");
  gw.set_fill_color(sc2,invisible);
  gw.set_color(sc2,red);
  gw.set_visible(sc2,true);
    
  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

