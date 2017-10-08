/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_pset.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/geowin.h>
#include<LEDA/rat_geo_alg.h>
#include<LEDA/rat_point_set.h>
#include<LEDA/bitmaps/button21.h>

using namespace leda;

typedef rat_point   POINT;
typedef rat_segment SEGMENT;
typedef rat_circle  CIRCLE;


class geo_delau : public geowin_redraw, 
                  public geowin_update<list<POINT>, list<POINT> >
{
  rat_point_set S;
  
  void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  { edge e;
    forall_edges(e,S)
    { node v = S.source(e);
      node w = S.target(e);
      edge r = S.reversal(e);
      if (index(v) > index(w)) continue;
      if (S.is_hull_edge(e) || S.is_hull_edge(r)) 
         W.set_color(blue);
      else if (S.is_diagram_edge(e)) 
             W.set_color(red);
           else W.set_color(green);

      W << SEGMENT(S.pos(v), S.pos(w));
     }
   }

  void update(const list<POINT>& L, list<POINT>&)
  { S = rat_point_set(L); }
  
  bool insert(const POINT& new_obj)
  { S.insert(new_obj); return true; }
  
  bool del(const POINT& del_obj)
  { S.del(del_obj); return true; }

  bool change(const POINT& old_obj, const POINT& new_obj)
  { S.del(old_obj); 
    S.insert(new_obj);
    return true;
  }
};


geo_delau Delau;


int main()
{
  GeoWin gw("Point Sets");

  list<POINT> L;
  
  geo_scene sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);


  geo_scene sc1 = gw.new_scene( Delau, Delau, sc_input,"DT"); 
  gw.set_color(sc1,blue);
  gw.set_all_visible(true);
 

  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

