/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  file_handler.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/system/file.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


enum { file_load, file_save, file_exit, file_all, file_cancel };

static panel*       f_panel; 
static string       dir_name;
static string       filename = "default_file_name";
static list<string> dir_list;
static list<string> file_list;
static panel_item   dir_item;
static panel_item   file_item;

static window* win_ptr;

static void change_dir(char* dname) 
{ 
  set_directory(dname);

  dir_name = get_directory();

  file_list = get_files(dir_name);
  file_list.sort();

  // delete hidden files
  list_item it;
  forall_items(it,file_list)
  { string s = file_list[it];
    if (s[0] == '.') file_list.del_item(it);
   }

  dir_list  = get_directories(dir_name);
  dir_list.sort();

  f_panel->add_menu(dir_item,dir_list,8);
  f_panel->add_menu(file_item,file_list,8);
  f_panel->redraw_panel();
}


// example file I/O functions

static int read_file(string fname) 
{ cout << "Reading file fname." << endl;
  return 0;
 }

static int write_file(string fname)
{ cout << "Writing file fname." << endl;
  return 0;
 }




void file_handler(int what)
{
  panel P;
  f_panel = &P;

  P.set_item_width(250);

  P.buttons_per_line(4);

  switch (what) {

  case file_all:  P.text_item("\\bf File Panel");
                  break;

  case file_load: P.text_item("\\bf Load from File");
                  break;

  case file_save: P.text_item("\\bf Save to File");
                  break;

  }

  dir_list.append(".");
  file_list.append(filename);

  dir_item  = P.string_item("directory",dir_name,dir_list,8,change_dir);
  file_item = P.string_item("file name",filename,file_list,8);

  change_dir((char*)".");

  switch (what) {

  case file_all:  P.button("load",file_load);
                  P.button("save",file_save);
                  break;

  case file_load: P.button("load",file_load);
                  break;

  case file_save: P.button("save",file_save);
                  break;

  }

  P.button("cancel",file_cancel);

  int but = P.open(*win_ptr,window::center,window::center);

  switch(but) {
  
    case file_load : {
      switch ( read_file(filename) ) {
       case 0 : // ok
                break;
       case 1 : win_ptr->acknowledge("Read Error 1: ...");
                break;
       case 2 : win_ptr->acknowledge("Read Error 2: ...");
                break;
      }
      break;
    }
      

    case file_save : 
      switch ( write_file(filename) ) {
       case 0 : // ok
                break;
       case 1 : win_ptr->acknowledge("Write Error 1: ...");
                break;
       case 2 : win_ptr->acknowledge("Write Error 2: ...");
                break;
      }
      break;
      

    case file_cancel :
      break;
  }
}


int main()
{

  menu file_menu;
  file_menu.button("Load File",file_load,file_handler);
  file_menu.button("Save File",file_save,file_handler);
  file_menu.button("Load/Save",file_all,file_handler);
  file_menu.button("Exit",file_exit);

  menu dummy_menu;
  dummy_menu.button("button1");
  dummy_menu.button("button2");
  dummy_menu.button("button3");
  dummy_menu.button("button4");

  window W(500,600);
  win_ptr = &W;

  W.menu_button("File", file_menu);
  W.menu_button("Edit", dummy_menu);
  W.menu_button("Help", dummy_menu);

  W.display(window::center,window::center);

  while (W.read_mouse() != file_exit);

  return 0;
}

