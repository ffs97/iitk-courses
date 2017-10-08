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


#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/rat_geo_alg.h>

#if defined(__BORLANDC__)
#define NO_MEMBER_TEMPLATES
#endif

using namespace leda;

geo_scene sec;
GeoResultScene<list<rat_point>, list<rat_circle> >* aec;

GeoWin* GW_ptr;
  
void sec_help(const list<rat_point>& L, list<rat_circle>& C)
{
  C.clear();
  if (! L.empty()) {
   rat_circle cr=SMALLEST_ENCLOSING_CIRCLE(L);
   C.append(cr);
  } 
}

void aec_help(const list<rat_point>& L, list<rat_circle>& C)
{
  C.clear();
  if (! L.empty()) {
   ALL_EMPTY_CIRCLES(L,C);

    // now let us set some attributes:
    list_item it;
    int cw=0;
    forall_items(it,C) { 
      rat_circle& rc = C[it];
#if !defined(NO_MEMBER_TEMPLATES)
      GW_ptr->set_obj_fill_color(aec,(void*)(&rc),color(cw % 15));
      GW_ptr->set_obj_color(aec,(void*)(&rc),color(cw % 10)); 
      GW_ptr->set_obj_line_width(aec,(void*)(&rc),4);
#else
	  aec->set_obj_fill_color((void*)(&rc),color(cw % 15));
      aec->set_obj_color((void*)(&rc),color(cw % 10)); 
      aec->set_obj_line_width((void*)(&rc),4);
#endif
      cw++;
    }
  } 
}

int main()
{
  GeoWin GW("All empty circles - object attribute test");
  GW_ptr= &GW;
  
  list<rat_point> L;
  geo_scene input =  GW.new_scene(L); 
  GW.set_color( input, red );
  GW.set_point_style(input, disc_point);

  aec = GW.new_scene( aec_help, input, string("All empty circles")); 
  
  sec=GW.new_scene( sec_help, input, "Smallest enclosing circle");
  GW.set_color( sec, blue );
  GW.set_fill_color(sec,invisible);
  GW.set_line_width(sec, 3);
  
  GW.set_all_visible(true);

  GW.edit(input);
  
  return 0;  
}
