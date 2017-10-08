/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  panel_demo_handle_obj.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/system/file.h>
#include <LEDA/graphics/bitmaps/button32.h>

using namespace leda;

static unsigned char* bm_bits [] = {
  triang_bits,
  voro_bits,
  empty_circle_bits,
  encl_circle_bits,
  grid_bits,
  hull_bits,
};

// Action objects ...

class bool_action : public window_handler {
public:
  void operator()(int val) 
  { cout << "Bool handler:" << val << "\n"; }
};


class choice_action : public window_handler {
public:
  void operator()() 
  { cout << "Choice handler:" << get_int() << "\n"; }
};


class color_action : public window_handler {
public:
  void operator()() 
  { cout << "Color handler:" << get_int() << "\n"; }
};

class style_action : public window_handler {
public:
  void operator()() 
  { cout << "My action:" << get_int() << "\n"; }
};


class string_action : public window_handler {
public:
  void operator()() 
  { cout << "String handler:" << get_char_ptr() << "\n"; }
};



int main()
{ 
  bool   B     = false;
  color  col   = blue2;
  double R     = 3.1415;
  int    c     = 0;
  int    c1    = 0;
  int    c2    = 0;
  int    cm    = 5;
  int    N     = 100;
  string s0    = "string0";
  string s     = "dummy";
  string s1    = "menu";

  int but_num = 5;

  list<string> M1 = get_files(".");
  list<string> M2 = get_files(".");


  list<string> CML;
  CML.append("0");
  CML.append("1");
  CML.append("2");
  CML.append("3");
  CML.append("4");

  bool_action    b_action;  
  string_action  s_action;
  color_action   c_action;
  
  style_action   ps_action;
  style_action   ls_action;
  style_action   lw_action;
  style_action   int_action;
  style_action   but_action;
  
  choice_action  ch_action;

  point_style ps;
  line_style  ls;
  int         lw;


  for(;;)
  {
    panel P("PANEL DEMO");

    P.load_button_font("lucidasans-12");

    string text;
    text += " The panel section of a window is used for displaying text";
    text += " and for updating the values of variables. It consists";
    text += " of a list of panel items and a list of buttons.";
    text += " All operations adding panel items or buttons to the panel";
    text += " section of a window have to be called before";
    text += " the window is displayed for the first time.";
  
    P.text_item("");
    P.text_item("\\bf\\blue A Text Item");
    P.text_item("");
    P.text_item(text);
  
    P.text_item("");
    P.text_item("\\bf\\blue A Bool Item");
    P.bool_item("bool item",B, b_action);

    P.text_item("\\bf\\blue A Color Item");
    P.color_item("color item",col, c_action);

    P.text_item("");
    P.text_item("\\bf\\blue A Slider Item");
    P.int_item("slider item(1,20)",but_num,-1,20, int_action);

    P.text_item("");
    P.text_item("\\bf\\blue Simple Items");
    P.string_item("string item",s,s_action);
    P.int_item("int item",N);
    P.double_item("double item",R);
    
    P.text_item("");
    P.pstyle_item("Point style", ps, ps_action);
    P.lstyle_item("Line style", ls, ls_action);
    P.lwidth_item("Line width", lw, lw_action);

    P.text_item("");
    P.text_item("\\bf\\blue String Menu Items");
    P.string_item("string menu",s0,M1,8,s_action);

    P.text_item("");
    P.text_item("\\bf\\blue Choice Items");
    P.choice_item("simple choice",c,"one","two","three","four","five");
    P.choice_mult_item("multiple choice",cm,CML,ch_action);
    P.int_item("integer choice",c1,0,80,20,int_action);
    P.choice_item("bitmap choice",c2,6,32,32,bm_bits,ch_action);

    P.text_item("");
    P.text_item("\\bf\\blue Buttons");
  
    int i;
    int but;
    for(i=0; i < but_num; i++) but = P.button(string("button %d",i));
    
    P.set_object(but, but_action);
    
    P.button("new", 50, but_action);
    P.fbutton("fnew", 100, but_action);
  
    P.display();

    if (P.open() == 0) break;
  }
 
  return 0;
}
  
