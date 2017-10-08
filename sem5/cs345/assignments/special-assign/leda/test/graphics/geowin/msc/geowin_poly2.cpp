/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_poly2.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/geowin.h>
#include <LEDA/plane_alg.h>
#include <iostream.h>

using namespace leda;

int main()
{
  GeoWin GW("POLYGON Input window");
  GeoWin GW2("POLYGON Output window");
  
  list<rat_polygon> L, LRG;

  geo_scene input =  geowin_new_scene(GW,L); 
  GeoEditScene<list<rat_polygon> > *output = geowin_new_scene(GW2,LRG);

  GW2.display(); GW.display();

  while( GW.edit(input) )
  {
    rat_polygon rp;
    rat_gen_polygon rgp2;
    bool fl=true;

    forall(rp,L){
      if (fl) rgp2=rgp2.unite(rat_gen_polygon(rp));
      else rgp2=rgp2.intersection(rat_gen_polygon(rp));
      if (fl) fl=false;
    }
    LRG= rgp2.polygons();
    int cw=0;

    list_item it;
    forall_items(it,LRG) { rat_polygon& ra =LRG[it]; output->set_obj_fill_color((void*)(&ra),color(cw % 15)); cw++;}
    GW2.redraw();
  }

  return 0;
}
