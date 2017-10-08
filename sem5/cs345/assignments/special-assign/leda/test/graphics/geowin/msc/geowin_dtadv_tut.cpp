/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_dtadv_tut.c
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

using namespace leda;

typedef rat_point   POINT;
typedef rat_segment SEGMENT;

window* out_win;

void diagramm(const POINT& p1,const POINT& p2)
{  out_win->set_color(red);
  (*out_win) << SEGMENT(p1,p2);
}

void triang(const POINT& p1,const POINT& p2)
{  out_win->set_color(green);
  (*out_win) << SEGMENT(p1,p2);
}

void hull(const POINT& p1,const POINT& p2)
{  out_win->set_color(blue);
  (*out_win) << SEGMENT(p1,p2);
}

class geo_delau : public geo_redraw_base, public geo_update_base<list<POINT>, list<POINT> >
{
public:
  rat_point_set S;

  virtual ~geo_delau() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  { out_win= &W; S.draw_edges(&diagramm, &triang, &hull); }

  virtual void update(const list<POINT>& L, list<POINT>&)
  {  S= rat_point_set(L);  }

};

geo_delau Delau;

class geo_locate :  public geo_redraw_base, public geo_update_base<list<POINT>, list<SEGMENT> >
{
public:
  list<SEGMENT>  nearest;

  virtual ~geo_locate() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {
   SEGMENT s;
   forall(s, nearest) {
     W.draw_disc(s.target().to_point(),1,red);
     W.draw_edge_arrow(s.to_segment(),c1);
   }
  }

  virtual void update(const list<POINT>& L, list<SEGMENT>& sl)
  { 
    nearest.clear();

    POINT p;
    SEGMENT s;
    forall(p,L){
      node nd = Delau.S.nearest_neighbor(p);
      if (nd != NULL){
       nearest.append(SEGMENT(p,Delau.S.pos(nd)));
       sl.append(SEGMENT(p,Delau.S.pos(nd)));
      }
    }
  }
};

int main()
{
  GeoWin gw("Advanced Delaunay Triangulation Data Type");
  
  list<POINT> L;
  list<POINT> LOC_PT;

  geo_scene sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);

  geo_scene sc_loc   =  gw.new_scene(LOC_PT); 
  gw.set_color(sc_loc,black);

  geo_scene sc1 = gw.new_scene( Delau, Delau, sc_input,"DT"); 
  gw.set_color(sc1,blue);

  geo_locate LOC;
  geo_scene sc2 = gw.new_scene( LOC, LOC, sc_loc, "Nearest neighbor");
  gw.set_color(sc2,blue2);
  
  gw.set_all_visible(true);

  gw.add_dependence(sc_input,sc2);
  
  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

