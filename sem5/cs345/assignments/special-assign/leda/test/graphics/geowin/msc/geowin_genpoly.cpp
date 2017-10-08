/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_genpoly.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>
#include <LEDA/plane_alg.h>

using namespace leda;

static void f_unite( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  Lout.clear();   
  if( Lin.length() < 2 ) return;
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];

  rat_gen_polygon p3=p1.unite(p2);
  Lout.append(p3);
}

static void f_intersec( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  Lout.clear(); 
  if( Lin.length() < 2 ) return;
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];

  rat_gen_polygon p3=p1.intersection(p2);
  Lout.append(p3);
}

static void f_diff( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  Lout.clear(); 
  if( Lin.length() < 2 ) return;
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];

  rat_gen_polygon p3=p1.diff(p2);
  Lout.append(p3);
}

static void f_sym_diff( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  Lout.clear(); 
  if( Lin.length() < 2 ) return;
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];

  rat_gen_polygon p3=p1.sym_diff(p2);
  Lout.append(p3);
}

int main()
{
  list<rat_gen_polygon>* H1;

  GeoWin GW("Genpolydemo");
  
  list<rat_gen_polygon> L;

  geo_scene input =   GW.new_scene(L); 
  GW.set_fill_color(input,invisible);

  geo_scene output1=  GW.new_scene(f_unite,input, string("Union"),H1,H1);
  GW.set_fill_color( output1, green);

  geo_scene output2=  GW.new_scene(f_intersec,input, string("Intersection"),H1,H1);
  GW.set_fill_color( output2, blue);

  geo_scene output3=  GW.new_scene(&f_diff,input, string("Difference"),H1,H1);
  GW.set_fill_color( output3, violet);

  GW.set_all_visible(true);

  geo_scene output4=  GW.new_scene(&f_sym_diff,input, string("SymDiff"),H1,H1);  
  GW.set_fill_color( output4, red);
  
  GW.set_bg_pixmap("pool"); 

  GW.edit(input);

  return 0;
}
