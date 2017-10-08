/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_minkowski2.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/plane_alg.h>
#include <LEDA/system/misc.h>

using namespace leda;


#define POLYGON     rat_polygon
#define GEN_POLYGON rat_gen_polygon
#define SEGMENT     rat_segment
#define TRIANGLE    rat_triangle
#define POINT       rat_point
#define VECTOR      rat_vector
#define NB          rational

/*
#define POLYGON     polygon
#define GEN_POLYGON gen_polygon
#define SEGMENT     segment
#define TRIANGLE    triangle
#define POINT       point
#define VECTOR      vector
#define NB          double
*/

window* win;

LEDA_BEGIN_NAMESPACE
void rat_segment_points(list<rat_point>&, rat_segment, int);
void segment_points(list<point>&, segment, int);
LEDA_END_NAMESPACE

bool anim_val=false;

void animation(window* w, const POLYGON& rpoly, const POLYGON& act, const GEN_POLYGON& gp )
{
 char* prect;
 win->start_buffering();
 color cold = win->set_color(blue2); 
 win->clear();
 (*win) << gp; 
 win->set_color(black);
 (*win) << act; 
 win->set_color(cold);
 win->stop_buffering(prect);

 list<SEGMENT> LSeg = gp.segments();
 list<point> pos;
 list<point> act_pts;
 point piter;
 
 SEGMENT iter;
 forall(iter,LSeg){
  act_pts.clear();
  segment_points(act_pts, iter.to_float(), 5);
  forall(piter, act_pts) pos.append(piter);
 }
 win->put_pixrect(prect);
 
 polygon rpoly_fl = rpoly.to_float();
 list<polygon> polys;
 list<point> ref_pt;
 forall(piter,pos) {
   polys.append(rpoly_fl.translate(piter.xcoord(),piter.ycoord()));
   ref_pt.append(piter);
 }
 polygon poly_iter;
 
 list_item it = ref_pt.first();
  
 win->start_buffering();
 forall(poly_iter,polys){
  win->put_pixrect(prect);
  (*win) << ref_pt[it]; 
  it=ref_pt.cyclic_succ(it);
  (*win) << poly_iter;
  win->flush_buffer();
  leda_wait(0.03);
 }
 win->stop_buffering(prect); 
}

geo_scene obstacle_scene;

static void ms(const list<POLYGON>& Lin, list<GEN_POLYGON>& Lout)
{ 
  GeoWin* gw = GeoWin::get_call_geowin();
  list<POLYGON> obstacle_list;
  gw->get_objects(obstacle_scene,obstacle_list);

  Lout.clear();
  if( Lin.length() < 1 ) return;
  POLYGON p1=Lin.head();  // the robot ...
  POLYGON pact;

  
  forall(pact,obstacle_list){
   GEN_POLYGON result = MINKOWSKI_DIFF(pact,p1);   
   Lout.append(result);
   if (anim_val) animation(win, p1, pact, result);
  }
}

int main()
{
  GeoWin GW("Minkowski differences of polygons");
  
  GW.set_show_grid(true);
  GW.set_grid_dist(10);
  
  win = &(GW.get_window());
  
  list<POLYGON> L;
  list<POLYGON> L2;

  geo_scene robot_scene = GW.new_scene(L); 
  GW.set_fill_color(robot_scene, invisible);
  GW.set_visible(robot_scene,true);
  GW.set_name(robot_scene, "Roboter");
  
  obstacle_scene = GW.new_scene(L2); 
  GW.set_fill_color(obstacle_scene, invisible);
  GW.set_visible(obstacle_scene,true);  
  GW.set_name(obstacle_scene, "Obstacles");  
  
  geowin_update<list<POLYGON>, list<GEN_POLYGON> > up(ms);   

  geo_scene mink =  GW.new_scene(up, robot_scene, "Minkowski differences of polygons");
  GW.set_color(mink, blue2 );
  GW.set_fill_color(mink, blue);
  GW.set_visible(mink, true);

  GW.add_dependence(obstacle_scene,mink);

  GW.init_menu();
  GW.get_window().bool_item(" Animation ?",anim_val);

  GW.edit(robot_scene);

  return 0;
}
