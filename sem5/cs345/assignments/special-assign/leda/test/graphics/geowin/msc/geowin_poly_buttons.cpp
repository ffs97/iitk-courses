/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_poly_buttons.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>
#include <LEDA/pixmaps/button32.h>

using namespace leda;

int MODE;

static void f_unite( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];
  rat_gen_polygon p3=p1.unite(p2);
  Lout.append(p3);
}

static void f_intersec( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];
  rat_gen_polygon p3=p1.intersection(p2);
  Lout.append(p3);
}

static void f_diff( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];
  rat_gen_polygon p3=p1.diff(p2);
  Lout.append(p3);
}

static void f_sym_diff( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  rat_gen_polygon p1=Lin[Lin[0]];
  rat_gen_polygon p2=Lin[Lin[1]];
  rat_gen_polygon p3=p1.sym_diff(p2);
  Lout.append(p3);
}

static void poly_operation( const list<rat_gen_polygon>& Lin, list<rat_gen_polygon>& Lout)
{
  Lout.clear(); 
  if( Lin.length() < 2 ) return;
  switch(MODE){
  case 1: { f_intersec(Lin,Lout); break; }
  case 2: { f_unite(Lin,Lout); break; }
  case 3: { f_diff(Lin,Lout); break; }
  case 4: { f_sym_diff(Lin,Lout); break; }
  }
}

geo_scene sc1,sc2;

void geo_call(GeoWin& gw, int w,  string label) 
{
  MODE=w;

  switch(MODE){
  case 1: { gw.message("Intersection"); sc2->update(); gw.redraw(); break; }
  case 2: { gw.message("Union"); sc2->update(); gw.redraw(); break; }
  case 3: { gw.message("Difference"); sc2->update(); gw.redraw(); break; }
  case 4: { gw.message("Symdiff"); sc2->update(); gw.redraw(); break; }
  case 5: {
     gw.message("Point Inside test on the first polygon of the input scene"); 
     list<rat_gen_polygon> L; 
     gw.get_objects(sc1,L);
     if (L.length() < 1) break;
     // inside test
     rat_gen_polygon p1=L[L[0]];
     int i;

     int x0 = (int)gw.get_window().xmin();
     int x1 = (int)gw.get_window().xmax();
     int y0 = (int)gw.get_window().ymin();
     int y1 = (int)gw.get_window().ymax();

     for (i=0;i<2000;i++) { 
           rat_point p(rand_int(x0,x1),rand_int(y0,y1),1);
           if (p1.inside(p))  gw.get_window().draw_disc(p.to_point(),1,red);
           else gw.get_window().draw_disc(p.to_point(),1,blue);
     }

  }
  } 
}

int main()
{
  list<rat_gen_polygon>* H1;

  GeoWin gw("Input 2 polygons !"); 
  MODE=1; //..intersection

  list<rat_gen_polygon> IN;
  sc1 = gw.new_scene(IN);
  gw.set_fill_color(sc1,invisible);

  sc2 = gw.new_scene(poly_operation,sc1,string("Poly Operation"),H1,H1);
  gw.set_color( sc2, blue );
  gw.set_color2( sc2, blue ); 
  gw.set_fill_color( sc2, green);
  gw.set_visible( sc2, true);  

  gw.init_menu();

  // add some pixmap buttons
  char* pr1  = gw.get_window().create_pixrect(intersect_xpm);
  char* pr2  = gw.get_window().create_pixrect(puzzle_xpm);
  char* pr3  = gw.get_window().create_pixrect(scissors_xpm);
  char* pr4  = gw.get_window().create_pixrect(intersect2_xpm);
  char* pr5  = gw.get_window().create_pixrect(watch_xpm);

  GeoFunction<int> f1(1,"");
  GeoFunction<int> f2(2,"");
  GeoFunction<int> f3(3,"");
  GeoFunction<int> f4(4,"");
  GeoFunction<int> f5(5,"");

  GeoFunctions* fp1= &f1;  GeoFunctions* fp2= &f2; GeoFunctions* fp3= &f3;
  GeoFunctions* fp4= &f4;  GeoFunctions* fp5= &f5;

  gw.add_button_call(fp1,"Intersection",&gw.get_window(),pr1);
  gw.add_button_call(fp2,"Union",&gw.get_window(),pr2);
  gw.add_button_call(fp3,"Difference",&gw.get_window(),pr3);
  gw.add_button_call(fp4,"Symdiff",&gw.get_window(),pr4);
  gw.add_button_call(fp5,"Inside",&gw.get_window(),pr5);

  gw.edit(sc1);

  return 0;
}
