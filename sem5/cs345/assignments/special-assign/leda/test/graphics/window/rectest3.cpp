/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rectest3.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/graphics/window.h>
#include <LEDA/geo/rat_rectangle.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int main()
{
 window W;
 W.init(0,400,0,1);
 W.display();

 rat_point p1(50,50,1),p2(200,200,1);
 rat_rectangle r(p1,p2);

 W << r;

 rat_point p;
 int counter=0;

 while(counter<10){
  W >> p;
  cout << r.cs_code(p) << "\n";
  cout << "\n" << r << "\n";
  r=r.include(p);
  W << r;
  counter++; 
 }

 rat_segment inter;

 counter=0;
 rat_line t;

 while(counter<10){
   W >> t;
   bool flag=r.clip(t,inter);
   if (flag) { W.set_color(red); W << inter; W.set_color(black); }
   counter++;
 }


 counter=0;
 rat_ray str;

 while(counter<10){
   W >> str;
   bool flag=r.clip(str,inter);
   if (flag) { W.set_color(green); W << inter; W.set_color(black); }
   counter++;
 }

 return 1;
}
