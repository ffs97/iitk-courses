/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  simple_demo.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/gl_window.h>

#include "gl_support.h"

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// these global variables are used
// to set the colors of the sides of the pyramid ...
double rv1(1.0), gv1(0.0), bv1(0.0);
double rv2(0.0), gv2(1.0), bv2(0.0);
double rv3(0.0), gv3(0.0), bv3(1.0); 

class gl_rotation_window : public gl_window
{
  float  rtri;        
  float  rquad;   
  float  blue_value;     
  
  list<rgb_color> cube_colors;
  
  // these triangles are the sides of our
  // pyramid ...
  d3_triangle t1, t2, t3, t4;

public:
 
  gl_rotation_window (char* title, int width, int height, float bv) : gl_window(width, height, title)
  {
    rtri  = 0.0f;
    rquad = 0.0f;  
    blue_value = bv;          
    
    // init colors of cubes sides ...
    cube_colors.push_back(rgb_color(1.0,0.2,1.0));
    cube_colors.push_back(rgb_color(0.8,0.5,1.0));    
    cube_colors.push_back(rgb_color(0.6,0.3,0.8));
    cube_colors.push_back(rgb_color(0.5,0.2,0.8));  
    cube_colors.push_back(rgb_color(0.5,0.1,0.7));
    cube_colors.push_back(rgb_color(0.3,0.1,0.5));   
    
    d3_point p1(0.0, 1.0, 0.0), p2(-1.0, -1.0, 1.0);
    d3_point p3(1.0,-1.0, 1.0), p4(1.0, -1.0, -1.0);
    d3_point p5(-1.0, -1.0, -1.0);
    
    t1 = d3_triangle(p1,p2,p3);
    t2 = d3_triangle(p1,p3,p4);   
    t3 = d3_triangle(p1,p4,p5);     
    t4 = d3_triangle(p1,p5,p2);  
  }
  
  void draw()
  { 
    // init
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, (GLfloat)blue_value, 0.5f);  
    glClearDepth(1.0f);                  
    glEnable(GL_DEPTH_TEST);            
  
    // resize
    glViewport(0,0,width(),height());            
    glMatrixMode(GL_PROJECTION);            
    glLoadIdentity();                  
    gluPerspective(45.0f,(GLfloat)width()/(GLfloat)height(),0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);              
    glLoadIdentity();     
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    
    glLoadIdentity();                    
    glTranslatef(-1.0f,0.0f,-6.0f);    
    glRotatef(rtri,0.0f,1.0f,0.0f);    
    
    // pyramid ....
    rgb_color c1(rv1,gv1,bv1), c2(rv2,gv2,bv2), c3(rv3,gv3,bv3);  
    gl_draw_triangle(t1, c1, c2, c3);
    gl_draw_triangle(t2, c1, c3, c2);
    gl_draw_triangle(t3, c1, c2, c3);
    gl_draw_triangle(t4, c1, c3, c2);

    glLoadIdentity();                
    glTranslatef(1.5f,0.0f,-7.0f);  
    glRotatef(rquad,1.0f,0.5f,1.0f);
    
    // draw a solid cube ...
    glSolidCube(1.5, true, cube_colors);

    rtri  += 2.0f;                      
    rquad -= 2.0f;                    
  }
    
};

void redraw(gl_rotation_window* win)
{
  win->gl_start();
  win->draw();
  win->gl_finish();
}

int main()
{
  gl_rotation_window  W((char*)"Simple OpenGL Window in a LEDA window",500,500, 0.0);  
  gl_rotation_window  W2((char*)"Simple OpenGL Window 2",100,100, 1.0);    
 
  window M(W.width(),28); 
  M.double_item("red (0-1):",rv1);
  M.double_item("green (0-1):",gv1);  
  M.double_item("blue (0-1):",bv1);  
  M.button("Exit",1);
  M.make_menu_bar(-1);

  typedef void (*GL_REDRAW)(gl_window*);

  W.set_redraw( (GL_REDRAW) redraw); 
  W.display(0,window::center);
  M.display(0,0,W);
  
  W2.set_redraw( (GL_REDRAW) redraw); 
  W2.display(390,5,W);

  W.redraw(); W2.redraw();
  while (1)
  { switch (M.get_mouse())
    { case 1 : return 0;
    };
    W.redraw(); W2.redraw();
  }
 
  return 0;
}  
