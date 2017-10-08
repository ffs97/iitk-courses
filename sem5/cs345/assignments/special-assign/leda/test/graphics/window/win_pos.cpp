/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  win_pos.c
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

  int w = read_int("w = ");
  int h = w;


  window W1(w,h);
  W1.display(window::center,window::center);

  int x0,y0,x1,y1;
  W1.frame_box(x0,y0,x1,y1);


  window W2(w,h);
  W2.display(x1+1,y0);

  window W3(w,h);
  W3.display(-(x0-1),y0);

  window W4(w,h);
  W4.display(x0,y1+1);

  window W5(w,h);
  W5.display(x0,-(y0-1));

  W1.read_mouse();


  return 0;
}
