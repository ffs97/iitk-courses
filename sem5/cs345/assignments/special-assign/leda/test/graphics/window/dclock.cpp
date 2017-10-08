/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dclock.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_PREFIX

#include <LEDA/graphics/window.h>
#include <LEDA/system/file.h>

using namespace leda;

#if defined(LEDA_STD_HEADERS)
#include <ctime>
#include <cmath>
using namespace std;
#else
#include <time.h>
#include <math.h>
#endif


const char* month[] =
{ "Januar",
  "Februar",
  "M\344rz",
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


static leda_color cl_col;
static leda_color dt_col;

static bool disp_sec = true;
static bool disp_date = true;
static int  seconds;
static int  minutes;
static int  hours;
static leda_string date;


void set_date()
{ time_t clock; 
  time(&clock);
  tm* T = localtime(&clock); 
  date = leda_string(" %2d. %s 19%0d",T->tm_mday,month[T->tm_mon], T->tm_year%100);
 }


void display_time(leda_window* wp)
{ 
  seconds++;

  if (seconds == 60)
  { time_t clock; 
    time(&clock);
    tm* T = localtime(&clock); 
    seconds = T->tm_sec;
    minutes = T->tm_min;
    hours = T->tm_hour;
    if (hours + minutes == 0) set_date();
  }

  double xc = (wp->xmax() - wp->xmin())/2;
  double yc = (wp->ymax() - wp->ymin())/2;

  double y = yc + wp->pix_to_real(7);

  wp->start_buffering();
  wp->clear();
  wp->set_bold_font();

  if (disp_sec)
    wp->draw_ctext(xc,y,leda_string("%2d:%02d:%02d",hours,minutes,seconds),cl_col);
  else
    wp->draw_ctext(xc,y,leda_string("%2d:%02d",hours,minutes),cl_col);


    if (disp_date)
    { wp->set_text_font();
      y = yc - wp->pix_to_real(16);
      wp->draw_ctext(xc,y,date,dt_col);
    }

  wp->flush_buffer();
  wp->stop_buffering();
}

#if defined(WINMAIN)

int main() {

  int argc; 
  char** argv;
  get_command_line(argc,argv);

#else

int main(int argc, char** argv) {

#endif

  leda_panel P("Digital Clock Settings");
  P.text_item("\\blue Colors");
  P.color_item("time",cl_col);
  P.color_item("date",dt_col);
  P.text_item("\\blue Display");
  P.bool_item("date",disp_date);
  P.bool_item("seconds",disp_sec);
  P.button("ok");
  P.button("quit");

  seconds = 59;
  set_date();

  cl_col = leda_white;
  dt_col = leda_grey1;

  int x = 0;
  int y = 0;

  if (argc >= 3)
  { x = atoi(argv[1]);
    y = atoi(argv[2]);
    if (argc == 5)
    { cl_col = argv[3];
      dt_col = argv[4];
     }
   }

  char* root_pixels = 0; //leda_window::root_pixrect(x,y,x+132,y+50);

  leda_window W(132,50);


  if (root_pixels)
     W.set_bg_pixrect(root_pixels);
  else
     W.set_bg_color(leda_blue);

  W.load_text_font("lucidasans-12");
  W.load_bold_font("T32");

  W.set_border_width(0);
  W.set_cursor(XC_box_spiral);
  W.set_redraw(display_time);

  //W.display(W,x,y);
  W.display(x,y);

  display_time(&W);

  W.start_timer(1000,display_time);

  for(;;)
  { W.read_mouse();
    if (P.open(leda_window::center,leda_window::center) == 1) break;
   }

  return 0;
}
