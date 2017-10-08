/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bitmap.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/graphics/window.h>
#include <LEDA/bitmaps/leda_icon.xbm>

using namespace leda;

int main()
{ 
  window W(400,400);
  
  W.set_bg_color(yellow);
  W.display();
  
  // we must set the size hints after display ...
  // W.set_size_hints(300,500,300,500);
  W.set_size_hints(400,400,400,400);

  // construct bitmap from the bitmap data in 
  // <LEDA/bitmaps/leda_icon.xbm>

  char* bm = W.create_bitmap(leda_icon_width,
                             leda_icon_height, 
                             leda_icon_bits);

  // copy copies of bm into the window

  point p;
  while (W >> p) W.put_bitmap(p.xcoord(),p.ycoord(),bm,blue);

  W.del_bitmap(bm);

  W.screenshot("bitmap.ps");
  return 0;
}
