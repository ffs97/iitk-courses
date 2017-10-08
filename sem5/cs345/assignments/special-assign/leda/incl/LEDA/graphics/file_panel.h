/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  file_panel.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FILE_PANEL_H
#define LEDA_FILE_PANEL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600152
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graphics/window.h>

LEDA_BEGIN_NAMESPACE


typedef void (*file_panel_handler)(string);

// handler object ...
class __exportC file_panel_handle_base {
protected:
  file_panel_handler  FPH;
public:
  file_panel_handle_base() { FPH = NULL; }
  file_panel_handle_base(file_panel_handler f) { FPH = f; }
  virtual ~file_panel_handle_base() {}
  
  virtual void operator()(string s) const
  { if (FPH != NULL) FPH(s); }
};


class __exportC file_panel {

panel P;

window* wp;

string*      dir_name;
list<string> dir_list0;
list<string> dir_list;
panel_item   dir_item;
panel_item   txt_item;

string*      file_name;
list<string> file_list;
panel_item   file_item;

string       load_string;
string       save_string;
string       pat_string;
string       descr_string;

list<string> descr_list;
list<string> pat_list;

int          panel_init;

bool         mswin;

file_panel_handler load_handler;
file_panel_handler save_handler;
file_panel_handler cancel_handler;

//load/save/cancel objects ...
const file_panel_handle_base* load_ptr; 
const file_panel_handle_base* save_ptr; 
const file_panel_handle_base* cancel_ptr; 


void*  dummy_ptr;

string home_dir;

void chdir();

void init(window*,string& ,string&);

static void change_filter(char*);

static void change_dir(char* dname);
static void update_dir_menu(int);
static void update_file_menu(int);

public:

 panel& get_panel() { return P; }

 void init_panel();

 file_panel(string& fname, string& dname);
 file_panel(window& W, string& fname, string& dname);
~file_panel();

 void set_load_handler(file_panel_handler f)  { load_handler  = f;  }
 void set_save_handler(file_panel_handler f)  { save_handler  = f;  }
 void set_cancel_handler(file_panel_handler f){ cancel_handler = f; }
 
 void set_load_object(const file_panel_handle_base& f);
 void set_save_object(const file_panel_handle_base& f);
 void set_cancel_object(const file_panel_handle_base& f);

 void set_load_string(string s) { load_string = s; }
 void set_save_string(string s) { save_string = s; }
 void set_pattern(string s)     { descr_string = pat_string = s; }
 void set_pattern(string s1, string s2) { descr_string = s1, pat_string = s2; }

 void set_mswin(bool b) { mswin = b; }

 void set_frame_label(string s) { P.set_frame_label(s); }
 
 string get_pattern() const { return pat_string; }

 void x_open();
 void x_open(int x, int y);

 void ms_open();
 void ms_open(int x, int y);

 void open()             { if (mswin) ms_open();    else x_open();    }
 void open(int x, int y) { if (mswin) ms_open(x,y); else x_open(x,y); }

};

#if LEDA_ROOT_INCL_ID == 600152
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif
