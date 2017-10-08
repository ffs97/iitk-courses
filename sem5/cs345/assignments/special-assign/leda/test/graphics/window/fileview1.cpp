/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fileview1.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/file_panel.h>
#include <LEDA/system/file.h>
#include <fstream.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


enum { file_load, file_save, file_all, file_exit };

#define MAX_LINES 5000

static window* win_ptr;

static string  dname = ".";
static string  fname = "none";

static list<string> txt;
static list_item    tit0;
static list_item    tit1;

static int tpos;

static double txt_h;
static double txt_x;
static double txt_y0;
static double txt_y1;

static int win_lines;

static void draw_text() 
{ if (txt.empty()) return;
  win_ptr->start_buffering();
  win_ptr->clear();
  win_ptr->set_fixed_font();
  list_item it = tit0;
  double y = txt_y0;
  do { win_ptr->draw_text(txt_x,y,txt[it]);
       it = txt.succ(it);
       y -= txt_h;
  } while (it != tit1);
  win_ptr->flush_buffer();
  win_ptr->stop_buffering();
  int l = tpos + win_lines;
  string stat("~~~Line~~ \\bf %3d \\rm ~~~of~~ \\bf %3d",l,txt.length());
  win_ptr->set_status_string(stat);
}


static void redraw(window* wp) 
{ wp->start_buffering();
  wp->flush_buffer(); 
  wp->stop_buffering();
 }


static void scroll_down(int i)   
{ if (tit1 != txt.last())
  { tit0 = txt.succ(tit0);
    tit1 = txt.succ(tit1);
    draw_text();
    tpos++;
    double dy = double(tpos)/(txt.length() - win_lines);
    win_ptr->set_scrollbar_pos(dy);
   }
}


static void scroll_up(int i)
{ if (tit0 != txt.first())
  { tit0 = txt.pred(tit0);
    tit1 = txt.pred(tit1);
    draw_text();
    tpos--;
    double dy = double(tpos)/(txt.length() - win_lines);
    win_ptr->set_scrollbar_pos(dy);
   }
 }


static void scroll_drag(int i) 
{ if (i < 0) return;
  int pos = int((i/1000.0)*(txt.length()-win_lines));
  if (pos != tpos)
  { tpos = pos;
    tit0 = txt.get_item(tpos);
    tit1 = txt.get_item(tpos+win_lines-1);
    draw_text();
   }
 }



static void read_file(string fn) 
{ ifstream in(fn);

  if (!in.good())
  {  error_handler(0,string("Cannot open file ") + fn);
     draw_text();
     return;
   }

  txt.clear();

  while (in && txt.length() < MAX_LINES) 
  { string s = read_line(in);
    txt.append(s);
   }

  win_ptr->set_fixed_font();

  txt_h = win_ptr->text_height("H") + win_ptr->pix_to_real(1);

  double wh =  win_ptr->ymax() - win_ptr->ymin() - win_ptr->pix_to_real(12);

  win_lines = int(wh/txt_h);

  tpos = 0;
  tit0 = txt.first();
  tit1 = (win_lines < txt.length()) ? txt.get_item(win_lines-1) : txt.last();

  double f = double(win_lines)/txt.length();

  if (f < 1)
     win_ptr->open_scrollbar(scroll_up,scroll_down,scroll_drag,f);
  else
     win_ptr->close_scrollbar();

  win_ptr->set_frame_label(fn);

  double d = win_ptr->pix_to_real(2);

  txt_x  = win_ptr->xmin() + d;
  txt_y0 = win_ptr->ymax() - d;
  txt_y1 = txt_y0 - (win_lines-1)*txt_h;

  draw_text();
}


static void write_file(string fn)
{ error_handler(0,string("Writing file %s not implemented.",~fn)); }


static void file_handler(int what)
{ 
  file_panel FP(*win_ptr,fname,dname);

  switch (what) {
  case file_load: FP.set_load_handler(read_file);
                  break;
  case file_save: FP.set_save_handler(write_file);
                  break;
  case file_all:  FP.set_load_handler(read_file);
                  FP.set_save_handler(write_file);
                  break;
  }
  FP.open();
}


void LedaFileViewer(string fn, string dn)
{
  menu file_menu;
  file_menu.button("Load File",file_load,file_handler);
  file_menu.button("Save File",file_save,file_handler);
  file_menu.button("Load/Save",file_all, file_handler);
  file_menu.button("Exit",file_exit);

  menu dummy_menu;
  dummy_menu.button("button1");
  dummy_menu.button("button2");
  dummy_menu.button("button3");
  dummy_menu.button("button4");

  window W(585,648, "LEDA FileViewer");
  win_ptr = &W;

  W.button("File", file_menu);
  W.button("Edit", dummy_menu);
  W.button("Help", dummy_menu);
  W.button("Exit ",file_exit);

  W.make_menu_bar();

  W.set_redraw(redraw);


  if (dn != "") dname = dn;

  set_directory(dname);


  W.display(window::center,window::center);
  W.open_status_window();


  if (fn != "") 
  { fname = fn;
    read_file(fname);
   }


  for(;;)
  { 
    int    val;
    double x,y;
    int    e = W.read_event(val,x,y);

    if (e == button_press_event && val == file_exit) break;

    if (e == key_press_event)
    {  
       if (val == 'q') break;

       switch (val) {
         case 'k':
         case KEY_UP:   scroll_up(0);
                        break;
         case 'j':
         case KEY_DOWN: scroll_down(0);
                        break;
         case 'g':
         case KEY_HOME: tpos = 0;
                        tit0 = txt.first();
                        tit1 = txt.get_item(win_lines-1);
                        draw_text();
                        win_ptr->set_scrollbar_pos(0);
                        break;
         case 'G':
         case KEY_END:  tpos = txt.length() - win_lines;
                        tit0 = txt.get_item(tpos);
                        tit1 = txt.last();
                        draw_text();
                        win_ptr->set_scrollbar_pos(1);
                        break;
       }

       while (W.get_event(val,x,y) == key_press_event);
     }
   }

}


int main(int argc, char** argv)
{
  string fname;
  string dname;
  
  if (argc > 1) fname = argv[1];
  if (argc > 2) dname = argv[2];

  LedaFileViewer(fname,dname);
}
