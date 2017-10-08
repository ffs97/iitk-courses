/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  error.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>

#ifndef LEDA_ERROR_H
#define LEDA_ERROR_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600226
#include <LEDA/internal/PREAMBLE.h>
#endif

LEDA_BEGIN_NAMESPACE

/*{\Manpage {error} {} {Error Handling} }*/

/*{\Mtext
LEDA tests the preconditions of many (not all!) operations. Preconditions
are never tested, if the test takes more than constant time. If the test
of a precondition fails an error handling routine is called. It takes 
an integer error number $i$ and a $char*$ error message string $s$ as 
arguments. The default error handler writes $s$ to the diagnostic output 
(|cerr|) and terminates the program abnormally if $i \ne 0$.
Users can provide their own error handling function $handler$ by calling 
\begin{Mverb}
 set_error_handler(handler)
\end{Mverb}
After this function call |handler| is used instead of the default error 
handler. |handler| must be a function of type  |void handler(int, const char*)|.
The parameters are replaced by the error number and the error message
respectively.\\

{\bf New:}\\
Starting with version 4.3 LEDA provides an {\em exception error handler}\\
|void exception_error_handler(int num, const char* msg)|\\
This handler uses the \CC exception mechanism and throws an exception of 
type |leda_exception| instead of terminating the program. An object of type 
|leda_exception| stores a pair consisting of an error number and 
an error message. Operations |e.get_msg()| and |e.get_num()| can be called to 
retrieve the corresponding values from an exception object $e$.
}*/


#ifndef LEDA_DISABLE_EXCEPTIONS

class __exportC leda_exception {

protected:

 char   msg[256]; // error message
 int    num;      // error number

public:
 leda_exception(const char* s, int n);
 const char*  get_msg() const { return msg; }
 int          get_num() const { return num; }
};

#endif



//------------------------------------------------------------------------------
// Error Handling
//------------------------------------------------------------------------------

typedef void (*LedaErrorHandler)(int,const char*);

extern __exportD LedaErrorHandler std_error_handler;

extern __exportF void default_error_handler(int,const char*);

extern __exportF void exception_error_handler(int,const char*);


/*{\Moperations 2 5 }*/

inline void error_handler(int err_no, const char* msg)  
{ std_error_handler(err_no,msg); }
/*{\Mfunc  reports error messages by passing |err_no| and |msg| to the 
           default error handler. }*/

extern __exportF LedaErrorHandler set_error_handler(void (*err_handler)(int,const char*));
/*{\Mfunc  sets the default error handler to function |err_handler|.
           Returns a pointer to the previous error handler. }*/

extern __exportF LedaErrorHandler get_error_handler();
/*{\Mfunc  returns a pointer to the current default error handler. }*/


extern __exportF void catch_system_errors(bool b=true);
/*{\Mfunc  after a call to this function system errors 
           ( e.g. bus errors and segmentation faults) are handled by
           LEDA's error handler. }*/



extern __exportF bool leda_assert(bool cond, const char* err_msg, int err_no=0);
/*{\Mfunc  calls |error_handler(err_no,err_msg)| if |cond = false|
           and returns |cond|. }*/




#define LEDA_EXCEPTION(i,msg) \
error_handler(i,string("%s (%s:%d)",msg,__FILE__,__LINE__))





#ifndef LEDA_CHECKING_OFF
#define LEDA_OPT_PRECOND(cond,fstr) \
if (!(cond)) { cerr << "LEDA PRECONDITION VIOLATION: "fstr"\n"; abort(); }
#else
#define LEDA_OPT_PRECOND(cond,fstr)
#endif

#define LEDA_PRECOND(cond,fstr) \
if (!(cond)) { cerr << "LEDA PRECONDITION VIOLATION: "fstr"\n"; abort(); }



#if LEDA_ROOT_INCL_ID == 600226
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
