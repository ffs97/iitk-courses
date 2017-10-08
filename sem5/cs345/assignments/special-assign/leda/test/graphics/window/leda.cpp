/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  leda.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graph/graph.h>
#include <LEDA/system/stream.h>
#include <LEDA/core/array.h>
#include <LEDA/graphics/window.h>
#include <math.h>

using namespace leda;


typedef GRAPH<vector,int> polyhedron;

void rotate(float alpha1,float alpha2, vector& p)
{ 
  // rotate 3d-point p about the origin
  // by alpha2 in yz-plane and alpha1 in xy-plane

    double R  = hypot(p[1],p[2]);
    double phi = asin(p[1]/R);
  
    if (p[2] < 0) phi = LEDA_PI - phi;
  
    p[1]  = ((R != 0) ? R*sin(phi+alpha2) : 0);
    p[2]  = ((R != 0) ? R*cos(phi+alpha2) : 0);

    R   = hypot(p[0],p[2]);
    phi = asin(p[0]/R);

    if (p[2] < 0) phi = LEDA_PI - phi;
  
    p[0]  = ((R != 0) ? R*sin(phi+alpha1) : 0);
    p[2]  = ((R != 0) ? R*cos(phi+alpha1) : 0);
}


inline point project(vector p)   // project p into xy-plane
{ return point(p[0],p[1]); }


void draw_poly(window& W, polyhedron& poly, vector trans)
{ edge e;
  forall_edges(e,poly) 
  { point a = project(poly[source(e)] + trans );
    point b = project(poly[target(e)] + trans );
    W.draw_segment(a,b,white);
   }
 }


void make_side(polyhedron& poly, node* L, float z)
{
  L[0]= poly.new_node(vector(-70,-20,z));
  L[1]= poly.new_node(vector(-40,-20,z));
  L[2]= poly.new_node(vector(-40,-10,z));
  L[3]= poly.new_node(vector(-60,-10,z));
  L[4]= poly.new_node(vector(-60,+20,z));
  L[5]= poly.new_node(vector(-70,+20,z));

  poly.new_edge(L[0],L[5]);
  int i;
  for(i = 1; i<=5; i++) poly.new_edge(L[i], L[i-1]);

  L[6] = poly.new_node(vector(-30,-20,z));
  L[7] = poly.new_node(vector( -5,-20,z));
  L[8] = poly.new_node(vector( -5,-10,z));
  L[9] = poly.new_node(vector(-20,-10,z));
  L[10]= poly.new_node(vector(-20, -5,z));
  L[11]= poly.new_node(vector( -5, -5,z));
  L[12]= poly.new_node(vector( -5, +5,z));
  L[13]= poly.new_node(vector(-20, +5,z));
  L[14]= poly.new_node(vector(-20,+10,z));
  L[15]= poly.new_node(vector( -5,+10,z));
  L[16]= poly.new_node(vector( -5,+20,z));
  L[17]= poly.new_node(vector(-30,+20,z));

  poly.new_edge(L[6],L[17]);
  for(i = 7; i<=17; i++) poly.new_edge(L[i], L[i-1]);

  L[18]= poly.new_node(vector( 5,-20,z));
  L[19]= poly.new_node(vector(20,-20,z));
  L[20]= poly.new_node(vector(35,-10,z));
  L[21]= poly.new_node(vector(35,+10,z));
  L[22]= poly.new_node(vector(20,+20,z));
  L[23]= poly.new_node(vector( 5,+20,z));

  poly.new_edge(L[18],L[23]);
  for(i = 19; i<=23; i++) poly.new_edge(L[i], L[i-1]);

  L[24]= poly.new_node(vector(15,-10,z));
  L[25]= poly.new_node(vector(20,-10,z));
  L[26]= poly.new_node(vector(25, -5,z));
  L[27]= poly.new_node(vector(25, +5,z));
  L[28]= poly.new_node(vector(20,+10,z));
  L[29]= poly.new_node(vector(15,+10,z));

  poly.new_edge(L[24],L[29]);
  for(i = 25; i<=29; i++) poly.new_edge(L[i], L[i-1]);

  L[30]= poly.new_node(vector(40,-20,z));
  L[31]= poly.new_node(vector(50,-20,z));
  L[32]= poly.new_node(vector(55, -5,z));
  L[33]= poly.new_node(vector(65, -5,z));
  L[34]= poly.new_node(vector(70,-20,z));
  L[35]= poly.new_node(vector(80,-20,z));
  L[36]= poly.new_node(vector(65, 20,z));
  L[37]= poly.new_node(vector(55, 20,z));

  poly.new_edge(L[30],L[37]);
  for(i = 31; i<=37; i++) poly.new_edge(L[i], L[i-1]);

  L[38]= poly.new_node(vector(55, 0,z));
  L[39]= poly.new_node(vector(65, 0,z));
  L[40]= poly.new_node(vector(60,15,z));
  poly.new_edge(L[38],L[40]);
  poly.new_edge(L[39],L[38]);
  poly.new_edge(L[40],L[39]);

}



void make_logo(polyhedron& poly)
{ 
  node L[41];
  node R[41];

  make_side(poly,L,-5);
  make_side(poly,R,+5);

  for(int i = 0; i<41; i++) poly.new_edge(L[i],R[i]);

}



int main(int argc, char** argv)
{ 
  window W(645,480, 
           "The LEDA Platform for Combinatorial and Geometric Computing");

  char* leda_pr;
  if (argc > 1)
    leda_pr = W.create_pixrect(argv[1]);
  else
    leda_pr = W.create_pixrect("../../incl/LEDA/pixmaps/leda.xpm");


  W.init(-120,120,-100);
  W.set_show_coordinates(false);
  W.set_bg_pixrect(leda_pr);

  W.display(window::center,window::center);
  W.put_pixrect(leda_pr);

  W.start_buffering();

  node v;

  int speed = 40;

  // define a polyhedron in 3d space

  polyhedron poly;

  make_logo(poly);

  //W.draw_point(0,0);

  forall_nodes(v,poly) rotate(0.3,0.5,poly[v]);

  vector dir(2);   // direction and speed of rotation

  vector trans0(0,0,0);
  vector trans(0,0,0);

  while (W.get_button() != MOUSE_BUTTON(1))
  { 
    dir[0] += rand_int(-10000,10000)/1000000.0;
    dir[1] += rand_int(-10000,10000)/1000000.0;
    dir = dir.norm()*(speed/500.0);

    //trans[0] += rand_int(-1000,1000)/5000.0;
    //trans[1] += rand_int(-1000,1000)/5000.0;
    //trans[1] += rand_int(-1000,1000)/5000.0;

    forall_nodes(v,poly) rotate(dir[0],dir[1],poly[v]);
 
    W.put_pixrect(leda_pr);
    draw_poly(W,poly,trans);   // draw new position
    W.flush_buffer();
  }

  W.stop_buffering();

 return 0;
}
