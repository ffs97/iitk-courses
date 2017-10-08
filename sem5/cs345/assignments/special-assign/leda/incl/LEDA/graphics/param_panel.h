/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  param_panel.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef PARAM_PANEL_H
#define PARAM_PANEL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600130
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graphics/panel.h>
#include <LEDA/system/param_handler.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {param_panel} {} {Panels for parameter handlers} {PP}}*/

class __exportC param_panel : public panel {
/*{\Mdefinition
An instance |\Mvar| of type |\Mname| is a |panel| which is associated with 
an object |P| of type |param_handler| 
(cf. section \ref{Param_Handler}). 
|\Mvar| can be used to display and edit the values of |P|.
Note that |P| must have been initialized before the construction of |\Mvar|.
}*/
public:
  /*{\Mcreation}*/

  param_panel(const param_handler& P) 
  { set_frame_label("param_handler("+P.File+")"); init(P); }
  /*{\Mcreate creates a |\Mname| for |P| with default header.}*/

  param_panel(const param_handler& P, string s) : panel(s) 
  { init(P); }
  /*{\Mcreate creates a |\Mname| for |P| with header $s$.}*/

  param_panel(const param_handler& P, int w, int h) : panel(w,h) 
  { set_frame_label("param_handler("+P.File+")"); init(P); }
  /*{\Mcreate creates a |\Mname| for |P| of width $w$ and height $h$.}*/

  param_panel(const param_handler& P, int w, int h,string s):panel(w,h,s) 
  { init(P); }
  /*{\Mcreate creates a |\Mname| for |P| of width $w$ and height $h$ with 
     header $s$.}*/

  ~param_panel() {}

private:
  void init(const param_handler& P);
};

#if LEDA_ROOT_INCL_ID == 600130
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
