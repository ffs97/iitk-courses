/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_rect_diff.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin_init.h>

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

#if defined(LEDA_STD_IO_HEADERS)
using std::ostream;
using std::istream;
using std::cout;
using std::endl;
#endif

#define RECTANGLE rectangle
//#define RECTANGLE rat_rectangle

class rect_diff : public geowin_update<list<RECTANGLE>, list<RECTANGLE> >
{
 public:
 
 void update(const list<RECTANGLE>& inp, list<RECTANGLE>& diff)
 {
   diff.clear();
   if (inp.size() != 2) return;
   RECTANGLE r1 = inp.head();
   RECTANGLE r2 = inp[inp.get_item(1)];
   
   r1.difference(r2,diff);
   
   bool res = r1.do_intersect(r2);
   
   if (res) cout << "intersection!\n";
   else cout << "no intersection!\n";
 }
};

int main()
{
 geowin_init_default_type((list<RECTANGLE>*)0,string("Rectangles"));

 GeoWin gw;

 list<RECTANGLE> rcts;
 
 geo_scene sc1 = gw.new_scene(rcts);
 gw.set_fill_color(sc1, invisible);
 
 rect_diff rd;
 geo_scene res = gw.new_scene(rd, sc1, "Difference");
 gw.set_color(res,black);
 gw.set_fill_color(res,green);
 
 gw.set_all_visible(true);
 
 gw.edit(sc1);

 return 0;
}
