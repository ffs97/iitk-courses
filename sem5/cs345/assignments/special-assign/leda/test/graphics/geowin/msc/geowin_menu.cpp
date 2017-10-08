/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_menu.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>
#include <LEDA/rat_geo_alg.h>

using namespace leda;

static void SWEEPSEGMENTS(const list<rat_segment>& L, 
			  GRAPH<rat_point,rat_segment>& G,  bool b)
{
  SWEEP_SEGMENTS(L, G, b, false);
}

// SWEEP, MULMULEY
typedef void (*INTER1)(const list<rat_segment>&, 
		       GRAPH<rat_point,rat_segment>&, bool);

void geo_call(GeoWin& gw, INTER1 fkt,  string label) 
{
  list<rat_segment> input;
  GRAPH<rat_point, rat_segment> G;
  bool embed = false;
  gw.get_objects(input);
  float t = used_time();
  fkt(input, G, embed);
  float T = used_time(t);
  string msg("%s    : %4d nodes  %4d edges   %6.2f s", label,
	     G.number_of_nodes(), G.number_of_edges(), T);
  gw.msg_open(msg);
}

int main()
{
  GeoWin GW("SEGMENT INTERSECTION");
  list<rat_segment> LRS;

  geowin_add_user_call(GW, "SWEEP",    INTER1(&SWEEPSEGMENTS));
  geowin_add_user_call(GW, "MULMULEY", INTER1(&MULMULEY_SEGMENTS));

  GW.new_scene(LRS);
  GW.edit();
  
  return 0;
}
