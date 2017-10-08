/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gml_gwgraph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GMLGWGRAPH_H
#define LEDA_GMLGWGRAPH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600140
#include <LEDA/internal/PREAMBLE.h>
#endif
//------------------------------------------------------------------- //
// class gml_gwgraph, parser for graph in GML format with additional //
//                    attributes like positions, colors, bends, ...   //
//                                                                    //
// by David Alberts (1998)                                            //
//------------------------------------------------------------------- //

#include<LEDA/graph/gml_graph.h>
#include<LEDA/graphics/graphwin.h>
#include<LEDA/core/d_array.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {gml_gwgraph} {} {Reading GraphWin Graphs in GML format}}*/

class __exportC gml_gwgraph : public gml_graph
{

/*{\Mdefinition
An instance $parser$ of the data type |\Mname| is a parser for graph
in GML format~\cite{??}. The class |\Mname| is derived from |gml_graph|
and extends its capabilities with respect to visual attributes of nodes
and edges. You can read GML files created by Graphlet, most Graphlet
attributes are dealed with. It is possible to do a transformation of the
y-coordinates which compensates the difference in the direction
of the y-axis between Graphlet and LEDA windows.

Since |\Mname| is derived from |gml_graph|, all |gml_graph| operations
(e.g., the |parse| methods, see manual page |gml_graph|) can be applied
to |\Mname| as well. The only difference between these classes with
respect to the possible methods are the contructors and the additional
|mirror_y_coordinates| method.

The most obvious use of this class, reading a graph in GML format
into a |GraphWin|, is also provided by the |GraphWin| method |read_gml|
(implemented using this class). Using this class directly provides
a means of customizing the parser using the functionality inherited
from the |gml_graph| parser, e.g., adding a rule for reading additional
application specific node information.
}*/

    enum gwgraph_rules
    {
      node_label=100, node_lpos=101, node_x=102, node_y=103,
      node_w=104, node_h=105, node_shape=106, node_color=107, 
      node_outline=108, node_lcolor=109, node_ltype=110, node_width=111,

      edge_label=200, edge_lpos=201, edge_width=202,
      edge_style=203, edge_color=204, edge_bend=205,
      edge_bendx=206, edge_bendy=207, edge_lcolor=208,
      edge_anchors=209, edge_sancx=210, edge_sancy=211,
      edge_tancx=212, edge_tancy=213, edge_smooth=214,
      edge_shape=215, edge_ltype=216, edge_direction=217
    };
    // 0-99 reserved for basic graph
    // 100000- reserved for custom rules

  public:

    /*{\Mcreation parser }*/

    gml_gwgraph(GraphWin* GW);
    /*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and initializes
                it for the |GraphWin| pointed to by |GW|.}*/

    gml_gwgraph(GraphWin* GW, char* filename, bool mirror=false);
    /*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and reads
                |graph| |G| from the file |filename|. If |mirror| is |true|,
                the y-coordinates are transformed (see above).}*/
    gml_gwgraph(GraphWin* GW, istream& ins, bool mirror=false);
    /*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and reads
                |graph| |G| from the input stream |ins|.}*/


    virtual ~gml_gwgraph() {}

    /*{\Moperations }*/

    bool mirror_y_coordinates(bool m)
    { bool aux = the_mirror; the_mirror = m; return aux; }
    /*{\Mop instructs |\Mname| to mirror the |y|-coordinates after
      parsing, if |m| is |true|. This is useful for visual
      synchronization with tools with increasing |y|-coordinates
      from the top to the bottom of the window. In LEDA windows
      |y|-coordinates are increasing from the bottom to the top.
      The return value indicates whether mirroring was to be performed
      before. This operation does not affect the coordinates of
      a graph which has already been parsed.
    }*/

  protected:

    void init_rules();
    bool interpret(gml_rule r, const gml_object* gobj);
    bool list_end(gml_rule r, const gml_object* gobj);

    GraphWin* gw;
    point current_bend;  // for gathering coordinates of current bend
    point current_sanc;  // ... current source anchor
    point current_tanc;  // ... current target anchor

    list<point> bends;

    bool old_flush;

    color get_color(const gml_object* gobj);

    void get_node_shape(const gml_object* gobj, node v);
    void get_edge_shape(const gml_object* gobj, edge e);
    void get_edge_style(const gml_object* gobj, edge e);
    void get_edge_direction(const gml_object* gobj, edge e);
    void get_node_ltype(const gml_object* gobj, node v);
    void get_edge_ltype(const gml_object* gobj, edge e);
    void get_node_lpos(const gml_object* gobj, node v);
    void get_edge_lpos(const gml_object* gobj, edge e);

    bool graph_intro(const gml_object* gobj);

    d_array<string,gw_position> the_pos;

    bool the_mirror;

    void mirror();

/*{\Mimplementation
The data type |\Mname| inherits from |gml_graph|.}*/

};

/*{\Mtext \bigskip
}*/

/*{\Mtext \subsubsection*{Writing GraphWin Graphs in GML format} }*/

__exportF bool write_gml(GraphWin& GW, ostream& out,
               void (*node_cb)(ostream&,const graph*, const node) = 0,
               void (*edge_cb)(ostream&,const graph*, const edge) = 0);
/*{\Mfuncl writes the graph in |GW| in GML format to the output stream
           |out|. The callback functions |node_cb| and
           |edge_cb| are called once while writing each node or
           edge, respectively. They can be used to write additional
           fields to the GML output. The user is in charge of
           keeping the syntactical correctness of the GML output
           when using the callbacks.
}*/

__exportF bool write_gml(GraphWin& GW, ostream& out,
               void (*node_cb)(ostream&,const GraphWin&, const node),
               void (*edge_cb)(ostream&,const GraphWin&, const edge) );
/*{\Mfuncl writes the graph in |GW| in GML format to the output stream
           |out|. The callback functions |node_cb| and
           |edge_cb| are called once while writing each node or
           edge, respectively. They can be used to write additional
           fields to the GML output. The user is in charge of
           keeping the syntactical correctness of the GML output
           when using the callbacks.
}*/

#if LEDA_ROOT_INCL_ID == 600140
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
