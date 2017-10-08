/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_triang.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define NO_FLOAT_ALGORITHMS

#include<LEDA/rat_window.h>
#include <LEDA/geowin.h>
#include <LEDA/rat_geo_alg.h>

using namespace leda;

   
class geo_update_extend : public geo_update_base<list<rat_point>,list<rat_segment> >
{
public:
  virtual ~geo_update_extend() {}

  virtual void update(const list<rat_point>& L,list<rat_segment>& T)
  {
    T.clear();
    GRAPH<rat_point,int> G;
    TRIANGULATE_POINTS(L,G); 
    edge_array<bool> drawn(G,false);
    edge e;
    forall_edges(e,G)
    { if (drawn[e]) continue;
      T.append(rat_segment(G[source(e)],G[target(e)]));
      edge r = G.reversal(e);
      if (r) drawn[r] = true;
    } 
   }
};


static void conv_hull(const list<rat_point>& L, list<rat_polygon>& P)
{ P.clear();
  list<rat_point> ch = CONVEX_HULL(L);
  if( ch.length() > 2 ) P.append(rat_polygon(ch));
}

static void closestpair(const list<rat_point>& L2, list<rat_segment>& seg)
{
  if (L2.size() < 2) return;
  rat_point p1,p2;
  list<rat_point> L=L2;
  CLOSEST_PAIR(L, p1, p2);
  seg.clear();
  seg.append(rat_segment(p1,p2));
}

int main()
{
  list<rat_point>* H1;
  list<rat_segment>* H2;
  list<rat_polygon>* H3;

  GeoWin GW("Triangulation Demo");
  
  list<rat_point> L;

  L.append(rat_point(12,13));
  L.append(rat_point(20,20));

  geo_scene sc_input = GW.new_scene(L); 
  
  geo_scene sc0 = GW.new_scene( &conv_hull, sc_input, string("CONVEX HULL"),H1,H3);
  GW.set_color(sc0,grey1);

  geo_update_extend tryfu;
  geo_scene sc1 = GW.new_scene(tryfu, sc_input, string("TRIANGULATION"));
  GW.set_color(sc1,blue);
  GW.set_color2(sc1,blue);

  geo_scene sc2 = GW.new_scene(closestpair, sc_input, string("CLOSEST_PAIR"),H1,H2);
  GW.set_color(sc2,blue2);
  GW.set_color2(sc2,blue2);
  GW.set_line_width(sc2,5); 
  
  GW.set_all_visible(true);

  GW.edit(sc_input);
  
  return 0;
}

