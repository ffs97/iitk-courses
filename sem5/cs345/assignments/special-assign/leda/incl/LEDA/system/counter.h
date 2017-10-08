/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  counter.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_COUNTER_H
#define LEDA_COUNTER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600231
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/string.h>

LEDA_BEGIN_NAMESPACE

/// counter ////////////////////////////////////////////////////////////////////////////////

/*{\Manpage {counter} {} {Counter} {c}}*/

class counter {
/*{\Mdefinition
The class |\Mtype| can be used during profiling to count how often certain code
is executed. An example is given below.
}*/
public:
/*{\Mcreation}*/

	counter(const string& name, bool report_on_destruction = true) 
	 : Name(name), ReportOnDestruction(report_on_destruction) { reset(); }
	/*{\Mcreate creates an instance |\Mvar| with the given |name|. 
	If |report_on_destruction| is true, then the counter reports its 
	value upon destruction.
	The initial value of the counter is zero.}*/

	counter() : ReportOnDestruction(false) { reset(); }
	/*{\Mcreate creates an unnamed instance |\Mvar| and sets the 
	|report_on_destruction| flag to false.
	The initial value of the counter is zero.}*/

	~counter()
	{ if (ReportOnDestruction) cout << "Counter(" << get_name() << ") = " << get_value() << "\n"; }

	const unsigned long operator=(const unsigned long val)
	{ return Value = val; }

	operator const unsigned long() const
	{ return Value; }

/*{\Moperations}*/
	void reset()
	{ Value = 0; }
	/*{\Mop sets the value of |\Mvar| to zero.}*/

	void set_value(const unsigned long val)
	{ Value = val; }
	/*{\Mop sets the value of |\Mvar| to |val|.}*/

	const unsigned long get_value() const
	{ return Value; }
	/*{\Mop returns the current value of |\Mvar|.}*/

	const unsigned long increment() { return ++Value; }
	/*{\Mop increments |\Mvar| and returns its new value.
	(We also provide the operator |++|.)}*/
	
	const unsigned long operator++() 
	{ return ++Value; }

	const unsigned long operator++(int) 
	{ return Value++; }

	const unsigned long operator+=(unsigned long inc) 
	{ return Value += inc; }

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

	friend ostream& operator<<(ostream& os, const counter& c) 
	{ return os << c.get_value(); }

private:
	unsigned long Value;
	string   Name;
	bool     ReportOnDestruction;
};

/*{\Mexample
In the example below we count how often the function |fibonacci| is executed.
\begin{verbatim}
#include <LEDA/system/counter.h>

unsigned fibonacci(unsigned n)
{
  static leda::counter cnt("fibonacci");
    // report upon destruction of cnt
  ++cnt;

  if (n < 1) return 0;
  else if (n == 1) return 1;
  else return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
  std::cout << fibonacci(40) << "\n";
  return 0; // reports "Counter(fibonacci) = 331160281" upon termination
}
\end{verbatim}
}*/

#if LEDA_ROOT_INCL_ID == 600231
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
