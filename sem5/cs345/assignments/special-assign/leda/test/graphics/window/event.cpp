/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  event.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <math.h>

using namespace leda;


// this program demonstrates the use of the "read_event" operation


int main()
{
  window W(450,500,"Event Demo");

  W.display();
  W.start_buffering();


  for(;;)
  { 
    int  val;

    double x,y,x0,y0;
    int b = W.read_mouse(x0,y0);

    if (b == MOUSE_BUTTON(3) ) break;  
  
    char* buf = W.get_window_pixrect();
  
    if (b == MOUSE_BUTTON(1))
    { // draw rectangle from (x0,y0) to current position while button down 
      while (W.read_event(val,x,y) != button_release_event) 
      { W.put_pixrect(buf);
        W.draw_rectangle(x0,y0,x,y,blue);
        W.flush_buffer();
       }
      buf = W.get_window_pixrect();
     }

    if (b == MOUSE_BUTTON(2))
    { // draw circle with center (x0,y0) through current position 
      while (W.read_event(val,x,y) != button_release_event) 
      { double r = hypot(x-x0,y-y0);
        W.put_pixrect(buf);
        W.draw_circle(x0,y0,r,red);
        W.flush_buffer();
       }
      buf = W.get_window_pixrect();
     }
  
  }

  W.stop_buffering();

  return 0;
}
