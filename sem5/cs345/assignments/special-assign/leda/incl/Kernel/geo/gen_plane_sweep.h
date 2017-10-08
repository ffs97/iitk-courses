/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gen_plane_sweep.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef GEN_PLANE_SWEEP_H
#define GEN_PLANE_SWEEP_H

/*{\Moptions print_title=yes }*/
/*{\Moptions section=subsection }*/    

#include <LEDA/geo/sweep_observer.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {gen_plane_sweep} {TRAITS} {A Generic Sweep Framework} {PS}}*/


// template<class _TRAITS>

template<class _TRAITS, class INPUT_ITERATOR, class OUTPUT>
class gen_plane_sweep {

// typedef gen_plane_sweep<_TRAITS>  Self;

typedef gen_plane_sweep<_TRAITS,INPUT_ITERATOR,OUTPUT>  Self;

/*{\Mdefinition
  
The data type |\Mname| provides a general framework for algorithms
following the plane sweep paradigm. The plane sweep paradigm can be
described as follows. A vertical line sweeps the plane from left to
right and constructs the desired output incrementally left behind the
sweep line. The sweep line maintains knowledge of the scenery of 
geometric objects and stops at points where changes of this knowledge
relevant for the output occur. These points are called events.

A general plane sweep framework structures the execution of the sweep
into phases and provides a storage place for all data structures necessary to
execute the sweep. An object |\Mvar| of type |\Mname| maintains
an object of type |TRAITS| which generally can be used to store
necessary structures. The content is totally dependent of the 
sweep specification and thereby varies within the application 
domain of the framework.

The only two types that the |TRAITS| class has to provide are
the input and output types |INPUT_ITERATOR| and |OUTPUT|.

The natural phases which determine a sweep are
\begin{Mverb}
  // INITIALIZATION
  initialize_structures();
  check_invariants();

  // SWEEP LOOP
  while ( event_exists() ) {
    process_event();
    check_invariants();
    procede_to_next_event();
  }

  // COMPLETION
  complete_structures();
  check_final();
\end{Mverb}

\begin{description}
\item[ Initialization ] -- initializing the data structures,
ensuring preconditions, checking invariants
\item[ Sweep Loop ] -- iterating over all events, while handling
the event stops, ensuring invariants and the soundness of all
data structures and maybe triggering some animation tasks.
\item[ Completion ] -- cleaning up some data structures and
completing the output.
\end{description}

The above subtasks are members of the class |TRAITS| which a 
model of our TRAITS concept has to provide:
\begin{Mverb}
  void TRAITS::initialize_structures();
  bool TRAITS::event_exists();
  void TRAITS::process_event();
  void TRAITS::procede_to_next_event();
  void TRAITS::complete_structures();
  void TRAITS::check_invariants();
  void TRAITS::check_final();
\end{Mverb}
See specification of the traits class for |\Mname|.


}*/

_TRAITS traits;

/*{\Mtypes 5}*/
public :

typedef _TRAITS TRAITS;
/*{\Mtypemember the traits class}*/ 

//#if !defined(__DECCXX)

#if 0
//some compilers dont like this

typedef typename TRAITS::INPUT_ITERATOR  INPUT_ITERATOR;
/*{\Mtypemember the input iterator.}*/ 

typedef typename TRAITS::OUTPUT OUTPUT;
/*{\Mtypemember the output container.}*/ 

#else

/*
typedef typename _TRAITS::INPUT_ITERATOR  INPUT_ITERATOR;
typedef typename _TRAITS::OUTPUT          OUTPUT;
*/

#endif


/*{\Mevents 6.5}*/

/*{\Mtext To enable animation of the sweep there are event hooks
inserted which allow an observer to attach certain visualization
actions to them. There are four such hooks: }*/

Event_hook<TRAITS&>   post_init_hook;
/*{\Mevent triggered just after initialization.}*/

Event_hook<TRAITS&>   pre_event_hook;
/*{\Mevent triggered just before the sweep event.}*/

Event_hook<TRAITS&>   post_event_hook;
/*{\Mevent triggered just after the sweep event.}*/

Event_hook<TRAITS&>   post_completion_hook;
/*{\Mevent triggered just after the completion phase.}*/

/*{\Mtext All of these are triggered during sweep with the instance
of the |TRAITS| class that is stored inside the plane sweep object.
Thus any animation operation attached to a hook can work on that class
object which maintains the sweep status.}*/


/*{\Mcreation PS}*/

gen_plane_sweep(INPUT_ITERATOR its, INPUT_ITERATOR ite, OUTPUT& output) : 
  traits(its,ite,output) {}
/*{\Mcreate creates a plane sweep object for a sweep on objects determined
by the iterator range defined by $\left[|its|,|ite|\right)$  and delivers 
the result of the sweep in |output|. The traits class |TRAITS| specifies 
the models of all types and the implementations of all function objects used
by |\Mname|. At this point, it suffices to say that |INPUT_ITERATOR| represents
the input data type and |OUTPUT| represents the result data type.}*/

gen_plane_sweep(OUTPUT& output) : traits(output) {}
/*{\Mcreate a simpler call of the above with |output| as |input|.}*/

/*{\Moperations}*/

void sweep()
/*{\Mop execute the plane sweep.}*/
{
  traits.initialize_structures();
  traits.check_invariants();
      post_init_hook(traits);

  while ( traits.event_exists() ) {
        pre_event_hook(traits);
    traits.process_event();
        post_event_hook(traits);
    traits.check_invariants();
    traits.procede_to_next_event();
  }
  traits.complete_structures();
  traits.check_final();
      post_completion_hook(traits);
}


/*{\Mexample
A typical sweep based on |\Mname| looks like the following little
program:
\begin{Mverb}
  std::list<POINT>  P; // fill input
  GRAPH<POINT,LINE> G; // the output
  gen_plane_sweep<triang_sweep_traits> triangulation(P.begin(),P.end(),G);
  triangulation.sweep();
\end{Mverb}}*/

};



LEDA_END_NAMESPACE

#endif // GEN_PLANE_SWEEP_H
