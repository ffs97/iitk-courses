/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  panel.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PANEL_H 
#define LEDA_PANEL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600145
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graphics/window.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
//   PANELS
//------------------------------------------------------------------------------

/*{\Manpage {panel} {} {Panels}}*/

class __exportC panel : public window {

/*{\Mdefinition
Panels are windows consisting of a panel section only (cf. section
\ref{Windows}). They are used for displaying text messages and updating the
values of variables.
}*/

public:

/*{\Mcreation P }*/

 panel() : window(-1,-1) {}
/*{\Mcreate creates an empty panel $P$.}*/

 panel(string s) : window(-1,-1,s) {}
/*{\Mcreate creates an empty panel $P$ with header $s$.}*/

 panel(int w, int h) : window(w,h) {}
/*{\Mcreate creates an empty panel $P$ of width $w$ and height $h$.}*/


 panel(int w, int h,string s) : window(w,h,s) {}
/*{\Mcreate creates an empty panel $P$ of width $w$ and 
            height $h$ with header $s$.}*/

// for backward compatibility
panel(string s, int w, int h) : window(w,h,s) {}


~panel() {}


/*{\Moperations 1.2 4.8 }*/

/*{\Mtext       
All window operations for displaying, reading, closing and adding panel 
items are available (see section \ref{panel-operations}). There are two 
additional operations for opening and reading panels.
}*/

// open = display + read + close

int  open(int x = window::center, int y = window::center)
{ return panel_open(x,y,0); } 
/*{\Mopl   |P.display(x,y)| $+$ |P.read_mouse()| $+$ |P.close()|.}*/


int open(window& W, int x=window::center, int y=window::center)
{ return panel_open(x,y,&W); } 
/*{\Mopl   |P.display(W,x,y)| $+$ |P.read_mouse()| $+$ |P.close()|.}*/


};


#if LEDA_ROOT_INCL_ID == 600145
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
