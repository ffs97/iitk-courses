/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rectest.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/graphics/rat_window.h>
#include <LEDA/geo/rectangle.h>
#include <LEDA/geo/rat_rectangle.h>
#include <LEDA/core/random_source.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



int main()
{
 window W;
 W.init(0,400,0);
 W.display();

 rat_point p1(150,150,1);
 rat_point p2(250,250,1);
 rat_rectangle r(p1,p2);

 W << r.to_rectangle();

 point p;
 while (W.read_mouse(p) != MOUSE_BUTTON(3))
 { cout << r.cs_code(rat_point(p)) << endl;
   r = r.include(rat_point(p));
   W << r.to_rectangle();
 }


 int N = 5000;

 random_source RS;
 
 for(int i=0; i<N; i++)
 { double x1,y1,x2,y2;
   RS >> x1 >> y1 >> x2 >> y2;
   rat_segment s(rat_point(400*x1,400*y1), rat_point(400*x2,400*y2));
   W << s;
   rat_segment rs;
   if (r.clip(s,rs)) W.draw_segment(rs.to_segment(),red);
 }

 W.read_mouse();

 return 1;
}

