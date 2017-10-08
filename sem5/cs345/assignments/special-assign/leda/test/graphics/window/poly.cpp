/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  poly.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geo/rat_polygon.h>
#include <LEDA/graphics/window.h>
#include <fstream.h>

using namespace leda;


polygon add_buffer(const polygon& P, double d)
{
  // enlarges polygon P by a parallel shift of all border segments by d units
  // to the exterior

  list<point> L;

  list<point> V = P.vertices();

  list_item it;
  forall_items(it,V)
  { list_item it_succ = V.cyclic_succ(it);
    list_item it_pred = V.cyclic_pred(it);

    point a = V[it_pred];
    point b = V[it];
    point c = V[it_succ];

    vector v1 = (a-b).norm();
    vector v2 = (c-b).norm();

    point a1 = a.translate(-v1[1]*d,v1[0]*d);
    point b1 = b.translate(-v1[1]*d,v1[0]*d);

    point b2 = b.translate(v2[1]*d,-v2[0]*d);
    point c2 = c.translate(v2[1]*d,-v2[0]*d);

    line L1(a1,b1);
    line L2(b2,c2);

    
    point p;

    if (!L1.intersection(L2,p)) cerr << "Error in add_buffer." << endl;

    vector v3 = p-b;

    if (v3.length() > 2*d)
    { p = b.translate(2*d*v3.norm());
      L.append(b1);
      L.append(p);
      L.append(b2);
     }
    else
      L.append(p);

   }

  return L;
}



bool fit_polygons(const polygon& frame, list<polygon>& L, double d)
{ 

  // transforms the polygons in L so that they form a partition of the
  // frame polygon "frame" by enlarging them (see add_buffer) by d units
  // If d is too small, i.e., frame is not totally covered by the enlarged
  // polygons, the operation return false and otherwise true.

  rat_polygon rframe(frame,256);

  list<rat_polygon> L1;
  polygon P;
  forall(P,L)
  { rat_polygon Q(add_buffer(P,d),256);
    list<rat_polygon> tmp = Q.intersection(rframe);
    if (tmp.length() != 1) error_handler(1,"fit_polygons: internal error 1");
    L1.append(tmp.head());
   }

  L.clear();

  list<rat_polygon> U;
  rat_polygon pol;

  forall(pol,L1)
    U = unite_general_polygons(U,pol);

  U = diff_general_polygons(rframe,U);

  if (U.length() > 0) 
  { forall(pol,U) L.append(pol.to_polygon());
    //forall(pol,L1) L.append(pol.to_polygon());
    return false;
   }
   

  list_item it;
  forall_items(it,L1)
  { list<rat_polygon> tmp;
    tmp.append(L1[it]);
    tmp = diff_general_polygons(tmp,U);
    if (tmp.length() == 0) 
       error_handler(1,string("fit_polygons: |tmp| = %d",tmp.length()));
    while (tmp.length() > 1) 
    { list_item it1 = tmp.first();
      list_item it2 = tmp.succ(it1);
      if (tmp[it1].area() > tmp[it2].area())
        tmp.del_item(it2);
      else
        tmp.del_item(it1);
     }
    U = unite_general_polygons(U,tmp);
    L1[it] = tmp.head();
  }


  forall(pol,L1) L.append(pol.to_polygon());
  return true;
}



// demo program

int main() {

  ofstream outfile("poly.out");

  list<point>  frame;

  frame.append(point(-30,-30));
  frame.append(point(+30,-30));
  frame.append(point(+30,+30));
  frame.append(point(-30,+30));

  int eps = 500;

  list<string> flist;

  flist.append("poly.in1");
  flist.append("poly.in2");
  flist.append("poly.in3");
  flist.append("poly.in4");

  string fname = "poly.in";

  window W;

  W.string_item("file",fname,flist);

  W.int_item("1000*eps", eps, 1, 2000);

  W.button("mouse input",1);
  W.button("read file", 2);
  W.button("write file", 3);
  W.button("run algorithm",  4);
  W.button("exit program", 0);

  W.set_show_coordinates(true);
  W.init(-50,+50,-50);
  W.display();


  list<polygon> L;

  for(;;)
  { int but = W.read_mouse();

    if (but == 0) break;

    
    switch (but) {

    case 1: { L.clear();
              W.clear();
              W.message("Draw list of polygons");
              W.draw_filled_polygon(frame,ivory);
              W.draw_polygon(frame,black);
              for(;;)
              { list<point> plist = W.read_polygon();
                if (plist.empty()) break;
                polygon P(plist,false);
                W.draw_polygon(P,blue);
                L.append(P);
               }
              break;
             }


   case 2: { ifstream in(fname);
             L.clear();
             W.clear();
             W.draw_filled_polygon(frame,ivory);
             W.draw_polygon(frame,black);
             polygon pol;
             while (in >> pol) 
             { L.append(pol);
               W.draw_polygon(pol,blue);
              }
             break;
            }

   case 3: { ofstream out(fname);
             polygon pol;
             forall(pol,L) out << pol;
             break;
            }


   case 4:
          { list<polygon> L1 = L;
            bool success = fit_polygons(frame, L1,eps/1000.0);
            polygon pol;

            W.start_buffering();
            W.clear();
            W.draw_filled_polygon(frame,ivory);
            W.draw_polygon(frame,black);

            if (!success)
            { W.message("Polygons do not cover frame, increase epsilon.");
              forall(pol,L)
                  W.draw_polygon(pol,blue);
              W.set_line_width(3);
             }
  
            int i = 15;
        
            forall(pol,L1)
            { if (++i == 16) i = 2;
              W.draw_filled_polygon(pol,color(i));
              W.draw_polygon(pol,black);
             }
            W.set_line_width(1);
            W.flush_buffer();
            W.stop_buffering();
             break;
           }
  }
           
 }

}
