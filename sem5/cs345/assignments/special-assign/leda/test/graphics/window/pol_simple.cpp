/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pol_simple.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geo/polygon.h>
#include <LEDA/graphics/window.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int main()
{

  window W;
  W.init(0,1000,0,50);
  W.set_show_coordinates(true);
  W.display();


  list<point> L = W.read_polygon();

 // polygon P0(L,false);
  polygon P0(L);


  list<polygon> pol = P0.simple_parts();

  int i = 2;

  polygon P;
  forall(P,pol)
  { cout << P << endl;
    double A = P.area();
    cout << "area = " << A << endl;
    cout << endl;
    color col = color(i++);
    if (A < 0) col =  white;
    W.draw_filled_polygon(P,col);
    W.draw_polygon(P,black);
   }
  W.read_mouse();

  return 0;
}


