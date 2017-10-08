/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>
#include <LEDA/geowin_additional.h>

using namespace leda;

int main()
{
/*
#ifndef INIT_GEOWIN_LEDA_DEFAULT_TYPES
  init_default_type((list<point>*)0,       string("Points"));
  init_default_type((list<segment>*)0,     string("Segments"));
  init_default_type((list<ray>*)0,         string("Rays"));
  init_default_type((list<line>*)0,        string("Lines"));
  init_default_type((list<circle>*)0,      string("Circles"));
  set_fill_fcn((list<circle>*)0);
  init_default_type((list<polygon>*)0,     string("SimplePolygons"));
  set_fill_fcn((list<polygon>*)0);
  init_default_type((list<gen_polygon>*)0, string("GeneralizedPolygons"));
  set_fill_fcn((list<gen_polygon>*)0);
  init_default_type((list<d3_point>*)0,    string("D3-Points"));
#endif
*/
 
  GeoWin gw;

  gw.edit();

  return 0;
}
