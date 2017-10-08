/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  font.c
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


char buf[256];

void draw(window* wp) { wp->draw_text(0,50,buf); }


int main()
{
  int sz = 3;
  window W(500,300);
  W.int_item("size",sz,1,24);
  W.init(0,499,0);
  W.set_redraw(draw);
  W.display();

  while (W.read_mouse() != MOUSE_BUTTON(3))
  {
  W.set_font(string("T%02d",sz));

  char text[256];
  unsigned char c = 0;
  for(c=0; c<128; c++) text[c] = c;
  text[128] = ' ';
  text[129] = ' ';
  text[130] = ' ';
  text[131] = 0;

  W.set_text_mode(opaque);
  W.draw_text(1,40,text);
  
  for(c=0; c<128; c++) 
  { W.draw_text(1,20,text+c);
    for (int i=16; i>3; i--)
    {  int y = i+3;
       int v = 0;
       for(int x = 2; x < 10; x++)
       { int pix = W.query_pix(x,y);
         W.draw_box(10*x,10*i+80,10*(x+1),10*(i-1)+80,white);
         W.draw_rectangle(10*x,10*i+80,10*(x+1),10*(i-1)+80,black);
         v <<= 1;
         if (pix <= 0) 
         {  W.draw_box(10*x,10*i+80,10*(x+1),10*(i-1)+80,blue);
            v |= 1;
           }
        }
        cout << string("%3d, ",v);
     }
     cout << endl;
  }
}
  return 0;
}
