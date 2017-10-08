/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sweep_observer.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef SWEEP_OBSERVER_H
#define SWEEP_OBSERVER_H

#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

// TR is traits
template <class TR>
class client_base 
{
public:
  client_base() {}
  virtual ~client_base() {}
  virtual void call(TR) const = 0;
};


// TR is traits
template <class TR>
class Event_hook 
{ 
  //typedef std::list< client_base<TR>* > clientlist;
  typedef list< client_base<TR>* > clientlist;
protected:
  clientlist clients;
public:
  Event_hook() : clients() {}
  ~Event_hook() {
    while ( !clients.empty() ) {
      delete (*clients.begin());
      clients.pop_front();
    }
  }

  void operator()(TR t) const
  { if ( clients.empty() ) return; 
    for ( typename clientlist::const_iterator it=clients.begin();
	  it != clients.end(); ++it )
      (*it)->call(t);
  }

  void attach(client_base<TR>* psb) 
  { clients.push_back(psb); }

};


// TR is traits, OBS is observer
template <class OBS, class TR>
class client : public client_base<TR>
{
protected:
  OBS& obs_ref;
  void (OBS::* p_fnc)(TR);
  // pointer to member of Observer which works on an object of type TR
public:
  client( OBS& obs, void (OBS::* p_fnc_init)(TR)  ) : 
    obs_ref(obs), p_fnc(p_fnc_init)  {}

  void call(TR t) const
  { (obs_ref.*p_fnc)(t); }
};


template <class OBS, class TR>
inline void attach(Event_hook<TR>& h, 
       OBS& obs, void (OBS::* p_fct)(TR t))
{
  client<OBS,TR>* ps = new client<OBS,TR>(obs,p_fct);
  h.attach( (client_base<TR>*) ps);
}


/*{\Manpage {sweep_observer} {GPS,VTRAITS} {Observing the plane sweep class}}*/  

/*{\Mdefinition 
The data type |\Mname| provides an observer approach to 
the visualization of a sweep algorithm realized by |gen_plane_sweep|
by means of an event mechanism. It allows to connect the events of an 
intstance of |GPS| to the visualization operations provided
by the traits class |VTRAITS|.}*/
 
template <class GPS, class VTRAITS>
class sweep_observer  {

  VTRAITS vt;

  typedef typename GPS::TRAITS      GPSTRAITS;
  typedef typename VTRAITS::VDEVICE VDEVICE;
  
  void post_init_animation(GPSTRAITS& gpst)
  { vt.post_init_animation(gpst); }

  void pre_event_animation(GPSTRAITS& gpst)
  { vt.pre_event_animation(gpst); }

  void post_event_animation(GPSTRAITS& gpst)
  { vt.post_event_animation(gpst); }

  void post_completion_animation(GPSTRAITS& gpst)
  { vt.post_completion_animation(gpst); }

  public :

  /*{\Mcreation obs}*/        

  sweep_observer() : vt() {}
  /*{\Mcreate creates a visualization device as provided by |VTRAITS|
    to visualize a sweep object of type |GPS|.}*/ 

  sweep_observer(GPS& gps);
  /*{\Mcreate creates a visualization device as provided by |VTRAITS|
    to visualize a sweep object of type |GPS| and makes it an observer 
    of |gps|.}*/ 

   /*{\Moperations 2cm 2cm}*/

   void attach(GPS& gps);
   /*{\Mop makes |\Mvar| an observer of |gps|.}*/ 

   VDEVICE& device() 
   { return vt.device(); }

   
};

/*{\Mexample 
A typical sweep observation based on |\Mname| looks like the following little
program:
\begin{Mverb}
  typedef  gen_plane_sweep<triang_sweep_traits>  triang_sweep;
  triang_sweep Ts(...);
  sweep_observer< triang_sweep, 
                  cgal_window_stream_ts_visualization > Obs(Ts);
  Ts.sweep();
\end{Mverb}
This would visualize the sweep actions in the observer window by means
of the visualization functions provided in 
|cgal_\-window_\-stream_\-ts_\-visualization|
}*/    


template <class GPS, class VTRAITS>
sweep_observer<GPS,VTRAITS>::
sweep_observer(GPS& gps) : vt() { attach(gps); }

template <class GPS, class VTRAITS>
void 
sweep_observer<GPS,VTRAITS>::attach(GPS& gps) 
{
#if ((__GNUC__==3 && __GNUC_MINOR__>=4) || __GNUC__> 3)
  attach(gps.post_init_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::post_init_animation); 
  attach(gps.pre_event_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::pre_event_animation); 
  attach(gps.post_event_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::post_event_animation); 
  attach(gps.post_completion_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::post_completion_animation); 

#else
  ::attach(gps.post_init_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::post_init_animation); 
  ::attach(gps.pre_event_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::pre_event_animation); 
  ::attach(gps.post_event_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::post_event_animation); 
  ::attach(gps.post_completion_hook, *this, 
	&sweep_observer<GPS,VTRAITS>::post_completion_animation); 
#endif
}


#if defined(UNDEFINED_LIST)
#define list leda_list
#endif


LEDA_END_NAMESPACE

#endif // SWEEP_OBSERVER_H


