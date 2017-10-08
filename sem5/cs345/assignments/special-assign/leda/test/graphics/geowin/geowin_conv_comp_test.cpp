/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_conv_comp_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>

#include <LEDA/rat_geo_alg.h>
#include <LEDA/float_geo_alg.h>
#include <LEDA/stack.h>
#include <LEDA/map.h>

using namespace leda;

/*
typedef rat_point     POINT;
typedef rat_segment   SEGMENT;
typedef rat_polygon   POLYGON;
*/


typedef point     POINT;
typedef segment   SEGMENT;
typedef polygon   POLYGON;



void constr_poly(const list<POINT>& L, list<POLYGON>& P)
{ 
  P.clear();
  if (L.length() > 1) P.append(POLYGON(L,POLYGON::NO_CHECK)); 
}


class convex_components : public geowin_update<list<POINT>, list<SEGMENT> >, public geowin_redraw
{
 list<POLYGON> PL;
 POLYGON P;
 list<SEGMENT> SEG;
 SEGMENT hullseg;
 GRAPH<POINT,SEGMENT> GH;

 void draw(window& W,color col1, color col2, double,double,double,double)
 {
   color cold = W.set_fill_color(blue2);
   forall(P,PL) W << P;
   edge ei;
   forall_edges(ei,GH) W << SEGMENT(GH[source(ei)],GH[target(ei)]);
   
   color cold2 = W.set_color(red);
   W << hullseg;
   SEGMENT Siter;
   W.set_color(green);
   forall(Siter,SEG) { W << Siter; }
   W.set_color(cold2);
   W.set_fill_color(cold);
 }

 void update(const list<POINT>& L, list<SEGMENT>& S)
 { S.clear();
   PL.clear();
   SEG.clear();
   GH.clear();
   constr_poly(L,PL);
  
   forall(P,PL)
   { if (!P.is_simple()) continue;
    GRAPH<POINT,SEGMENT> G;
    list<edge> in;
    edge ehull = CONVEX_COMPONENTS(P.vertices(),G,in);
    edge e;
    forall(e,in) S.append(SEGMENT(G[source(e)],G[target(e)]));
    hullseg = SEGMENT(G[source(ehull)],G[target(ehull)]);
    GH = G;
   } 
   SEG = S;
 }
};

int main()
{
  GeoWin GW("Convex components of polygons");
 
  // Inputscene
  list<POINT> L;
  geo_scene sc_input =  geowin_new_scene(GW,L); 
  GW.set_color(sc_input,black);
  GW.set_fill_color(sc_input,yellow);
  GW.set_point_style(sc_input,circle_point);
  
  convex_components convcomp;

  geo_scene sc_cc = geowin_new_scene(GW,convcomp,convcomp,sc_input,"Convex components"); 
  GW.set_color(sc_cc,red);
  GW.set_line_width(sc_cc,2);
  GW.set_all_visible(true);
  
  GW.edit(sc_input);
  
  return 0;  
}
