/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  texture.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/pixmaps/texture/blue.xpm>
#include <LEDA/graphics/pixmaps/texture/clouds.xpm>
#include <LEDA/graphics/pixmaps/texture/dirt.xpm>
#include <LEDA/graphics/pixmaps/texture/pool.xpm>
#include <LEDA/graphics/pixmaps/texture/sharks.xpm>
#include <LEDA/graphics/pixmaps/texture/slate.xpm>
#include <LEDA/graphics/pixmaps/texture/space.xpm>
#include <LEDA/graphics/pixmaps/texture/wall.xpm>
#include <LEDA/graphics/pixmaps/texture/waves.xpm>
#include <LEDA/graphics/pixmaps/texture/wood.xpm>

#include <LEDA/graphics/pixmaps/leda.xpm>
#include <LEDA/graphics/pixmaps/algosol.xpm>

#define num_texture 12

static const char** xpm_texture[] = {
algosol_xpm,
leda_xpm,
blue_xpm,
clouds_xpm,
dirt_xpm,
pool_xpm,
sharks_xpm,
slate_xpm,
space_xpm,
wall_xpm,
waves_xpm,
wood_xpm
};

static const char* name_texture[] = {
"algosol",
"leda",
"blue",
"clouds",
"dirt",
"pool",
"sharks",
"slate",
"space",
"wall",
"waves",
"wood"
};

#if defined(__GNUC__)
inline char texture_unused_warning()
{ return xpm_texture[0][0][0] + name_texture[0][0]; }
#endif


