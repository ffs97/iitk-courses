/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gl_glut.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/gl_window.h>

#ifdef __win32__

#include <GL/glaux.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")  
#pragma comment(lib,"glaux.lib")

#else

#include <GL/glut.h>
#define USE_GLUT
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// camera position ...
double  xpos = 7.0f;
double  ypos = 2.0f;
double  zpos = 10.0f;

// scaling factor 
double  scale = 1.8;

// rotation angle
GLfloat rot  = 0.0f;
bool    rotation_mode = false;

gl_window* wptr;

// what kind of solid ? 
int solid_kind = 11;
// solid or wire frame drawing ?
bool draw_solid = true;

// color and shininess information ...
int rv=255,gv=80,bv=40;
int shiny = 100;

void call_back(int v) { solid_kind = v; wptr->redraw(); }
void color_fcn(int) { wptr->redraw(); }

void redraw(gl_window* win)
{
  win->gl_start();
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);	
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
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
  GLfloat light_position[] = { 5.6f , 5.4f, 5.6f ,1.0f};
	
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intensity);							

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -30.0, 30.0, -1000, 1000);
  
  // position and aim the camera
  glMatrixMode(GL_MODELVIEW); 
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
  
  glRotatef(rot,0.0f,1.0f,0.0f);
  
  if (rotation_mode) rot += 2.0f;   

  glScaled(scale,scale,scale);	
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shin);  

#if defined(USE_GLUT)  
  switch(solid_kind) {
    case 11: {
       if (draw_solid) glutSolidCube(3.0); 
       else glutWireCube(3.0);
       break;
    }
    case 12: {
        if (draw_solid) glutSolidSphere(3.0, 32, 32); 
	else glutWireSphere(3.0, 32, 32);
	break;
    }
    case 13: {
        if (draw_solid) glutSolidTeapot(3.0); 
	else glutWireTeapot(3.0);
	break;
    }
    case 14: {
        if (draw_solid) glutSolidTorus(3.0,2.5, 32, 32); 
	else glutWireTorus(3.0,2.5, 32, 32);
	break;
    }
    case 15: {
        if (draw_solid) glutSolidOctahedron(); 
	else glutWireOctahedron();
	break;
    }
    case 16: {
        if (draw_solid) glutSolidIcosahedron(); 
	else glutWireIcosahedron(); 
	break;
    }
    case 17: {
        if (draw_solid) glutSolidTetrahedron(); 
	else glutWireTetrahedron(); 
	break;    
    }
    case 18: {
        if (draw_solid) glutSolidDodecahedron(); 
	else glutWireDodecahedron(); 
	break;    
    }  
    case 19: {
        if (draw_solid) glutSolidCone(3.0,8.0,32,32); 
	else glutWireCone(3.0,8.0,16,16); 
	break;    
    }      
  }
#else
  switch(solid_kind) {
    case 11: {
       if (draw_solid) auxSolidCube(3.0); 
       else auxWireCube(3.0);
       break;
    }
    case 12: {
        if (draw_solid) auxSolidSphere(3.0); 
	else auxWireSphere(3.0);
	break;
    }
    case 13: {
        if (draw_solid) auxSolidTeapot(3.0); 
	else auxWireTeapot(3.0);
	break;
    }
    case 14: {
        if (draw_solid) auxSolidTorus(3.0,2.5); 
	else auxWireTorus(3.0,2.5);
	break;
    }
    case 15: {
        if (draw_solid) auxSolidOctahedron(3.0); 
	else auxWireOctahedron(3.0);
	break;
    }
    case 16: {
        if (draw_solid) auxSolidIcosahedron(3.0); 
	else auxWireIcosahedron(3.0); 
	break;
    }
    case 17: {
        if (draw_solid) auxSolidTetrahedron(3.0); 
	else auxWireTetrahedron(3.0); 
	break;    
    }
    case 18: {
        if (draw_solid) auxSolidDodecahedron(3.0); 
	else auxWireDodecahedron(3.0); 
	break;    
    }  
    case 19: {
        if (draw_solid) auxSolidCone(3.0,8.0); 
	else auxWireCone(3.0,8.0); 
	break;    
    }      
  }
#endif
    
  glPopMatrix();
  glFlush();
	
  win->gl_finish();
	
  win->set_color(yellow);	
  win->draw_text(20,30,"Using GLUT/AUX together with LEDA; Choose objects from the Solids menu");	
}

int main()
{
  gl_window W(800,600,"GLUT/AUX Demo");
  W.init(0,800,0);
  wptr = &W;
  
  menu solids;
  solids.button("Cube",11,call_back);
  solids.button("Sphere",12,call_back);
  solids.button("Teapot",13,call_back);
  solids.button("Torus",14,call_back);
  solids.button("Octahedron",15,call_back);
  solids.button("Icosahedron",16,call_back);
  solids.button("Tetrahedron",17,call_back);
  solids.button("Dodecahedron",18,call_back);
  solids.button("Cone",19,call_back);  
  
  panel P("Color choice");
  P.int_item("red",rv,0,255,color_fcn);
  P.int_item("green",gv,0,255,color_fcn);  
  P.int_item("blue",bv,0,255,color_fcn);   
  P.int_item("shiny",shiny,0,800,color_fcn); 
  
  window M(W.width(),28);
  M.button("Solids",solids);    
  M.button("Left",2);
  M.button("Right",3);
  M.button("Up",4);
  M.button("Down",5);  
  M.button("In",8);
  M.button("Out",9);   
  M.button("Rotate",10);   
  M.button("Exit",1); 
  M.bool_item("Solid",draw_solid);
  
  W.set_redraw(redraw);  
  W.display(0,window::center);
  M.display(0,0,W);
  P.display();
 		
  W.redraw();
  while (1)
  { 
    int but = M.get_mouse();
  
    switch (but)
    { case 1 : return 0;
      case 2 : xpos += 1.5; break;
      case 3 : xpos -= 1.5; break;
      case 4 : ypos += 1.5; break;
      case 5 : ypos -= 1.5; break; 
      case 8 : scale += 0.2; break;
      case 9 : scale -= 0.2; break;   
      case 10: rotation_mode = !rotation_mode; break;             
    }
    
    if (but != NO_BUTTON || rotation_mode) W.redraw();
  }
  
  return 0;
}
