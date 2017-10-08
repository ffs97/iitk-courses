/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  xpm.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/graphics/pixmaps/leda_icon.xpm>

using namespace leda;
 
int main(int argc, char** argv)
{
  window W(500,500);
 
  W.display();
 
  char* leda_pr;

  if (argc > 1) 
     leda_pr = W.create_pixrect(argv[1]);
  else
     leda_pr = W.create_pixrect(leda_icon);
 
  W.start_buffering();
 
  int but = NO_BUTTON;

  while (but != MOUSE_BUTTON(3))
  { if (but == MOUSE_BUTTON(2)) W.clear();
    char* win_pr = W.get_window_pixrect();
    double x,y;
    while (W.read_event(but,x,y) != button_press_event)
    { W.put_pixrect(win_pr);
      W.put_pixrect(x,y,leda_pr);
      W.flush_buffer();
     }
    W.del_pixrect(win_pr);
   }

  W.stop_buffering();
  W.del_pixrect(leda_pr);

  return 0;
}

