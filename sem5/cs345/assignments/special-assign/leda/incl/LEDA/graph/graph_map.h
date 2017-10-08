/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_map.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPH_MAP_H
#define LEDA_GRAPH_MAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600191
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// graph_map:  base type for node/edge/face arrays and node/edge/face maps
//------------------------------------------------------------------------------


template <class graph_t>
class graph_map {

protected:
typedef typename graph_t::node node;
typedef typename graph_t::edge edge;
typedef typename graph_t::face face;


private:

int           kind;  // node/edge/face
GenPtr*       table;
int           table_size;
graph_t*      g;
int           g_index;
list_item     g_loc;
GenPtr        def_entry; 

GenPtr*       old_table;
int           old_index; // value < 0 indicates that only array access is allowed


int  next_power(int s) const
{ if (s==0) return 0;
  int p = 1;
  while (p < s) p <<= 1;
  return p;
 }




void resize_table(int sz)
{ 
 if (old_table) LEDA_DEL_VECTOR(old_table);

  old_table = table;

  GenPtr* old_stop  = table + table_size;
  
  table_size = sz;
  table = LEDA_NEW_VECTOR(GenPtr,sz);

  GenPtr* p = old_table; 
  GenPtr* q = table;
  while (p < old_stop) *q++ = *p++;

  if (table_size == 0) old_table = 0;

  init_table(q,table+sz);
}



public:

virtual void read_entry(istream&, GenPtr&)
{ LEDA_EXCEPTION(1,"Compile with -DLEDA_GMAP_IO for graph_map I/O."); }

virtual void write_entry(ostream&, GenPtr&) const
{ LEDA_EXCEPTION(1,"Compile with -DLEDA_GMAP_IO for graph_map I/O."); }


void init_table(GenPtr* start, GenPtr* stop)
{  if (g_index == -1)
     for(GenPtr* q=start; q < stop; q++) init_entry(*q);
   else
     //if (g && g_index > 0) /* changed by s.n. */
     if (g)
     { 
       switch (kind) {
       case 0 : { node v;
                  for(v=g->first_node(); v!=g->stop_node(); v=g->next_node(v))
                     init_entry(v->data(g_index));
                  break;
                 }
       case 1 : { edge e;
                  for(e=g->first_edge(); e!=g->stop_edge(); e=g->next_edge(e))
                     init_entry(e->data(g_index));
                  break;
                 }
       case 2 : { face f;
                  for(f=g->first_face(); f!=g->stop_face(); f=g->next_face(f))
                     init_entry(f->data(g_index));
                  break;
                 }
       }
     }
}

void init_table() { init_table(table,table+table_size); }

void clear_table()
{ 
  if (g_index == -1)
    { GenPtr* stop = table + table_size;
      for(GenPtr* q=table; q < stop; q++) clear_entry(*q);
     }
  else
     if (g && g_index > 0)
     { switch (kind) {
       case 0 : { node v;
                  for(v=g->first_node(); v!=g->stop_node(); v=g->next_node(v))
                     clear_entry(v->data(g_index));
                  break;
                 }
       case 1 : { edge e;
                  for(e=g->first_edge(); e!=g->stop_edge(); e=g->next_edge(e))
                     clear_entry(e->data(g_index));
                  break;
                 }
       case 2 : { face f;
                  for(f=g->first_face(); f!=g->stop_face(); f=g->next_face(f))
                     clear_entry(f->data(g_index));
                  break;
                 }
       }
     }

 }


virtual void init_entry(GenPtr&)  const {}
virtual void clear_entry(GenPtr&) const {}
virtual void copy_entry(GenPtr&)  const {}

void init_def_entry() { init_entry(def_entry); }
void clear_def_entry() { clear_entry(def_entry); }


int        slot_index() const { return g_index; }
list_item  get_loc()   const { return g_loc;   }
int        get_kind() const { return kind;   }
graph_t*   get_owner() const { return g; }
void       set_owner(graph_t* p) { g = p; }


virtual int cmp_entry(GenPtr,GenPtr) const  { return 0; }

virtual int elem_type_id() const { return UNKNOWN_TYPE_ID; }


int     size() const { return table_size; }
GenPtr& access(int i)     { return table[i]; }
GenPtr  read(int i) const { return table[i]; }


GenPtr& table_map_access(int i)
{ 
  if (i >= table_size) 
    resize_table(next_power(i+1));
  else
    { if (old_table) 
      { table[old_index] = old_table[old_index];
/*
  assert(old_index < table_size);
*/

        LEDA_DEL_VECTOR(old_table);
        old_table = 0;
       }
      old_index = i;
     }
  return table[i]; 
}

void enable_map_access() { old_index = 0; }

void re_init_entry(node v)
{ if (g_index > -1) 
    init_entry(v->data(g_index));
  else 
   { int i = g->index(v);
     if (i < table_size) 
     { clear_entry(table[i]);
       init_entry(table[i]);
      }
	 else if (old_index >= 0) { // map_access enabled
	   resize_table(next_power(i+1));
	 }
    }
 }

void re_init_entry(edge e)
{ if (g_index > -1) 
    init_entry(e->data(g_index));
  else 
   { int i = g->index(e);
     if (i < table_size) 
     { clear_entry(table[i]);
       init_entry(table[i]);
      }
	 else if (old_index >= 0) { // map_access enabled
	   resize_table(next_power(i+1));
	 }
    }
 }


void re_init_entry(face f)
{ if (g_index > -1) 
    init_entry(f->data(g_index));
  else 
   { int i = g->index(f);
     if (i < table_size) 
     { clear_entry(table[i]);
       init_entry(table[i]);
      }
	 else if (old_index >= 0) { // map_access enabled
	   resize_table(next_power(i+1));
	 }
    }
 }




GenPtr& slot_access(node v) const { return v->data(g_index); }
GenPtr& slot_access(edge e) const { return e->data(g_index); }
GenPtr& slot_access(face f) const { return f->data(g_index); }


GenPtr& array_access(node v) const
{ if (g_index > -1) 
    return v->data(g_index);
  else {
#if ! defined(LEDA_CHECKING_OFF)
   if (g->index(v) >= table_size || !g->member(v)) 
     LEDA_EXCEPTION(999,"node_array: illegal node");
#endif
   return table[g->index(v)];
  }
}


GenPtr& array_access(edge e) const
{ if (g_index > -1) 
    return e->data(g_index);
  else {
#if ! defined(LEDA_CHECKING_OFF)
   if (g->index(e) >= table_size || !g->member(e)) 
     LEDA_EXCEPTION(999,"edge_array: illegal edge");
#endif
   return table[g->index(e)];
  }
}


GenPtr& array_access(face f) const
{ if (g_index > -1) 
    return f->data(g_index);
  else {
#if ! defined(LEDA_CHECKING_OFF)
   if (g->index(f) >= table_size || !g->member(f))
     LEDA_EXCEPTION(999,"face_array: illegal face");
#endif
   return table[g->index(f)];
  }
}


const GenPtr& array_read(node v) const { return array_access(v); }
const GenPtr& array_read(edge e) const { return array_access(e); }
const GenPtr& array_read(face f) const { return array_access(f); }


GenPtr& map_access(node v)
{ 
#if ! defined(LEDA_CHECKING_OFF)
if (!g->member(v)) LEDA_EXCEPTION(999,"node_map: illegal node");
#endif
  if (g_index > -1) 
    return v->data(g_index); 
  else
    return table_map_access(g->index(v));
}

GenPtr& map_access(edge e)
{
#if ! defined(LEDA_CHECKING_OFF)
if (!g->member(e)) LEDA_EXCEPTION(999,"edge_map: illegal edge");
#endif
  if (g_index > -1) 
    return e->data(g_index); 
  else
    return table_map_access(g->index(e));
}

GenPtr& map_access(face f)
{
#if ! defined(LEDA_CHECKING_OFF)
if (!g->member(f)) LEDA_EXCEPTION(999,"face_map: illegal face");
#endif
  if (g_index > -1) 
    return f->data(g_index); 
  else
    return table_map_access(g->index(f));
}





const GenPtr& map_read(node v) const
{ 
#if ! defined(LEDA_CHECKING_OFF)
if (!g->member(v)) LEDA_EXCEPTION(999,"node_map: illegal node");
#endif
  if (g_index > -1) 
    return v->data(g_index); 
  else
   { int i = g->index(v);
     if (i < table_size)
        return table[i];
     else
        return def_entry;
    }
}


const GenPtr& map_read(edge e) const
{
#if ! defined(LEDA_CHECKING_OFF)
if (!g->member(e)) LEDA_EXCEPTION(999,"edge_map: illegal edge");
#endif
  if (g_index > -1) 
    return e->data(g_index); 
  else
   { int i = g->index(e);
     if (i < table_size)
        return table[i];
     else
        return def_entry;
    }
}


const GenPtr& map_read(face f) const
{
#if ! defined(LEDA_CHECKING_OFF)
if (!g->member(f)) LEDA_EXCEPTION(999,"face_map: illegal face");
#endif
  if (g_index > -1) 
    return f->data(g_index); 
  else
   { int i = g->index(f);
     if (i < table_size)
        return table[i];
     else
        return def_entry;
    }
}

int init(const graph_t* G, int sz, int k, bool use_slot=false) 
{ 
  clear_table();

  if (g != G)
  { if (g && g_index != 0) g->unregister_map(this);
    kind = k;
    g = (graph_t*)G;
    if (g) g_loc = g->register_map(this,g_index,use_slot);
  }

  if (g_index > -1) 
  { table = 0;
    table_size = 0;
   }
  else
  { int tsz = next_power(sz);
    if (tsz != table_size)
    { if (table_size > 0) LEDA_DEL_VECTOR(table);
      table = 0;
      table_size = tsz;
      if (table_size > 0) table = LEDA_NEW_VECTOR(GenPtr,table_size);
     }
   }
  if (old_table) { LEDA_DEL_VECTOR(old_table); old_table = 0; }

  return g_index; 
}

void init(const graph_t* G, int k) 
{ kind = k; 
  g = (graph_t*)G;
  g_index = 0;
  table = 0;
  table_size = 0;
  old_table = 0;
}



graph_map(const graph_t* G, int k) 
{ kind = k; 
  g = (graph_t*)G;
  g_index = 0;
  table = 0;
  table_size = 0;
  old_table = 0;
  old_index = -1; // by default array access
}


graph_map(const graph_t* G, int sz, int k) 
{ 
  kind = k; 
  g = (graph_t*)G;
  g_index = -1;
  old_table = 0;
  old_index = -1; // by default array access

  if (g) g_loc = g->register_map(this,g_index);

  if (g_index > -1) 
  { table = 0;
    table_size = 0;
    return; 
   }

  def_entry = 0;
  table = 0;
  table_size = next_power(sz);
  if (table_size > 0) table = LEDA_NEW_VECTOR(GenPtr,table_size);
}


graph_map(const graph_map<graph_t>& M)
{ old_table = 0;
  old_index = M.old_index < 0 ? -1 : 0;
  kind = M.kind;
  g_index = M.g_index;
  g = M.g;


  def_entry = 0;
  table = 0;
  table_size = M.table_size;

  if (M.g_index > -1) 
  { g_loc = 0;
    return;
   }

  if (g) g_loc = g->register_map(this,g_index);

  if (table_size > 0)
  { table = LEDA_NEW_VECTOR(GenPtr,table_size);
    GenPtr* p = table;
    GenPtr* stop = M.table+M.table_size;
    for(GenPtr* q=M.table; q < stop; q++) 
    { *p = *q;
      M.copy_entry(*p); 
      p++;
     }
   }
}


graph_map<graph_t>& operator=(const graph_map<graph_t>& M)
{ if (&M == this) return *this;
  clear_table();
  if (table_size > 0) LEDA_DEL_VECTOR(table);
  if (old_table) { LEDA_DEL_VECTOR(old_table); old_table = 0; }
  if (g && g_index != 0) g->unregister_map(this);
  table = 0;
  kind = M.kind;
  g = M.g;
  if (M.g_index == 0)
  { g_index = 0;
    table = 0;
    return *this;
   }
  g_index = -1;
  if (g) g_loc = g->register_map(this,g_index);
  table_size = M.table_size;
  if (table_size > 0)
  { table = LEDA_NEW_VECTOR(GenPtr,table_size);
    GenPtr* p = table;
    GenPtr* stop = M.table+M.table_size;
    for(GenPtr* q=M.table; q < stop; q++) 
    { *p = *q;
      copy_entry(*p); 
      p++;
     }
   }
  return *this;
}

void unregister() { if (g && g_index != 0 && g_loc) g->unregister_map(this); }

virtual 
~graph_map()
{ unregister();
  if (table) LEDA_DEL_VECTOR(table);
  if (old_table) LEDA_DEL_VECTOR(old_table);
 } 


// dummy I/O operators

friend ostream& operator<<(ostream& ostr, const graph_map<graph_t>&) 
{ return ostr; }

friend istream& operator>>(istream& istr, graph_map<graph_t>&) 
{ return istr; }

};


#if LEDA_ROOT_INCL_ID == 600191
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif
