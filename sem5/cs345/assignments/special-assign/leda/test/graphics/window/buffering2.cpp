/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  buffering2.c
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
  window W1(100,100);
  W1.set_bg_color(grey3);
  W1.init(-1,+1,-1);
  W1.start_buffering();
  W1.draw_disc(0,0,0.8,blue);  W1.draw_circle(0,0,0.8,black);
  W1.draw_disc(0,0,0.6,yellow);W1.draw_circle(0,0,0.6,black);
  W1.draw_disc(0,0,0.4,green); W1.draw_circle(0,0,0.4,black);
  W1.draw_disc(0,0,0.2,red);   W1.draw_circle(0,0,0.2,black);
  char* pr;
  W1.stop_buffering(pr);
  
  window W(400,400);
  W.set_icon_pixrect(pr);
  W.display(window::center,window::center);

  point p;
  while (W >> p) W.put_pixrect(p,pr);

  W.del_pixrect(pr);

  W.screenshot("buffering2");
  return 0;
}
