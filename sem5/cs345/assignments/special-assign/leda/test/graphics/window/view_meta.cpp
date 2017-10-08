/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  view_meta.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/window.h>

using namespace leda;

string wmfname;

void redraw(window* wp)
{ window& W = *wp;
  W.clear();
  //W.draw_rectangle(0,0,100,100,blue);
  W.load_metafile(0,0,100,100,wmfname);
  W.flush_buffer();
}

main(int argc, char** argv)
{
   if (argc < 2) return 0;

   wmfname = argv[1];

   window W;
   W.init(-10,110,-10);
   W.set_redraw(redraw);
   W.set_clear_on_resize(false);
   W.display();
   W.start_buffering();
   redraw(&W);
   W.read_mouse();
   return 0;
}
