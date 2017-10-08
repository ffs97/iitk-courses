/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  file_panel.c
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


enum { FILE_LOAD, FILE_SAVE, FILE_ALL, FILE_EXIT };


string dname = ".";
string fname = "none";

window* win_ptr;


// dummy I/O handler

void read_file(string fn) { cout << "reading file " << fn << endl; }

void write_file(string fn) { cout << "writing file " << fn << endl; }



void file_handler(int what)
{ 
  file_panel FP(*win_ptr,fname,dname);

  switch (what) {
  case FILE_LOAD: FP.set_load_handler(read_file);
                  break;
  case FILE_SAVE: FP.set_save_handler(write_file);
                  break;
  case FILE_ALL:  FP.set_load_handler(read_file);
                  FP.set_save_handler(write_file);
                  break;
  }
  FP.open();
}


int main()
{
  menu file_menu;
  file_menu.button("Load File",FILE_LOAD,file_handler);
  file_menu.button("Save File",FILE_SAVE,file_handler);
  file_menu.button("Load/Save",FILE_ALL, file_handler);
  file_menu.button("Exit",FILE_EXIT);

  menu dummy_menu;
  dummy_menu.button("button1");
  dummy_menu.button("button2");
  dummy_menu.button("button3");

  window W(500,600);
  win_ptr = &W;

  W.button("File", file_menu);
  W.button("Help", dummy_menu);
  W.button("Exit", FILE_EXIT);
  W.make_menu_bar();

  W.display(window::center,window::center);

  while (W.read_mouse() != FILE_EXIT);

  return 0;

}
