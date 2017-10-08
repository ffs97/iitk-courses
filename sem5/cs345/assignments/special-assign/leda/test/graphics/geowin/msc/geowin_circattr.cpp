/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_circattr.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define NO_FLOAT_ALGORITHMS

#include<LEDA/rat_window.h>
#include <LEDA/geowin.h>
#include <LEDA/rat_geo_alg.h>

using namespace leda;

geo_scene sec,aec;

static void sec_help(const list<rat_point>& L, list<rat_circle>& C)
{
  C.clear();
  if (! L.empty()) {
   rat_circle cr=SMALLEST_ENCLOSING_CIRCLE(L);
   C.append(cr);
  } 
}

static void aec_help(const list<rat_point>& L, list<rat_circle>& C)
{
  C.clear();
  if (! L.empty()) {
   ALL_EMPTY_CIRCLES(L,C);

    // now let us set some attributes:
    list_item it;
    GeoBaseScene<list<rat_circle> > *output = ( GeoBaseScene<list<rat_circle> > *)aec;
    int cw=0;
    forall_items(it,C) { 
      rat_circle& rc = C[it]; 
      //output->set_obj_filled((void*)(&rc),true);
      output->set_obj_fill_color((void*)(&rc),color(cw % 15)); 
      cw++;
    }
  } 
}

int main()
{
  list<rat_point>* H1;
  list<rat_circle>* H2;

  GeoWin GW( "Circle Attr. Test");
  
  // Inputscene
  list<rat_point> L;
  geo_scene input =  GW.new_scene(L); 
  GW.set_color( input, red );
  GW.set_color2( input, red ); 
  GW.set_visible( input, true);

  aec = GW.new_scene( aec_help, input, string("All empty circles"),H1,H2); 
  GW.set_visible(aec, true);
  
  sec=GW.new_scene( sec_help, input, string("Smallest enclosing circle"),H1,H2);
  GW.set_color( sec, blue );
  GW.set_color2( sec, blue ); 
  GW.set_fill_color(sec,invisible);
  GW.set_visible( sec, true);

  while( GW.edit(input) );
  
  return 0;  
}
