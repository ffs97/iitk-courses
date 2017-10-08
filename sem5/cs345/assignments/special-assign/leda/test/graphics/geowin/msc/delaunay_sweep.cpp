/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  delaunay_sweep.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//#define GEOWIN_USE_NAMESPACE

#define LEDA_CHECKING_OFF

#include <LEDA/sortseq.h>
#include <LEDA/p_queue.h>
#include <LEDA/graph.h>
#include <LEDA/rat_kernel.h>
#include <LEDA/rat_kernel_types.h>
#include <LEDA/plane_sweep.h>
#include <LEDA/geowin.h>
#include <math.h>

using namespace leda;

//using namespace GEOWIN;

#define WAVE_PART rat_wave_part
  
struct WAVE_PART
{
  static POINT INFINITY1;
  static POINT INFINITY2;       
    
  POINT  pred;
  POINT  p;
  POINT  succ;
    
  WAVE_PART(const POINT pp) :  pred(pp), p(pp), succ(pp) {}
  WAVE_PART(const WAVE_PART& wp) :  pred(wp.pred), p(wp.p), succ(wp.succ) {}

  bool describes_circle_event()
  {
    return ( !identical(pred, WAVE_PART::INFINITY1) &&
	     !identical(succ, WAVE_PART::INFINITY2) &&
	     !identical(succ, pred)  && orientation(pred, p, succ) < 0 );
  }

  WAVE_PART* split(WAVE_PART* new_w)
  {
    WAVE_PART* w = new WAVE_PART(p);
    w->succ      = new_w->p;
    w->pred      = pred;
    pred         = new_w->p;
    new_w->succ  = new_w->pred = p;
    return w;
  }
    
  LEDA_MEMORY(WAVE_PART);
};
 
struct del_event_info
{
  POINT p;
  bool  is_site;
  int   refs;
  
  del_event_info() : is_site(false), refs(1) {}
  del_event_info(POINT pp) : p(pp), is_site(true), refs(1) {}
  del_event_info(const del_event_info& ev) 
    : p(ev.p), is_site(ev.is_site), refs(ev.refs) {}  
  
  LEDA_MEMORY(del_event_info);
};

  
inline ostream& operator << (ostream& os, const del_event_info& di) 
{
  if(di.is_site) cout << "site event : " << di.p;
  else           cout << "circle event : ";
  cout << " reference count : " << di.refs << endl;
  return os;
}

int compare(const CIRCLE& c1, const CIRCLE& c2);

typedef sortseq<CIRCLE, del_event_info>  XST;



inline ostream& operator << (ostream& os, const WAVE_PART& w)
{
  os << '[';
  if( identical(w.pred, WAVE_PART::INFINITY1) ) os << " -oo "; 
  else os << w.pred.to_point();
  os << " ---> " << w.p.to_point() << " ---> ";
  if( identical(w.succ, WAVE_PART::INFINITY2) ) os << " +oo "; 
  else  os << w.succ.to_point();
  os  << ']';
  return os;
}

int compare(WAVE_PART*const & w1, WAVE_PART*const & w2);
int compare_at_pos(const CIRCLE& p_sweep,  WAVE_PART*const & w1, 
		   WAVE_PART*const & w2);



typedef sortseq<WAVE_PART*, edge>         YST;
typedef list<POINT>                       LoP;
typedef GRAPH<POINT,int>                  GPI;
typedef plane_sweep<LoP, YST, XST, GPI  > DelaunaySweep;



POINT WAVE_PART::INFINITY1;     
POINT WAVE_PART::INFINITY2;     

int compare(const CIRCLE& c1, const CIRCLE& c2)
{
  if( c1.is_trivial() && c2.is_trivial() ) 
    return compare(c1.point1(), c2.point1());
  
  POINT p1 = c1.center();
  POINT p2 = c2.center();
  COORD s1 = c1.sqr_radius();
  COORD s2 = c2.sqr_radius();
  
  // c1 < c2 <=>  p1.xcoord() + sqrt(s1) < p2.xcoord() + sqrt(s2)
  //         <=>  p1.xcoord() - p2.xcoord() < sqrt(s2) - sqrt(s1)

  int c = compare(p1.xcoord(), p2.xcoord());

  if( c > 0 )                       //  p1.xcoord() - p2.xcoord() > 0
    if ( !(s2 > s1) )   return 1;   //  sqrt(s2) - sqrt(s1)       < 0
    else;
  else
    if( c < 0 )                     //  p1.xcoord() - p2.xcoord() < 0
      if ( !(s1 > s2) )  return -1; //  sqrt(s2) - sqrt(s1)       > 0
      else;
    else                            //  p1.xcoord() - p2.xcoord() = 0
      if ( s2 > s1 )     return -1; //  sqrt(s2) - sqrt(s1)       < 0
      else
        if ( s2 < s1 )   return 1;  //  sqrt(s2) - sqrt(s1)       > 0
        else                        //  sqrt(s2) - sqrt(s1)       = 0
          return compare(p1.ycoord(), p2.ycoord());  

  // sign(p1.xcoord() - p2.xcoord()) = sign(sqrt(s2) - sqrt(s1))
  // 1x square => 
  
  COORD A   = p1.xcoord() - p2.xcoord();
  COORD AA  = A*A;
  COORD B   = s1 + s2;
 
  int cc = compare(AA, B);
  if( cc > 0  )  return c;
  if( cc == 0 )  return compare(p1.ycoord(), p2.ycoord());
  
  // 2x square => 
  
  COORD C  = AA - B;
  COORD CC = C*C;
  COORD D  = 4*s1*s2;
  
  cc = compare( CC, D );
  
  if( cc > 0 )   return -c;
  if( cc == 0 )  return compare(p1.ycoord(), p2.ycoord());
  return c;
}


int compare_at_pos(const CIRCLE& c_sweep, WAVE_PART*const & w1, 
                   WAVE_PART*const & w2)
{
  POINT p_sweep = c_sweep.point1();
  
  if( w1 == w2 )                          return 0;
  if( identical(w2->succ, WAVE_PART::INFINITY2) )  return -1;
  if( identical(w1->succ, WAVE_PART::INFINITY2) )  return 1;
  
  WAVE_PART w(*w2);
  int c = -1;
  if( identical(p_sweep, w2->p) )
    {
      w=WAVE_PART(*w1);
      c = 1;
    }
  else 
    if ( !identical(w1->p, p_sweep)) 
      error_handler(1, "compare_at_pos: Precondition verletzt!");
  
  POINT p = w.p;
  POINT q = w.succ;
  
  int   lower_p = compare(p, q) < 0 ? 1 : -1;
  
  COORD A = p.xcoord() - p_sweep.xcoord();
  COORD B = q.xcoord() - p_sweep.xcoord();
  COORD C = q.ycoord() - p.ycoord();
  COORD D = p.xcoord() - q.xcoord();
  
  
  COORD x1 = p_sweep.ycoord() - p.ycoord();
  COORD x2;
  
  if ( C == 0 ) 
    if ( lower_p == -1 )
      {
        if ( x1 < 0 );
        else
          { 
            x1 = x1*x1;
            A = p_sweep.xcoord()*p_sweep.xcoord();
            B = p_sweep.xcoord()*(p.xcoord() + q.xcoord());
            C = p.xcoord()*q.xcoord();
            x2 =  A-B+C;
            if ( x1 >= x2 )  return -c;
          }
        return c;
      }
    else
      {
        if ( x1 >= 0 ); 
        else
          {
            x1 = x1*x1;
            A = p_sweep.xcoord()*p_sweep.xcoord();
            B = p_sweep.xcoord()*(p.xcoord() + q.xcoord());
            C = p.xcoord()*q.xcoord();
            x2 = A-B+C;
            if ( x1 > x2 )  return c;
          }
        return -c;
      }
  
  if ( D == 0 )
    if (p_sweep.ycoord() <  (p.ycoord() + q.ycoord())/2 ) return c;
    else                                                  return -c;
  
  x1 = x1 - A*C/D;
  x2 = C/D;
  
  if ( lower_p == -1 )
    if ( x1 < 0 ) return c;
    else
      {
        x1 = x1*x1;
        x2 = x2*x2*(A*(B + D*D/(C*C)*B)) ;
        if ( x2 <= x1 ) return -c;
        else            return  c;
      }
  else
    if ( x1 >= 0 ) return -c; 
    else
      {
        x1 = x1*x1;
        x2 = x2*x2*(A*(B + D*D/(C*C)*B));
        if ( x2 < x1 )    return  c;
        else              return -c;
      }
  
  return c;     
}


bool initialize(DelaunaySweep& DS, const list<POINT>& L)
{
  if( L.empty() ) return true;
  
  DelaunaySweep::output& G = DS.get_output();
  
  
    G.clear();

    list<POINT> L1  = L;
    L1.sort();

    array<POINT> A(L.length());
    int n = 0;
    A[0] = L1.pop();
    POINT p;
    forall(p, L1) if (p != A[n]) A[++n] = p;
    n++;

    node dummy_node = G.new_node(A[0]);

    node v1 = G.new_node(A[0]);
    edge e1 = G.new_edge(dummy_node, v1, HULL_EDGE); 
    edge e2 = G.new_edge(v1, dummy_node, HULL_EDGE); 
    G.set_reversal(e1, e2);
    if ( n == 1 )  return true;   // |L| == 1

    node v2;
    
    if( n == 2 )
      {
        v2 = G.new_node(A[1]);
        e1 = G.new_edge(v1, v2, HULL_EDGE);
        e2 = G.new_edge(v2, v1, HULL_EDGE);
        G.set_reversal(e1, e2);
        return true;              // |L| == 2
      }

  
    int i = 2;
    while( i < n && !orientation(A[0], A[1], A[i]) ) i++;
    
    if( i == n )
      {
        i = 1;
        do
          {
            v2 = G.new_node(A[i]);
            e1 = G.new_edge(v1, v2, HULL_EDGE);
            e2 = G.new_edge(v2, v1, HULL_EDGE);
            G.set_reversal(e1, e2);
            i++;  
            v1 = v2;
          }
        while ( i < n );
        return true;                // L contains collinear points only
      }
    
    WAVE_PART* w1 = new WAVE_PART(A[0]);
    w1->pred = WAVE_PART::INFINITY1;
    w1->succ = WAVE_PART::INFINITY2;
    DS.insert(w1, e2);
    WAVE_PART* w2;
    int j = 1;
    
    if ( !compare(A[0].xcoord(), A[i-1].xcoord() ) )
      for(; j < i; j++ )
        {
          w2 = new WAVE_PART(A[j]);
          v2 = G.new_node(A[j]);
          w2->pred = w1->p;
          w2->succ = WAVE_PART::INFINITY2;
          w1->succ = w2->p;
          
          e1 = G.new_edge(v1, v2, HULL_EDGE);
          e2 = G.new_edge(v2, v1, HULL_EDGE);
          G.set_reversal(e1, e2);
          
          DS.insert(w2, e2);
          
          w1 = w2;
          v1 = v2;
        }

  
    for( ; j < n; j++ ) DS.new_event(CIRCLE(A[j]), del_event_info(A[j]));
    



  return true;
}
     

bool transit(DelaunaySweep& DS)
{ 
  DelaunaySweep::output& G          = DS.get_output();
  DelaunaySweep::ystructure& Y      = DS.get_ystructure();
  DelaunaySweep::xstructure& X      = DS.get_xstructure();
  DelaunaySweep::event   event_item = DS.current_event();
  DelaunaySweep::xkey    p_sweep    = DS.event_key(event_item);
  DelaunaySweep::xinf    event_inf  = DS.event_inf(event_item);
  
  node n, n1 = nil;
  edge e, e1 = nil, e2 = nil, e1_succ = nil, e2_pred = nil;
  CIRCLE ci;
  
  // If the event_item is linked with an item in the [[Y-structure]],
  // at event point a circle event occurs. An input point may be placed
  // at this point too.
  // In the other case the current event is a site event only.
  DelaunaySweep::yitem   yit = DS.get_yref(event_item);
  DelaunaySweep::ykey    w;
  
  DelaunaySweep::yitem   yit_first = nil;
  DelaunaySweep::yitem   yit_last  = nil;
  DelaunaySweep::yitem   yit_succ  = nil;
  
  
  if( event_inf.is_site )   // 1. site event
    {
      w   = new WAVE_PART(event_inf.p);
      n1  = G.new_node(event_inf.p);
      
      WAVE_PART* wloc;
      yit  = Y.locate(w);
      wloc = DS.key(yit);
      yit_last  = yit;
      yit_first = DS.pred(yit_last);
      ci = CIRCLE( wloc->p, wloc->pred, event_inf.p);
      
      yit = Y.insert_at(yit_last, w, 0);      
      w = wloc->split(w);
      yit_first = Y.insert_at(yit, w, DS.inf(yit_last));
      
      if (!ci.is_degenerate() &&  ci.center().ycoord() == event_inf.p.ycoord())
        {
          DS.set_xref(yit_first, event_item);
          DS.set_yref(event_item, yit_first);
        }
      
      e = DS.inf(yit_last);
      e1 = G.new_edge( n1, G.source(e), HULL_EDGE );
      e2 = G.new_edge( e, n1, HULL_EDGE, LEDA::after );
      G.set_reversal(e1, e2);
      DS.inf(yit_last) = e2;
      DS.inf(yit)      = e1;
      yit = yit_first;
    }
  
  if ( DS.get_yref(event_item) )  
    {
      // 2. determine upperst item of subsequence of disappearing wave parts
      while( DS.get_xref(yit) == event_item ) yit = DS.succ(yit);       
      
      // 3. + 4. insert edges and remove wave parts
      yit_succ = yit;
      yit_last = yit_last ? yit_last : yit;
      yit = DS.pred(yit_succ);
      n1 = G.source(DS.inf(yit_succ));
      e2_pred = e =  DS.inf(yit_succ);
      G[e] = DIAGRAM_EDGE;
      n = G.source(DS.inf(yit));
      e = DS.inf(yit);
      
      do
        {
          yit_first = DS.pred(yit);
          w = DS.key(yit);
          DS.del_item(yit);
          delete w;
          yit = yit_first;
          G[e] = DIAGRAM_EDGE;
          n = G.source(DS.inf(yit));
          e1_succ = G.reversal(e);
          e = DS.inf(yit);
        }
      while( DS.get_xref(yit) == event_item );
      
      // 5. close the face
      DS.key(yit_first)->succ = DS.key(yit_succ)->p;
      DS.key(yit_succ)->pred  = DS.key(yit_first)->p;
      
      e1 = G.new_edge( e1_succ, n1, DIAGRAM_EDGE,  LEDA::before );  
      e2 = G.new_edge( e2_pred, n , HULL_EDGE,  LEDA::after );  
      G.set_reversal(e1, e2);
      
      DS.inf(yit_succ) = e2;
    }
  
  // 6. Delete non relevant circle events
  DelaunaySweep::event xit1 = DS.get_xref(yit_first);
  DelaunaySweep::event xit2 = DS.get_xref(yit_last);
  
  if ( xit1 )
    if ( DS.event_inf(xit1).refs == 1 ) DS.del_event(xit1);
    else  { DS.set_yref(xit1, nil); DS.event_inf(xit1).refs--; }
  
  if ( xit2 )
    if ( DS.event_inf(xit2).refs == 1 ) DS.del_event(xit2);
    else { DS.set_yref(xit2, nil); DS.event_inf(xit2).refs--; }
    
  xit1 = nil, xit2= nil;
  
  // 7. Insert circle events for wave parts that become adjacent
  w = DS.key(yit_first);
  if( w->describes_circle_event() )
    {
      ci = CIRCLE(w->p, w->pred, w->succ);
      if( !(xit1 = X.lookup(ci)) ) xit1 = DS.new_event(ci, del_event_info());
      else                         DS.event_inf(xit1).refs++;
      DS.set_yref(xit1, yit_first);
    }
  
  w = DS.key(yit_last);
  if( w->describes_circle_event() )
    {
      ci = CIRCLE(w->p, w->pred, w->succ);
      if( !(xit2 = X.lookup(ci)) ) xit2 = DS.new_event(ci, del_event_info());
      else                         DS.event_inf(xit2).refs++;
      DS.set_yref(xit2, yit_last);
    }
  
  DS.set_xref(yit_first, xit1);
  DS.set_xref(yit_last,  xit2);
  
  return true;
  
}



bool finish(DelaunaySweep& DS, const list<POINT>&)
{
  DelaunaySweep::ystructure& Y = DS.get_ystructure(); 
  DelaunaySweep::output& G = DS.get_output();
  DelaunaySweep::yitem       yit;
  DelaunaySweep::ykey        w;
 
  if( !G.first_node() )  return true;

  G.del_node(G.first_node());

  while( !Y.empty() )
    {
      yit = Y.max();
      w = DS.key( yit );
      DS.del_item(yit);
      delete w;
    }
 
  return true; 
}


void DELAUNAY_FORTUNE(const list<POINT>& S, GRAPH<POINT, int>& G)
{  
  G.clear();
  DelaunaySweep DS(&S, &G);
  
  DS.set_init_handler(initialize);
  DS.set_event_handler(transit);
  DS.set_finish_handler(finish);
  
  if( DS.init(S) )
    if ( DS.sweep() ) DS.finish(S);
    else  cout << "Fehler beim Sweepen !" << endl;
  else cout << "Initialisierung fehlgeschlagen !" << endl;
} 



static GeoWin* Gwp = 0;
static window* Wp = 0;
DelaunaySweep* Dsweep =0;

class WaveFrontRep
{
  public :
  
  point                        p_sweep;
  DelaunaySweep::ystructure*   yst;
  
  WaveFrontRep() : yst(0) {}
  WaveFrontRep(const WaveFrontRep& w) :  p_sweep(w.p_sweep), yst(w.yst) {}
  
  double yparab( double y, point p ) const;
  void xparab( double x, point p, double& y1, double& y2 ) const; 
  void computeX(WAVE_PART* w, double& x0) const;
  void computeY(WAVE_PART* w, double& y0, double& y1) const;
  void plotter(window& w, point p, double y0, double y1, color col) const;
  void show_ystructure() const;
  
  friend unsigned long ID_Number(const WaveFrontRep& x)
  { 
    return ID_Number(x.p_sweep); 
  }
  friend const char* leda_tname(WaveFrontRep* w) {  return "WaveFrontRep"; }
  friend identical(const WaveFrontRep& w1, const WaveFrontRep& w2)
  {
    return &w1 == &w2;
  }
};


double WaveFrontRep::yparab( double y, point p ) const
{
  if ( p_sweep.xcoord() - p.xcoord()  == 0 ) return Wp->xmin();
  
  double A = p_sweep.xcoord()*p_sweep.xcoord();
  double B = (y - p.ycoord())*(y - p.ycoord());
  double C = p.xcoord()*p.xcoord();
  double res = (A - B - C)/(2*(p_sweep.xcoord() - p.xcoord()));
  return res;
}

void WaveFrontRep::xparab( double x, point p, double& y1, double& y2 ) const
{
  double A = p_sweep.xcoord()*p_sweep.xcoord();
  double B = 2*x*(p.xcoord() - p_sweep.xcoord());
  double C = p.xcoord()*p.xcoord();
  double D = sqrt(B + A - C);
  
  y1 = p.ycoord() - D;
  y2 = p.ycoord() + D;
}

void  WaveFrontRep::computeX(WAVE_PART* w, double& x0) const
{
  double x;
  point  p;
  
  x0 = Wp->xmin();
  if ( !identical(w->pred, WAVE_PART::INFINITY1) ) 
    {
      p = w->pred.to_point();
      x = yparab(w->p.to_point().ycoord(), p);
      if ( x > x0 ) x0 = x;
    }
  if ( !identical(w->succ, WAVE_PART::INFINITY2 ) )
    {
      p = w->succ.to_point();
      x = yparab(w->p.to_point().ycoord(), p);
      if ( x > x0 ) x0 = x;
    }
}

void WaveFrontRep::computeY(WAVE_PART* w, double& y0, double& y1) const
{
  y0 = Wp->ymin();
  y1 = Wp->ymax();
  
  double A, B, C, D, E, F;
  point p, q;
  int   tol;
  
  xparab( Wp->xmin(), w->p.to_point(), E, F );
  
  if( E > y0 ) y0 = E;
  if( F < y1 ) y1 = F;
  
  if( !identical(w->succ, WAVE_PART::INFINITY2 ) )
    {
      p = w->p.to_point();
      q = w->succ.to_point();
      
      A = p.xcoord() - p_sweep.xcoord();
      B = q.xcoord() - p_sweep.xcoord();
      C = q.ycoord() - p.ycoord();
      D = p.xcoord() - q.xcoord();
      
      if ( C == 0 ) 
	{
	  A = p_sweep.xcoord()*p_sweep.xcoord();
	  B = p_sweep.xcoord()*(p.xcoord() + q.xcoord());
	  C = p.xcoord()*q.xcoord();
	  tol = compare(p, q) < 0 ? -1 : 1;
	  F =  p.ycoord() + tol*sqrt(A-B+C); 
	}
      else
	if ( D == 0 )
	  if ( A != 0 && C > 0) F = (q.ycoord() + p.ycoord())/2;
          else; 
	else
	  {
	    if ( C > 0 )
	      F = (A*C)/D + C/D*sqrt(A*(B+D*D/(C*C)*B));
	    else
	      F = (A*C)/D - C/D*sqrt(A*(B+D*D/(C*C)*B));
	    
	    F = F + p.ycoord();
	  }
    }
  
  
  if( !identical(w->pred, WAVE_PART::INFINITY1) )
    {
      p = w->p.to_point();
      q = w->pred.to_point();
      
      A = p.xcoord() - p_sweep.xcoord();
      B = q.xcoord() - p_sweep.xcoord();
      C = q.ycoord() - p.ycoord();
      D = p.xcoord() - q.xcoord();
      
      if ( C == 0 ) 
	{
	  A = p_sweep.xcoord()*p_sweep.xcoord();
	  B = p_sweep.xcoord()*(p.xcoord() + q.xcoord());
	  C = p.xcoord()*q.xcoord();
	  tol = compare(p, q) < 0 ? -1 : 1;
	  E =  p.ycoord() - tol*sqrt(A-B+C); 
	}
      else
	if ( D == 0 ) 
	  if ( A != 0 && C < 0) E = (q.ycoord() + p.ycoord())/2;
	  else;
	else
	  {
	    if ( C > 0 )
	      E = (A*C)/D - C/D*sqrt(A*(B+D*D/(C*C)*B));
	    else
	      E = (A*C)/D + C/D*sqrt(A*(B+D*D/(C*C)*B));
	    
	    E = E + p.ycoord();
	  }
    }
  
  if( E > y0 ) y0 = E;
  if( F < y1 ) y1 = F;
}

void WaveFrontRep::plotter(window& w, point p, double y0, double y1, 
			   color col) const
{
  w.set_color(col);
  
  double y;
  double dy = 1/w.scale();
  double x_new;
  
  if (y1 <= y0) return;
  
  int lw = w.get_line_width();
  w.set_line_width(1);
  
  list<point> L;
  
  x_new = yparab(y0, p);
  L.append(point(x_new, y0));
  
  for(y = y0+dy; y < y1; y+=4*dy)
    {
      x_new = yparab(y, p);
      L.append(point(x_new, y));
    }
  
  x_new = yparab(y1, p);
  L.append(point(x_new, y1));
  
  w.draw_spline( L, 4, col);
  w.set_line_width(lw);  
}

void WaveFrontRep::show_ystructure() const
{
  if(yst)
    { 
      DelaunaySweep::yitem   dit;
      
      forall_items(dit, *yst)
	{
	  WAVE_PART* w = yst->key(dit);
	  double y0, y1;
	  
	  computeY(w, y0, y1);
	  plotter(*Wp, w->p.to_point(), y0, y1, grey2);
	}
    }
}


// //////////////////////////////////////////////////////////////////////
// routines for using the class in GeoWin

// ps_file output

ps_file& operator<<(ps_file& ps,  const WaveFrontRep& c1)
{
 return ps;
}

// Presentation of the Sweepline :
window& operator<<(window& w, const WaveFrontRep& c1)
{ 
  point r1 = c1.p_sweep;
  point r2 = r1.translate(0, 1);
  w.draw_line(r1, r2, green);
  c1.show_ystructure();
  return w;
}

window& operator >> (window& w, WaveFrontRep& p)
{
  return w;
}

ostream& operator << (ostream& os, const WaveFrontRep& w) { return os; }
istream& operator >> (istream& is, WaveFrontRep& w)      {  return is; }


bool geowin_intersects(const WaveFrontRep& c1, double x1, double y1, 
		       double x2, double y2, bool)
{
  double xc = c1.p_sweep.xcoord();
  return (xc >= x1 && xc <= x2);
}

void geowin_fit(const WaveFrontRep& obj, double& x1, double& x2,
		double& y1, double& y2)
{
}

WaveFrontRep geowin_move_view(const WaveFrontRep& obj, double dx, double dy)
{
  point p = obj.p_sweep.translate(dx, dy);
  WaveFrontRep w;
  w.p_sweep = p; 
  w.yst = obj.yst;
  
  return w;
}


bool sweepline_pre_change_handler(GeoWin&, 
				  const WaveFrontRep& c1, 
				  const WaveFrontRep& c2) 
{ 
  
  point p1 = c1.p_sweep;
  point p2 = c2.p_sweep;
  
  if( compare(p1, p2) > 0 ) return false;
  return true; 
}



static void animate( DelaunaySweep& )
{
  Gwp->redraw();
}

//s.n.
//void update_delaunay(const list<WaveFrontRep>& L, GRAPH<rat_point, int>& G)
void update_delaunay(const list<WaveFrontRep>& L, list<rat_point>&)
{
  if( !L.empty() )
    { 
      point pp = L.head().p_sweep;
      rat_point ppp = rat_point(pp);
      rat_circle stop = rat_circle(ppp, ppp, ppp);
      Dsweep->animate(stop, 0);
    }
}


extern bool initialize(DelaunaySweep& DS, const list<rat_point>& L);
extern bool transit(DelaunaySweep& GS );
extern bool finish(DelaunaySweep& GS, const list<rat_point>& L);

bool FINISHED=false;

void finish_sweep(GeoWin& gw)
{
  if( Dsweep != NULL && ! FINISHED) 
    {
      Dsweep->sweep();
      Dsweep->finish(Dsweep->get_input());
      gw.redraw();
      FINISHED=true;
      /*
      //geo_scene out = gw.get_scene_with_name("Sweep : Output");
      //gw.edit(out);
      gw.activate(gw.get_scene_with_name("Sweepline"));
      delete Dsweep;
      gw.destroy(gw.get_scene_with_name("Sweepline"));
      Dsweep = NULL;
      gw.reset_actions();
      gw.enable_menus();
     */
    }
}

void break_sweep(GeoWin& gw)
{
  if( Dsweep != NULL) 
    {
      Dsweep->finish(Dsweep->get_input());
      //geo_scene out = gw.get_scene_with_name("Sweep : Output");
      //gw.edit(out);
      gw.activate(gw.get_scene_with_name("Sweepline"));
      delete Dsweep;
      gw.destroy(gw.get_scene_with_name("Sweepline"));
      Dsweep = NULL;
      gw.reset_actions();
      gw.enable_menus();
    }
}


// s.n.
static GRAPH<rat_point,int> Gtmp;

void redraw_graph(const list<rat_point>&, window& W, 
                      color col1, color col2, double,double,double,double)
{ edge_array<bool> drawn(Gtmp,false);
  edge e;
  forall_edges(e,Gtmp) 
  { edge r = Gtmp.reversal(e);
    if (r)
    { if (drawn[e]) continue;
       drawn[r] = drawn[e] = true;
     }
    node v = source(e);
    node w = target(e);
    W.draw_segment(Gtmp[v].to_point(),Gtmp[w].to_point(),col1);
  }  
}


void start_sweep( GeoWin& gw)
{
  
  if( Dsweep ) return;
  
  // is a sweep running ?
  FINISHED=false;
   
  // mouse actions:
  gw.clear_actions();
  gw.set_action(A_LEFT| A_DRAG | A_OBJECT, geo_object_dragging);
  gw.del_pin_point();
  
  // get input
  list<rat_point> L;
  //gw.get_objects(L);
  geowin_get_objects(gw, gw.get_active_scene(), L);

  // make a scene for the sweepline
  list<WaveFrontRep>* Hlp1;
  list<rat_point>* Hlp2;

  GeoEditScene< list<WaveFrontRep> >* SweeplineScene 
    = (GeoEditScene< list<WaveFrontRep> >*)
      geowin_new_scene(gw, (list<WaveFrontRep>*)0); 

  gw.set_name(SweeplineScene, string("Sweepline"));
  
  SweeplineScene->set_pre_change_handler(sweepline_pre_change_handler);
  SweeplineScene->set_box_intersection_fcn(geowin_intersects);
  SweeplineScene->set_get_bounding_box_fcn(geowin_fit);
  SweeplineScene->set_move_fcn(geowin_move_view);
  
  // scene for graph :
  // link it to the  SweeplineScene 

  // s.n.
  GeoBaseScene<list<rat_point> >* tmp;
  tmp = geowin_new_scene(gw, &update_delaunay, SweeplineScene, "Sweep : Output",Hlp1,Hlp2);
  gw.set_visible(tmp, true);
  gw.set_color(tmp, blue);
  gw.set_color2(tmp, blue);

  tmp->set_redraw_fcn(redraw_graph);

  
  // create a DelaunaySweepObjekt
  // s.n.
  //Dsweep = new DelaunaySweep(&L, &(tmp->get_objref()));   
  Dsweep = new DelaunaySweep(&L, &Gtmp);

  Dsweep->set_init_handler(initialize);
  Dsweep->set_event_handler(transit);
  Dsweep->set_finish_handler(finish);
  
  if( !Dsweep->init(L) )
    {
      gw.message(string("Initialisierung fehlgeschlagen !" ));
      finish_sweep(gw);
      return;
    }
  
  DelaunaySweep::xkey swp = Dsweep->sweep_position();
  list<WaveFrontRep> LL;
  WaveFrontRep startfront;
  circle ccc = swp.to_circle();
  point ppp  = ccc.center();
  startfront.p_sweep = ppp.translate(ccc.radius(), 0);
  startfront.yst =&(Dsweep->get_ystructure());
  LL.append(startfront);
  
  SweeplineScene->set_objects(LL);
  gw.set_visible(SweeplineScene, true); 
  gw.activate(SweeplineScene);
  
  gw.disable_menus();
  
  gw.redraw();
}


int main()
{
  GeoWin GW("DELAUNAY SWEEP DEMO");

  list<rat_point> L;
  geo_scene sc = GW.new_scene(L);
  GW.set_visible(sc,true);
  
  geowin_add_user_call(GW, "Start  Sweep", start_sweep);
  geowin_add_user_call(GW, "Finish Sweep", finish_sweep);
  geowin_add_user_call(GW, "Break  Sweep", break_sweep);
  
  Wp = &GW.get_window();
  Gwp = &GW;
  Wp->set_node_width(3);

  GW.display(window::center,window::center);

  GW.edit();
  break_sweep(GW);
 
  return 0;
}


