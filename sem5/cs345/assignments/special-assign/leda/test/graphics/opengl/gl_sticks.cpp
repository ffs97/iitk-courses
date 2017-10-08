/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gl_sticks.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/gl_window.h>
#include <LEDA/core/random_source.h>

#include "gl_support.h"

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// -------------------------------------------------------------------
// camera position ...
double  xpos = 7.0;
double  ypos = 2.0;
double  zpos = 10.0;

// scaling factor ...
double  scale = 1.8;

// we sort this array ...
double* my_arr;
int     size;

// ... and we highlight elements at these positions with a special 
// color
int highlight1 = -1, highlight2 = -1;

// -------------------------------------------------------------------
// drawing functions for sticks and the ground

void stick(double xpos, double height)
{
  glPushMatrix();
  glTranslated(xpos*1.5,0.5*height,0.0);
  glScaled(1.0, height, 1.0);
  leda::glSolidCube(1.0);
  glPopMatrix();
}

void ground()
{
  glPushMatrix();
  glTranslated(8.0,-0.3,0.0);
  glScaled(20.0, 0.02, 10.0);
  leda::glSolidCube(1.0);
  glPopMatrix();
}

// -------------------------------------------------------------------


int bubble_find(double* arr, int size)
{
  int i;
  for(i=1;i<size;i++){
     if (arr[i] < arr[i-1]) {
        // swap them ...
	highlight1 = i; highlight2 = i-1;
        return i;
     }
  }  
  return -1;
}


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

  // material information for "normal" sticks
  GLfloat mat_ambient[] = { 0.7f , 0.5f, 0.7f ,1.0f};
  GLfloat mat_diffuse[] = { 0.6f , 0.4f, 0.6f ,1.0f};
  GLfloat mat_specular[] = { 0.9f , 0.9f, 0.9f ,1.0f};
  GLfloat mat_shin[] = { 100.0f };
	
  // and for highlighted sticks ...
  GLfloat mat_ambient_hl_red[] = { 1.0f , 0.0f, 0.0f ,1.0f};
  GLfloat mat_diffuse_hl_red[] = { 1.0f , 0.0f, 0.0f ,1.0f};
  GLfloat mat_ambient_hl_green[] = { 0.0f , 1.0f, 0.0f ,1.0f};
  GLfloat mat_diffuse_hl_green[] = { 0.0f , 1.0f, 0.0f ,1.0f};  	
	
  // light information ...	
  GLfloat light_intensity[] = { 0.5f , 0.6f, 0.7f ,1.0f};
  GLfloat light_position[] = { 5.6f , 5.4f, 5.6f ,1.0f};
	
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_intensity);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_intensity);							

  glMatrixMode(GL_PROJECTION); // set the view volume shape
  glLoadIdentity();
  glOrtho(-30.0, 30.0, -30.0, 30.0, -1000, 1000);
  glMatrixMode(GL_MODELVIEW); // position and aim the camera
  glLoadIdentity();
	
  // set camera	position, direction and normal ....	
  gluLookAt(xpos, ypos, zpos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

  // draw background polygon ...
  static GLfloat col1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
  static GLfloat col2[] = { 0.4f, 0.8f, 0.9f, 1.0f };
  static GLfloat col3[] = { 0.0f, 1.0f, 1.0f, 1.0f };  
      
  glBegin(GL_POLYGON);
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col1 );
     glNormal3f(0.0f, 0.1f, 1.0f); 
     glVertex3f(-90.0, -90.0, -10.0);
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col2 ); 
     glNormal3f(0.2f, 0.1f, 0.9f);        
     glVertex3f(90.0, -90.0, -10.0); 
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col3 ); 
     glNormal3f(0.1f, 0.4f, 1.0f);    
     glVertex3f(90.0, 90.0, -50.0);  
     glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col1 ); 
     glNormal3f(0.1f, 0.4f, 1.0f);    
     glVertex3f(-90.0, 90.0, -50.0);          
  glEnd();     

  glTranslated(-12.0,-10.0,0.0);
  glScaled(scale,scale,scale);	
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shin);  
  ground();

  int i;
  for(i=0;i<size;i++){
	  mat_ambient[1] = mat_ambient[1] + 0.04f;
	  mat_diffuse[1] = mat_diffuse[1] + 0.04f;

	  // highlight two of the sticks ...
	  if (i == highlight1 || i == highlight2) {
	     if (my_arr[highlight2] > my_arr[highlight1]) {
	       glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_hl_red);
	       glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_hl_red);
	     }
	     else {
	       glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_hl_green);
	       glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_hl_green);	     
	     }
	     stick(i, my_arr[i]);	  
	  }
	  else {
	     glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	     glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	     stick(i, my_arr[i]);
	  }
  }

  glPopMatrix();
  glFlush();
	
  win->gl_finish();
	
  win->set_color(yellow);	
  win->draw_text(20,30,"Visualizing sorting with OpenGL and LEDA");	
}

int main()
{
  gl_window W(750,550,"OpenGL Stick Demo");
  W.init(0,750,0);
  
  window M(W.width(),28);  
  
  M.button("Sort", 10,"sort step by step using bubble sort");
  M.button("Gen",11,"generate input");  
  M.button("Left",2);
  M.button("Right",3);
  M.button("Up",4);
  M.button("Down",5);  
  M.button("Z +",6);
  M.button("Z -",7);   
  M.button("In",8);
  M.button("Out",9);      
  M.button("Exit",1); 
  
  W.set_redraw(redraw);  
  W.display(0,window::center);
  M.display(0,0,W);
 
  int i;
  my_arr = new double[10];
  for(i = 0; i < 10; i++) my_arr[i] = (double)(i*2);
  my_arr[2]= (double) 7;
  my_arr[9]= (double) 1.5;
  size = 10;
  
  int pos = -1;
		
  W.redraw();
  while (1)
  { switch (M.read_mouse())
    { case 1 : return 0;
      case 2 : xpos += 0.5; break;
      case 3 : xpos -= 0.5; break;
      case 4 : ypos += 0.5; break;
      case 5 : ypos -= 0.5; break; 
      case 6 : zpos += 0.5; break;
      case 7 : zpos -= 0.5; break;
      case 8 : scale += 0.1; break;
      case 9 : scale -= 0.1; break;                   
      case 10 : {  // bubble sort step ...
       if (pos == -1) {
        pos = bubble_find(my_arr, size);
       }
       else { // swap and reset pos ...
        swap(my_arr[pos], my_arr[pos-1]);
        pos = -1;
       }
       break;         
      }
      case 11 : { // generate input
        highlight1 = -1; highlight2 = -1;
	random_source RS(1,20);
	int val;
        for(i = 0; i < 10; i++) { RS >> val; my_arr[i] = val; }
      }
    };
    W.redraw();
  }
  
  delete [] my_arr;
  
  return 0;
}
