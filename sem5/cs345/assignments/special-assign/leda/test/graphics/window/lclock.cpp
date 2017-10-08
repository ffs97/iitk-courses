/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  lclock.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

#if defined(LEDA_STD_HEADERS)
#include <ctime>
#else
#include <time.h>
#endif

#if defined(__BORLANDC__)
#include <time.h>
#endif

static void display_time(window* wp)
{ time_t clock; 
  time(&clock);
  tm* T = localtime(&clock); 
  int s = T->tm_sec;
  int m = T->tm_min;
  int h = T->tm_hour;
  //double x = (wp->xmax() - wp->xmin())/2;
  //double y = (wp->ymax() - wp->ymin())/2;
  string time("%02d:%02d:%02d",h,m,s);
  wp->set_frame_label(time);
}


int main()
{ panel P;
  P.button("continue");
  P.button("exit");
  P.buttons_per_line(2);
  P.display();
  P.start_timer(1000,display_time);
  P.read_mouse();
  return 0;
}
