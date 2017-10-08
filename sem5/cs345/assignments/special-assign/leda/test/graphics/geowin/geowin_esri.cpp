/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_esri.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/geowin_io.h>
#include <LEDA/geo/geo_alg.h>

using namespace leda;

static list<segment> edge_list;
geo_scene sc2;

void algo_poly_area(GeoWin& gw, list<polygon>& PL)
{
  double A = 0;

  polygon pol;
  forall(pol, PL) A += pol.area();

  panel P;
  P.text_item("");
  P.text_item(string("area = %.2f",A));
  P.text_item("");
  P.button("OK");

  edge_list.clear();
  gw.redraw();
  gw.open_panel(P);
  
  sc2->init_data();
}



void algo_poly_bbox(GeoWin& gw, list<polygon>& PL)
{
  edge_list.clear();
  polygon pol;
  forall(pol, PL){
    rectangle r =  BoundingBox(pol);
    edge_list.append(segment(r.upper_left(),r.upper_right()));
    edge_list.append(segment(r.upper_right(),r.lower_right()));
    edge_list.append(segment(r.lower_right(),r.lower_left()));
    edge_list.append(segment(r.lower_left(),r.upper_left()));
  }
  gw.redraw();
  
  sc2->init_data();
}


void algo_poly_triang(GeoWin& gw, list<polygon>& L)
{
  edge_list.clear();

  polygon P;
  forall(P,L)
  { if (!P.is_simple()) continue;
    GRAPH<point,segment> G;
    list<edge> out,in,bound;
    TRIANGULATE_POLYGON(P,G,in,out,bound);
    edge e;
    forall(e,in) edge_list.append(segment(G[source(e)],G[target(e)]));
    forall(e,bound) edge_list.append(segment(G[source(e)],G[target(e)]));
  } 

  gw.redraw();
  
  sc2->init_data();
}

void algo_poly_convex(GeoWin& gw, list<polygon>& L)
{
  edge_list.clear();

  polygon P;
  forall(P,L)
  { if (!P.is_simple()) continue;
    GRAPH<point,segment> G;
    list<edge> in,bound;
    CONVEX_COMPONENTS(P,G,in,bound);
    edge_list.clear();
    edge e;
    forall(e,in) edge_list.append(segment(G[source(e)],G[target(e)]));
    forall(e,bound) edge_list.append(segment(G[source(e)],G[target(e)]));
  } 

  gw.redraw();
  
  sc2->init_data();
}



void algo_poly_clear(GeoWin& gw, list<polygon>&) { 
  edge_list.clear(); 
  sc2->init_data();
}



int main() 
{
  GeoWin gw("ESRI shapefile import");
  gw.message("Use File->Import Data to import data from files storing polygons in ESRI shapefile format");
  
  gw.init(-118, -85, 5);
  
  geowin_import_esri_objects<polygon, rat_polygon>  IP1;
  
  list<polygon> L;
  GeoEditScene<list<polygon> >* sc1 = gw.new_scene(L);  

  //geo_scene sc1 = gw.new_scene(L);  

  gw.add_import_object(sc1, IP1, "ESRI poly", "Import ESRI");

  geowin_add_algorithm(sc1, algo_poly_area,   "compute area");
  geowin_add_algorithm(sc1, algo_poly_bbox,   "bounding box");
  geowin_add_algorithm(sc1, algo_poly_triang, "triangulation");
  geowin_add_algorithm(sc1, algo_poly_convex, "convex parts");
  geowin_add_algorithm(sc1, algo_poly_clear,  "clear");

  gw.set_handle_defining_points(sc1,geowin_hide);

  //gw.set_visible(sc1,true);
  gw.set_z_order(sc1,1);

  sc2  = gw.new_scene(edge_list);
  gw.set_color(sc2,blue);
  gw.set_visible(sc2,true);
  gw.set_z_order(sc2,0);
    
  gw.edit();

  return 0;
}

