/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  draw0.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geo/plane.h>
#include <LEDA/graphics/window.h>

using namespace leda;

enum { POINT, SEGMENT, RAY, LINE, CIRCLE, RECT, POLY};

static window W(550,600);


void redraw(window* wp, double x0, double y0, double x1, double y1) 
{ wp->flush_buffer(x0,y0,x1,y1); }

int main()
{
  color fg_col = blue;
  color fill_col = ivory;
  color bg_col = white;

  point_style pstyle = cross_point;
  line_style  lstyle = solid;

  int   shape = CIRCLE;
  int   x_origin = 0;
  int   y_origin = 0;

  list<string> shapes;
  shapes.append("point");
  shapes.append("seg");
  shapes.append("ray");
  shapes.append("line");
  shapes.append("circ");
  shapes.append("rect");
  shapes.append("poly"); 

  window W(500,600);

  W.color_item ("foreground",fg_col);
  W.color_item ("fill color",fill_col);
  W.color_item ("background",bg_col);
  W.choice_item("shape",shape,shapes);
  W.pstyle_item("pstyle",pstyle);
  W.lstyle_item("lstyle",lstyle);
  W.text_item("");
  W.button("clear",1);
  W.button("quit",2);

  W.init(0,1000,0,25);
  W.set_redraw(redraw);
  W.display(window::center,window::center);

  W.start_buffering();

  for(;;)
  { 
    point p;
    int k = W.read_mouse(p);

    if (k==2) break;

    W.set_fg_color(fg_col);
    W.set_fill_color(fill_col);
    W.set_bg_color(bg_col);

    W.set_line_style(lstyle);
    W.set_point_style(pstyle);

    if (k==1) 
    { W.clear();
      W.flush_buffer();
      continue;
     }

    if (k == MOUSE_BUTTON(3))
    { W.scroll_window(p);
      continue;
     }

    put_back_event();
  
    switch (shape) {

        case POINT: 
             { point p;
               if (W >> p) W << p;
               break;
              }
  
        case SEGMENT: 
             { segment s;
               if (W >> s) W << s;
               break;
              }

        case RAY: 
             { ray r;
               if (W >> r) W << r;
               break;
              }

        case LINE: 
             { line l;
               if (W >> l) W << l;
               break;
              }
  
        case CIRCLE: 
             { circle c;
               if (W >> c) W << c;
               break;
              }

        case RECT: 
             { rectangle R;
               if (W >> R) W << R;
               break;
              }
  
        case POLY: 
             { polygon P;
               if (W >> P) W << P;
               break;
              }
       }

     W.flush_buffer();
  }

  return 0;
}
