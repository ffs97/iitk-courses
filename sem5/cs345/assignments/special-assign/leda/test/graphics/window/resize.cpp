/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  resize.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/window.h>

using namespace leda;

void redraw(window* wp)
{
  wp->clear();
}

main()
{
   window W(400,400);
   W.display(300,200);
   W.read_mouse();

   window W1(10,10);
   W1.set_bg_color(yellow);
   W1.set_redraw(redraw);
   W1.display(W,200,200);
   

   for(int i=10; i< 300; i+=2) W1.resize(200-i/2,200-i/2,i,i);

   W1.read_mouse();

   W.resize(100,100,100,100);
   W.read_mouse();
   return 0;
}
