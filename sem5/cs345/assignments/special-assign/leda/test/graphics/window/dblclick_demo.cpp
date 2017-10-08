/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dblclick_demo.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

void redraw() {}

int main()
{
  window W(400,400,"Double Click Demo");

  double r = 1.75;
  unsigned int timeout = 300;

  W.set_grid_dist(4);
  W.set_grid_style(line_grid);
  //W.set_redraw(redraw);

  W.display();

  W.clear(20,20,80,80);

  for(;;)
  { 
    int b;
    double x,y;
    unsigned long t_down;
    while (W.read_event(b,x,y,t_down) != button_press_event); 

    // we have button b down at time t_down

    if (b == MOUSE_BUTTON(3) ) break;  

    // wait for release event

    unsigned long t;
    while (W.read_event(b,x,y,t) != button_release_event); 

    if (t-t_down < timeout && 
        W.read_event(b,x,y,t,timeout) == button_press_event)
      { // double click: erase node
        W.clear(x-r,y-r,x+r,y+r);
       }
    else
      { // single click: new node
        W.draw_disc(x,y,r,green2);
        W.draw_circle(x,y,r,black);
       }

   }

  return 0;
}
