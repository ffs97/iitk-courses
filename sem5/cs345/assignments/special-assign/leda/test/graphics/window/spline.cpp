/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  spline.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

int main() 
{
  window W("Draw Splines");

  W.display();

  polygon P;

  while (W >> P)
  { W << P;
    W.draw_spline(P,32,blue);
  }

  return 0;
}


    

    

