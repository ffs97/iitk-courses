/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bar_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/graphics/window.h>
#include <LEDA/graphics/bitmaps/button32.h>

using namespace leda;

int main()
{
  menu M;
  M.button("empty");
  window W;
  W.menu_button("File",M);
  W.menu_button("Edit",M);
  W.menu_button("Help",M);
  W.button(32,32,exit_bits,"",-1);
  W.button(32,32,circle_bits,"",-1);
  W.button(32,32,dice_bits,"",-1);
  W.display();
  W.read_mouse();
  return 0;
}


