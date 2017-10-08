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

typedef rat_vector  VECTOR;
typedef rat_point   POINT;
typedef rat_line    LINE;
typedef rat_ray     RAY;
typedef rat_circle  CIRCLE;
typedef rat_polygon POLYGON;


void draw_graph_edges(window& W, const GRAPH<POINT,int>& G, color col)
{ edge_array<bool> drawn(G,false);
  edge e;
  forall_edges(e,G) 
  { edge r = G.reversal(e);
    if (r)
    { if (drawn[e]) continue;
       drawn[r] = drawn[e] = true;
     }
    node v = source(e);
    node w = target(e);
    W.draw_segment(G[v].to_point(),G[w].to_point(),col);
  }  
}

class geo_redraw_crust : public geo_redraw_base, public geo_update_base<list<POINT>, list<POINT> >
{
public:
  GRAPH<POINT,int> CR;

  virtual ~geo_redraw_crust() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {  draw_graph_edges(W,CR,c1); }

  virtual void update(const list<POINT>& L, list<POINT>&)
  {  CRUST(L,CR);  }
};


static void convex_hull(const list<POINT>& L, list<POLYGON>& P)
{ P.clear();
  list<POINT> ch = CONVEX_HULL(L);
  if( ch.length() > 2 ) P.append(POLYGON(ch));
}



int main()
{
  GeoWin gw("Voronoi Demo");

  list<POINT>* H1;
  list<POLYGON>* H2;
  
  list<POINT> L;
  geo_scene sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);

  geo_scene sc2 = gw.new_scene(convex_hull,      sc_input, string("CONVEX HULL"),H1,H2);
  gw.set_color(sc2,red);
  gw.set_fill_color(sc2,invisible);
  gw.set_visible(sc2,true);

  geo_redraw_crust crs;
  geo_scene sc1 = gw.new_scene( crs, crs, sc_input, string("Crust")); 
  gw.set_color(sc1,blue);
  gw.set_visible(sc1,true);
    
  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

