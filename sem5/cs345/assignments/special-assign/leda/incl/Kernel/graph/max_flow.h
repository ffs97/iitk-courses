/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  max_flow.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MAXFLOW
#define LEDA_MAXFLOW

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600005
#include <LEDA/internal/PREAMBLE.h>
#endif

#if defined(mips) && !defined(__GNUC__)
#define __inline  inline
#else
#define __inline
#endif

/*{\Manpage{max_flow}{}{Maximum Flow}}*/

/*{\Mtext 



\settowidth{\typewidth}{|NT|}
\addtolength{\typewidth}{\colsep}
\settowidth{\callwidth}{MAX}
\computewidths

%\newlength{\setspacing}
%\setlength{\setspacing}{0.05em}

%\newcommand{\sset}[1]{\{\hspace{\setspacing}#1\hspace{\setspacing} \}}
\newcommand{\Rnn}{I\! \!R_{\ge 0}}

Let $G = (V,E)$ be a directed graph, let $s$ and $t$ be distinct
vertices in $G$ and let $\mathit{cap}: E \longrightarrow \Rnn$ be a 
non-negative function on the edges of $G$. For an edge $e$, we call
$\mathit{cap}(e)$ the \emph{capacity} of $e$. An \emph{$(s,t)$-flow} or simply
\emph{flow} is a function $f:E \longrightarrow \Rnn$ satisfying the
capacity constraints and the flow conservation constraints:
\[
\begin{array}{lcl}
(1) & 0\leq f(e)\leq cap(e) & \mbox{ for every edge } e\in E\\[0.5em]
(2) & \sum\limits_{e;\mathit{source}(e) = v} f(e) =
      \sum\limits_{e;\mathit{target}(e) = v} f(e) & \mbox{ for every node } v\in V\backslash \sset{s,t}
\end{array} \]
The value of the flow is the net flow into $t$ (equivalently, the 
net flow out of
$s$). The net flow into $t$ is the flow into $t$ minus the flow 
out of $t$. A flow
is maximum if its value is at least as large as the value of any other flow.

All max flow implementations are template functions.
The template parameter  
|NT| can be instantiated with any number type. In order to use the template
version of the function the files

\verb+#include <LEDA/graph/graph_alg.h>+\\
\verb+#include <LEDA/graph/templates/max_flow.h>+

must be included.

There are pre-instantiations for the number types |int| and |double|.
The pre-instantiated versions have the same function names except for 
the suffix \verb+_T+.  In order to use them either

\verb+#include <LEDA/graph/max_flow.h>+

or

\verb+#include <LEDA/graph/graph_alg.h>+ 

has to be included (the latter file includes the former). The connection
between template functions and pre-instantiated functions is discussed in
detail in the section ``Templates for Network Algorithms'' of the LEDA book.

Special care should be taken when using the template 
functions with a number
type |NT| that can incur rounding error, e.g., the type |double|.
The section ``Algorithms on Weighted Graphs and Arithmetic Demand'' 
of the LEDA book contains
a general discussion of this issue.
The template functions
are only guaranteed to perform correctly if all arithmetic performed is 
without rounding error. This is the case if all numerical values in the input
are integers (albeit stored as a number of type |NT|) and 
if none of the intermediate
results exceeds the maximal integer representable by the 
number type ($2^{53} - 1$ in the case of |doubles|).
All intermediate results are sums and differences
of input values, in particular, the algorithms do not use divisions and
multiplications. 

The algorithms have the following arithmetic demands. Let
$C$ be the maximal absolute value of any edge capacity. 
If all capacities are integral
then all intermediate values are bounded by $d\cdot C$, where $d$ is the 
out-degree of the source.


The pre-instantiations for number type |double|
compute the maximum flow for a modified capacity function |cap1|, where
for every edge $e$
\[ |cap1|[e]= |sign|(|cap|[e]) \lfloor \Labs{ |cap[e]| } \cdot S \rfloor / S \]
and $S$ is the largest power of two such that $S < 2^{53}/(d\cdot C)$.

The value of the maximum flow for the modified capacity function and the 
value of the maximum flow  for the original capacity function differ by at
most $m \cdot d \cdot C \cdot 2^{-52}$.

The following functions are available:

}*/

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE

template <class NT> __inline
NT MAX_FLOW_T(const graph& G, node s, node t, const edge_array<NT>& cap,
                                                    edge_array<NT>& f);
/*{\Mfunc computes a maximum $(s,t)$-flow $f$ in the network 
$(G,s,t,|cap|)$ and returns the value of the flow. 

The implementation uses the preflow-push method of 
Goldberg and Tarjan~\cite{GT88} with the local and global relabeling
heuristic and the gap heuristic. The highest level rule is 
used to select active
nodes. The section on maximum flow of the LEDA book gives full information.  
}*/


template <class NT> __inline
NT MAX_FLOW_T(const graph& G, node s, node t, const edge_array<NT>& cap,
                                              edge_array<NT>& f, 
                                              list<node>& st_cut);
/*{\Mfunc as above, also computes a minimum $s-t$ cut in $G$. 
}*/



template <class NT> __inline
bool CHECK_MAX_FLOW_T(const graph& G, node s, node t, const edge_array<NT>& cap,
                                                      const edge_array<NT>& f);
/*{\Mfunc checks whether $f$ is a maximum flow in the network
$(G,s,t,|cap|)$. The functions returns false if this is not the case.}*/



extern __exportF bool MAX_FLOW_SCALE_CAPS(const graph& G, 
                                          node s, edge_array<double>& cap);
/*{\Mfunc replaces |cap[e]| by |cap1[e]| for every edge $e$, where |cap1[e]|
is as defined above.  The function returns |false| 
if the scaling changed some capacity, and returns |true| otherwise.
}*/

template <class NT> __inline
NT MAX_FLOW_T(graph& G, node s, node t, const edge_array<NT>& lcap, 
                                        const edge_array<NT>& ucap,
                                        edge_array<NT>& f);
/*{\Mfunc computes a maximum $(s,t)$-flow $f$ in the network 
$(G,s,t,|ucap|)$ s.th.~$f(e) \leq |lcap[e]|$ for every edge $e$.
If a feasible flow exists, its value returned; otherwise the return value
is -1.
}*/


/* definition of pre-instantiations */

#include <LEDA/graph/instant/max_flow.h>



extern __exportF void max_flow_gen_rand(GRAPH<int,int>& G, node& s, node& t, 
                                                           int n, int m);
/*{\Mfunc A random graph with $n$ nodes, $m$ edges, and random edge
capacities in [2,11] for the edges out of $s$ and in [1,10] for all
other edges.}*/


extern __exportF void max_flow_gen_CG1(GRAPH<int,int>& G, node& s, node& t, 
                                                                   int n);
/*{\Mfunc A generator suggested by Cherkassky and Goldberg.}*/


extern __exportF void max_flow_gen_CG2(GRAPH<int,int>& G, node& s, node& t, 
                                                                   int n);
/*{\Mfunc Another generator suggested by Cherkassky and Goldberg.}*/


extern __exportF void max_flow_gen_AMO(GRAPH<int,int>& G, node& s, node& t, 
                                                                   int n);
/*{\Mfunc A generator suggested by Ahuja, Magnanti, and Orlin.}*/


#if LEDA_ROOT_INCL_ID == 600005
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif


