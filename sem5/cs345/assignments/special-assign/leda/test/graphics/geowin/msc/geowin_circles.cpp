/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_circles.c
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

class geo_annulus : public geo_redraw_base, public geo_update_base<list<rat_point>, list<rat_point> >
{
public:
  rat_point center1;
  rat_point ip1;
  rat_point op1;

  virtual ~geo_annulus() {}

  virtual void draw(window& W,color col1,color col2,double x1,double y1,double x2,double y2)
  {
   circle c1(center1.to_point(),ip1.to_point());
   circle c2(center1.to_point(),op1.to_point());
   W.draw_disc(c2,cyan);
   W.draw_disc(c1,white);
   W.draw_circle(c1,black);
   W.draw_circle(c2,black);
  }

  virtual void update(const list<rat_point>& L, list<rat_point>&)
  { 
      MIN_AREA_ANNULUS(L,center1,ip1,op1);
  }

};

int main()
{
  list<rat_point>* H1;
  list<rat_circle>* H2;
  list<rat_polygon>* H3;

  //#ifndef INIT_GEOWIN_LEDA_DEFAULT_TYPES
  //init_default_type((list<rat_point>*)0,   string("RationalPoints"));
  //#endif

  GeoWin GW( "Circledemo");

  // Inputscene
  list<rat_point> L;
  geo_scene input =  GW.new_scene(L); 
  GW.set_color( input, red );
  GW.set_color2( input, red ); 
  
  geo_scene hull,sec,aec,lec,asc;
  
  geo_annulus Ann;
  asc=GW.new_scene( Ann, Ann, input, string("min area annulus"));

  hull=GW.new_scene( conv_hull, input, string("Convex Hull"),H1,H3 );
  GW.set_color(hull,yellow);
  GW.set_color2(hull,yellow); 
 
  aec = GW.new_scene(ALL_EMPTY_CIRCLES, input, string("All empty circles"),H1,H2); 
  GW.set_color( aec, grey1 );
  GW.set_color2( aec, grey1 ); 
  GW.set_fill_color(aec,invisible);
  
  sec=GW.new_scene( sec_help, input, string("Smallest enclosing circle"),H1,H2);
  GW.set_color( sec, blue );
  GW.set_color2( sec, blue ); 
  GW.set_fill_color(sec,invisible);
  
  GW.set_all_visible(true);

  lec=GW.new_scene( lec_help, input, string("Largest empty circle"),H1,H2);
  GW.set_color( lec, blue2 );
  GW.set_color2( lec, blue2 ); 
  GW.set_fill_color(lec,invisible);

  
  while( GW.edit(input) );
  
  return 0;  
}
