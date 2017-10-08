/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  timer.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_TIMER_H
#define LEDA_TIMER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600232
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/misc.h>

LEDA_BEGIN_NAMESPACE

/// timer //////////////////////////////////////////////////////////////////////////////////

/*{\Manpage {timer} {} {Timer} {t}}*/

class timer {
/*{\Mdefinition
The class |\Mtype| facilitates time measurements.
An instance |\Mvar| has two states: |running| or |stopped|. It measures the 
time which elapses while it is in the state |running|.
The state depends on a (non-negative) internal counter, which is incremented 
by every |start| operation and decremented by every |stop| operation. 
The timer is |running| iff the counter is not zero. The use of a counter 
(instead of a boolean flag) to determine the state is helpful when a recursive
function |f| is measured, which is shown in the example below: 
\begin{verbatim}
#include <LEDA/system/timer.h>
leda::timer f_timer;

void f()
{
  f_timer.start();

  // do something ...
  f(); // recursive call
  // do something else ...

  f_timer.stop(); // timer is stopped when top-level call returns
}

int main()
{
  f();
  std::cout << "time spent in f " << f_timer << "\n"; return 0;
}
\end{verbatim}

Let us analyze this example. 
When |f| is called in |main|, the timer is in the state |stopped|. The first 
|start| operation (in the top-level call) increments the counter from zero 
to one and puts the timer into the state |running|. 
In a recursive call the counter is incremented at the beginning and 
decremented upon termination, but the timer remains in the state |running|. 
Only when the top-level call of |f| terminates and returns to |main|, the 
counter is decremented from one to zero, which puts the timer into the state 
|stopped|. So the timer measures the total running time of |f| (including 
recursive calls).
}*/
public:
/*{\Mtypes}*/
/*{\Moptions nextwarning=no }*/
/*
	typedef measure measure;
*/
	/*{\Mtypemember auxiliary class to facilitate measurements 
	                (see example below).}*/

/*{\Mcreation}*/

	timer(const string& name, bool report_on_destruction = true) 
	 : Name(name), ReportOnDestruction(report_on_destruction) { reset(); }
	/*{\Mcreate creates an instance |\Mvar| with the given |name|. 
	If |report_on_destruction| is true, then the timer reports upon its 
	destruction how long it has been running in total.
	The initial state of the timer is |stopped|.}*/

	timer() : ReportOnDestruction(false) { reset(); }
	/*{\Mcreate creates an unnamed instance |\Mvar| and sets the 
	|report_on_destruction| flag to false.
	The initial state of the timer is |stopped|.}*/

	~timer()
	{ if (ReportOnDestruction) cout << "Timer(" << get_name() << "): " << *this << " s\n"; }

/*{\Moperations}*/
	void reset()
	{ Running = 0; Total = 0; }
	/*{\Mop sets the internal counter and the total elapsed time to zero.}*/

	void start()
	{ if (Running++ == 0) LastStart = used_time(); }
	/*{\Mop increments the internal counter.}*/
	
	void stop()
	{ if (Running != 0 && --Running == 0) Total += used_time(LastStart); }
	/*{\Mop decrements the internal counter. 
	(If the counter is already zero, nothing happens.)}*/

	void restart()
	{ reset(); start(); }
	/*{\Mop short-hand for |t.reset() + t.start()|.}*/
	
	void halt()
	{ if (Running != 0) Total += used_time(LastStart); Running = 0; }
	/*{\Mop sets the counter to zero, which forces the timer into the state 
	|stopped| no matter how many |start| operations have been executed before.}*/

	bool is_running() const
	{ return Running != 0; }
	/*{\Mop returns if |\Mvar| is currently in the state |running|.}*/

	float elapsed_time() const 
	{ return is_running() ? Total + used_time()-LastStart : Total; }
	/*{\Mop returns how long (in seconds) |\Mvar| has been in the state 
	|running| (since the last |reset|).}*/

	void set_name(const string& name)
	{ Name = name; }
	/*{\Mop sets the name of |\Mvar|.}*/

	string get_name() const
	{ return Name; }
	/*{\Mop returns the name of |\Mvar|.}*/

	void report_on_desctruction(bool do_report = true)
	{ ReportOnDestruction = do_report; }
	/*{\Mop sets the flag |report_on_destruction| to |do_report|.}*/

	bool will_report_on_desctruction() const
	{ return ReportOnDestruction; }
	/*{\Mop returns whether |\Mvar| will issue a report upon its destruction.}*/

	friend ostream& operator<<(ostream& os, const timer& t) 
	{ return os << string("%.2f", t.elapsed_time()); }

	class measure {
	public:
		measure(timer& t) : Timer(t) { Timer.start(); }
		~measure() { Timer.stop(); }
	private:
		timer& Timer;
	};

private:
	unsigned Running; // counter -> allows for use recursive functions
	float    LastStart;
	float    Total;
	string   Name;
	bool     ReportOnDestruction;
};

/*{\Mexample
We give an example demonstrating the use of the class |measure|. Note that
the function below has several [[return]] statements, so it would be tedious 
to stop the timer ``by hand''.
\begin{verbatim}
#include <LEDA/system/timer.h>

unsigned fibonacci(unsigned n)
{
  static leda::timer t("fibonacci");
    // report total time upon destruction of t

  leda::timer::measure m(t);
    // starts the timer t when m is constructed, and stops t
    // when m is destroyed, i.e. when the function returns

  if (n < 1) return 0;
  else if (n == 1) return 1;
  else return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
  std::cout << fibonacci(40) << "\n";
  return 0; // reports "Timer(fibonacci): X.XX s" upon termination
}
\end{verbatim}
}*/

#if LEDA_ROOT_INCL_ID == 600232
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
