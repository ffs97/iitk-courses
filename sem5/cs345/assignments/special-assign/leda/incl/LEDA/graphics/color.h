/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  color.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_COLOR_H
#define LEDA_COLOR_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600141
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/internal/system.h>
#include <LEDA/system/basic.h>
#include <LEDA/graphics/x_window.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {color} {} {Colors}}*/

class __exportC color {

/*{\Mdefinition
The data type $color$ is the type of all colors available for
drawing operations in windows (cf. \ref{Windows}).
Each color is defined by a triple of integers $(r,g,b)$ with
$0\le r,g,b \le 255$, the so-called {\em rgb-value} of the color.
The number of available colors is restricted and depends on the 
underlying hardware. Colors can be created from rgb-values,
from names in a color data base (X11), or from the 16 integer 
constants (enumeration in \<LEDA/graphics/x\_window.h\>)
$black$, $white$, $red$, $green$, $blue$, $yellow$, $violet$, $orange$;
$cyan$, $brown$, $pink$, $green2$, $blue2$, $grey1$, $grey2$, $grey3$. 
}*/

  int  col_index;
  bool ok;

public:

/*{\Mcreation col}*/

  color();
/*{\Mcreate  creates a color with rgb-value $(0,0,0)$ (i.e. black). }*/

  color(int r, int g, int b);
/*{\Mcreate  creates a color with rgb-value $(r,g,b)$. }*/

  color(const char* name);
/*{\Mcreate  creates a color and initializes it with the rgb-value
             of color $name$ from the X11 color data base ($(0,0,0)$ 
             if $name$ does not exist). }*/

  color(color_enum i);

  color(int i);
/*{\Mcreate  creates a color and initializes it with one of the
             16 predefined colors. Here $i$ is one of the 16 integer
             constants $black$, $white$, $red$, $green$, $blue$, 
             $yellow$, $violet$, $orange$, $cyan$, $brown$, $pink$,                          $green2$, $blue2$, $grey1$, $grey2$, or $grey3$.  }*/


/*{\Moperations 1.5 6}*/

  void set_rgb(int r, int g, int b);
/*{\Mop    sets the rgb-value of |\Mvar| to $(r,g,b)$. }*/

  void get_rgb(int& r, int& g, int& b) const;
/*{\Mop    assigns the rgb-value of |\Mvar| to $(r,g,b)$. }*/

  void set_red(int r);
/*{\Mop    changes the  r-value of |\Mvar| to $r$. }*/

  void set_green(int g);
/*{\Mop    changes the  g-value of |\Mvar| to $g$. }*/

  void set_blue(int b);
/*{\Mop    changes the  b-value of |\Mvar| to $b$. }*/

  bool is_good() const { return ok; }

  int  get_index() const { return col_index; }

  operator int() const  { return col_index; }


#if defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x520)

friend bool operator==(const color& c1 , const color& c2)
{ return c1.col_index == c2.col_index; }

friend bool operator!=(const color& c1 , const color& c2)
{ return c1.col_index != c2.col_index; }

#endif

};

extern __exportF istream& operator>>(istream& in, color& c);
extern __exportF ostream& operator<<(ostream& out, const color& c);


#if LEDA_ROOT_INCL_ID == 600141
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
