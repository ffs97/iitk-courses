/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  draw1.c
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

enum { POINT, SEGMENT, RAY, LINE, CIRCLE, POLY};


class geo_object {

handle_base* handle;
color  col;
int    kind;

public:
geo_object(const point& p, color c)   : kind(POINT), col(c)
{ handle =  new point(p);   }

geo_object(const segment& p, color c) : kind(SEGMENT), col(c)
{ handle =  new segment(p); }

geo_object(const ray& p, color c)     : kind(RAY), col(c)
{ handle =  new ray(p);     }

geo_object(const line& p, color c)    : kind(LINE), col(c)
{ handle =  new line(p);    }

geo_object(const polygon& p, color c) : kind(POLY), col(c)
{ handle =  new polygon(p); }

geo_object(const circle& p, color c)  : kind(CIRCLE), col(c)
{ handle =  new circle(p);  }

geo_object() { handle = 0; }

geo_object(const geo_object& x) 
{ handle = new handle_base(*(x.handle));
  kind = x.kind;
  col = x.col;
}

~geo_object() { delete handle; }

void draw(window&);
void read(window&, int, color);

};

istream& operator>>(istream& in, geo_object&) { return in; }

ostream& operator<<(ostream& out,const geo_object&) { return out; }


void geo_object::draw(window& W) 
{ switch (kind) {
  case  POINT:   W.draw_point(*(point*)handle,col);
                 break;
  case  SEGMENT: W.draw_segment(*(segment*)handle,col);
                 break;
  case  RAY:     W.draw_ray(*(ray*)handle,col);
                 break;
  case  LINE:    W.draw_line(*(line*)handle,col);
                 break;
  case  CIRCLE:  W.draw_disc(*(circle*)handle,col);
                 W.draw_circle(*(circle*)handle,black);
                 break;
  case  POLY:    W.draw_filled_polygon(*(polygon*)handle,col);
                 W.draw_polygon(*(polygon*)handle,black);
                 break;
  }
}

void geo_object::read(window& W, int k, color c) 
{ 
  kind = k;
  col = c;

  switch (k) {
  case  POINT:  { point x; 
                  W >> x;
                  handle = new point(x);
                  break;
                 }
  case  SEGMENT:{ segment x;
                  W >> x;
                  handle = new segment(x);
                  break;
                 }
  case  RAY:    { ray x;
                  W >> x;
                  handle = new ray(x);
                  break;
                 }
  case  LINE:   { line x;
                  W >> x;
                  handle = new line(x);
                  break;
                 }
  case  CIRCLE: { circle x;
                  W >> x;
                  handle = new circle(x);
                  break;
                 }
  case  POLY:   { polygon x;
                  W >> x;
                  handle = new polygon(x);
                  break;
                 }
  }
}



static list<geo_object> L;

static char* image = 0;

static window W(500,600);

void redraw()
{ geo_object p;
  forall(p,L) p.draw(W);
 }


void scroll_x(int x) 
{ if (image)
  { W.put_pixrect(x,W.ymin(),image); 
    W.draw_box(W.xmin(),W.ymin(),x,W.ymax(),white);
   }
 }

void scroll_y(int y)
{ if (image)
  { W.put_pixrect(W.xmin(),y,image); 
    W.draw_box(W.xmin(),W.ymin(),W.xmax(),y,white);
   }
}



int main()
{
  color fg_col = blue;
  color bg_col = white;
  int   shape  = CIRCLE;
  int   x_origin = 0;
  int   y_origin = 0;

  list<string> shapes;
  shapes.append("point");
  shapes.append("seg");
  shapes.append("ray");
  shapes.append("line");
  shapes.append("circ");
  shapes.append("poly"); 

  W.color_item ("color",fg_col);
  W.choice_item("shape",shape,shapes);

  //W.int_item("scroll-x",x_origin,0,1000,scroll_x);
  //W.int_item("scroll-y",y_origin,0,1000,scroll_y);

  W.button("clear",1);
  W.button("quit",2);

  W.set_redraw(redraw);
  W.init(0,1000,0);

  W.open(window::center,window::center);

  for(;;)
  { 
    double x;
    double y;

    int k = W.read_mouse(x,y);

    W.set_fg_color(fg_col);
    W.set_bg_color(bg_col);

    if (k==1) 
    { W.clear();
      continue;
     }

    if (k==2) break;

    put_back_event();

    geo_object p;
    p.read(W,shape,fg_col);
    p.draw(W);
    L.append(p);

/*
      if (image) W.del_pixrect(image);

      image = W.get_pixrect(W.xmin(),W.ymin(),W.xmax(),W.ymax());
*/
  }

  return 0;
}
