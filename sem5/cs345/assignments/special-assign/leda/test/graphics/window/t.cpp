/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  t.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




#include <LEDA/graphics/window.h>

using namespace leda;

void redraw(window* wp)
{ std::cout << "w " << wp->width() << " h " << wp->height() << "\n"; }

int main()
{
  window W(500,300);
  W.set_redraw(redraw);
  W.display();
 
  while (1) W.redraw();
 
  return 0;
}

