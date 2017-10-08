/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_logo.c
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
#include <LEDA/logo.h>
#include <iostream.h>

using namespace leda;

typedef rat_gen_polygon GEN_POLYGON;

static void intersect_poly(const list<GEN_POLYGON>& L, list<GEN_POLYGON>& I)
{ I.clear();
  GEN_POLYGON P1 = L.head();
  GEN_POLYGON P2 = L.tail();
  I.append(P1.intersection(P2));
}

static void union_poly(const list<GEN_POLYGON>& L, list<GEN_POLYGON>& U)
{ U.clear();
  GEN_POLYGON P1 = L.head();
  GEN_POLYGON P2 = L.tail();
  U.append(P1.unite(P2));
}


int main()
{
  list<GEN_POLYGON>* H1;

  GeoWin gw("POLYGON DEMO");
  
  list<GEN_POLYGON> L;
  GEN_POLYGON logo(leda_logo(2),1024);
  L.append(logo);
  L.append(logo.translate(3,4));

  geo_scene input = gw.new_scene(L); 

  geo_scene sc1 = gw.new_scene(intersect_poly, input, string("Intersection"), H1,H1);
  gw.set_fill_color(sc1,blue);
  gw.set_visible(sc1,true);

  geo_scene sc2 = gw.new_scene(union_poly, input, string("Union"),H1,H1);
  gw.set_fill_color(sc2,green);
  gw.set_visible(sc2,false);

  gw.init(-6,6,-6);
  gw.display(window::max,window::min);
  gw.edit(input);
  
  return 0;
}
