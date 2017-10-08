/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ugraph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_UGRAPH_H
#define LEDA_UGRAPH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600218
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE


//-----------------------------------------------------------------------------
// ugraph: base class for all undirected graph
//-----------------------------------------------------------------------------


/*{\Manpage {ugraph} {} {Undirected Graphs}}*/

//LEDA_BEGIN_NAMESPACE

class __exportC ugraph : public graph {

/*{\Mdefinition
An instance $U$ of the data type $ugraph$ is an undirected graph as defined
in section \ref{Graphs}. }*/

public:

/*{\Mcreation U }*/

ugraph()  { make_undirected(); }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it to 
            the empty undirected graph. }*/

ugraph(const graph& G) : graph(G) { make_undirected(); }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it with 
            an undirected copy of $G$. }*/

ugraph(const ugraph& u) : graph(u) {} 

~ugraph() {}

ugraph& operator=(const ugraph& u) 
{ graph::operator=(u); return *this; }

};

/*{\Moperations 2 4}*/

/*{\Mtext       
see section \ref{Graphs}.
}*/


/*{\Mimplementation
see section \ref{Graphs}.
}*/



//------------------------------------------------------------------------------
// UGRAPH: generic ugraph
//------------------------------------------------------------------------------

/*{\Manpage {UGRAPH} {vtype,etype} {Parameterized Ugraph}}*/

template<class vtype, class etype>

class UGRAPH : public GRAPH<vtype,etype> {

/*{\Mdefinition
A parameterized undirected graph $G$ is an undirected graph whose nodes and
contain additional (user defined) data (cf. \ref{Parameterized Graphs}). Every 
node contains an element of a data type $vtype$, called the node type of $G$ 
and every edge contains an element of a data type $etype$ called the edge type 
of $G$. }*/

public:

/*{\creation G }*/

UGRAPH() { graph::make_undirected(); }
/*{\Mcreate creates an instance |\Mvar| of type $ugraph$ and initializes it to the
    empty undirected graph.}*/

UGRAPH(const GRAPH<vtype,etype>& G) : GRAPH<vtype,etype>(G) 
{ graph::make_undirected(); }

UGRAPH(const UGRAPH<vtype,etype>& U) : GRAPH<vtype,etype>(U) {}

~UGRAPH() {}


UGRAPH<vtype,etype>& operator=(const UGRAPH<vtype,etype>& a)
{ graph::clear_all_entries();
  graph::operator=(a);
  graph::copy_all_entries();
  return *this; 
 }


/*{\Moperations 2 5.0}*/


/*{\Mtext
see section \ref{Parameterized Graphs}.
}*/

};

//LEDA_END_NAMESPACE


/*{\Mimplementation
see section \ref{Parameterized Graphs}.
}*/


#if LEDA_ROOT_INCL_ID == 600218
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
