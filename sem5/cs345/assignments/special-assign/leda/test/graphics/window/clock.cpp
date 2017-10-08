/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  clock.c
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


#if defined(LEDA_STD_HEADERS)
#include <ctime>
#include <cmath>
#else
#include <time.h>
#include <math.h>
#endif


const char* month[] =
{ "Januar",
  "Februar",
  "Maerz",
  "April",
  "Mai",
  "Juni",
  "Juli",
  "August",
  "September",
  "Oktober",
  "November",
  "Dezember"
 };


double radius;
double mx,my;
int   h;
int   m;
int   s;

string  datum;

window* Wp;

color face_color   = white;
color text_color   = black;
color sec_color    = black;
color min_color    = white;
color hour_color   = white;
bool  seconds      = false;

list<point> make_hand(double rad, double l1, double l2,double pos)
{ circle c(mx,my,rad);
  double  phi = 2*LEDA_PI*pos/60;
  point  p(mx+l1*sin(phi),my+l1*cos(phi));
  double l = c.left_tangent(p).direction();
  double r = c.right_tangent(p).direction();

  list<point> L;
  L.append(p);
  L.append(p.translate_by_angle(l,l2));
  L.append(p.translate_by_angle(r,l2));

  return L;
}


void hour_hand(double pos)
{ double  length = 0.65 * radius;
  color      bg = (hour_color == black) ? white : black;
  list<point> P = make_hand(radius/11,length,1.3*length,pos);
  Wp->draw_filled_polygon(P,hour_color);
  Wp->draw_polygon(P,bg);
}

void min_hand(double pos)
{ double  length = 0.95*radius;
  color      bg = (min_color == black) ? white : black;
  list<point> P = make_hand(radius/13,length,1.20*length,pos);
  Wp->draw_filled_polygon(P,min_color);
  Wp->draw_polygon(P,bg);
  Wp->draw_disc(mx,my,radius/35,bg);
}

void sec_hand(double pos)
{ double  length = 0.95*radius;
  color      bg = (min_color == black) ? white : black;
  list<point> P = make_hand(radius/60,length,1.15*length,pos);
  Wp->draw_filled_polygon(P,sec_color);
  Wp->draw_disc(mx,my,radius/30,bg);
}


void get_time(int& h, int& m, int& s, string& datum)
{ time_t clock; 
  time(&clock);
  tm* T = localtime(&clock); 
  s = T->tm_sec;
  m = T->tm_min;
  h = T->tm_hour;
  datum = string("%d. %s 19%0d",T->tm_mday,month[T->tm_mon], T->tm_year%100);
 }


void draw_clock()
{ 
  int i;
  double x,y;

  mx = (Wp->xmax()-Wp->xmin())/2;
  my = (Wp->ymax()-Wp->ymin())/2;

  radius = (mx < my) ? 0.9*mx : 0.9*my;

  Wp->start_buffering();
  Wp->clear();
  Wp->draw_disc(mx,my,1.1*radius,face_color);
  Wp->draw_circle(mx,my,1.1*radius,black);

  for(i = 1; i<=60; i++)
  { x = mx + radius*sin(2*LEDA_PI*i/60);
    y = my + radius*cos(2*LEDA_PI*i/60);
    Wp->draw_filled_node(x,y,black);
   }

  for(i = 1; i<=12; i++)
  { x = mx + radius*sin(2*LEDA_PI*i/12);
    y = my + radius*cos(2*LEDA_PI*i/12);
    Wp->draw_disc(x,y,radius/30,text_color);
   }

  get_time(h,m,s,datum);
  //Wp->set_frame_label(datum);

  hour_hand(5*(h+m/60.0));
  min_hand(m);

  if (seconds) sec_hand(s);

  Wp->flush_buffer();
  Wp->stop_buffering();
}



void timer_action(window* wp) { draw_clock(); }



int main(int argc, char** argv)
{
  int height = window::screen_height()/3;
  int width  = height;
  int xpos   = 0;
  int ypos   = 0;

  if (argc > 1)
  { if (argc == 3)
      { width  = atoi(argv[1]);
        height = atoi(argv[2]);
       }
    else
      if (argc == 5)
        { width  = atoi(argv[1]);
          height = atoi(argv[2]);
          xpos   = atoi(argv[3]);
          ypos   = atoi(argv[4]);
         }
      else 
        { cout << "usage: clock [ width height [ xpos ypos ]]\n";
          exit(1);
         }
   }

  window W(width,height);
  Wp = &W;

  W.set_redraw(draw_clock);

  W.set_node_width(1);

  W.open(window::center,window::center);

  if (!W.mono())
  { face_color   = green2;
    text_color   = yellow;
    min_color    = red;
    hour_color   = blue2;
   }

  panel P("clock panel");

  P.bool_item("seconds", seconds);
  P.color_item("face   color", face_color);
  P.color_item("hour   color", hour_color);
  P.color_item("minute color", min_color);
  P.color_item("second color", sec_color);
  P.color_item("text   color", text_color);

  P.button("ok");
  P.button("quit");

  draw_clock();

  W.start_timer(990,timer_action);

  for(;;)
  { W.read_mouse();
    if (P.open() == 1) break;
    draw_clock();
   }

 return 0;
}
