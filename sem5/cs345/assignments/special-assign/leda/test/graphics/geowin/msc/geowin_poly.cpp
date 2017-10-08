/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_poly.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/rat_window.h>
#include <LEDA/geowin.h>
#include <LEDA/plane_alg.h>
#include <iostream.h>

using namespace leda;

static void construct_gen_polygon(const list<rat_polygon>& L, list<rat_gen_polygon>& GL)
{ GL.clear();
  GL.append(rat_gen_polygon(L));
}

int main()
{
  list<rat_polygon>* H1;
  list<rat_gen_polygon>* H2;

  GeoWin GW("POLYGON DEMO");
  GeoWin GW2("A Generalized Polygon");
  
  list<rat_polygon> L;
  geo_scene input =  GW.new_scene(L); 

  geo_scene sc = GW2.new_scene(construct_gen_polygon, input, 
                              string("A GENERALIZED POLYGON"),H1,H2);
  GW2.set_color(sc, grey1);
  GW2.set_visible(sc, true);

  GW2.display(window::max,window::min);

  GW.display(window::min,window::min);
  GW.edit(input);
  //GW.close();

  GW2.edit();
  
  return 0;
}
