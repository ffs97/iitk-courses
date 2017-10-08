/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  x_window.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_X_WINDOW_H
#define LEDA_X_WINDOW_H


#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 360999
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graphics/X11/cursorfont.h>

LEDA_BEGIN_NAMESPACE


// windows, events, colors ...

enum { 
  key_press_event, 
  key_release_event, 
  button_press_event, 
  button_release_event,
  panel_press_event,
  configure_event, 
  exposure_event, 
  motion_event, 
  destroy_event, 
  timer_event,
  no_event 
};


extern const char* event_name[];


enum {
 KEY_BACKSPACE,
 KEY_RETURN,
 KEY_ESCAPE,
 KEY_LEFT,
 KEY_RIGHT,
 KEY_UP,
 KEY_DOWN,
 KEY_HOME,
 KEY_END,
 KEY_PAGEUP,
 KEY_PAGEDOWN,
 KEY_TAB,
 KEY_PRINT,
 KEY_INSERT,
 KEY_DELETE,
 KEY_PAGE_UP,
 KEY_PAGE_DOWN,
 KEY_F1,
 KEY_F2,
 KEY_F3,
 KEY_F4,
 KEY_F5,
 KEY_F6,
 KEY_F7,
 KEY_F8,
 KEY_F9,
 KEY_F10,
 KEY_F11,
 KEY_F12,
 KEY_PRINT1
};


enum color_enum {
  DEF_COLOR = -16,
  invisible = -1,
  white  =  0,
  black  =  1,
  red    =  2,
  green  =  3,
  blue   =  4,
  yellow =  5,
  violet =  6,
  orange =  7,
  cyan   =  8,
  brown  =  9,
  pink   = 10,
  green2 = 11,
  blue2  = 12,
  grey1  = 13,
  grey2  = 14,
  grey3  = 15,
  ivory  = 16 
};

enum point_style  { pixel_point, cross_point, plus_point, circle_point, 
                    disc_point, rect_point, box_point };

enum line_style   {solid, dashed, dotted, dashed_dotted};
enum text_mode    {transparent, opaque};
enum drawing_mode {src_mode, xor_mode, or_mode, and_mode, diff_mode};
enum grid_style   {invisible_grid, point_grid, line_grid};


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 360999
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
