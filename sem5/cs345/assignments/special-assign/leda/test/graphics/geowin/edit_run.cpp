/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edit_run.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/geo_alg.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif

int main()
{
  GeoWin gw;

  list<point> L;
  GeoEditScene<list<point> >* sc =  gw.new_scene(L);       
  gw.set_color(sc,black);

  while (gw.edit(sc))
  { point a,b;
    double dist = CLOSEST_PAIR(L,a,b);
/*    
    list_item it;
    void* adr_a, *adr_b;
    forall_items(it,L){
     if (a == L[it]) { point& p= L[it]; adr_a=(void*)(&p); }
     if (b == L[it]) { point& p= L[it]; adr_b=(void*)(&p); }
    }
*/    
    gw.reset_obj_attributes(sc);
    //gw.set_obj_color(sc,adr_a,red); gw.set_obj_color(sc,adr_b,red);
    gw.set_obj_color(sc,a,red); gw.set_obj_color(sc,b,red);
    
    gw.message(string("squared distance: %f",dist));
    gw.redraw();
  }

  return 0; 
}
