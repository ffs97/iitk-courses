/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gl_window.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GL_WINDOW_H
#define LEDA_GL_WINDOW_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600131
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>

#ifdef __win32__
#include <windows.h>		
#endif

#include <LEDA/graphics/window.h>

#ifdef __unix__
#define LEDA_STD_INCLUDE
#include <GL/glx.h>
#undef LEDA_STD_INCLUDE
#endif

#include <GL/gl.h>
#include <GL/glu.h>

LEDA_BEGIN_NAMESPACE

class __exportC gl_window : public window
{  
  gl_window(const gl_window&);
  gl_window& operator=(const gl_window&);
  
  public:

  gl_window();
  gl_window(const char * l);
  gl_window(int w, int h);
  gl_window(int w, int h, const char* l);
 
  void display();
  void display(int x, int y);
  void display(window& W, int x, int y);
  void display(int x, int y, window& W);

  void set_redraw(void (*F)(gl_window*))
  { BASE_WINDOW::set_redraw((win_redraw_func1)F); } 
    
  void set_redraw(void (*F)(gl_window*,double,double,double,double))
  { BASE_WINDOW::set_redraw((win_redraw_func2)F); } 

  void gl_start(); 
  void gl_flush();
  void gl_finish();    
};


#if LEDA_ROOT_INCL_ID == 600131
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
