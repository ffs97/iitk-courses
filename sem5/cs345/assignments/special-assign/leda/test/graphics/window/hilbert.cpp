/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  hilbert.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

window W;

double x, y, dx, dy;

void A(int);
void B(int);
void C(int);
void D(int);


void plot(double new_x, double new_y)
{ W.draw_segment(x,y,new_x,new_y);
  x = new_x;
  y = new_y;
 }


void A(int i)
{ 
  if (i > 0)
  { D(i-1); plot(x-dx,y);
    A(i-1); plot(x,y-dy);
    A(i-1); plot(x+dx,y); 
    B(i-1);
   }
 }

void B(int i)
{ 
  if (i > 0)
  { C(i-1); plot(x,y+dy);
    B(i-1); plot(x+dx,y);
    B(i-1); plot(x,y-dy); 
    A(i-1);
   }
 }



void C(int i)
{ 
  if (i > 0)
  { B(i-1); plot(x+dx,y);
    C(i-1); plot(x,y+dy);
    C(i-1); plot(x-dx,y); 
    D(i-1);
   }
 }

void D(int i)
{ 
  if (i > 0)
  { A(i-1); plot(x,y-dy);
    D(i-1); plot(x-dx,y);
    D(i-1); plot(x,y+dy); 
    C(i-1);
   }
 }


int n = 5;

void hilbert()
{
   double lx = W.xmax() - W.xmin();
   double ly = W.ymax() - W.ymin();

   double x0 = W.xmin() + 0.98*lx;
   double y0 = W.ymin() + 0.98*ly;

   dx = 0.96 * lx/(1 << n);
   dy = 0.96 * ly/(1 << n);

   x = x0;
   y = y0;

   A(n);

   W.draw_segment(x0,y0,x0+dx,y0);
   W.draw_segment(x0+dx,y0,x0+dx,y);
   W.draw_segment(x0+dx,y,x,y);

  }


int main()
{   

 W.open();

 panel P("hilbert curve");
 P.int_item("n = ",n,1,10);

 P.button("continue",1);
 P.button("quit",0);

 W.set_redraw(hilbert);

 while (P.open() != 0)
 { W.clear();
   hilbert();
   W.read_mouse();
  }
 
  return 0;
}
