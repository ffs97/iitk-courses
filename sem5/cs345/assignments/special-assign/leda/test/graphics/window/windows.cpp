/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  windows.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;



int main()
{
   // we open five windows w1, w2, w3, w4, and w5
   // for points, segments, lines, circles, and logging

   window w1(500,400,"DRAW POINTS");
   w1.display(window::min,window::min);

   window w2(500,400,"DRAW SEGMENTS");
   w2.display(window::max,window::min);

   window w3(500,400,"DRAW LINES");
   w3.display(window::min,window::max);

   window w4(500,400,"DRAW CIRCLES");
   w4.display(window::max,window::max);

   window w5(240,480,"LOG WINDOW");
   w5.display(window::center,window::center);

   w5.set_show_coordinates(false);

   double  x,y;
   point   p;
   segment s;
   line    l;
   circle  c;

   window* w;


   // wait for mouse click and get pointer to the corresponding window 

   while(read_mouse(w,x,y) != MOUSE_BUTTON(3))  
   { 
     if (w == &w1) // draw points
     { put_back_event();
       w1 >> p;
       w1.draw_point(p,blue);
       w5.message("w1: point");
       continue;
      }

     if (w == &w2) // draw segments
     { put_back_event();
       w2 >> s;
       w2.draw_segment(s,violet);
       w5.message("w2: segment");
       continue;
      }

     if (w == &w3) // draw lines
     { put_back_event();
       w3 >> l;
       w3.draw_line(l,green);
       w5.message("w3: line");
       continue;
      }

     if (w == &w4) // draw circles
     { put_back_event();
       w4 >> c;
       w4.draw_circle(c,orange);
       w5.message("w4: circle");
       continue;
      }

     if (w == &w5) // clear log-window
     { w5.clear();
       continue;
      }

    }

   return 0;
}
