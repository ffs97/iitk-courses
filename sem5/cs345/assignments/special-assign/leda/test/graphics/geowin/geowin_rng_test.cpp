/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_rng_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/geowin.h>
#include<LEDA/geowin_init.h>
#include<LEDA/rat_geo_alg.h>
#include<LEDA/float_geo_alg.h>
#include<LEDA/rat_point_set.h>
#include<LEDA/point_set.h>

using namespace leda;

/*
typedef rat_point     POINT;
typedef rat_segment   SEGMENT;
typedef rat_circle    CIRCLE;
typedef rat_point_set POINTSET;
*/


typedef point     POINT;
typedef segment   SEGMENT;
typedef circle    CIRCLE;
typedef point_set POINTSET;


window* out_win;
bool bval;

void diagramm(const POINT& p1,const POINT& p2)
{  out_win->set_color(red);(*out_win) << SEGMENT(p1,p2);}

void triang(const POINT& p1,const POINT& p2)
{  out_win->set_color(green);(*out_win) << SEGMENT(p1,p2);}

void hull(const POINT& p1,const POINT& p2)
{  out_win->set_color(blue);(*out_win) << SEGMENT(p1,p2);}


class geo_delau : public geowin_redraw, public geowin_update<list<POINT>, list<POINT> >
{
public:
  POINTSET S;
  list<edge> RNG;
  bool flag;
  
  geo_delau(bool b) { flag = b;} 
  
  virtual ~geo_delau() {}
  
  void draw_rng(window& W)
  {
    color cold = W.set_color(green);
    int lwold = W.set_line_width(2);
    edge e;
    forall(e,RNG) {
      segment seg(S[S.source(e)].to_float(), S[S.target(e)].to_float());
      W << seg;
      if (bval) { // output lunes (forbidden regions) ...
        W.set_color(black); W.set_line_width(1);
        point p1 = seg.source(), p2 = seg.target();
	vector v = p2-p1, vh = v/2;
	vector v2 = v.rotate90() * 0.866;
	vector v3 = -v2;
	point c1 = p1 + vh + v2;
	point c2 = p1 + vh + v3;
	W.draw_arc(c2,p1,c1); W.draw_arc(c2,p2,c1);
	W.set_color(green); W.set_line_width(2);
      }
    }
    
    W.set_line_width(lwold);
    W.set_color(cold);
  }

  void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {
    out_win= &W;
    
    if (! flag) S.draw_edges(&diagramm, &triang, &hull);
    else draw_rng(W);
  }

  void update(const list<POINT>& L, list<POINT>&)
  { S= POINTSET(L); 
    RNG.clear();
    if (flag) RNG = S.relative_neighborhood_graph();
  }
  
};

geo_delau Delau(false);
geo_delau RelNBG(true);

int main()
{
  GeoWin gw("Relative neighborhood graph demo");
  
  list<POINT> L;
  
  GeoEditScene<list<POINT> >* sc_input =  geowin_new_scene(gw,L); 
  gw.set_color(sc_input,black);
  gw.set_point_style(sc_input,disc_point);

  geo_scene sc1 = geowin_new_scene(gw, Delau, Delau, sc_input,"DT"); 
  gw.set_color(sc1,blue);
  
  geo_scene sc2 = geowin_new_scene(gw, RelNBG, RelNBG, sc_input,"RNG"); 
  gw.set_color(sc1,blue);  
  
  gw.set_all_visible(true);
  
  gw.init_menu();
  gw.get_window().bool_item(" Show forbidden regions for RNG:",bval);

  gw.edit(sc_input);
  
  return 0;
}

