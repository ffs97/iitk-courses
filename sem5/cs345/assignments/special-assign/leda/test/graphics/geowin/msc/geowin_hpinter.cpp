/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_hpinter.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/geowin.h>
#include<LEDA/array.h>

using namespace leda;

#define POINT    rat_point
#define LINE     rat_line
#define SEGMENT  rat_segment
#define POLYGON  rat_gen_polygon 
#define NUMBER   integer
#define NUMBER2  rational

NUMBER max_dbl((double)10000);
NUMBER min_dbl((double)-10000);

POINT p_xmin_ymin(min_dbl,min_dbl);
POINT p_xmax_ymin(max_dbl,min_dbl);
POINT p_xmax_ymax(max_dbl,max_dbl);
POINT p_xmin_ymax(min_dbl,max_dbl);

// wegen fehlendem x/y _proj. fuer rat_line
SEGMENT LG1(p_xmin_ymin,p_xmin_ymax);
SEGMENT LG2(p_xmin_ymax,p_xmax_ymax);
SEGMENT LG3(p_xmax_ymax,p_xmax_ymin);
SEGMENT LG4(p_xmax_ymin,p_xmin_ymin);

POLYGON compute_poly(const LINE& l)
{
  POLYGON sb;
  int o1,o2,o3,o4;
  list<POINT> poly;
  POINT inter;

  cout << "line:" << l << "\n";

  o1=orientation(l,p_xmin_ymin); 
  if (o1!= -1) poly.push(p_xmin_ymin);

  o2=orientation(l,p_xmin_ymax);
  if (o1 != o2 && o1 != 0 && o2 != 0) {
    //poly.push(point(min_dbl, l.y_proj(min_dbl)));
    l.intersection(LG1,inter);
    poly.push(inter);   
  }
  if (o2 != -1) poly.push(p_xmin_ymax); 

  o3=orientation(l,p_xmax_ymax);
  if (o2 != o3 && o2 != 0 && o3 != 0) {
    //poly.push(point(l.x_proj(max_dbl),max_dbl));
    l.intersection(LG2,inter);
    poly.push(inter);   
  }
  if (o3 != -1) poly.push(p_xmax_ymax); 

  o4=orientation(l,p_xmax_ymin);
  if (o3 != o4 && o3 != 0 && o4 != 0) {
    //poly.push(point(max_dbl,l.y_proj(max_dbl)));
    l.intersection(LG3,inter);
    poly.push(inter);   
  }
  if (o4 != -1) poly.push(p_xmax_ymin);

  if (o1 != o4 && o1 != 0 && o4 != 0) {
    //poly.push(point(l.x_proj(min_dbl),min_dbl));
    l.intersection(LG4,inter);
    poly.push(inter);   
  }

  cout << o1 << " " << o2 << " " << o3 << " " << o4 << "\n";
  cout << "points:" << poly.size() << "\n";  
  cout.flush();
  cout << "Poly" << poly << "\n";
  cout.flush();
  sb= POLYGON(poly);
  cout << "gen_polygon constructed!\n";
  cout.flush();
  return sb;
}

POLYGON halfplane_intersection(const array<LINE>& AL, int left, int right)
{
 if (right-left > 0) { // split...
   int m;
   if (right-left > 1) {
    m= (right-left)%2;
    m= left+m;
   }
   else { m=left; }

   cout << "left/right:" << left << " " << right << "\n";
   cout << "m:" << m << "\n";
   cout.flush();
   
   POLYGON gp1= halfplane_intersection(AL,left,m);
   POLYGON gp2= halfplane_intersection(AL,m+1,right);
   cout << "vor intersection!\n"; cout.flush();
   return gp1.intersection(gp2);   
 }
 else {
  // compute a gen_polygon from a line ...
  // left == right

  cout << "vor compute_poly!\n"; cout.flush();
  POLYGON gb = compute_poly(AL[left]);
  cout << "nach compute_poly!\n"; cout.flush();

  return gb;
 }

}

void hp_inter(const list<LINE>& Lin, list<rat_polygon>& Lout)
{
 if (Lin.size() < 1) return;
 int HP_NUMB=Lin.size(),i;
 array<LINE> L(HP_NUMB);
 LINE lakt;
 i=0;
 forall(lakt,Lin) { L[i]=lakt; i++; }

 POLYGON GP = halfplane_intersection(L,0,HP_NUMB-1);
 
 Lout = GP.polygons();
}

POINT get_dual(const Line& l)
{
 POINT p1 = l.point1();
 POINT p2 = l.point2();

 NUMBER2 a,b,x1,x2,y1,y2;
 x1=p1.xcoord(); y1=p1.ycoord();
 x2=p2.xcoord(); y2=p2.ycoord();
 a=(y2-y1)/(x2-x1);
 b=y2-a*x2;

 return POINT(-a,b); 
}

void hp_inter(const list<LINE>& Lin, list<rat_polygon>& Lout)
{
 if (Lin.size() < 1) return;
 list<LINE> Vert; // vertical lines ...
 list<LINE> Ln;   // other lines ...

 LINE lakt;
 forall(lakt,Lin) {
  if (lakt.is_vertical()) Vert.append(lakt);
  else Ln.append(lakt);
 }

 POINT Hp;
 list<POINT> Hcand;

 forall(lakt,Ln) {
   Hp= get_dual(lakt);
   Hcand.append(Hp);
 }
}


int main()
{
 list<LINE>* H1;
 list<rat_polygon>* H2;

 GeoWin gw;
 list<LINE> L1;
 geo_scene sc1= gw.new_scene(L1);

 geo_scene res1= gw.new_scene(hp_inter, sc1, string("Halfplane intersection"), H1,H2);
 gw.set_fill_color(res1,red);
 gw.set_visible(res1,true);

 gw.edit(sc1);
 return 1;
}
