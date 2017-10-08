/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gml_graph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GML_GRAPH_H
#define LEDA_GML_GRAPH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600195
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------- //
// class gml_graph, parser for graph in GML format                   //
//                                                                    //
// by David Alberts (1997)                                            //
//------------------------------------------------------------------- //

#include<LEDA/graph/graph.h>
#include<LEDA/core/map.h>
#include<LEDA/graph/gml_parser.h>

LEDA_BEGIN_NAMESPACE

typedef bool (*gml_graph_rule)(const gml_object*, graph*);
typedef bool (*gml_node_rule)(const gml_object*, graph*, node);
typedef bool (*gml_edge_rule)(const gml_object*, graph*, edge);

inline istream& operator>> (istream& in, const gml_graph_rule&) { return in; }
inline istream& operator>> (istream& in, const gml_node_rule&)  { return in; }
inline istream& operator>> (istream& in, const gml_edge_rule&)  { return in; }


/*{\Manpage {gml_graph} {} {GML Parser for Graphs}}*/


class __exportC gml_graph : public gml_parser
{

/*{\Mdefinition
An instance $parser$ of the data type |\Mname| is a parser for graph
in GML format \cite{HI97}. It is possible to extend the parser by
user defined rules. This parser is used by the $read\_gml$ of class
$graph$. The following is a small example graph (a triangle) in GML format.
\vbox{
\begin{quote}
\small
\tt
\# This is a comment.\\
graph [\hspace*{10ex}\# Lists start with '['.\\
\hspace*{2ex}directed 1\hspace*{4ex}\# This is a directed graph (0 for undirected).\\
\\
\hspace*{2ex}\# The following is an object of type string.\\
\hspace*{2ex}\# It will be ignored unless you specify a rule for
                {\it graph.text}.\\
\hspace*{2ex}text "This is a string object."\\
\\
\hspace*{2ex}node [ id 1 ] \# This defines a node with id 1.\\
\hspace*{2ex}node [ id 2 ]\\
\hspace*{2ex}node [ id 3 ]\\
\\
\hspace*{2ex}edge [ \# This defines an edge leading from node 1 to node 2. \\
\hspace*{4ex}source 1\\
\hspace*{4ex}target 2\\
\hspace*{2ex}]\\
\hspace*{2ex}edge [\\
\hspace*{4ex}source 2\\
\hspace*{4ex}target 3\\
\hspace*{2ex}]\\
\hspace*{2ex}edge [\\
\hspace*{4ex}source 3\\
\hspace*{4ex}target 1\\
\hspace*{2ex}]\\
] \# Lists end with ']'.
\end{quote}
}

An input in GML format is a list of GML objects. Each object consists
of a key word and a value. A value may have one out of four possible
types, an integer (type $gml\_int$), a double (type $gml\_double$),
a string (type $gml\_string$), or a list of GML objects (type
$gml\_list$). Since a value can be a list of objects, we get a tree
structure on the input. We can describe a class $C$ of objects being
in the same list and having the same key word by the so-called path.
The path is the list of key words leading to an object in the class $C$.

In principle, every data structure can be expressed in GML format.
This parser specializes on graphs. A graph is represented by an object
with key word $graph$ and type $gml\_list$. The nodes of the graph
are objects with path $graph.node$ and type $gml\_list$. Each node
has a unique identifier, which is represented by an object of type
$gml\_int$ with path $graph.node.id$. An edge is an object of type
$gml\_list$ with the path $graph.edge$. Each edge has a source and
a target. These are objects of type $gml\_int$ with path $graph.edge.source$
and $graph.edge.target$, respectively. The integer values of $source$
and $target$ refer to node identifiers. There are some global graph
attributes, too. An object of type $gml\_int$ with path $graph.directed$
determines whether the graph is undirected (value 0) or directed
(every other integer). The type of node parameters and edge parameters
in parameterized graph (see manual page GRAPH) can be given by
objects of type $gml\_string$ with path $graph.nodeType$ and
$graph.edgeType$, respectively. Parameters of nodes and edges
are represented by objects of type $gml\_string$ with path
$graph.node.parameter$ and $graph.edge.parameter$, respectively.

No list has to be in a specific order, e.g., you can freely mix
$node$ and $edge$ objects in the $graph$ list. If there are several
objects in a class where just one object is required like $graph.node.id$,
only the last such object is taken into account.

Objects in classes with no predefined rules are simply ignored. This
means that an application $A$ might add specific objects to a graph
description in GML format and this description is still readable
for another application $B$ which simply does not care about the
objects which are specific for $A$.

This parser supports reading user defined objects by providing
a mechanism for dealing with those objects by means of callback
functions. You can specify a rule for, e.g., objects with path
$graph.node.weight$ and type $gml\_double$ like in the following
code fragment.
\vbox{
\small
\begin{quote}
\tt
\ldots\\
bool get\_node\_weight(const gml\_object* gobj, graph* G, node v)\\
\{\\
\hspace*{2ex}double w = gobj->get\_double();\\
\hspace*{2ex}{\it do something with w, the graph and the
                  corresponding node v}\\
\hspace*{2ex}return true;   {\it or false if the operation failed}\\
\}\\
\ldots\\
main()\\
\{ \\
\hspace*{2ex}char* filename;\\
\hspace*{2ex}\ldots\\
\hspace*{2ex}graph G;\\
\hspace*{2ex}gml\_graph parser(G);\\
\hspace*{2ex}parser.append("graph"); parser.append("node"); parser.append("weight");\\
\hspace*{2ex}parser.add\_node\_rule\_for\_cur\_path(get\_node\_weight,gml\_double);\\
\hspace*{2ex}// {\it or short} parser.add\_node\_rule(get\_node\_weight,gml\_double,"weight");\\
\hspace*{2ex}bool parsing\_ok = parser.parse(filename);\\
\hspace*{2ex}\ldots\\
\}
\end{quote}
}

You can add rules for the graph, for nodes, and for edges. The
difference between them is the type. The type of node rules is
as in the example above
\mbox{\tt bool (*gml\_node\_rule)(const gml\_object*, graph*, node)},
the type for edge rules is
\mbox{\tt bool (*gml\_edge\_rule)(const gml\_object*, graph*, edge)},
and the type for graph rules is
\mbox{\tt bool (*gml\_graph\_rule)(const gml\_object*, graph*)}.
A GML object is represented by an instance of class {\tt gml\_object}.
You can get its value by using \mbox{\tt double\ gml\_object::get\_double()},
\mbox{\tt int\ gml\_object::get\_int()} or
\mbox{\tt char*\ gml\_object::get\_string()}. If one of your rules
returns $false$ during parsing, then parsing fails and the graph is
cleared.
}*/

  public:

    enum //graph_rules
    {
      new_graph=0, directed=1, nodeType=2, edgeType=3,
      new_node=4, node_index=5, node_param=6,
      new_edge=7, edge_source=8, edge_target=9, edge_param=10,
      custom_rule_start=100000
    };


    /*{\Mcreation parser }*/

    gml_graph(graph& G) : the_graph(&G), rules_set(false), node_by_id(0), 
                          next_rule(custom_rule_start) {}
    /*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and initializes
                it for |graph| |G|.}*/

    gml_graph(graph& G, const char* filename) : the_graph(&G), rules_set(false),
                          node_by_id(0), next_rule(custom_rule_start)
      { parse(filename); }
    /*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and reads
                |graph| |G| from the file |filename|.}*/

    gml_graph(graph& G, istream& ins) : the_graph(&G), rules_set(false),
                          node_by_id(0), next_rule(custom_rule_start)
      { parse(ins); }
    /*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and reads
                |graph| |G| from the input stream |ins|.}*/

    virtual ~gml_graph() {}



    /*{\Moperations 1 4 }*/


    /*{\Mtext
    \medskip
    {\bf 3.1 Parsing}
    \medskip }*/

    bool parse(const char* filename);
    /*{\Mop parses the input taken from the file |filename| using
            the current set of rules. The graph specified in the
            constructor is set up accordingly. This operations returns
            |false| and clears the graph, if syntax or parse errors
            occur. Otherwise |true| is returned.}*/

    bool parse(istream& ins);
    /*{\Mop parses the input taken from the input stream |ins|.}*/

    bool parse_string(string s);
    /*{\Mop parses the input taken from string |s|.}*/


    /*{\Mtext
    \medskip
    {\bf 3.2 Path Manipulation}
    \medskip }*/


    void reset_path() { gml_parser::reset_path(); }
      /*{\Mop resets the current path to the empty path. }*/

    void append(const char* key) { gml_parser::append(key); }
      /*{\Mop appends |key| to the current path. }*/

    void goback()          { gml_parser::goback(); }
      /*{\Mop removes the last key word from the current path.
              If the current path is empty this operation has no effect. }*/


    /*{\Mtext
    \medskip
    {\bf 3.3 User Defined Rules}
    \medskip }*/

    void add_graph_rule_for_cur_path(gml_graph_rule f, gml_value_type t)
	{ add_graph_rule(f,t,0); }
    /*{\Mop adds graph rule |f| for value type |t| and for the current path.}*/
    void add_node_rule_for_cur_path(gml_node_rule f, gml_value_type t)
	{ add_node_rule(f,t,0); }
    /*{\Mop adds node rule |f| for value type |t| and for the current path.}*/
    void add_edge_rule_for_cur_path(gml_edge_rule f, gml_value_type t)
	{ add_edge_rule(f,t,0); }
    /*{\Mop adds edge rule |f| for value type |t| and for the current path.}*/

    void add_graph_rule(gml_graph_rule f, gml_value_type t, char* key=0);
    /*{\Mop adds graph rule |f| for value type |t| and path $graph.key$
            to |\Mvar|, if key is specified. Otherwise, |f| is added
            for the current path.}*/
    void add_node_rule(gml_node_rule f, gml_value_type t, char* key=0);
    /*{\Mop adds node rule |f| for path $graph.node.key$ (or the current
            path, if no key is specified) and value type |t| to |\Mvar|.}*/
    void add_edge_rule(gml_edge_rule f, gml_value_type t, char* key=0);
    /*{\Mop adds edge rule |f| for path $graph.edge.key$ (or the current
            path, if no key is specified) and value type |t| to |\Mvar|.}*/

    void add_new_graph_rule(gml_graph_rule f)
      { new_graph_rules.append(f); }
      /*{\Mop adds graph rule |f| to |\Mvar|. During parsing |f| is called
              whenever an object $o$ with path $graph$ and type
              $gml\_list$ is encountered. |f| is called before objects
              in the list of $o$ are parsed.}*/
    void add_new_node_rule(gml_node_rule f)
      { new_node_rules.append(f); }
      /*{\Mop adds node rule |f| for path $graph.node$ and value type 
              $gml\_list$ to |\Mvar|. |f| is called before objects in
              the corresponding list are parsed.}*/
    void add_new_edge_rule(gml_edge_rule f)
      { new_edge_rules.append(f); }
      /*{\Mop adds edge rule |f| for path $graph.edge$ and value type 
              $gml\_list$ to |\Mvar|. |f| is called before objects in
              the corresponding list are parsed.}*/

    void add_graph_done_rule(gml_graph_rule f)
      { graph_done_rules.append(f); }
      /*{\Mop adds graph rule |f| to |\Mvar|. During parsing |f| is called
              whenever an object $o$ with path $graph$ and type $gml\_list$
              is encountered. |f| is called after all objects in the list
              of $o$ are parsed.}*/
    void add_node_done_rule(gml_node_rule f)
      { node_done_rules.append(f); }
      /*{\Mop adds node rule |f| to |\Mvar| for path $graph.node$ and
              value type $gml\_list$. |f| is called after all objects
              in the corresponding list are parsed.}*/
    void add_edge_done_rule(gml_edge_rule f)
      { edge_done_rules.append(f); }
      /*{\Mop adds edge rule |f| to |\Mvar| for path $graph.edge$ and
              value type $gml\_list$. |f| is called after all objects
              in the corresponding list are parsed.}*/


  protected:

    void init_rules();
    void cleanup() { if(errors()) the_graph->clear(); }

    bool interpret(gml_rule r, const gml_object* gobj);
    bool list_end(gml_rule r, const gml_object* gobj);

    bool graph_intro(const gml_object* gobj);
    bool graph_end(const gml_object* gobj);
    bool edge_end(const gml_object* gobj);

    graph*                  the_graph;
    node                    current_node;
    bool                    has_id;        // current node has an id
    edge                    current_edge;

    bool                    right_node_type;
    bool                    right_edge_type;

    bool                    rules_set;     // init_rules has already been called

    node                    dummy1,dummy2;

    map<int,node>*          node_by_id;

    map<edge,int>*          edge_s;
    map<edge,int>*          edge_t;

    map<int,gml_graph_rule> graph_rules;
    map<int,gml_node_rule>  node_rules;
    map<int,gml_edge_rule>  edge_rules;

    slist<gml_graph_rule>   new_graph_rules;
    slist<gml_node_rule>    new_node_rules;
    slist<gml_edge_rule>    new_edge_rules;

    slist<gml_graph_rule>   graph_done_rules;
    slist<gml_node_rule>    node_done_rules;
    slist<gml_edge_rule>    edge_done_rules;

    int                     next_rule;
    int                     node_count;
};

/*{\Mimplementation
The data type gml\_graph is realized using lists and maps. It
inherits from gml\_parser which uses gml\_object, gml\_objecttree,
and gml\_pattern. gml\_pattern uses dictionaries.}*/


#if LEDA_ROOT_INCL_ID == 600195
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
