/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  xpm_fill.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

using std::cerr;
using std::cout;
using std::endl;

char* pr;

void redraw(window* wp)
{ wp->set_pixrect(pr); }

int main(int argc, char** argv)
{
 if (argc <= 1) 
 { cerr << string("usage: %s xpm_file",argv[0]) << endl;
   return 1;
  }

  window ww;
  pr = ww.create_pixrect(argv[1]);
  int w = ww.get_width(pr);
  int h = ww.get_height(pr);

  window W(w,h);
  //W.set_bg_pixrect(pr);
  W.set_redraw(redraw);
  W.display();
  redraw(&W);
  W.read_mouse();
  return 0;
}
   
