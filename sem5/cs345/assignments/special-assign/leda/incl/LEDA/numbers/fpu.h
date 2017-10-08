/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FPU_H
#define LEDA_FPU_H

#include <LEDA/numbers/fpu/fpu_functions.h>


LEDA_BEGIN_NAMESPACE

/* --------------------------------------------------------------
 *
 * this is the file leda_fpu.h
 *
 * purpose: declares class leda_fpu
 * author:  Christoph Burnikel,
 *          Max Planck Institut fuer Informatik,
 *          66123 Saarbruecken, GERMANY
 * last modified: 07/99
 *
 * Please let me know about extensions, corrections, bug-fixes, 
 * any problems and comments you have.
 * Send mail to burnikel@mpi-sb.mpg.de
 *
-------------------------------------------------------------- */

/*{\Manpage {leda_fpu} {} {Floating-Point Unit Acess Control} }*/

/*{\Mdefinition
Class |\Mtype| provides an interface for controlling certain advanced features 
of double--precision arithmetic that are realized in the hardware 
of most modern PC's and workstations. 
This class cannot be instantiated and all its member functions are
static.
Note that class |\Mtype| can only be used on such machines that conform 
(at least partially) to the IEEE754 standard for floating-point arithmetic.
In the the implementation section we give a list of processor families 
for which class |\Mtype| is available.
The most important goals of class |leda_fpu| are
\begin{itemize} 
\item Rounding control
\item Exception control
\item Control of arithmetic traps and trap handling
\end{itemize}
Rounding modes, exceptions and traps are controlled in hardware 
by certain bits in the so-called {\em floating-point control register} 
(FCR). Note that this register is also occasionally called 
{\em floating-point status register} (FSR) and that for some machines
there are separate FCR and FSR registers. 

There are four rounding modes for
|double| arithmetic specified by IEEE754:
\begin{itemize}
\item |ieee_nearest|  : always round towards the nearest representable value
\item |ieee_positive| : always round up to the next bigger representable value
\item |ieee_negative| : always round down to the next smaller representable value
\item |ieee_tozero|   : chop off all bits after the least significant bit of the
                        exact result.
\end{itemize}


There are five arithmetic exceptions whose occurence is by default noted down 
in the FSR. These are:
\begin{itemize}
\item |ieee_inexact|   : inexact operation (loss of precision)
\item |ieee_overflow|  : arithmetic overflow (exponent of result too large)
\item |ieee_underflow| : arithmetic underflow (exponent of result too small)
\item |ieee_invalid|   : invalid operation (e.g. $\infty-\infty$, $\sqrt{-1}$,...)
\item |ieee_divbyzero| : division by zero (e.g. $1/0$).
\end{itemize}
If one of the previous arithmetic exceptions occurs, a ''sticky bit'' in the
FCR is raised which stays raised until explicitly cleared.
A more precise description of the meaning of these exceptions can be 
found in the original paper on the IEEE754 standard \cite{IEEE754} or 
in Goldberg's survey paper \cite{Goldberg-IEEE}.

IEEE754 demands that there exists a mechanism to activate exception handlers 
via arithmetic traps. First of all we provide a function to {\em inactivate} 
all arithmetic traps. For the LEDA library this is probably the most important 
option regarding arithmetic traps. Sometimes however, in particular if one 
debugs a program suspected of having problems with arithmetic exceptions, 
it is convenient to have a message displayed as soon as an arithmetic 
exception occurs. Here it is important to (1) get informed about the precise 
type of the exception and (2) to get this information immediately (or at least 
as quick as possible). Unfortunately in real systems these wishes are not 
always satisfied, either because implementations are only partially compliant 
to IEEE754 or because the combination of operating system and compiler do not 
let the programmer use all of the floating point features of which 
the processor is capable. This means that the behaviour of the trap handling 
utilities of class |leda_fpu| is somewhat platform-dependent. Class |leda_fpu| provides 
several policies for trap handling, which are selected by the function 
|leda_fpu::set_exception_policy(exc)| for a subset |exc| of the IEEE754 exceptions:
\begin{itemize}
\item |leda_fpu::ignore|: inactivate the traps for the exceptions given 
      by |exc|, and always clear the associated sticky exception bits in the FSR. 
      The effect is that - using the functions of class |leda_fpu| - it is not
      possible to detect if an exception in |exc| has occured while this policy
      is active. 
\item |leda_fpu::silence|: inactivate the traps for |exc|, but keep the 
      respective sticky exception bits set for further investigation. This is 
      the default policy. 
\item |leda_fpu::immediate|: handle the exceptions |exc| directly by 
      a handler routine that is specified via |leda_fpu::set_imediate_handler|.
      If possible, use the IEEE754 trap mechanism to call the handler;
      otherwise call the handler from software routines of class |leda_fpu|,
      as soon as possible. Note that often the trap handler cannot recognize 
      the exact type of exception that has caused the trap, and that the program
      might not be able to continue after returning from the handler function. 
\item |leda_fpu::retrospective|: inactivate the traps for |exc|. At program 
      termination, call a function specified by |leda_fpu::set_retrospective_handler| 
      that usually displays a warning message for every exception that has 
      occured during the time when the |leda_fpu::retrospective| policy was active
      for this exception. 
\end{itemize}

There are various restrictions for the usage of class |\Mtype|. First of all, 
it is only implementable for machines that conform to the IEEE754 standard,
and currently is implemented for a subset of these machines. 
Moreover, there are processor lines for which there are subtle bugs in 
the hardware implementation of certain floating-point computations. 
For example, on some older DEC alpha processors, directed rounding
did not work correctly (even though appropriate compiler flags were used
to generate machine code for ``dynamic rounding''). 
In other cases, the computing environment (operating system/compiler)
makes it impossible for the trap handlers to return safely to the main
program. To cope with all these platform-dependencies, there is a
set of functions to check the behaviour on the given platform. 
This makes it possible to write platform-independent code. 

Further restrictions have to do with compiler optimization. Some 
compilers compute the values of variables which they regard as constants 
at compile-time, depending on the level of optimization.
This may defeat the FPU rounding, precision or exception control. 
One way to guard against this kind of trouble is to put all such 
computations in non-inline functions and compile these functions
in a different translation unit. When using class |\Mtype|, please check 
first the manual of your compiler to avoid optimization that is expressedly 
not IEEE conform; e.g. IBM Cset requires the option -qstrict 
at optimization level -O3.

Please inform us about any other trouble you encounter using class |\Mtype|.
Although this software was carefully tested, there may be other problems
that we are not aware of. In such cases, please contact met at 
burnikel@mpi-sb.mpg.de or the LEDA administration at ledares@mpi-sb.mpg.de .
}*/


/*{\Moperations 2 2
}*/


class __exportC leda_fpu {

private:

  leda_fpu();

  // constructor private because the class leda_fpu is a singleton
  // initialized by the function leda_fpu::set_defaults()

// rounding diagnosis:

  static fcr_type available_rounding_modes;
  // all rounding modes that can be adjusted 

  static bool all_rounding_modes_available;
  // if all IEEE rounding modes can be adjusted 

  static bool sqrt_rounding_okay;
  // true if sqrt is rounded according to IEEE for the various 
  // rounding modes 
  
  static fcr_type check_available_rounding_modes(void);
  // initialises available_rounding_modes 
  static bool check_sqrt_rounding(void);
  // initialises  sqrt_rounding_okay 


// types of exceptions:

  static volatile fcr_type pending_exceptions;
  // the pending IEEE exceptions 

  static volatile fcr_type retrospective_exceptions;
  // those exceptions that shall be reported at program termination

  static volatile fcr_type ignored_exceptions;
  // exceptions that shall be always ignored (policy 'ignore') 

  static volatile fcr_type silenced_exceptions;
  // exceptions that we let pass silently (default policy 'silence')

  static volatile fcr_type observed_exceptions;
  // already observed exceptions come here for a final, 
  // retrospective diagnosis

  static volatile fcr_type trapping_exceptions;
  // exceptions that shall invoke a trap handler 


// basic exception actions:

  static fcr_type exception_recently_raised(fcr_type x);
  // returns all newly raised exceptions 

  static fcr_type store_all_recent_exceptions(void);
  // stores all pending exceptions internally  

  // the following functions return true iff the respective 
  // exception is correctly raised 

  static fcr_type check_inexact_exception(void);
  static fcr_type check_underflow_exception(void);
  static fcr_type check_overflow_exception(void);
  static fcr_type check_divbyzero_exception(void);
  static fcr_type check_invalid_exception(void);


// basic trap actions:

  // the next functions are self-explanatory 

  static void activate_trap(fcr_type exceptions);
  static void inactivate_trap(fcr_type exceptions);
  static void inactivate_all_traps(void);


// trap diagnosis:

  static bool sigfpe_signal_okay;
  // true iff SIGFPE signal handler can be reached after raise(SIGFPE); 

  static bool check_sigfpe_signal(void);
  // sets sigfpe_signal_okay 

  static fcr_type recoverable_exceptions;
  // all exceptions which can be detected; if a trap handler is
  // unavoidable, it must be possible to return from this trap 
  // handler
  
  static fcr_type avoidable_traps;
  // all exceptions for which the SIGFPE signal can be avoided 

  static fcr_type catchable_exceptions;
  // all exceptions that can be catched by a SIGFPE signal handler 

  static fcr_type recoverable_traps;
  // all exceptions for which a SIGFPE signal handler may return 
  // (only those that can be catched, of course)

  static fcr_type detectable_trap_types;
  // all exceptions for which the SIGFPE signal handler sees the type of
  // the exception 

  static bool all_traps_avoidable;
  // set if all IEEE traps can be avoided 

  static bool all_exceptions_catchable;
  // set if all IEEE exceptions can be made to trap 

  static bool all_exceptions_recoverable;
  // set if all IEEE exceptions are recoverable 

  static bool all_traps_recoverable;
  // set if trap handlers may return from all IEEE exceptions 

  static bool all_trap_types_detectable;
  // set if signal handlers can determine the specific IEEE
  // exception that raised the signal 

  
  static fcr_type check_traps(void);
  // sets catchable_exceptions, recoverable_traps, detectable_trap_types

  static void do_check_traps(void);
  // calls check_traps()
  
  static fcr_type check_recoverable_exceptions(void);
  // sets recoverable_exceptions, avoidable_traps

  static void do_check_recoverable_exceptions(void);
  // calls check_recoverable_exceptions()
   

// trap action

#ifndef FPU_MIPS_GCC_WORKAROUND
  static void sigfpe_action(int sig);
#else
  static void sigfpe_action(...);
#endif
  // private method set as global signal handler 
  // calls sigfpe_ignore and possibly sigfpe_immediate 
 
  static void (*sigfpe_immediate)(fcr_type);
  // the handler function that is called in trap mode 'immediate' 

  static void (*sigfpe_retrospective)(fcr_type);
  // the handler function that is called in trap mode 'immediate' 

  static void sigfpe_ignore();
  // cannot be changed by the user 

  static void default_sigfpe_immediate(fcr_type exception);
  // default value of sigfpe_immediate 

  static void default_sigfpe_retrospective(fcr_type exception);
  // default value of sigfpe_retrospective 


  
public:


  static int set_defaults(void);
  /*{\Mstaticl 
     Puts the floating-point unit in a state that is uniform on all
     platforms: Rounding to |double| precision in mode |ieee_nearest|, 
     with all traps disabled and the sticky exception bits cleared  }*/


  static void round_nearest(void);
  /*{\Mstatic Sets global rounding to the nearest representable value }*/

  static void round_up(void);
  /*{\Mstatic Sets global rounding to the next bigger representable value }*/

  static void round_down(void);
  /*{\Mstatic Sets global rounding to the next smaller representable value }*/

  static void round_tozero(void);
  /*{\Mstatic Global rounding is done by chopping off all bits after the 
     least significant bit of the floating-point format }*/

  static fcr_type set_rounding(fcr_type x);
  /*{\Mstatic Sets global rounding to one of the values |ieee_nearest|,
     |ieee_positive|, |ieee_negative|, |ieee_tozero|; the old 
     rounding mode is returned as an |fcr_type| }*/

  static void raise_inexact_exception(void);
  /*{\Mstatic raise the IEEE754 inexact-operation exception }*/
  static void raise_underflow_exception(void);
  /*{\Mstatic raise the IEEE754 underflow exception }*/
  static void raise_overflow_exception(void);
  /*{\Mstatic raise the IEEE754 overflow exception }*/
  static void raise_divbyzero_exception(void);
  /*{\Mstatic raise the IEEE754 division-by-zero exception }*/
  static void raise_invalid_exception(void);
  /*{\Mstatic raise the IEEE754 invalid-operation exception }*/

  static fcr_type exceptions_raised(fcr_type exc);
  /*{\Mstatic returns all exceptions among the set exc that have
     actually occured }*/

  static fcr_type unraise_exceptions(fcr_type exc);
  /*{\Mstatic clears all pending exceptions from the set exc, stores 
     the others and returns all previously pending exceptions from exc. }*/

  static fcr_type unraise_all_exceptions(void);
  /*{\Mstatic clears and returns all pending exceptions.  }*/


  enum exception_policy { 
    ignore=0, 
    retrospective=1,
    immediate=2,
    silence=3
  };

  static void set_exception_policy
  (fcr_type x, exception_policy=leda_fpu::silence);
  /*{\Mstaticl selects the trap policy for the given exceptions
      to one of \{ignore,silence,retrospective,immediate\} }*/

  static fcr_type ignore_all_exceptions(void);
  /*{\Mstaticl sets exception policy 'ignore' for all exceptions }*/

  static fcr_type silence_all_exceptions(void);
  /*{\Mstaticl sets exception policy 'silence' for all exceptions }*/

  static void process_all_exceptions(void);
  /*{\Mstatic processes all pending exceptions; in particular, 
    trap handlers are called for all pending exceptions
    for which |leda_fpu::immediate| is active }*/

  static void set_immediate_handler(void (*my_handler)(fcr_type));
  /*{\Mstaticl sets the trap handler for the trap policy {\em immediate}.
     Caution: C++ exceptions can be thrown from |my_handler| but 
     they will never get catched. }*/

  static void set_retrospective_handler(void (*my_handler)(fcr_type));
  /*{\Mstaticl sets the trap handler for the trap policy {\em retrospective} }*/


  static fcr_type get_computable_exceptions(void);
  /*{\Mstaticl returns those exceptions for which correct IEEE values are 
      always computed (usually all)
   }*/
  static bool exceptions_computable(fcr_type exc) {
    return ((get_computable_exceptions()&exc) == exc);
  }
  /*{\Mstaticl returns true iff all exceptions in the set |exc| are 
      always correctly computed according to IEEE754
  }*/

  static fcr_type get_avoidable_traps(void);
  /*{\Mstaticl returns all exceptions for which the SIGFPE trap can be   
     avoided (usually all) 
  }*/
  static bool traps_avoidable(fcr_type exc) {
    return ((get_avoidable_traps()&exc) == exc);
  }
  /*{\Mstaticl returns true iff none of the exceptions in the set |exc| 
     causes a trap in the |leda_fpu::set_defaults()| setting. 
  }*/

  static fcr_type get_recoverable_exceptions(void);
  /*{\Mstaticl returns all exceptions that can be detected
  }*/
  static bool exception_recoverable(fcr_type exc) {
    return ((get_recoverable_exceptions()&exc) == exc);
  }
  /*{\Mstaticl returns true iff all of the exceptions in exp 
    can be detected 
  }*/

  static fcr_type get_recoverable_traps(void);
  /*{\Mstaticl returns all exceptions for which the SIGFPE trap handler
               may return  
  }*/
  static bool traps_recoverable(fcr_type exc) {
    return ((get_recoverable_traps()&exc) == exc);
  }
  /*{\Mstaticl returns true iff trap handlers may return from all
    of the exceptions in the set |exc|. 
  }*/

  static fcr_type get_trapping_exceptions();
  /*{\Mstaticl to be called from a trap handler; returns the set of exceptions
      that has caused the trap (0 if this exception could not be determined).  
  }*/

  static void display_exceptions(fcr_type exceptions);
  /*{\Mstaticl called from a trap handler; display the given exceptions
     in long format
  }*/
  static void display_exceptions_short(fcr_type exception);
  /*{\Mstaticl called from a trap handler; display the given exceptions
     shortly
  }*/

  static void diagnosis();
  /*{\Mstaticl does a partial analysis of the IEEE754 compliance of
     the system and displays the results
  }*/

  ~leda_fpu();

};

/*{\Mimplementation
Many of the methods of class |\Mtype| cannot be implemented using standard 
C/\CC, since these high-level languages do not provide access to the 
floating-point control and status registers.
Although some compiler libraries contain procedures that provide part
of the functionality of class |\Mtype|, none of the libraries provides 
the complete functionality.
Moreover, the interfaces vary not only from machine to machine, but also
from compiler version to compiler version.
Here we decided to write class |\Mtype| from scratch, using a few lines
of GNU assembler code which has to be re-written for every processor family 
but can be used for all compilers on the same machine.
For this reason, the functions of class |\Mtype| currently can only be inlined 
when GNU compilers are used.

Our code was developed and tested for the following platforms.
Here the particular version of the operating system should not matter
and using other compilers on the supported architectures should not be 
a problem. However, beware of the restrictions mentioned in the 
definition section.
\begin{itemize}
\item SPARC/solaris with GNU(g++)
\item MIPS/irix with MipsPro(CC)
\item x86/linux with GNU(g++)
\item alpha/linux with GNU(g++)
\end{itemize}
In principle, portations of class |\Mtype| are possible for all processors 
having a FPU compliant to IEEE754.
Notable exceptions are Cray and VAX architectures, which are not IEEE754
compliant. Moreover, the DEC alpha architecture supports IEEE754 only partially
in hardware which implies several restrictions: Directed rounding and the
correct computation of overflow, underflow, and division by zero exceptions
may require the use of special compile options on these machines. 

\bibliographystyle{alpha}
\bibliography{/KM/usr/burnikel/bib/my}
}*/

LEDA_END_NAMESPACE

#endif

