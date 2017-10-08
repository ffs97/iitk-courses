/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_minkowski_new.c
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


void ms(const list<POLYGON>& Lrobo, const list<POLYGON>& obstacle_list, list<GEN_POLYGON>& Lout)
{ 
  Lout.clear();
  if( Lrobo.length() < 1 ) return;
  POLYGON p1=Lrobo.head();  // the robot ...
  POLYGON pact;
  
  forall(pact,obstacle_list){
   GEN_POLYGON result = MINKOWSKI_DIFF(pact,p1);   
   Lout.append(result);
  }
}

int main()
{
  GeoWin GW("Minkowski differences of polygons");
  
  GW.set_show_grid(true);
  GW.set_grid_dist(10);
  
  list<POLYGON> L;
  list<POLYGON> L2;

  geo_scene robot_scene = GW.new_scene(L,"Robot"); 
  GW.set_fill_color(robot_scene, invisible);
  GW.set_visible(robot_scene,true);
  GW.set_description(robot_scene, "This scene holds one robot!\n");
  
  geo_scene obstacle_scene = GW.new_scene(L2,"Obstacles"); 
  GW.set_fill_color(obstacle_scene, invisible);
  GW.set_visible(obstacle_scene,true);  
  GW.set_description(obstacle_scene, "This scene holds some obstacles !\n");  
  
  geowin_update<list<POLYGON>, list<GEN_POLYGON> > up(ms,0);   
  list<geo_scene> influencers;
  influencers.append(robot_scene);
  influencers.append(obstacle_scene);

  geo_scene mink =  GW.new_scene(up, influencers, "Minkowski differences of polygons");
  GW.set_color(mink, blue2 );
  GW.set_fill_color(mink, blue);
  GW.set_visible(mink, true);

  GW.edit(robot_scene);

  return 0;
}
