/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  time_client.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/window.h>
#include <LEDA/socket.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
#endif


#if defined(LEDA_STD_HEADERS)
#include <ctime>
#else
#include <time.h>
#endif


void display_time(window* wp)
{ 
  window& W = *wp;

  leda_socket sock;
  sock.set_host("leda.informatik.uni-trier.de");
  sock.set_port(9666);

  // connect to time server

  if (!sock.connect())
  { cerr << sock.get_error() << endl;
    exit(1);
   }

  // receive time string
  
  string str;
  sock.receive_string(str);


  // display time in window

  double  x = (W.xmax() - W.xmin())/2;
  double  y = (W.ymax() - W.ymin())/2;
  W.clear();
  W.set_font("T32");
  W.draw_ctext(x,y,str(0,4));
}


int main()
{
  window W(100,50, "dclock");
  W.set_bg_color(grey1);
  W.set_redraw(display_time);

  W.display(window::center,window::center);

  display_time(&W);
  W.start_timer(60000,display_time);

  while (W.read_mouse() != MOUSE_BUTTON(3));

  return 0;
}
