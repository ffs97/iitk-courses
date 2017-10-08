/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_input_object.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/geowin.h>

using namespace leda;

LEDA_BEGIN_NAMESPACE
void segment_points( list<point>& L, segment s, int n);
LEDA_END_NAMESPACE

class alt_input_rectangle : public GeoInputObject<point>
{
public:
 void operator()(GeoWin& gw, list<point>& LP)
 {
  window& w = gw.get_window();
  rectangle r;
  segment s;
  w >> r;
  list<point> L1,L2,L3,L4;
  s=segment(r.upper_left(),r.upper_right()); segment_points(L1,s,25); LP.conc(L1);
  s=segment(r.lower_left(),r.lower_right()); segment_points(L2,s,25); LP.conc(L2); 
  s=segment(r.upper_left(),r.lower_left()); segment_points(L3,s,25); LP.conc(L3); 
  s=segment(r.upper_right(),r.lower_right()); segment_points(L4,s,25); LP.conc(L4);   
 }
};

int main(){
 GeoWin gw;
 list<point> L;
 
 GeoEditScene<list<point> >* sc1 = gw.new_scene(L);
 alt_input_rectangle RI;
 
 gw.set_input_object(sc1,RI,string("Rectangle"));
 gw.edit(sc1);
 
 return 0;
}
