/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_d3.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/geowin_init_d3.h>
#include <LEDA/graphics/geowin_init.h>

using namespace leda;

int main() 
{
  //geowin_init_default_type((list<d3_segment>*)NULL, string("D3-Segments"), d3_segments_d3);
  //geowin_init_default_type((list<d3_line>*)NULL, string("D3-Line"), d3_lines_d3);
  //geowin_init_default_type((list<d3_ray>*)NULL, string("D3-Ray"), d3_rays_d3);
  geowin_init_default_type((list<d3_simplex>*)NULL, string("D3-Simplex"), d3_simplex_d3);
  geowin_init_default_type((list<d3_sphere>*)NULL, string("D3-Sphere"), d3_spheres_d3);  

  //geowin_init_default_type((list<d3_rat_segment>*)NULL, string("D3-rat-Segments"), d3_rat_segments_d3);
  //geowin_init_default_type((list<d3_rat_line>*)NULL, string("D3-rat-Line"), d3_rat_lines_d3);
  //geowin_init_default_type((list<d3_rat_ray>*)NULL, string("D3-rat-Ray"), d3_rat_rays_d3);
  geowin_init_default_type((list<d3_rat_simplex>*)NULL, string("D3-rat-Simplex"), d3_rat_simplex_d3);
  geowin_init_default_type((list<d3_rat_sphere>*)NULL, string("D3-rat-Sphere"), d3_rat_spheres_d3); 
    
  GeoWin gw;
  gw.edit();
  return 0;
}

