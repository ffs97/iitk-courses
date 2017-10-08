/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gl_solids.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/gl_window.h>
#include <LEDA/geo/d3_hull.h>

#include "gl_support.h"

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// camera positions ...
float  xpos = 7.0f;
float  ypos = 2.0f;
float  zpos = 10.0f;
float  scale = 2.8f;

gl_window* wptr;
int solid_kind = 11;
bool draw_solid = true;

int rv=255,gv=80,bv=40;
int shiny = 100;


void call_back(int v) { solid_kind = v; wptr->redraw(); }
void color_fcn(int) { wptr->redraw(); }


// special polyhedra generators ...

namespace leda {

template<class POINT,class NT>
POINT centroid(const POINT& p1, const POINT& p2, const POINT& p3)
{
  NT x = p1.xcoord() + p2.xcoord() + p3.xcoord();
  NT y = p1.ycoord() + p2.ycoord() + p3.ycoord();
  NT z = p1.zcoord() + p2.zcoord() + p3.zcoord();
  return POINT(x,y,z);
}

template<class POINT,class NT>
POINT centroid(const list<POINT>& L)
{
  POINT pi;
  NT x(0), y(0), z(0);
  
  forall(pi,L){
    x = x + pi.xcoord();
    y = y + pi.ycoord();
    z = z + pi.zcoord();
  }
  
  return POINT(x,y,z);
}

void compute_icosahedron_graph(GRAPH<d3_point,int>& poly)
{
  poly.clear();
  
  // vertices ...
  double v = 0.618034; // (std::sqrt(5) -1)/2;
  
  // vertices of icosahedron ...
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
  
  list<d3_point> L;
  L.push_back(p0); L.push_back(p1); L.push_back(p2); 
  L.push_back(p3); L.push_back(p4); L.push_back(p5);
  L.push_back(p6); L.push_back(p7); L.push_back(p8);
  L.push_back(p9); L.push_back(p10); L.push_back(p11);  
  
  D3_HULL(L, poly);    
}  


// truncated cube ...

void compute_truncated_cube(double d, double trunc, GRAPH<d3_point,int>& poly)
{
  poly.clear();
  
  d = d/2;
  
  double t = d - d * trunc;

  // 24 vertices 8 up, 8 down, 4 additonal front , 4 additional back ...
  list<d3_point> verts;
  
  // up
  verts.push_back(d3_point(-t,d,d)); verts.push_back(d3_point(t,d,d)); 
  verts.push_back(d3_point(-t,d,-d)); verts.push_back(d3_point(t,d,-d));
  
  verts.push_back(d3_point(d,d,-t)); verts.push_back(d3_point(d,d,t)); 
  verts.push_back(d3_point(-d,d,-t)); verts.push_back(d3_point(-d,d,t));  

  // down
  verts.push_back(d3_point(-t,-d,d)); verts.push_back(d3_point(t,-d,d)); 
  verts.push_back(d3_point(-t,-d,-d)); verts.push_back(d3_point(t,-d,-d));
  
  verts.push_back(d3_point(d,-d,-t)); verts.push_back(d3_point(d,-d,t)); 
  verts.push_back(d3_point(-d,-d,-t)); verts.push_back(d3_point(-d,-d,t));  
  
  // front
  verts.push_back(d3_point(d,t,d)); verts.push_back(d3_point(d,-t,d)); 
  verts.push_back(d3_point(-d,-t,d)); verts.push_back(d3_point(-d,t,d));  
  
  // back
  verts.push_back(d3_point(d,t,-d)); verts.push_back(d3_point(d,-t,-d)); 
  verts.push_back(d3_point(-d,-t,-d)); verts.push_back(d3_point(-d,t,-d));  
    
  D3_HULL(verts, poly);
}

void glSolidTruncCube(double d, double trunc)
{
  GRAPH<d3_point,int> surface;
  
  compute_truncated_cube(d,trunc,surface);
  
  surface.compute_faces();
  
  list<d3_point> poly;
  d3_point pt;
  d3_point cm; // mass center (of a side polygon)
  d3_point origin(0,0,0);

  edge e;  
  face f;
  forall_faces(f, surface) {
    poly.clear();
    
    // get the points of a face ...
    forall_face_edges(e,f){
      poly.push_back(surface[source(e)]);
    }    
    
    glBegin(GL_POLYGON);
      cm = centroid<d3_point,double>(poly);       
      vector v = cm - origin;
      v = v.norm();
      glNormal3d((GLdouble) v.xcoord(),(GLdouble) v.ycoord(),(GLdouble) v.zcoord());
    
      forall(pt,poly) gl_vertex(pt);   
    glEnd();
  }
}

void glWireTruncCube(double d, double trunc)
{
  GRAPH<d3_point,int> poly;
  compute_truncated_cube(d, trunc, poly);
  
  edge e;
  d3_point ps, pt;

  glBegin(GL_LINES);
   forall_edges(e,poly) {
    ps = poly[source(e)];
    pt = poly[target(e)];
    leda::gl_vertex(ps.to_float());
    leda::gl_vertex(pt.to_float());
   }
  glEnd();  
}

} // end namespace



void redraw(gl_window* win)
{
  win->gl_start();
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);	
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glEnable(GL_NORMALIZE);	
	
  glClearColor(0.0f, 0.0f, 0.0f,0.0f);
  glViewport(0, 0, win->width(), win->height());
  
  GLfloat red_val = ((GLfloat)rv)/((GLfloat)255.0);
  GLfloat green_val = ((GLfloat)gv)/((GLfloat)255.0);
  GLfloat blue_val = ((GLfloat)bv)/((GLfloat)255.0);    

  GLfloat mat_specular[] = { red_val , green_val , blue_val ,1.0f};
  GLfloat mat_shin[] = { (GLfloat)shiny };
  GLfloat mat_ambient[] = { red_val , green_val , blue_val ,1.0f};
  GLfloat mat_diffuse[] = { red_val , green_val , blue_val ,1.0f};

  GLfloat light_intensity[] = { 0.5f , 0.6f, 0.7f ,1.0f};
  //GLfloat light_position[] = { 5.6f , 5.4f, 5.6f ,1.0f};
  
  GLfloat light_position[] = { xpos , ypos, zpos/2.0f ,1.0f};
	
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intensity);							

  glMatrixMode(GL_PROJECTION); // set the view volume shape
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -30.0, 30.0, -1000, 1000);
  glMatrixMode(GL_MODELVIEW); // position and aim the camera
  glLoadIdentity();
	
  // set camera		
  gluLookAt(xpos, ypos, zpos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

  // draw background polygon ...
  static GLfloat col1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
  static GLfloat col2[] = { 0.4f, 0.8f, 0.9f, 1.0f };
  static GLfloat col3[] = { 0.0f, 1.0f, 1.0f, 1.0f };  
      
  glBegin(GL_POLYGON);
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col1 );
     glNormal3d(0.0, 0.1, 1.0); 
     glVertex3f(-90.0, -90.0, -10.0);
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col2 ); 
     glNormal3d(0.2, 0.1, 0.9);        
     glVertex3f(90.0, -90.0, -10.0); 
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col3 ); 
     glNormal3d(0.1, 0.4, 1.0);    
     glVertex3f(90.0, 90.0, -50.0);  
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col1 ); 
     glNormal3d(0.1, 0.4, 1.0);    
     glVertex3f(-90.0, 90.0, -50.0);          
  glEnd();     

  glScaled(scale,scale,scale);	
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shin);  
  
  if (! draw_solid) glLineWidth(2.0f);
  
  switch(solid_kind) {
    case 11: {
       if (draw_solid) leda::glSolidCube(3.0); 
       else leda::glWireCube(3.0);
       break;
    }
    
    case 12: {
        // we can use GLU ...
	GLUquadricObj *qobj = gluNewQuadric();
    
        if (draw_solid) {
          gluQuadricDrawStyle(qobj, (GLenum) GLU_FILL);
          gluQuadricNormals(qobj, (GLenum) GLU_SMOOTH);   
          gluSphere(qobj, 2.0, 32, 32);	    
	}
	else {
          gluQuadricDrawStyle(qobj, (GLenum) GLU_LINE);
          gluQuadricNormals(qobj, (GLenum) GLU_FLAT);   
          gluSphere(qobj, 2.0, 32, 32);	    	
	}
	break;
	
	gluDeleteQuadric(qobj);
    }
    
    case 15: {
        if (draw_solid) leda::glSolidOctahedron(); 
	else leda::glWireOctahedron();
	break;
    }
    case 16: {
        if (draw_solid) leda::glSolidIcosahedron(); 
	else leda::glWireIcosahedron(); 
	break;
    }
    case 17: {
        if (draw_solid) leda::glSolidTetrahedron(); 
	else leda::glWireTetrahedron(); 
	break;    
    }
    case 19: {
        if (draw_solid) leda::glSolidCone(3.0,8.0,32); 
	else leda::glWireCone(3.0,8.0,32); 
	break;    
    }      
    case 20: {
        if (draw_solid) leda::glSolidCylinder(3.0,8.0,32); 
	else leda::glWireCylinder(3.0,8.0,32); 
	break;    
    } 
    
    case 22: {
        if (draw_solid) leda::glSolidTruncCube(3.0, 0.33); 
	else leda::glWireTruncCube(3.0, 0.33); 
	break;    
    }           
  
  }
    
  glPopMatrix();
  glFlush();
	
  win->gl_finish();
	
  win->set_color(yellow);		
}

int main()
{
  gl_window W(750,550,"Well known polyhedra");
  W.init(0,750,0);
  wptr = &W;
  
  menu solids;
  solids.button("Cube",11,call_back);
  solids.button("Sphere",12,call_back);
  
  solids.button("Octahedron",15,call_back);
  solids.button("Icosahedron",16,call_back);
  solids.button("Tetrahedron",17,call_back);
  solids.button("Cone",19,call_back);
  solids.button("Cylinder",20,call_back);  
 
  solids.button("Truncated Cube",22,call_back);    
  
  panel P("Color choice");
  P.int_item("red",rv,0,255,color_fcn);
  P.int_item("green",gv,0,255,color_fcn);  
  P.int_item("blue",bv,0,255,color_fcn);   
  P.int_item("shiny",shiny,0,100,color_fcn); 
  
  window M(W.width(),28);
  M.button("Solids",solids);    
  M.button("Left",2);
  M.button("Right",3);
  M.button("Up",4);
  M.button("Down",5);  
  M.button("In",8);
  M.button("Out",9);      
  M.button("Exit",1); 
  M.bool_item("Solid",draw_solid);
  
  W.set_redraw(redraw);  
  W.display(0,window::center);
  M.display(0,0,W);
  P.display();
 		
  W.redraw();
  while (1)
  { switch (M.read_mouse())
    { case 1 : return 0;
      case 2 : xpos += 1.5f; break;
      case 3 : xpos -= 1.5f; break;
      case 4 : ypos += 1.5f; break;
      case 5 : ypos -= 1.5f; break; 
      case 8 : scale += 0.2f; break;
      case 9 : scale -= 0.2f; break;                   
    }
    W.redraw();
  }
  
  return 0;
}
