/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rgb.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif




int main()
{  
   window W(500,500);
   int n = 125;

   W.init(0,n,0);
   W.set_show_coordinates(false);
   W.display();

   int i = 0;
   for(int r = 0; r < 256; r+=63)
     for(int g = 0; g < 256; g+=63)
       for(int b = 0; b < 256; b+=63)
       { W.draw_filled_rectangle(i,0,i+1,n,color(r,g,b));
         cout << r << " " << g << " " << b << endl;
         i++;
        }

    W.read_mouse();

  return 0;
}
  
