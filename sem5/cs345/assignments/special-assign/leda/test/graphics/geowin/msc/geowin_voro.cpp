/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_voro.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//#define GEOWIN_USE_NAMESPACE

#include<LEDA/rat_window.h>
#include<LEDA/rat_geo_alg.h>
#include<LEDA/geowin.h>
#include<LEDA/geowin_additional.h>

using namespace leda;

//using namespace GEOWIN;

typedef rat_vector  VECTOR;
typedef rat_point   POINT;
typedef rat_line    LINE;
typedef rat_ray     RAY;
typedef rat_circle  CIRCLE;
typedef rat_polygon POLYGON;

#define SET_REDRAW(sc,func,T)\
  ((GeoBaseScene<list<T> >*)sc)->set_redraw_fcn(func)


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

void draw_graph_edges_2(window& W, const GRAPH<POINT,int>& G, color col)
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
    W.draw_circle(circle(G[v].to_point(),G[w].to_point()),red);
    W.draw_circle(circle(G[w].to_point(),G[v].to_point()),red);
    W.draw_segment(G[v].to_point(),G[w].to_point(),col);
  }  
}

 

void draw_voro_edges(window& W, const GRAPH<CIRCLE,POINT>& VD, color col)
{
  edge_array<bool> drawn(VD,false);

  edge e;
  forall_edges(e,VD) 
  { 
    if (drawn[e]) continue;

    drawn[VD.reversal(e)] = drawn[e] = true;

    node v = source(e);
    node w = target(e);

    if (VD.outdeg(v) == 1 && VD.outdeg(w) == 1)
      { LINE l = p_bisector(VD[v].point1(),VD[v].point3());
        W.draw_line(l.to_line(),col);
       }
    else
      if (VD.outdeg(v) == 1) 
         { POINT  cw  = VD[w].center();
           VECTOR vec = VD[v].point3() - VD[v].point1();
           POINT  cv  = cw + vec.rotate90();
           W.draw_ray(cw.to_point(),cv.to_point(),col);
          }
       else
         if (VD.outdeg(w) == 1) 
          { POINT  cv  = VD[v].center();
            VECTOR vec = VD[w].point3() - VD[w].point1();
            POINT  cw  = cv + vec.rotate90();
            W.draw_ray(cv.to_point(),cw.to_point(),col);
           }
         else 
          { POINT  cv  = VD[v].center();
            POINT  cw  = VD[w].center();
            W.draw_segment(cv.to_point(),cw.to_point(),col);
         }
            
  }  
}


GRAPH<CIRCLE,POINT> F_VD;
GRAPH<POINT,int>    DT;
GRAPH<POINT,int>    F_DT;
GRAPH<POINT,int>    MST;

class geo_redraw_vd : public geo_redraw_base, public geo_update_base<list<POINT>, list<POINT> >
{
public:
  GRAPH<CIRCLE,POINT> VD;

  virtual ~geo_redraw_vd() {}

  virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
  {  draw_voro_edges(W,VD,c1); }

  virtual void update(const list<POINT>& L, list<POINT>&)
  { 
    VORONOI(L,VD); 
  }

};

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



void f_voro_redraw(const list<POINT>&, window& W, color c1, color c2,
                                       double, double, double, double)
{ draw_voro_edges(W,F_VD,c1); }


void delau_redraw(const list<POINT>&, window& W, color c1, color c2,
                                      double, double, double, double)
{ draw_graph_edges_2(W,DT,c1); }


void f_delau_redraw(const list<POINT>&, window& W, color c1, color c2,
                                        double, double, double, double)
{ draw_graph_edges(W,F_DT,c1); }


void mst_redraw(const list<POINT>&, window& W, color c1, color c2,
                                    double, double, double, double)
{ draw_graph_edges(W,MST,c1); }



static void convex_hull(const list<POINT>& L, list<POLYGON>& P)
{ P.clear();
  list<POINT> ch = CONVEX_HULL(L);
  if( ch.length() > 2 ) P.append(POLYGON(ch));
}

static void min_spanning_tree(const list<POINT>& L, list<POINT>&)
{ MIN_SPANNING_TREE(L,MST); }

void f_voronoi(const list<POINT>& L, list<POINT>&)
{ F_VORONOI(L,F_VD); }

void delaunay_triang(const list<POINT>& L, list<POINT>&)
{ DELAUNAY_TRIANG(L,DT); }

void f_delaunay_triang(const list<POINT>& L, list<POINT>&)
{ F_DELAUNAY_TRIANG(L,F_DT); }



int main()
{
  list<POINT>* H1;
  list<POLYGON>* H2;

  //#ifndef INIT_GEOWIN_LEDA_DEFAULT_TYPES
  //init_default_type((list<POINT>*)0,   string("RationalPoints"));
  //#endif

  GeoWin gw("Voronoi Demo");
  
  list<POINT> L;
  geo_scene sc_input =  gw.new_scene(L); 
  gw.set_color(sc_input,black);
  gw.set_point_style(sc_input,disc_point);

  geo_redraw_vd vob;
  geo_scene sc1 = gw.new_scene( vob, vob, sc_input,"VORONOI"); 
  gw.set_color(sc1,blue);
  gw.set_visible(sc1,true);
  
  geo_scene sc2 = gw.new_scene(f_voronoi,        sc_input, "F_VORONOI",H1,H1);
  SET_REDRAW(sc2,f_voro_redraw,POINT);
  gw.set_color(sc2,green);

  geo_scene sc3 = gw.new_scene(delaunay_triang,  sc_input, "DELAUNAY",H1,H1);
  SET_REDRAW(sc3,delau_redraw,POINT);
  gw.set_color(sc3,orange);

  geo_scene sc4 = gw.new_scene(f_delaunay_triang,sc_input, "F_DELAUNAY",H1,H1);
  SET_REDRAW(sc4,f_delau_redraw,POINT);
  gw.set_color(sc4,red);

  geo_scene sc5 = gw.new_scene(min_spanning_tree,sc_input, "MINSPAN_TREE",H1,H1);
  SET_REDRAW(sc5,mst_redraw,POINT);
  gw.set_color(sc5,black);

  geo_scene sc6 = gw.new_scene(convex_hull,      sc_input, "CONVEX HULL",H1,H2);
  gw.set_color(sc6,grey1);

  geo_redraw_crust crs;
  geo_scene sc7 = gw.new_scene( crs, crs, sc_input,"Crust"); 
  gw.set_color(sc7,pink);
  
  gw.display(window::center,window::center);
  gw.edit(sc_input);
  
  return 0;
}

