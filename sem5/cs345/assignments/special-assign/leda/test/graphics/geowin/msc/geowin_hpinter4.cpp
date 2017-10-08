/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin_hpinter4.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/geowin.h>
#include<LEDA/array.h>
#include<LEDA/map.h>
#include<LEDA/rat_geo_alg.h>
#include<LEDA/rat_rectangle.h>
#include<LEDA/rat_vector.h>

using namespace leda;

#define POINT    rat_point
#define LINE     rat_line
#define SEGMENT  rat_segment
#define POLYGON  rat_gen_polygon 
#define RECTANGLE rat_rectangle
#define RAY      rat_ray
#define NUMBER   integer
#define NUMBER2  rational
#define VECTOR   rat_vector


window* win;


list<POINT> LOWER_PART(const list<POINT>& hull)
{
 list<POINT> lower;
 list_item left,right,it;
 
 if (hull.empty()) return lower;
 left= hull.first(); right=hull.first();
 
 forall_items(it,hull){
    if (hull[it].xcoord() < hull[left].xcoord() ||
        (hull[it].xcoord() == hull[left].xcoord() && hull[it].ycoord() < hull[left].ycoord()))
    { left=it; }
    if (hull[it].xcoord() > hull[right].xcoord() ||
        (hull[it].xcoord() == hull[right].xcoord() && hull[it].ycoord() < hull[right].ycoord()))
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
    if (hull[it].xcoord() < hull[left].xcoord() ||
        (hull[it].xcoord() == hull[left].xcoord() && hull[it].ycoord() > hull[left].ycoord()))
    { left=it; }
    if (hull[it].xcoord() > hull[right].xcoord() ||
        (hull[it].xcoord() == hull[right].xcoord() && hull[it].ycoord() > hull[right].ycoord()))
    { right=it; }
 }
 
 for(it=right; it!=left; it=hull.cyclic_succ(it))
 {
    upper.push(hull[it]);
 }
 upper.push(hull[it]);
 
 return upper; 
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

bool handle_verticals(const list<LINE>& VL,NUMBER2& left,NUMBER2& right, bool& cl, bool& cr)
// returns true, if intersection is not empty,
// false otherwise ....
// precondition: VL not empty
{
 cl = false; cr = false;
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
 if (!cl && cr) return true;   // open left 
 if (cl && !cr) return true;   // open right
 return false; //verticals, but intersection empty ...
}

void build_lists(const list<LINE>& Lin, list<RAY>& LR, list<LINE>& LL, list<SEGMENT>& LS)
{
  if (Lin.size() ==0 ) return;
  if (Lin.size() ==1 ) { 
    LL.append(Lin.head()); 
    POINT p1= Lin.head().point1();    
    POINT p2= Lin.head().point2();
    RAY r1(p2,p1);
    VECTOR vec = -(p1.to_vector()-p2.to_vector());
    RAY r2(p2,p2+ vec);
    LR.append(r1);
    LR.append(r2);
    return; 
  }
  LINE l1=Lin.head();
  LINE l2=Lin.tail();
  LL.append(l1); LL.append(l2);
  if (Lin.size() ==2 ) {
    POINT IN;
    VECTOR vec;
    RAY r1,r2;
    l1.intersection(l2,IN);
    POINT p1= l1.point1(); if (p1==IN) p1=l1.point2();
    POINT p2= l2.point1(); if (p2==IN) p2=l2.point2();
    
    if (orientation(l1,p2)<0) { //rightturn
      r2 = RAY(IN,p2);
    }
    else {
      vec= -(p2.to_vector() - IN.to_vector());
      r2 = RAY(IN, IN + vec);
    }
    
    if (orientation(l2,p1)<0) { //rightturn
      r1 = RAY(IN,p1);
    }
    else {
      vec= -(p1.to_vector() - IN.to_vector());
      r1 = RAY(IN, IN + vec);
    }
    
    LR.append(r1);
    LR.append(r2);     
    return; 
  }
  
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
  
  //construct 2 rays...
  VECTOR vec;
  SEGMENT seg1=LS.head();
  SEGMENT seg2=LS.tail();
  POINT pt1=Lin.head().point1();
  if (pt1==seg1.source()) pt1=Lin.head().point2();
  POINT pt2=Lin.tail().point1();
  if (pt2==seg2.target()) pt2=Lin.tail().point2(); 
  
  l1=LINE(seg1.source(),seg1.target());
  l2=LINE(seg2.source(),seg2.target());
  RAY r1,r2;
  
  if (orientation(l2,pt2)<0) { 
      r2 = RAY(seg2.target(),pt2);
  }
  else {
      vec= -(pt2.to_vector() - seg2.target().to_vector());
      r2 = RAY(seg2.target(), seg2.target() + vec);
  }
    
  if (orientation(l1,pt1)<0) { 
      r1 = RAY(seg1.source(),pt1);
  }
  else {
      vec= -(pt1.to_vector() - seg1.source().to_vector());
      r1 = RAY(seg1.source(), seg1.source() + vec);
  }   
  
  LR.append(r1); LR.append(r2);
}


bool get_bbox(const list<SEGMENT>& Lseg, RECTANGLE& r)
// return the bounding box of the segment chain in r
{
 if (Lseg.empty() ) return false;
 NUMBER2 xmin,xmax,ymin,ymax;
 SEGMENT s;
 // init 
 POINT p=Lseg.tail().target();
 xmin= p.xcoord(); xmax=p.xcoord();
 ymin= p.ycoord(); ymax=p.ycoord();
 
 forall(s,Lseg){
   p=s.source();
   if (p.xcoord() < xmin) xmin=p.xcoord();
   if (p.xcoord() > xmax) xmax=p.xcoord();
   if (p.ycoord() < ymin) ymin=p.ycoord();
   if (p.ycoord() > ymax) ymax=p.ycoord();    
 }
 
 r = RECTANGLE(POINT(xmin,ymin),POINT(xmax,ymax));
 
 return true;
}


POLYGON construct_polygon(const list<RAY>& Lr, const list<SEGMENT>& Ls, RECTANGLE box, list<POINT>& BACK)
{
 list<POINT> ADDIT;
 
 POLYGON pol;
 SEGMENT s1(box.lower_left(),box.upper_left());
 SEGMENT s2(box.upper_left(),box.upper_right());
 SEGMENT s3(box.upper_right(),box.lower_right());
 SEGMENT s4(box.lower_right(),box.lower_left());
 RAY r1=Lr.head(),r2=Lr.tail();
 POINT pout1,pout2,pact;
 bool b;
 int index;
 SEGMENT segs[4]; // segment array
 segs[0] = s1; segs[1]= s4; segs[2] = s3; segs[3] = s2;
 
 b=r1.intersection(s1,pout1); 
 if (! b){
   b=r1.intersection(s2,pout1); 
   if (! b){
     b=r1.intersection(s3,pout1);
     if (! b) { b=r1.intersection(s4,pout1); index=1; }
     else index=2;
   }
   else index=3;
 }
 else index=0;
 
 b=r2.intersection(s1,pout2);
 if (! b){
   b=r2.intersection(s2,pout2);
   if (! b){
     b=r2.intersection(s3,pout2);
     if (! b) b=r2.intersection(s4,pout2);
   }
 } 
 
 //(*win) << pout1; (*win) << pout2;
 
 ADDIT.append(pout1);
 while (! segs[index].contains(pout2)){
  if (ADDIT.tail() != segs[index].source() && ADDIT.head() != segs[index].source()) 
    ADDIT.append(segs[index].source());
  index++;
  if (index==4) index=0;
 }
 if (ADDIT.tail() != pout2 && ADDIT.head() != pout2) ADDIT.append(pout2);
 
 BACK=ADDIT;
 
 SEGMENT sakt;
 forall(sakt,Ls) {
  if (ADDIT.head() != sakt.source() && ADDIT.tail() != sakt.source()) ADDIT.push(sakt.source());
 }
 
 if (! Ls.empty()) {
  if (ADDIT.head() != Ls.tail().target() && ADDIT.tail() != Ls.tail().target())  
    ADDIT.push(Ls.tail().target());
 }
 else {
  if (! collinear(r1.point1(),r1.point2(),r2.point2())) {
     if (ADDIT.head() != r1.source() && ADDIT.tail() != r1.source()) ADDIT.push(r1.source());
  }
 }
 
 pol=POLYGON(ADDIT);
 return pol;
}

RECTANGLE stretch(RECTANGLE input)
{
  NUMBER2 xmin=input.xmin() - NUMBER2(50.0);
  NUMBER2 ymin=input.ymin() - NUMBER2(50.0);
  NUMBER2 xmax=input.xmax() + NUMBER2(50.0);
  NUMBER2 ymax=input.ymax() + NUMBER2(50.0);  
  
  return RECTANGLE(POINT(xmin,ymin),POINT(xmax,ymax));
}

void hp_inter_dual(const list<LINE>& Lin, list<SEGMENT>& Lseg)
{
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
 bool closed_right, closed_left;
 NUMBER2 p_left,p_right;
 bool something;

 if (! Vert.empty()) {
  something=handle_verticals(Vert, p_left, p_right, closed_left, closed_right);
  if (! something) return;
  // case, that intersection of the verticals was empty ...
 }

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

 // compute the convex hull of dual points ...
 list<POINT> output1 = CONVEX_HULL(Hcand_lower);
 list<POINT> output2 = CONVEX_HULL(Hcand_upper);

 // compute the upper/lower hulls ...
 list<POINT> lower = LOWER_PART(output1);
 list<POINT> upper = UPPER_PART(output2);
 
 forall(Hp,lower) Lout1.append(M1[Hp]);
 forall(Hp,upper) Lout2.append(M2[Hp]);

 list<LINE> Lline1,Lline2;  //begrenzende Linien (l_first-seg-l_last)
 list<SEGMENT> Lseg1,Lseg2; //begrenzende Segmente
 list<RAY> LR1,LR2;
 
 // build lists from the output of chull...
 build_lists(Lout1,LR1,Lline1,Lseg1);
 build_lists(Lout2,LR2,Lline2,Lseg2);
 
 // get four lines
 LINE l1,l2,l3,l4;

 list<POINT> INTER;
 POINT P1,P2,P3,P4,Pakt;
 bool flag;
 
 if (! (Lline1.size()==0 || Lline2.size()==0)){
  if (Lline1.size() == 1) { l1 = Lline1.head(); l2 = Lline1.head(); }
  else { l1= Lline1.head(); l2= Lline1.tail(); }

  if (Lline2.size() == 1) { l3 = Lline2.head(); l4 = Lline2.head(); }
  else { l3= Lline2.head(); l4= Lline2.tail(); }
 
  // compute intersections ... 
  flag = l1.intersection(l3,P1); if (flag) INTER.append(P1);
  l1.intersection(l4,P2); if (flag) INTER.append(P2);
  l2.intersection(l3,P3); if (flag) INTER.append(P3);
  l2.intersection(l4,P4); if (flag) INTER.append(P4);
 }
 
  RECTANGLE rec1,rec2,rec;
  // get bounding boxes of segment chains
  bool fl1= get_bbox(Lseg1,rec1);
  bool fl2= get_bbox(Lseg2,rec2);
 
  win->set_point_style(disc_point);  

 if (! Lline1.size()==0 ){
  if (!fl1) { // init box ...
   rec1 = RECTANGLE(Lline1.head().point1(),Lline1.head().point2());
   rec1 = rec1.include(Lline1.tail().point1());
   rec1 = rec1.include(Lline1.tail().point2());
  }
 }

 if (! Lline2.size()==0 ){
  if (!fl2) { // init box ...
   rec2 = RECTANGLE(Lline2.head().point1(),Lline2.head().point2());
   rec2 = rec2.include(Lline2.tail().point1());
   rec2 = rec2.include(Lline2.tail().point2());  
  }
 }
  
  // change the bounding boxes with the POINTS from segment intersection ...
  // cout << INTER.size() << "\n";
  forall(Pakt,INTER){
    rec1 = rec1.include(Pakt); rec2 = rec2.include(Pakt);
  }

 if (! Lline1.size()==0) rec= rec1; else rec=rec2;
 if (! (Lline1.size()==0 || Lline2.size()==0)) rec= rec.include(rec2);

  rec= stretch(rec); 
  (*win) << rec;
  
  //build 2 polygons ...
  list<POINT> BACK1,BACK2;
  
  POLYGON pol1;
  if (! (Lline1.size()==0)) pol1 = construct_polygon(LR1,Lseg1,rec,BACK1);
  POLYGON pol2;
  if (! (Lline2.size()==0)) pol2 = construct_polygon(LR2,Lseg2,rec,BACK2);
  
  BACK1.conc(BACK2);
  
  (*win).set_fill_color(yellow);
  (*win) << pol1;
  (*win).set_fill_color(green);  
  (*win) << pol2;
  (*win).set_fill_color(blue); 

  POLYGON pol3;

  if (! (Lline1.size()==0 || Lline2.size()==0)) pol3 = pol1.intersection(pol2);
  else {
   if (Lline1.size()==0) pol3=pol2; else pol3=pol1;
  }
  
  if (! Vert.empty()){
    cout << "There are vertical lines to handle !\n";
    NUMBER2 xmin,xmax,ymin,ymax;
    ymin= rec.ymin();
    ymax= rec.ymax();
    if (! closed_left) xmin=rec.xmin(); else xmin=p_left;
    if (! closed_right) xmax=rec.xmax(); else xmax=p_right;
    list<POINT> hlp;
    hlp.append(POINT(xmin,ymin)); hlp.append(POINT(xmax,ymin));
    hlp.append(POINT(xmax,ymax)); hlp.append(POINT(xmin,ymax));
    POLYGON VP(hlp);
    // another intersection operation ...
    pol3 = pol3.intersection(VP);
  }

  (*win) << pol3;
  (*win).set_fill_color(red);
  list<POINT> PV = pol3.vertices();
  POINT pvakt;
  
  forall(pvakt,PV){
     if (BACK1.search(pvakt)) (*win) << circle(pvakt.to_point(),6);
  }
  
  (*win).set_fill_color(invisible); 
  (*win).read_mouse();
// }
 
 Lout1.conc(Lout2);
 Lseg.clear();
 Lseg.conc(Lseg1);
 Lseg.conc(Lseg2);
 RAY rakt;
 forall(rakt,LR1) Lseg.append(SEGMENT(rakt.point1(),rakt.point2()));
 forall(rakt,LR2) Lseg.append(SEGMENT(rakt.point1(),rakt.point2()));
}


int main()
{
 list<LINE>* H1;
 //list<rat_polygon>* H2;
 list<rat_segment>* H3;

 GeoWin gw;
 
 window& wh = gw.get_window();
 win= &wh;
 
 list<LINE> L1;
 geo_scene sc1= gw.new_scene(L1);

 geo_scene res1= gw.new_scene(hp_inter_dual, sc1, string("Halfplane intersection"), H1,H3);
 gw.set_color(res1,red);
 gw.set_line_width(res1,4);
 gw.set_visible(res1,true);

 gw.edit(sc1);
 return 1;
}
