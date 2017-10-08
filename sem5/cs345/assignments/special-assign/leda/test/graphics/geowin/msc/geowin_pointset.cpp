/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_pointset.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/geowin.h>
#include<LEDA/geowin_additional.h>

#include<LEDA/rat_geo_alg.h>
#include<LEDA/rat_point_set.h>

using namespace leda;

typedef rat_point   POINT;
typedef rat_segment SEGMENT;
typedef rat_circle  CIRCLE;

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
    if (updateflag)  S= rat_point_set(L); 
    else updateflag=true;
  }

};

geo_delau Delau;

class geo_rangesearch : public geo_redraw_base, public geo_update_base<list<CIRCLE>, list<POINT> >
{
public:
  list<POINT> found;
  
  virtual ~geo_rangesearch() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {
    POINT p;
    forall(p,found){ W.draw_disc(p.to_point(),3,c1); }
  }
  
  virtual void update(const list<CIRCLE>& L, list<POINT>&)
  {
    found.clear();
    CIRCLE c;
    forall(c,L){
      list<node> nl = Delau.S.range_search(c);
      node nd;
      forall(nd,nl){
        found.append(Delau.S.pos(nd));
      }
    }
  }
};

class geo_locate :  public geo_redraw_base, public geo_update_base<list<POINT>, list<POINT> >
{
public:
  list<SEGMENT>  nearest;
  list<POINT>    p1_list;
  list<POINT>    p2_list;
  list<POINT>    p3_list;

  virtual ~geo_locate() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {
   POINT p1,p2,p3;
   list_item l2=p2_list.first(), l3= p3_list.first();
   forall(p1,p1_list){
        p2 = p2_list[l2]; p3=p3_list[l3];
        W.draw_filled_triangle(p1.to_point(),p2.to_point(),p3.to_point(),green);
        W.draw_triangle(p1.to_point(),p2.to_point(),p3.to_point(),black);
        l2=p2_list.succ(l2); l3=p3_list.succ(l3);
   }

   SEGMENT s;
   forall(s, nearest) {
     W.draw_disc(s.target().to_point(),3,red);
     W.draw_edge_arrow(s.to_segment(),c1);
   }
  }

  virtual void update(const list<POINT>& L, list<POINT>&)
  { 
    nearest.clear();
    edge e,e2;
    p1_list.clear(); p2_list.clear(); p3_list.clear();
    POINT p;
    SEGMENT s;
    forall(p,L){
      node nd = Delau.S.nearest_neighbor(p);
      if (nd != NULL){
       nearest.append(SEGMENT(p,Delau.S.pos(nd)));
      }
      e=Delau.S.locate(p);
      if (e!=NULL && Delau.S.is_diagram_edge(e)  && !Delau.S.is_hull_edge(e)){
       s=Delau.S.seg(e);
       p1_list.append(s.source()); p2_list.append(s.target());
       e2 = Delau.S.face_cycle_succ(e);
       if (e2 !=NULL) p3_list.append(Delau.S.seg(e2).target());
      
      }
    }
  } 
};

bool addhandle(GeoWin& gw, const POINT& rp)
{
 Delau.S.insert(rp);
 Delau.updateflag=false;
 return true;
}

int main()
{
  //#ifndef INIT_GEOWIN_LEDA_DEFAULT_TYPES
  //init_default_type((list<POINT>*)0,   string("RationalPoints"));
  //#endif

  GeoWin gw("Advanced Delaunay Triangulation Data Type");
  
  list<POINT> L;
  list<POINT> LOC_PT;
  list<CIRCLE> LCIRC;
  
  GeoEditScene<list<POINT> >* sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);

  geo_scene sc_loc   =  gw.new_scene(LOC_PT); 
  gw.set_color(sc_loc,black);
  
  geo_scene sc_circles = gw.new_scene(LCIRC);
  gw.set_color(sc_circles,blue2);
  gw.set_fill_color(sc_circles,invisible);

  geo_scene sc1 = gw.new_scene( Delau, Delau, sc_input,"DT"); 
  gw.set_color(sc1,blue);

  gw.set_pre_add_handler(sc_input,addhandle);

  geo_locate LOC;
  geo_scene sc2 = gw.new_scene( LOC, LOC, sc_loc, "Nearest neighbor");
  gw.set_color(sc2,blue2);
  
  geo_rangesearch RS;
  geo_scene sc3 = gw.new_scene( RS, RS, sc_circles, "Range search");
  gw.set_color(sc3,blue); 
 
  gw.set_all_visible(true);
 
  gw.add_dependence(sc_input,sc2);

  string msg1("Activate other input scenes for demonstration of");
  string msg2("nearest neighbor algorithm and Range search. Use"); 
  string msg3("Scenes-activate menu button for activating a scene.");

  gw.msg_open(msg1); gw.msg_open(msg2); gw.msg_open(msg3);
  
  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

