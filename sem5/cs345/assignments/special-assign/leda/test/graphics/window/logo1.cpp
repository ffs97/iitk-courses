/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  logo1.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <math.h>
#include <fstream.h>

using namespace leda;

window W(600,250);

color col;
color bg_col;
color li_col;

bool  draw_arc;
bool  base_line;

int   lw;
int   DIST;
int   THICK;
int   REFINE;

void set_defaults()
{ col       = blue;
  bg_col    = grey2;
  li_col    = white;
  draw_arc  = false;
  base_line = false;
  lw        = 1;
  DIST      = 32;
  THICK     = 19;
  REFINE    = 400;
}


#if !defined(M_PI)
#define	M_PI 3.14159265358979323846
#endif


void poly_arc(point p, point q, point c, list<point>& pol)
{ segment s0(c,p);
  segment s1(c,q);
  double delta = M_PI/REFINE;
  double r = s0.length();
  double a;
  if (s0.angle() < s1.angle())
    for(a=s0.angle(); a <= s1.angle(); a+=delta) 
         pol.append(c.translate_by_angle(a,r));
  else
    for(a=s0.angle(); a >= s1.angle(); a-=delta) 
         pol.append(c.translate_by_angle(a,r));
 }

  


void draw_logo(ostream& out)
{
   W.set_mode(src_mode);
   W.set_line_width(lw);

   double d    = 0.01*THICK;
   double d1   = 0.95 * d;
   double eps  = 0.001*DIST;
   double eps2 = 1.75*eps;

   circle c1(0,-1,sqrt(5.0));
   circle c2(0,-1,sqrt(5.0)-d);
   circle c3(0,0,1);
   circle c4(0,0,1-d);

   point A,B,C,D,E,F,G,H,I,J;
   segment s0,s1,s2;

// W.draw_disc(c1,col);
// W.draw_disc(c2,bg_col);

   if (draw_arc)
   { W.set_line_width(1);
     W.draw_circle(c1,li_col);
     W.draw_circle(c2,li_col);
     W.set_line_width(lw);
    }

   W.draw_box(-10,-10,10,0,bg_col);

   list<point> pol;

   double R0 = sqrt(5.0);
   double R1 = sqrt(5.0)-d;

   // L

   s0 = segment(-1-eps2,0,-1-eps2,2);
   s1 = segment(-2,d,0,d);

   A = c1.intersection(s0).head(); 
   B = c2.intersection(s0).head(); 
   C = c2.intersection(s1).head(); 
   D = point(-1-eps2,d);
   E = point(-1-eps2,0);
   F = point(-2,0);

   pol.append(A);
   pol.append(B);
   poly_arc(B,C,point(0,-1),pol);
   pol.append(C);
   pol.append(D);
   pol.append(E);
   pol.append(F);
   poly_arc(F,A,point(0,-1),pol);
   
   W.draw_filled_polygon(pol,col);
   if (lw > 0) W.draw_polygon(pol,li_col);

   out << pol << endl;


   // E


/*
   s0 = segment(0,0.6,-2,0.6);
   s1 = segment(0,0.4,-2,0.4);
   s2 = segment(0,0.2,-2,0.2);
*/

   double sp = (1 - 2*d - d1)/2;
   double sp1 = 0.9*sp;

   s0 = segment(0,d+sp1+d1,-2,d+sp1+d1);
   s1 = segment(0,d+sp1,-2,d+sp1);
   s2 = segment(0,d,-2,d);

   A = point(-eps,1);
   B = point(-eps,1-d);
   C = c4.intersection(s0).head();
   D = point(-eps,C.ycoord());
   E = point(-eps,C.ycoord()-d1);
   F = c4.intersection(s1).head();
   G = c4.intersection(s2).head();
   H = point(-eps,G.ycoord());
   I = point(-eps,0);
   J = point(-1,0);

   pol.clear();

   pol.append(A);
   pol.append(B);
   poly_arc(B,C,point(0,0),pol);
   pol.append(C);
   pol.append(D);
   pol.append(E);
   pol.append(F);
   poly_arc(F,G,point(0,0),pol);
   pol.append(G);
   pol.append(H);
   pol.append(I);
   pol.append(J);
   poly_arc(J,A,point(0,0),pol);

   W.draw_filled_polygon(pol,col);
   if (lw > 0) W.draw_polygon(pol,li_col);

   out << pol << endl;


   // D

   s0 = segment(0,d,2,d);
   s1 = segment(d+eps,0,d+eps,2);

   A = c4.intersection(s0).head();
   B = c4.intersection(s1).head();
   C = point(d+eps,d);

   D = point(1,0);
   E = point(eps,1);
   F = point(eps,0);

   pol.clear();
   pol.append(D);
   poly_arc(D,E,point(0,0),pol);
   pol.append(E);
   pol.append(F);
   W.draw_filled_polygon(pol,col);
   if (lw > 0) W.draw_polygon(pol,li_col);

   pol.reverse_items();
   out << pol << endl;

   pol.clear();
   pol.append(A);
   poly_arc(A,B,point(0,0),pol);
   pol.append(B);
   pol.append(C);
   W.draw_filled_polygon(pol,bg_col);
   if (lw > 0) W.draw_polygon(pol,li_col);

   out << pol << endl;

   // A

   s0 = segment(1+eps2,0,1+eps2,2);
   s1 = segment(0,d,2,d);

   A = point(2,0);
   B = c1.intersection(s0).head();
   C = point(1+eps2,0);
   D = point(1+eps2+d,0);
   E = point(1+eps2+d,d);
   F = c2.intersection(s1).head();
   s1 = segment(0,0,2,0);
   G = c2.intersection(s1).head();

   pol.clear();
   pol.append(A);
   poly_arc(A,B,point(0,-1),pol);
   pol.append(B);
   pol.append(C);
   pol.append(D);
   pol.append(E);
   pol.append(F);
   poly_arc(F,G,point(0,-1),pol);
   pol.append(G);
   W.draw_filled_polygon(pol,col);
   if (lw > 0) W.draw_polygon(pol,li_col);

   pol.reverse_items();
   out << pol << endl;

   s0 = segment(0,d+d1,2,d+d1);
   s1 = segment(1+eps2+d,0,1+eps2+d,2);

   A = c2.intersection(s0).head();
   B = c2.intersection(s1).head();
   C = point(1+eps2+d,d+d1);

   pol.clear();
   pol.append(A);
   poly_arc(A,B,point(0,-1),pol);
   pol.append(B);
   pol.append(C);
   W.draw_filled_polygon(pol,bg_col);
   if (lw > 0) W.draw_polygon(pol,li_col);

   out << pol << endl;

   if (base_line) W.draw_segment(2,0,-2,0);


   // bounding box

   pol.clear();
   pol.append(point(-2-d, -d));
   pol.append(point( 2+d, -d));
   pol.append(point( 2+d,1+d));
   pol.append(point(-2-d,1+d));

   out << pol << endl;

}

void draw_logo()
{ ofstream out("logo.poly");
  draw_logo(out);
 }



int main()
{
   set_defaults();

   panel P("LEDA Logo");

   P.color_item("fg color",col);
   P.color_item("bg color",bg_col);
   P.color_item("line color",li_col);
   P.int_item  ("l_width",lw,0,5,1);
   P.int_item  ("vertices",REFINE,1,500);
   P.int_item  ("distance",DIST,1,250);
   P.int_item  ("thickness",THICK,1,40);
   P.bool_item ("draw arc",draw_arc);
   P.bool_item ("base line",base_line);

   P.button("ok",0);
   P.button("default",1);
   P.button("quit",2);

   W.display();
   W.set_redraw(draw_logo);
   W.init(-2.5,+2.5,-0.5);

   for(;;)
   { W.set_bg_color(bg_col);
     W.clear();
     draw_logo();
     W.read_mouse();
     int but = P.open();
     if (but == 1) set_defaults();
     if (but == 2) break;
    }


   return 0;
}
