/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_label.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


// Label test program ...

#include <LEDA/geowin.h>

using namespace leda;

// function for computing position
void pfcn(double x1, double x2, double y1, double y2, void* obj_ptr,bool b, double& xp, double& yp)
{ xp = x1; yp = y1; }

void pfcn_scene(double x1, double x2, double y1, double y2, void* scn, bool b, double& xp, double& yp)
{
  //cout << "ymin/ ymax:" << y1 << "/" << y2 << "\n";
  xp = (x1+x2)/2;
  yp = (y1+y2)/2;  
}

int main() {
  GeoWin gw;
  
  list<polygon> LP;
  list<point> LP1, LP2, LP3;
  
  point pa1(0,0),pa2(20,10),pa3(10,50);
  point pb1(100,100),pb2(180,100),pb3(140,140);
  point pc1(200,10),pc2(250,40),pc3(220,100);
  
  LP1.append(pa1); LP1.append(pa2); LP1.append(pa3); 
  LP2.append(pb1); LP2.append(pb2); LP2.append(pb3);
  LP3.append(pc1); LP3.append(pc2); LP3.append(pc3);
  
  polygon pol1(LP1);
  polygon pol2(LP2);
  polygon pol3(LP3);  
  
  LP.append(pol1); LP.append(pol2); LP.append(pol3);
  
  GeoBaseScene<list<polygon> >* sc1 = gw.new_scene(LP);
  
  // scene texts ...
  geowin_text t1("Text 1", 0,0, italic_font, 14, "", black );
  t1.set_position_fcn(pfcn);
  
  geowin_text t2("Text 2", 0,0,bold_font, 10, "", red);
  geowin_text t3("Changed Label", 5,0,bold_font, 20, "", green);
  geowin_text t4("Szenentext1", 30,120, roman_font, 10, "", blue);
  geowin_text t5("Middle of the window", 0, 0, bold_font, 20, "", blue2);
  t5.set_position_fcn(pfcn_scene);
  
  // setting labels
  list<polygon>::iterator it = LP.begin();
  //gw.set_obj_label(sc1, it, "Hallo 1");
  gw.set_obj_text(sc1, it++, t1);
  //gw.set_obj_label(sc1, it, "Hallo 2");
  gw.set_obj_text(sc1,it++,t2);
  //gw.set_obj_label(sc1, it, "Hallo 3");
  gw.set_obj_text(sc1,it++,t3);
  
  // set label - new variant
  //gw.set_obj_label(sc1, pol2, "Polygon 2");
  //gw.set_obj_label(sc1, pol3, "Polygon 3");  
  // get label ..
  //string lb;
  //gw.get_obj_label(sc1, pol3, lb);
  //cout << lb << "\n";
  
  gw.add_text(sc1, t4); gw.add_text(sc1, t5);
  
  
  gw.edit(sc1);
  
  return 0;
}
