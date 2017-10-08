/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_minkowski_anim.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>
#include <LEDA/misc.h>

using namespace leda;

#include "minkowski.c"

window* win;

void rat_segment_points(list<rat_point>&, rat_segment, int);
void segment_points(list<point>&, segment, int);


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


static void ms( const list<POLYGON>& Lin, list<GEN_POLYGON>& Lout)
{ 
  Lout.clear();
  if( Lin.length() < 2 ) return;
  POLYGON p1=Lin[Lin[0]];
  POLYGON pact;

  int sz = Lin.length(),i;
  
  for(i=1;i<sz;i++){
   GEN_POLYGON result;
   pact = Lin[Lin[i]];
 
   //minkowski_sum(pact,p1,result);
   minkowski_difference(pact,p1,result);
   
   Lout.append(result);
   
   animation(win, p1, pact, result);
  }
}

int main()
{
  GeoWin GW("Minkowski differences of polygons");
  
  GW.set_show_grid(true);
  GW.set_grid_dist(10);
  
  win = &(GW.get_window());
  
  list<POLYGON> L;

  geo_scene input = GW.new_scene(L); 
  GW.set_fill_color(input, invisible);
  GW.set_visible(input,true);
  
  geowin_update<list<POLYGON>, list<GEN_POLYGON> > up(ms);   

  geo_scene output2=  GW.new_scene(up, input, "Minkowski differences of polygons");
  GW.set_color( output2, blue2 );
  GW.set_fill_color( output2, blue);
  GW.set_visible(output2, true);

  GW.edit(input);

  return 0;
}
