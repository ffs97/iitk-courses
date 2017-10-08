/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  precision.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/graphics/rat_window.h>

using namespace leda;


void set_precision(int prec)
{ window* wp = window::get_call_window();
  wp->set_precision(prec);
}


int main()
{
  window W;
  
  int prec = 12; 
  W.int_item("precision", prec, 1,53, set_precision);
  W.display(window::center,window::center);

  //W.set_grid_mode(5);

  rat_point p;
  while (W >> p)
  { string str;
    str += "(";
    str += p.X().to_string();
    str += ",";
    str += p.Y().to_string();
    str += ",";
    str += p.W().to_string();
    str += ")";
    W.draw_text(p.to_float(),str);
   }

  return 0;
}
