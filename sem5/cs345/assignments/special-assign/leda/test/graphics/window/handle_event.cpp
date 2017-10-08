/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  handle_event.c
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


int my_event_handler(window* wp, int e, int val, double x, double y, 
                                                            unsigned long t)
{ 
  if (e == button_press_event || e == button_release_event)
     cout << e << " " << val << " " << x << " " << y << " " << t << endl;

  //return e;
  return no_event; // read_mouse will not terminate
}

int main()
{
  window W;
  W.set_event_handler(my_event_handler);
  W.display(window::center,window::center);
  W.read_mouse();
  return 0;
}
