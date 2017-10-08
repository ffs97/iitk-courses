/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edit.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

main() {

window W;
W.display();
W.start_buffering();

string s = "hallo";
int cursor = s.length();

int x = 50;
int y = 50;

W.draw_text_with_cursor(x,y,s,cursor);
W.flush_buffer();

while (W.string_edit(x,y,s,cursor)) 
{ W.clear();
  W.draw_text_with_cursor(x,y,s,cursor);
  W.flush_buffer();
}



}







