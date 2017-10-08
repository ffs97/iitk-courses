/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  panel.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/graphics/bitmaps/button32.h>
#include <LEDA/system/file.h>

using namespace leda;

using std::cout;
using std::endl;


static unsigned char* bm_bits [] = {
  triang_bits,
  voro_bits,
  empty_circle_bits,
  encl_circle_bits,
  grid_bits,
  hull_bits,
};


int main()
{ 
  bool   B     = false;
  color  col   = blue2;
  double R     = 3.1415;
  int    c     = 0;
  int    c1    = 0;
  int    c2    = 0;
  int    cm    = 0;
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


  for(;;)
  {
    panel P("PANEL DEMO");

    P.load_button_font("lucidasans-12");
    //P.load_button_font("B12");

    string text;
    text += " The panel section of a window is used for displaying text";
    text += " messages and for updating the values of variables. It consists";
    text += " of a list of panel items and a list of buttons.";
    text += " The operations in this section add panel items or buttons to ";
    text += " the panel section of W. Note that they have to be called before";
    text += " the window is displayed the first time.";
  
    P.text_item(text);
    P.text_item("");
  
    P.bool_item("bool_item",B);
    /* panel_item it1 = */ P.color_item("color_item",col);
    P.int_item("int_item(1,20)",but_num,-1,20);

    P.string_item("string_item(M1)",s0,M1,8);
    P.string_item("string_item(M2)",s1,M2);
    P.string_item("string_item",s);

    P.int_item("int_item",N);
    P.double_item("double_item",R);
    P.choice_item("choice_item",c,"zero","gone","two","three", "four");
    P.choice_mult_item("choice_mult_item",cm,CML);
    P.int_item("int_item(0,80,20)",c1,0,80,20);
    /* panel_item it2 = */ P.choice_item("bitmap",c2,6,32,32,bm_bits);
  
    int i;
    for(i=0; i < but_num; i++) P.button(string("%d",i));
  
    P.button("quit");
    //P.button(19,19,c_bits,"hallo");

    P.display();

/*
    panel_item it;
    forall_items(it,P) 
    { string l = P.get_label(it);
      if (l != "" && Yes(string("Disable Item %s ?",~l)))
         P.disable_item(it);
     }
*/
  
    int button = P.open();

    cout << "cm = " << cm << endl;
    cout << endl;
    cout << "button = " << button << endl;
    cout << endl;
  
    if (button == i) break;
   }
 
  return 0;
}
  
