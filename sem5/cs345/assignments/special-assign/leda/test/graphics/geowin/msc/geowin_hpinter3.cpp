/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_hpinter3.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/array.h>
#include<LEDA/map.h>
#include<LEDA/rat_geo_alg.h>

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


list<POINT> LOWER_PART(const list<POINT>& hull)
{
 list<POINT> lower;
 list_item left,right,it;
 
 if (hull.empty()) return lower;
 left= hull.first(); right=hull.first();
 
 forall_items(it,hull){
    if (hull[it].xcoord() < hull[left].xcoord()) 
    { left=it; }
    if (hull[it].xcoord() > hull[right].xcoord()) 
    { right=it; }
 }
 
 for(it=left; it!=right; it=hull.cyclic_succ(it))
 {
    lower.append(hull[it]);
 }
 lower.append(hull[it]);
 
 return lower; 
}

list<POINT> UPPER_PART(const list<POINT>& hull)
{
 list<POINT> upper;
 list_item left,right,it;
 
 if (hull.empty()) return upper;
 left= hull.first(); right=hull.first();
 
 forall_items(it,hull){
    if (hull[it].xcoord() < hull[left].xcoord()) 
    { left=it; }
    if (hull[it].xcoord() > hull[right].xcoord()) 
    { right=it; }
 }
 
 for(it=right; it!=left; it=hull.cyclic_succ(it))
 {
    upper.append(hull[it]);
 }
 upper.append(hull[it]);
 
 return upper; 
}



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

bool get_dual(const LINE& l,POINT& pd)
{
 POINT p1 = l.point1();
 POINT p2 = l.point2();
 POINT swp;
 bool flag;

 if (p1.xcoord() > p2.xcoord()) { swp=p1; p1=p2; p2=swp; flag=false; }
 else flag=true;

 NUMBER2 a,b,x1,x2,y1,y2;
 x1=p1.xcoord(); y1=p1.ycoord();
 x2=p2.xcoord(); y2=p2.ycoord();
 a=(y2-y1)/(x2-x1);
 //cout << "a:" << a << "\n";
 b=y2-a*x2;
 //cout << "b:" << b << "\n";
 //cout << POINT(-a,b) << "\n";
 pd=POINT(-a,b);
 return flag; 
}

bool handle_verticals(const list<LINE>& VL,NUMBER2 left,NUMBER2 right, bool cl, bool cr)
{
 LINE lakt;
 forall(lakt,VL)
 {
  NUMBER2 y1 = lakt.point1().ycoord();
  NUMBER2 y2 = lakt.point2().ycoord();
  NUMBER2 x  = lakt.point1().xcoord();

  if (y1<y2)
  { //halfplane open right
    if (!cl) { cl=true; left=x; }
    else if (x>left) left=x;
  }
  else
  { //halfplane open left
    if (!cr) { cr=true; right=x; }
    else if (x<right) right=x;
  }
 }

 if (cl && cr && left<=right) return true; // closed left and right
 if (!cl && !cr) return true;  // no verticals
 if (!cl && cr) return true;   // open left 
 if (cl && !cr) return true;   // open right
 return false; //verticals, but intersection empty ...
}

void build_lists(const list<LINE>& Lin, list<LINE>& LL, list<SEGMENT>& LS)
{
  if (Lin.size() ==0 ) return;
  if (Lin.size() ==1 ) { LL.append(Lin.head()); return; }
  LL.append(Lin.head()); LL.append(Lin.tail());
  if (Lin.size() ==2 ) return; 
  
  // size >= 3 ...
  list_item it;
  LINE  l_akt, l_next;
  POINT prev,akt;

  forall_items(it,Lin) {
    prev=akt;
    l_akt=Lin[it];
    l_next=Lin[Lin.cyclic_succ(it)];
    l_next.intersection(l_akt,akt); 

    if (it != Lin.last() && it != Lin.first()) { // build a segment ...
      LS.append(SEGMENT(prev,akt));
    }
  }  
}

void hp_inter_dual(const list<LINE>& Lin, list<SEGMENT>& Lseg)
{
 //Lout1.clear();
 list<LINE> Lout1; 
 list<LINE> Lout2;

 if (Lin.size() < 1) return;
 list<LINE> Vert; // vertical lines ...
 list<LINE> Ln;   // other lines ...

 LINE lakt;
 forall(lakt,Lin) {
  if (lakt.is_vertical()) Vert.append(lakt);
  else Ln.append(lakt);
 }

 // handle the vertical lines ...
 bool closed_right = false;
 bool closed_left = false;
 NUMBER2 p_left,p_right;
 
 bool something=handle_verticals(Vert, p_left, p_right, closed_left, closed_right);

 POINT Hp;
 list<POINT> Hcand_lower;
 list<POINT> Hcand_upper;
 map<POINT,LINE> M1;
 map<POINT,LINE> M2; 

 forall(lakt,Ln) {
   bool flag = get_dual(lakt,Hp);

   if (flag) {
    Hcand_lower.append(Hp);
    M1[Hp]=lakt;
   }
   else {
    Hcand_upper.append(Hp);
    M2[Hp]=lakt;
   }
 }

 list<POINT> output1 = CONVEX_HULL(Hcand_lower);
 list<POINT> output2 = CONVEX_HULL(Hcand_upper);

 list<POINT> lower = LOWER_PART(output1);
 list<POINT> upper = UPPER_PART(output2);
 
 forall(Hp,lower){
   //cout << "   " << Hp.to_point() << "\n";
   //cout << M1[Hp] << "\n";
   Lout1.append(M1[Hp]);
 }
 forall(Hp,upper){
   //cout << "   " << Hp.to_point() << "\n";
   //cout << M1[Hp] << "\n";
   Lout2.append(M2[Hp]);
 } 

 list<LINE> Lline1,Lline2;  //begrenzende Linien (l_first-seg-l_last)
 list<SEGMENT> Lseg1,Lseg2; //begrenzende Segmente

 // build segment and line lists from the output of chull...
 build_lists(Lout1,Lline1,Lseg1);
 build_lists(Lout2,Lline2,Lseg2);

 Lout1.conc(Lout2);
 Lseg.clear();
 Lseg.conc(Lseg1);
 Lseg.conc(Lseg2);
}


int main()
{
 list<LINE>* H1;
 list<rat_polygon>* H2;
 list<rat_segment>* H3;

 GeoWin gw;
 list<LINE> L1;
 geo_scene sc1= gw.new_scene(L1);

 geo_scene res1= gw.new_scene(hp_inter_dual, sc1, string("Halfplane intersection"), H1,H3);
 gw.set_color(res1,red);
 gw.set_visible(res1,true);

 gw.edit(sc1);
 return 1;
}
