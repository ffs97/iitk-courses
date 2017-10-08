/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_dthandler.c
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
  bool updateflag;

  geo_delau() { updateflag=true; }

  virtual ~geo_delau() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {
    out_win= &W;
    S.draw_edges(&diagramm, &triang, &hull);
  }

  virtual void update(const list<POINT>& L, list<POINT>&)
  { 
    if (updateflag)  { cout << "update calculation!\n"; S= rat_point_set(L); }
    else updateflag=true;
  }

};

geo_delau Delau;

bool addhandle_pre(GeoWin& gw, const POINT& rp)
{
 Delau.S.insert(rp);
 Delau.updateflag=false;
 return true;
}

bool delhandle_pre(GeoWin& gw, const POINT& rp)
{
 Delau.S.del(rp);
 Delau.updateflag=false;
 return true;
}

void delhandle_post(GeoWin& gw, const POINT& rp)
{
 cout << rp << "was deleted\n";
}

bool changehandle_pre(GeoWin& gw, const POINT& rp1, const POINT& rp2)
{
 Delau.S.del(rp1);
 Delau.S.insert(rp2);
 Delau.updateflag=false;
 return true;
}

int main()
{
  GeoWin gw("Advanced Delaunay Triangulation Data Type");
  
  list<POINT> L;
  GeoEditScene<list<POINT> >* sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);

  geo_scene sc1 = gw.new_scene( Delau, Delau, sc_input,"DT"); 
  gw.set_color(sc1,blue);
  gw.set_visible(sc1,true);
  
  gw.set_pre_add_handler(sc_input, addhandle_pre);
  gw.set_pre_del_handler(sc_input, delhandle_pre);
  gw.set_post_del_handler(sc_input, delhandle_post);
  gw.set_pre_change_handler(sc_input, changehandle_pre);  
  
  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

