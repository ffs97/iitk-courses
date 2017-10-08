/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  zoom_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


// mit top die SIZE - Informationen des Prozesses ansehen;
// man sieht, wie sich langsam SIZE langsam steigert ...

#include <LEDA/graphics/window.h>

using namespace leda;


void setup_font(window& w, double size)
{     
  int sz = w.real_to_pix(size);

  if (sz<8) sz=8;
  
  cout << "font size:" << sz << "\n";

  string font_name;
  font_name = string("F%d",sz);
                
  w.set_font(font_name);
}


void rd_fcn(window* win)
{
   cout << "redraw function !\n";
   win->start_buffering();
   win->clear();
   
   // set font ...
   string font_name = string("F%d",7);
   win->set_font(font_name); 
   
   color cold   = win->set_color(yellow);
   color cold2  = win->set_fill_color(yellow);
   
   setup_font(*win,3);
   
   win->draw_disc(300,300,2);
   
   win->set_color(cold);
   win->set_fill_color(cold2);   
   
   win->flush_buffer();
   win->stop_buffering();
}

int main()
{
 window W(600,600);
 W.init(0,600,0);
 W.display(window::center, window::center);

 int a;
 W.set_redraw(rd_fcn);

 // now zoom ... 
 while (true) { 
   for (a=0;a<5;a++) W.zoom(2.0, 10);
   for (a=0;a<5;a++) W.zoom(0.5, 10);
 }

 return 0;
}
