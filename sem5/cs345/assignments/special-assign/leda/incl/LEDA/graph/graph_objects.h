/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_objects.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPH_OBJECTS_H
#define LEDA_GRAPH_OBJECTS_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600217
#include <LEDA/internal/PREAMBLE.h>
#endif

#if GRAPH_REP == 0
#include <LEDA/graph/graph_objects0.h>
#endif

#if GRAPH_REP == 1
#include <LEDA/graph/graph_objects1.h>
#endif

#if GRAPH_REP == 2
#include <LEDA/graph/graph_objects2.h>
#endif



LEDA_BEGIN_NAMESPACE

inline graph* graph_of(node_struct*);
inline graph* graph_of(edge_struct*);
inline graph* graph_of(face_struct*);


// default (dummy) input operators

inline istream& operator>>(istream& in, node_struct*& v) { v = 0; return in; }
inline istream& operator>>(istream& in, edge_struct*& e) { e = 0; return in; }
inline istream& operator>>(istream& in, face_struct*& f) { f = 0; return in; }

// default compare

inline int compare(node_struct* const& v, node_struct* const& w) 
{ return int((char*)v - (char*)w); }

inline int compare(edge_struct* const& x, edge_struct* const& y) 
{ return int((char*)x - (char*)y); }

inline int compare(face_struct* const& x, face_struct* const& y) 
{ return int((char*)x - (char*)y); }

// type id

inline int leda_type_id(const node_struct**) { return PTR_TYPE_ID; }
inline int leda_type_id(const edge_struct**) { return PTR_TYPE_ID; }
inline int leda_type_id(const face_struct**) { return PTR_TYPE_ID; }


#if LEDA_ROOT_INCL_ID == 600217
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
