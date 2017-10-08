/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gl_support.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GL_SUPPORT_H
#define LEDA_GL_SUPPORT_H

#include <LEDA/graphics/gl_window.h>
#include <LEDA/core/list.h>
#include <LEDA/geo/d3_simplex.h>
#include <LEDA/geo/d3_triangle.h>
#include <LEDA/internal/std/math.h>

#ifdef _MSC_VER

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"comdlg32.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")  

#endif



LEDA_BEGIN_NAMESPACE

class rgb_color {
  double rv, gv, bv;
public:
  
  rgb_color() : rv(0), gv(0), bv(0) { }
  rgb_color(double ri, double gi, double bi) : rv(ri), gv(gi), bv(bi)
  { }
  
  rgb_color(const rgb_color& c) 
  { rv = c.rv; gv=c.gv; bv=c.bv; }
  
  double r() const { return rv; }  
  double g() const { return gv; } 
  double b() const { return bv; }
};

#if !defined(DONT_PROVIDE_RGB_COLOR_CONTAINER_FUNCTIONALITY)

int compare(const rgb_color& c1, const rgb_color& c2)
{ return 0; }

#if defined(LEDA_STD_IO_HEADERS)
std::istream& operator>>(std::istream& in, rgb_color& c)
{
   double r,g,b;
   in >> r; in >> g; in >> b;
   c = rgb_color(r,g,b);
   return in; 
}

std::ostream& operator<<(std::ostream& out, const rgb_color& c)
{  
  out << c.r() << " " << c.g() << " " << c.b() << "\n"; 
  return out;
}
#else
istream& operator>>(istream& in, rgb_color& c)
{
   double r,g,b;
   in >> r; in >> g; in >> b;
   c = rgb_color(r,g,b);
   return in; 
}

ostream& operator<<(ostream& out, const rgb_color& c)
{  
  out << c.r() << " " << c.g() << " " << c.b() << "\n"; 
  return out;
}
#endif

#endif

template<class GL_ARR>
void set_gl(GL_ARR* arr, const rgb_color& C)
{
  arr[0] = C.r();
  arr[1] = C.g();
  arr[2] = C.b();
}

void gl_color(const rgb_color& c)
{ glColor3d((GLdouble)c.r(), (GLdouble) c.g(), (GLdouble) c.b()); }

void gl_draw_triangle(const d3_triangle& t, const d3_point& inside);
void gl_draw_triangle(const d3_point& p1, const d3_point& p2, const d3_point& p3, const d3_point& inside); 

void gl_normal(const d3_point& a, const d3_point& b, const d3_point& c, const d3_point& other);
void gl_normal(const d3_point& a, const d3_point& b, const d3_point& c);

// add gl vertex / vertices ...
void gl_vertex(const d3_point& p)
{ glVertex3d((GLdouble)p.xcoord(), (GLdouble) p.ycoord(), (GLdouble) p.zcoord()); }

void gl_vertices(const d3_point& p1, const d3_point& p2)
{ 
  glVertex3d((GLdouble)p1.xcoord(), (GLdouble) p1.ycoord(), (GLdouble) p1.zcoord()); 
  glVertex3d((GLdouble)p2.xcoord(), (GLdouble) p2.ycoord(), (GLdouble) p2.zcoord());  
}

void gl_vertices(const d3_point& p1, const d3_point& p2, const d3_point& p3)
{ 
  glVertex3d((GLdouble)p1.xcoord(), (GLdouble) p1.ycoord(), (GLdouble) p1.zcoord()); 
  glVertex3d((GLdouble)p2.xcoord(), (GLdouble) p2.ycoord(), (GLdouble) p2.zcoord());  
  glVertex3d((GLdouble)p3.xcoord(), (GLdouble) p3.ycoord(), (GLdouble) p3.zcoord());    
}



// add gl normal vector of triangle(a,b,c)
// this vector should point to the side != sideof((a,b,c),other)
// prec: a,b,c,other must be not coplanar !!


// coordinate version of get_normal ...
// this time without orientation test ...

template<class NT>
vector get_normal_vec(const NT& ax, const NT& ay, const NT& az,
                  const NT& bx, const NT& by, const NT& bz,
                  const NT& cx, const NT& cy, const NT& cz)
{ 
  vector a((double)ax,(double)ay,(double)az);
  vector b((double)bx,(double)by,(double)bz);
  vector c((double)cx,(double)cy,(double)cz);

  vector v1 = b-a;
  vector v2 = c-a;
  
  double a1 = v1.xcoord();
  double a2 = v1.ycoord();
  double a3 = v1.zcoord();
  double b1 = v2.xcoord();
  double b2 = v2.ycoord();
  double b3 = v2.zcoord();    
    
  double vx = a2*b3-a3*b2;
  double vy = a3*b1-a1*b3;
  double vz = a1*b2-a2*b1;
    
  vector vec(vx,vy,vz);  
  vec = vec.norm();
  return vec;
}



vector get_normal(const d3_point& a, const d3_point& b, const d3_point& c,
                  const d3_point& other)
{
  // cross product ...
  vector v1 = b-a;
  vector v2 = c-a;
  
  double a1 = v1.xcoord();
  double a2 = v1.ycoord();
  double a3 = v1.zcoord();
  double b1 = v2.xcoord();
  double b2 = v2.ycoord();
  double b3 = v2.zcoord();    
    
  double vx = a2*b3-a3*b2;
  double vy = a3*b1-a1*b3;
  double vz = a1*b2-a2*b1;
    
  vector vec(vx,vy,vz);  
  vec = vec.norm(); // lenght == 1
  
  // check side ...
  int ori  = orientation(a,b,c,other);
  int ori2 = orientation(a,b,c,a+vec);
  
  if (ori != ori2) return vec;
  return -vec;
}

vector get_normal(const d3_point& a, const d3_point& b, const d3_point& c)
{
  // cross product ...
  vector v1 = b-a;
  vector v2 = c-a;
  
  double a1 = v1.xcoord();
  double a2 = v1.ycoord();
  double a3 = v1.zcoord();
  double b1 = v2.xcoord();
  double b2 = v2.ycoord();
  double b3 = v2.zcoord();    
    
  double vx = a2*b3-a3*b2;
  double vy = a3*b1-a1*b3;
  double vz = a1*b2-a2*b1;
    
  vector vec(vx,vy,vz);  
  vec = vec.norm(); // lenght == 1
  
  return vec;
}


// improvements: don't compute normals every time
// draw the sides of the cube in a more efficient way


// Cube:

void glWireCube(double d)
{
  if (d==0) return;
  
  d = d/2;
  
  glBegin(GL_LINES);
    glVertex3d(d,d,d); glVertex3d(-d,d,d);
    glVertex3d(-d,d,d);glVertex3d(-d,d,-d);
    glVertex3d(-d,d,-d);glVertex3d(d,d,-d);
    glVertex3d(d,d,-d);glVertex3d(d,d,d);
    
    glVertex3d(d,-d,d); glVertex3d(-d,-d,d);
    glVertex3d(-d,-d,d);glVertex3d(-d,-d,-d);
    glVertex3d(-d,-d,-d);glVertex3d(d,-d,-d);
    glVertex3d(d,-d,-d);glVertex3d(d,-d,d);
    
    glVertex3d(d,d,d); glVertex3d(d,-d,d);
    glVertex3d(-d,d,d);glVertex3d(-d,-d,d);
    glVertex3d(-d,d,-d);glVertex3d(-d,-d,-d);
    glVertex3d(d,d,-d);glVertex3d(d,-d,-d);    
  glEnd(); 
}


void glSolidCube(double d)
{
  if (d==0) return;
  
  d = d/2;
  d3_point other(0,0,0); // inside ...
  d3_point p1(d,d,d); 
  d3_point p2(-d,d,d);
  d3_point p3(-d,-d,d);
  d3_point p4(d,-d,d);    
  d3_point p5(d,d,-d);
  d3_point p6(-d,d,-d);
  d3_point p7(-d,-d,-d);
  d3_point p8(d,-d,-d);  
  
  // draw back and front
  glBegin(GL_QUADS);
     //gl_normal(p1,p2,p3,other);
     glNormal3d((GLdouble) 0.0, (GLdouble) 0.0, (GLdouble) 1.0);
     gl_vertex(p1);
     gl_vertex(p2);
     gl_vertex(p3);
     gl_vertex(p4);

     //gl_normal(p5,p6,p7,other);
     glNormal3d((GLdouble) 0.0, (GLdouble) 0.0, (GLdouble) -1.0);     
     gl_vertex(p5);
     gl_vertex(p6);
     gl_vertex(p7);
     gl_vertex(p8);
 
     // .. top and bottom ...
     //gl_normal(p6,p5,p1,other);
     glNormal3d((GLdouble) 0.0, (GLdouble) 1.0, (GLdouble) 0.0);     
     gl_vertex(p6);
     gl_vertex(p5);
     gl_vertex(p1);
     gl_vertex(p2);
 
     //gl_normal(p7,p8,p4,other);
     glNormal3d((GLdouble) 0.0, (GLdouble) -1.0, (GLdouble) 0.0);     
     gl_vertex(p7);
     gl_vertex(p8);
     gl_vertex(p4);
     gl_vertex(p3);
  
     // ... and the two remaining sides
     //gl_normal(p5,p8,p4,other);
     glNormal3d((GLdouble) 1.0, (GLdouble) 0.0, (GLdouble) 0.0);     
     gl_vertex(p5);
     gl_vertex(p8);
     gl_vertex(p4);
     gl_vertex(p1);

     //gl_normal(p6,p7,p3,other);
     glNormal3d((GLdouble) -1.0, (GLdouble) 0.0, (GLdouble) 0.0);     
     gl_vertex(p6);
     gl_vertex(p7);
     gl_vertex(p3);
     gl_vertex(p2);
  glEnd();    
}



// variant takes colors for sides or all vertices 
// side_mode == true :  6 colors for the sides ...
// side_mode == false:  24 colors for all the vertices of the 6 sides

void glSolidCube(double d, bool side_mode, const list<rgb_color>& L)
{
  if (d==0) return;
  
  d = d/2;
  d3_point other(0,0,0); // inside ...
  d3_point p1(d,d,d); 
  d3_point p2(-d,d,d);
  d3_point p3(-d,-d,d);
  d3_point p4(d,-d,d);    
  d3_point p5(d,d,-d);
  d3_point p6(-d,d,-d);
  d3_point p7(-d,-d,-d);
  d3_point p8(d,-d,-d);  
  
  list_item it = L.first();
  
  // check here maybe the number of colors in the list ???
  
  // draw back and front
  glBegin(GL_QUADS);
     glNormal3d((GLdouble) 0.0, (GLdouble) 0.0, (GLdouble) 1.0);    
     gl_color(L[it]); it = L.succ(it);    
     gl_vertex(p1);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }
     gl_vertex(p2);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p3);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p4);

     glNormal3d((GLdouble) 0.0, (GLdouble) 0.0, (GLdouble) -1.0);          
     gl_color(L[it]); it = L.succ(it);     
     gl_vertex(p5);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p6);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p7);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p8);
 
     // .. top and bottom ...
     glNormal3d((GLdouble) 0.0, (GLdouble) 1.0, (GLdouble) 0.0); 
     gl_color(L[it]); it = L.succ(it);         
     gl_vertex(p6);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p5);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p1);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p2);
 
     glNormal3d((GLdouble) 0.0, (GLdouble) -1.0, (GLdouble) 0.0);  
     gl_color(L[it]); it = L.succ(it);        
     gl_vertex(p7);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p8);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p4);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p3);
  
     // ... and the two remaining sides
     glNormal3d((GLdouble) 1.0, (GLdouble) 0.0, (GLdouble) 0.0);  
     gl_color(L[it]); it = L.succ(it);        
     gl_vertex(p5);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p8);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p4);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p1);

     glNormal3d((GLdouble) -1.0, (GLdouble) 0.0, (GLdouble) 0.0);    
     gl_color(L[it]); it = L.succ(it);      
     gl_vertex(p6);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p7);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p3);
     if (! side_mode) { gl_color(L[it]); it = L.succ(it); }     
     gl_vertex(p2);
  glEnd();    
}

// Tetrahedron ...

void glWireTetrahedron()
{
  // vertices 
  // 0 - (1,1,1)
  // 1 - (1,-1,-1)
  // 2 - (-1,-1,1)
  // 3 - (-1,1,-1)
  // faces
  // (1,2,3)  (0,3,2)
  // (0,1,3)  (0,2,1)
  d3_point p0(1.0,1.0,1.0);
  d3_point p1(1.0,-1.0,-1.0);
  d3_point p2(-1.0,-1.0,1.0);
  d3_point p3(-1.0,1.0,-1.0);  

  glBegin(GL_LINES);
    gl_vertex(p1); gl_vertex(p2);
    gl_vertex(p2); gl_vertex(p3);
    gl_vertex(p3); gl_vertex(p1);
    
    gl_vertex(p0); gl_vertex(p3);
    gl_vertex(p3); gl_vertex(p2);
    gl_vertex(p2); gl_vertex(p0);
    
    gl_vertex(p0); gl_vertex(p1);
    gl_vertex(p1); gl_vertex(p3);
    gl_vertex(p3); gl_vertex(p0);
    
    gl_vertex(p0); gl_vertex(p2);
    gl_vertex(p2); gl_vertex(p1);
    gl_vertex(p1); gl_vertex(p0);        
  glEnd(); 
}


void glSolidTetrahedron()
{
  d3_point p0(1.0,1.0,1.0);
  d3_point p1(1.0,-1.0,-1.0);
  d3_point p2(-1.0,-1.0,1.0);
  d3_point p3(-1.0,1.0,-1.0);  

  glBegin(GL_TRIANGLES);
    gl_normal(p1,p2,p3);
    gl_vertex(p1); gl_vertex(p2); gl_vertex(p3);
    
    gl_normal(p0,p3,p2);
    gl_vertex(p0); gl_vertex(p3); gl_vertex(p2);
    
    gl_normal(p0,p1,p3);
    gl_vertex(p0); gl_vertex(p1); gl_vertex(p3);
    
    gl_normal(p0,p2,p1);
    gl_vertex(p0); gl_vertex(p2); gl_vertex(p1);        
  glEnd(); 
}


// Octahedron

void glWireOctahedron()
{
  d3_point p0(0.0,0.0,-1.0);
  d3_point p1(1.0,0.0,0.0);
  d3_point p2(0.0,0.0,1.0);
  d3_point p3(-1.0,0.0,0.0);
  d3_point high(0.0,1.0,0.0);
  d3_point low(0.0,-1.0,0.0);
  
  glBegin(GL_LINE_LOOP);
    gl_vertex(p0); gl_vertex(p1);
    gl_vertex(p2); gl_vertex(p3);       
  glEnd();
  glBegin(GL_LINE_LOOP);
    gl_vertex(p0); gl_vertex(high);
    gl_vertex(p2); gl_vertex(low);       
  glEnd();
  glBegin(GL_LINE_LOOP);
    gl_vertex(p1); gl_vertex(high);
    gl_vertex(p3); gl_vertex(low);       
  glEnd();    
  
}

void glSolidOctahedron()
{
  d3_point p0(0.0,0.0,-1.0);
  d3_point p1(1.0,0.0,0.0);
  d3_point p2(0.0,0.0,1.0);
  d3_point p3(-1.0,0.0,0.0);
  d3_point high(0.0,1.0,0.0);
  d3_point low(0.0,-1.0,0.0);
  
  d3_point inside(0.0, 0.0, 0.0);
  
  glBegin(GL_TRIANGLES);
    gl_normal(high,p0,p1,inside);
    gl_vertex(high); gl_vertex(p0); gl_vertex(p1);
    gl_normal(high,p1,p2,inside);
    gl_vertex(high); gl_vertex(p1); gl_vertex(p2);
    gl_normal(high,p2,p3,inside);
    gl_vertex(high); gl_vertex(p2); gl_vertex(p3);
    gl_normal(high,p3,p0,inside);
    gl_vertex(high); gl_vertex(p3); gl_vertex(p0); 
    
    gl_normal(low,p0,p1,inside);
    gl_vertex(low); gl_vertex(p0); gl_vertex(p1);
    gl_normal(low,p1,p2,inside);
    gl_vertex(low); gl_vertex(p1); gl_vertex(p2);
    gl_normal(low,p2,p3,inside);
    gl_vertex(low); gl_vertex(p2); gl_vertex(p3);
    gl_normal(low,p3,p0,inside);
    gl_vertex(low); gl_vertex(p3); gl_vertex(p0);                 
  glEnd();
}


// Icosahedron

void glWireIcosahedron()
{
  // vertices ...
  double v = 0.618034; // (std::sqrt(5) -1)/2;
  
  d3_point p0(0,1,v);
  d3_point p1(0,1,-v);
  d3_point p2(1,v,0);
  d3_point p3(1,-v,0);
  d3_point p4(0,-1,-v);
  d3_point p5(0,-1,v);
  d3_point p6(v,0,1);
  d3_point p7(-v,0,1);
  d3_point p8(v,0,-1);
  d3_point p9(-v,0,-1);
  d3_point p10(-1,v,0);
  d3_point p11(-1,-v,0);  
  
  glBegin(GL_LINE_STRIP);
    gl_vertex(p11);
    gl_vertex(p9);
    gl_vertex(p1);
    gl_vertex(p0);
    gl_vertex(p7);
    gl_vertex(p11);
    gl_vertex(p4);
    gl_vertex(p5);
    gl_vertex(p6);
    gl_vertex(p2);
    gl_vertex(p8);
    gl_vertex(p4);
    
    gl_vertex(p9);
    gl_vertex(p8);
    gl_vertex(p1);
    gl_vertex(p2);
    gl_vertex(p0);
    gl_vertex(p6);
    gl_vertex(p7);
    gl_vertex(p5);
    gl_vertex(p11);
  glEnd();
  
  glBegin(GL_LINE_STRIP);
    gl_vertex(p10);
    gl_vertex(p1);
    gl_vertex(p9);
    gl_vertex(p11);
    gl_vertex(p10);
    gl_vertex(p9);
  glEnd();
  
  glBegin(GL_LINE_STRIP);
    gl_vertex(p7);
    gl_vertex(p0);
    gl_vertex(p10);
    gl_vertex(p11);
    gl_vertex(p7);
    gl_vertex(p10);
  glEnd();  
  
  glBegin(GL_LINE_STRIP);
    gl_vertex(p5);
    gl_vertex(p4);
    gl_vertex(p3);
    gl_vertex(p6);
    gl_vertex(p5);
    gl_vertex(p3);
  glEnd();
  
  glBegin(GL_LINE_STRIP);
    gl_vertex(p3);
    gl_vertex(p4);
    gl_vertex(p8);
    gl_vertex(p2);
    gl_vertex(p3);
    gl_vertex(p8);
  glEnd();    
  
}

void glSolidIcosahedron()
{
  // vertices ...
  double v = 0.618034; // (std::sqrt(5) -1)/2;
  
  d3_point p0(0,1,v);
  d3_point p1(0,1,-v);
  d3_point p2(1,v,0);
  d3_point p3(1,-v,0);
  d3_point p4(0,-1,-v);
  d3_point p5(0,-1,v);
  d3_point p6(v,0,1);
  d3_point p7(-v,0,1);
  d3_point p8(v,0,-1);
  d3_point p9(-v,0,-1);
  d3_point p10(-1,v,0);
  d3_point p11(-1,-v,0);  

  // a point inside the convex polyhedron
  d3_point ins(0.0, 0.0, 0.0);
     
  gl_draw_triangle(p11,p4,p5, ins);
  gl_draw_triangle(p7,p11,p5, ins);
  gl_draw_triangle(p7,p5,p6, ins);
  gl_draw_triangle(p7,p6,p0, ins);
  gl_draw_triangle(p0,p6,p2, ins);
  gl_draw_triangle(p0,p2,p1, ins);
  gl_draw_triangle(p1,p2,p8, ins);
  gl_draw_triangle(p1,p8,p9, ins);
  gl_draw_triangle(p9,p8,p4, ins);  
  gl_draw_triangle(p9,p4,p11, ins);
  gl_draw_triangle(p11,p7,p10, ins);
  gl_draw_triangle(p11,p10,p9, ins);
  gl_draw_triangle(p10,p7,p0, ins); 
  gl_draw_triangle(p10,p0,p1, ins);
  gl_draw_triangle(p10,p1,p9, ins);
  gl_draw_triangle(p6,p5,p3, ins);
  gl_draw_triangle(p2,p6,p3, ins); 
  gl_draw_triangle(p2,p3,p8, ins);
  gl_draw_triangle(p3,p5,p4, ins);
  gl_draw_triangle(p8,p3,p4, ins); 
}

// Cone

template<class POINT,class NT>
void generate_ellipse(const POINT& center, const NT& W, const NT& H, int numb, 
                      list<POINT>& curve)
{
  // use the parametric curve form ...
  // t goes from 0 ... 2 PI
  
  curve.clear();
  
  numb = numb*2;
  
  NT step = ((NT(2) * NT(LEDA_PI))/ NT(numb));
  NT xt,yt;
  NT t = 0;
  
  int i;
  for(i=0;i<numb;i++){
     xt = W * cos(t);
     yt = H * sin(t);
     
     // translate by center 
     xt = xt + center.xcoord();
     yt = yt + center.ycoord();
    
     curve.push_back(POINT(xt,yt));
     
     t = t + step;
  }
}



void glWireCone(double rad, double height, int numb)
{
  // high and low y coord ...
  double yhi  = height/2.0;
  double ylow = -yhi;
  
  d3_point hp(0,yhi,0);
   
  list<point> d2;
  
  generate_ellipse<point, double>(point(0,0), rad, rad, numb, d2);
  // close it ...
  d2.push_back(d2.head());
  
  point pi;
  d3_point pact, plast;
  bool first = true;
  
  glBegin(GL_LINES);
  forall(pi,d2){
    pact = d3_point(pi.xcoord(), ylow, pi.ycoord());
    gl_vertices(pact, hp);
    
    if (! first) gl_vertices(pact,plast);
    
    plast = pact;
    first = false;
  }
  glEnd();
} 

void glSolidCone(double rad, double height, int numb)
{
  // high and low y coord ...
  double yhi  = height/2.0;
  double ylow = -yhi;
  
  d3_point hp(0,yhi,0);
  d3_point ins(0,0,0);
   
  list<point> d2;
  
  generate_ellipse<point, double>(point(0,0), rad, rad, numb, d2);
  // close it ...
  d2.push_back(d2.head());
  
  point pi;
  d3_point pact, plast;
  bool first = true;
  
  glBegin(GL_POLYGON);
   glNormal3d(0.0,-1.0,0.0);
   forall(pi,d2){
    pact = d3_point(pi.xcoord(), ylow, pi.ycoord());
    gl_vertex(pact);    
   } 
  glEnd();
  
  forall(pi,d2){
    pact = d3_point(pi.xcoord(), ylow, pi.ycoord());
 
    if (! first) gl_draw_triangle(pact,plast,hp, ins);
    
    plast = pact;
    first = false;
  }
} 


//Cylinder

void glWireCylinder(double rad, double height, int numb)
{
  // high and low y coord ...
  double yhi  = height/2.0;
  double ylow = -yhi;
   
  list<point> d2;
  
  generate_ellipse<point, double>(point(0,0), rad, rad, numb, d2);
  // close it ...
  d2.push_back(d2.head());
  
  point pi;
  d3_point pact1, pact2, plast1, plast2;
  bool first = true;
  
  glBegin(GL_LINES);
  forall(pi,d2){
    pact1 = d3_point(pi.xcoord(),  ylow, pi.ycoord());
    pact2 = d3_point(pi.xcoord(), yhi, pi.ycoord());
    
    gl_vertices(pact1, pact2);
    
    if (! first) { gl_vertices(pact1,plast1); gl_vertices(pact2,plast2); }
    
    plast1 = pact1; plast2 = pact2;
    first = false;
  }
  glEnd();
} 

void glSolidCylinder(double rad, double height, int numb)
{
  // high and low y coord ...
  double yhi  = height/2.0;
  double ylow = -yhi;
  
  d3_point ins(0,0,0);
   
  list<point> d2;
  
  generate_ellipse<point, double>(point(0,0), rad, rad, numb, d2);
  // close it ...
  d2.push_back(d2.head());
  
  point pi;
  d3_point pact1, pact2, plast1, plast2;
  bool first = true;
  
  // high and low polygon ... 
  glBegin(GL_POLYGON);
   glNormal3d(0.0,-1.0,0.0);
   forall(pi,d2){
    pact1 = d3_point(pi.xcoord(), ylow, pi.ycoord());
    gl_vertex(pact1);    
   } 
  glEnd();
  glBegin(GL_POLYGON);
   glNormal3d(0.0,1.0,0.0);
   forall(pi,d2){
    pact1 = d3_point(pi.xcoord(), yhi, pi.ycoord());
    gl_vertex(pact1);    
   } 
  glEnd();    
  
  forall(pi,d2){
    pact1 = d3_point(pi.xcoord(),  ylow, pi.ycoord());
    pact2 = d3_point(pi.xcoord(),  yhi, pi.ycoord());  
 
    if (! first) {
     gl_draw_triangle(plast1,pact1,pact2, ins);
     gl_draw_triangle(plast2,pact2,plast1, ins);
    }
    
    plast1 = pact1; plast2 = pact2;
    first = false;
  }
} 

// end cylinder


void gl_normal(const d3_point& a, const d3_point& b, const d3_point& c,
               const d3_point& other)
{
   vector v = get_normal(a,b,c,other);
   glNormal3d( (GLdouble) v.xcoord(), (GLdouble) v.ycoord(), (GLdouble) v.zcoord());   
}

void gl_normal(const d3_point& a, const d3_point& b, const d3_point& c)
{
   vector v = get_normal(a,b,c);
   glNormal3d( (GLdouble) v.xcoord(), (GLdouble) v.ycoord(), (GLdouble) v.zcoord());   
}


// draw triangle version
// here without other point and normal informations ...

void gl_draw_triangle(const d3_triangle& t, 
                      const rgb_color& c1, const rgb_color& c2, const rgb_color& c3) 
{
  d3_point p1 = t.point1(); 
  d3_point p2 = t.point2(); 
  d3_point p3 = t.point3(); 
  
  glBegin(GL_TRIANGLES);
     gl_color(c1);    
     gl_vertex(p1);
     gl_color(c2);  
     gl_vertex(p2);
     gl_color(c3);
     gl_vertex(p3);  
  glEnd();
}

void gl_draw_triangle(const d3_triangle& t, const d3_point& inside) 
{
  d3_point p1 = t.point1(); 
  d3_point p2 = t.point2(); 
  d3_point p3 = t.point3(); 
  
  glBegin(GL_TRIANGLES);
     gl_normal(p1,p2,p3,inside);
     gl_vertex(p1);
     gl_vertex(p2);
     gl_vertex(p3);  
  glEnd();
}

void gl_draw_triangle(const d3_point& p1, const d3_point& p2, const d3_point& p3, 
                      const d3_point& inside) 
{
  glBegin(GL_TRIANGLES);
     gl_normal(p1,p2,p3,inside);
     gl_vertex(p1);
     gl_vertex(p2);
     gl_vertex(p3);  
  glEnd();
}

LEDA_END_NAMESPACE

#endif
