/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_circles2.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//#define GEOWIN_USE_NAMESPACE

#define NO_FLOAT_ALGORITHMS

#include<LEDA/rat_window.h>
#include <LEDA/geowin.h>
#include <LEDA/geowin_additional.h>

#include <LEDA/rat_geo_alg.h>

using namespace leda;

//using namespace GEOWIN;


static void conv_hull(const list<rat_point>& L, list<rat_polygon>& P)
{
  P.clear();
  list<rat_point> ch = CONVEX_HULL(L);
  if( ch.length() > 2 ) P.append(rat_polygon(ch));
}

static void lec_help(const list<rat_point>& L, list<rat_circle>& C)
{
  C.clear();
  if (! L.empty()) {
   rat_circle cr=LARGEST_EMPTY_CIRCLE(L);
   C.append(cr); 
  }
}

static void sec_help(const list<rat_point>& L, list<rat_circle>& C)
{
  C.clear();
  if (! L.empty()) {
   rat_circle cr=SMALLEST_ENCLOSING_CIRCLE(L);
   C.append(cr);
  } 
}

int main()
{
  //#ifndef INIT_GEOWIN_LEDA_DEFAULT_TYPES
  //init_default_type((list<rat_point>*)0,   string("RationalPoints"));
  //#endif

  GeoWin GW( "Circledemo");
  
  // Inputscene
  list<rat_point> L;
  geo_scene input =  geowin_new_scene(GW,L); 
  GW.set_color( input, red );
  GW.set_color2( input, red ); 
  
  geo_scene hull,sec,aec,lec;

  list<rat_point>* H1;
  list<rat_polygon>* H2;
  list<rat_circle>* H3;

  hull= geowin_new_scene( GW,conv_hull, input, string("Convex Hull"),H1,H2 );
  GW.set_color(hull,yellow);
  GW.set_color2(hull,yellow); 

  aec = geowin_new_scene(GW,ALL_EMPTY_CIRCLES, input, string("All empty circles"),H1,H3); 
  GW.set_color( aec, grey1 );
  GW.set_color2( aec, grey1 ); 
  GW.set_fill_color(aec,invisible);

  sec= geowin_new_scene( GW, sec_help, input, "Smallest enclosing circle",H1,H3);
  GW.set_color( sec, blue );
  GW.set_color2( sec, blue ); 
  GW.set_fill_color(sec,invisible);
  
  GW.set_all_visible(true);

  lec= geowin_new_scene( GW, lec_help, input, "Largest empty circle",H1,H3);
  GW.set_color( lec, blue2 );
  GW.set_color2( lec, blue2 ); 
  GW.set_fill_color(lec,invisible);

  
  while( GW.edit(input) );
  
  return 0;  
}
