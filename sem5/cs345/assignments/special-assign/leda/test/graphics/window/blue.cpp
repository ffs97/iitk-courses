/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  blue.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/list.h>
#include <LEDA/graphics/window.h>

#include <ctype.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
using std::ifstream;
#endif




int main(int argc, char** argv)
{  

   //ifstream colors("/usr/lib/X11/rgb.txt");
   ifstream colors("/usr/openwin/lib/rgb.txt");

   int r,g,b;
   char c;
   string s;

   string pat = "blue";

   if (argc > 1) pat = argv[1];

   list<string> L;

   while (colors)
   { 
     colors >> c;
     if (!isdigit(c)) 
     { read_line(colors);
       continue;
      }
     colors.putback(c);

     colors >> r >> g >> b;

     do colors >> c; while (isspace(c));
     colors.putback(c);

     s = read_line(colors);

     if (s.pos(pat) != -1) 
     { string s1("#%02x%02x%02x",r,g,b);
       L.append(s1);
       cout << s1 << "  " << s << endl;
      }
    }
   cout << endl;

   int n = L.length();

   window W(500,500);

   W.init(0,n,0);
   W.set_show_coordinates(false);
   W.display();

   int i = 0;
   forall(s, L) 
   { //cout << s << endl;
     W.draw_filled_rectangle(i,0,i+1,n,color(~s));
     //W.draw_rectangle(i,0,i+1,n,black);
     i++;
    }
    cout << i << endl;

    W.read_mouse();

  return 0;
}
  
