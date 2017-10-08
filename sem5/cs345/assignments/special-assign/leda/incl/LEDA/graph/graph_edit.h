/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_edit.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/graph/graph.h>

/*{\Manpage {graph_edit} {} {A Simple Graph Editor} }*/

LEDA_BEGIN_NAMESPACE

extern __exportF void graph_edit(window& W,GRAPH<point,int>& G,bool dir=true,bool redraw=true);

/*{\Mfunc     starts a simple graph editor in window $W$ allowing to display

              and manipulate graph $G$. If the boolean flag $dir$ is $true$
              $G$ is displayed as a directed graph, otherwise, it is displayed
              undirected. }*/

LEDA_END_NAMESPACE


