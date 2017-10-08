/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_d3hull.c
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
#include <LEDA/d3_hull.h>

using namespace leda;

#define D3_POINT    d3_point
#define D2_POINT    point
#define D2_POLYGON  polygon

static void hull_2d( const list<D3_POINT>& Lin, list<D2_POLYGON>& Pol)
{
  Pol.clear();
  D3_POINT p;
  list<D2_POINT> inp;
  list<D2_POINT> Lout;
  forall(p,Lin) inp.append(p.project_xy());

  Lout=CONVEX_HULL(inp);

  if( Lout.length() > 2 ) Pol.append(D2_POLYGON(Lout ));
}


static void show_d3_points(geo_scene sc, d3_window& W, GRAPH<D3_POINT,int>& H)
{
 cout << "Routine!\n"; cout.flush();
 
 GeoWin* gw = get_geowin(sc);
 list<D3_POINT> L;
 gw->get_objects(sc,L);
 D3_POINT p;
 forall(p,L) H.new_node(p);
 
 GRAPH<D3_POINT,int> G;

 D3_HULL(L,G); 

 H.join(G);

 cout << "after join!\n"; cout.flush();
 node_array<vector> pos(H);
 node v;
 forall_nodes(v,H) pos[v] = H[v].to_vector();
 
 cout << "before init!\n"; cout.flush();
 W.init(pos);
 
 cout << "after init!\n"; cout.flush();
}

int main()
{
  list<D3_POINT>* H1;
  list<D2_POLYGON>* H2;

  GeoWin GW( "D3 - Hulldemo");
 
  list<D3_POINT> L;

  geo_scene input =   GW.new_scene(L); 
  GW.set_color( input, green );
  GW.set_d3_fcn(input, show_d3_points);

  geo_scene output1=  GW.new_scene(hull_2d,input, string("Convex hull"),H1,H2);
  GW.set_color( output1, green );
  GW.set_fill_color(output1, green2 );
  GW.set_color2( output1, blue ); 
  GW.set_visible(output1, true );  

  string msg1("To show 3d output (points and the 3d convex hull of the points)");
  string msg2("use the Show d3 output menu button in the Window menu.");

  GW.msg_open(msg1); GW.msg_open(msg2);

  GW.edit(input);
  return 0;
}
