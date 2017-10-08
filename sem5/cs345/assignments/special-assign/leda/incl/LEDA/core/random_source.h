/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  random_source.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RANDOM_SOURCE_H
#define LEDA_RANDOM_SOURCE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600012
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// random numbers
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {random_source} {} {Random Sources} }*/


class __exportC random_source {

/*{\Mdefinition 
An instance of type |\Mname| is a random source. It allows to generate
uniformly distributed random bits, characters, integers, and doubles.
It can be in  either of two modes: In bit mode it generates a random bit string
of some given length $p$ ($1 \le p \le 31$) and in integer mode it generates a random
integer in some given range $[ low .. high]$  ($low \le high < low + 2^{31}$). The mode can be changed any time, either globally or for a single operation. The output of the random
source can be converted to a number of formats (using standard conversions).}*/


  long  table[32];
  long* ptr0;
  long* ptr1;
  long* ptr2;
  long* ptr_end;
  
  unsigned long pat;

  int  prec;
  int  low;
  int  diff;
  bool bit_mode;

  unsigned long buffer;

  int mseed;

static atomic_counter count;

void          init_table();

// copy constructor and assignment not implemented (private)

random_source(const random_source&) {}
random_source& operator=(const random_source&) { return *this; }

public:

unsigned long get_rand31();
unsigned long get_rand32();
void get_str(char*,unsigned int,int=-1);

/*{\Mcreation S }*/

random_source();
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname|, 
             puts it into bit mode, and 
             sets the precision to 31. }*/

random_source(int p);
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname|, 
             puts it into bit mode, and 
             sets the precision to $p$ ($1 \le p \le 31$). }*/

random_source(int low, int high);
/*{\Mcreate  creates an instance |\Mvar| of type |\Mname|, 
             puts it into integer mode, and 
             sets the range to $[ low .. high]$. }*/


/*{\Moperations 3.2 5 }*/

unsigned long get()
{ 
 unsigned long r = get_rand32();
#if defined(WORD_LENGTH_64)
 r = (r << 32) | get_rand32();
#endif
 return r;
}
/*{\Mop         returns a random unsigned integer of maximal precision ($32$ 
                bits on 32-bit systems and $64$ bits on 64-bit systems). }*/

  
void set_seed(int s);
/*{\Mop         resets the seed of the random number generator to $s$. }*/

int reinit_seed();
/*{\Mop         generates and sets a new seed $s$. The return value is $s$.}*/

void set_range(int low, int high);
/*{\Mopl        sets the mode to integer mode and 
                changes the range to $[|low| .. |high|]$. }*/

int set_precision(int p);
/*{\Mop         sets the mode to bit mode, changes the precision 
                to $p$ bits and returns previous precision. }*/

int get_precision() { return prec; }
/*{\Mop         returns current precision of |\Mvar|. }*/


// extract random numbers with precision or range specified in constructor

random_source& operator>>(char& x);
/*{\Mbinop      extracts a character $x$ of default precision or range
                and returns |\Mvar|, i.e., it first generates an unsigned
                integer of the desired precision or in the desired range
                and then converts it to a character (by standard conversion).
                 }*/

random_source& operator>>(unsigned char& x);
/*{\Mbinop      extracts an unsigned character $x$ of default precision or 
                range and returns |\Mvar|. }*/

random_source& operator>>(int& x);
/*{\Mbinop      extracts an integer $x$ of default precision or range and 
                returns |\Mvar|. }*/

random_source& operator>>(long& x);
/*{\Mbinop      extracts a long integer $x$ of default precision or range and 
                returns |\Mvar|. }*/


random_source& operator>>(unsigned int& x);
/*{\Mbinop      extracts an unsigned integer $x$ of default precision or
                range and returns |\Mvar|. }*/

random_source& operator>>(unsigned long& x);
/*{\Mbinop      extracts a long unsigned integer $x$ of default precision or 
                range and returns |\Mvar|. }*/
       


random_source& operator>>(double& x);
/*{\Mbinop      extracts a double precision floating point number $x$ in 
                $[0,1]$, i.e, $u/(2^{31} -1)$ where $u$ is a random integer 
                in $[0..2^{31}-1]$, and returns |\Mvar|.}*/

random_source& operator>>(float& x);
/*{\Mbinop      extracts a single precision floating point number $x$ in 
                $[0,1]$, i.e, $u/(2^{31} -1)$ where $u$ is a random integer 
                in $[0..2^{31}-1]$, and returns |\Mvar|.}*/


random_source& operator>>(bool& b);
/*{\Mbinop      extracts a random boolean value (true or false). }*/


// raw read (31 bit)


// operator()

int operator()();    
/*{\Mfunop         returns an integer of default precision or range.}*/

int operator()(int prec);
/*{\Mfunop         returns an integer of supplied precision $prec$. }*/

int operator()(int low, int high);
/*{\Mfunop         returns an integer from the supplied range $[low..high]$. }*/


};


class random_source_mt : public random_source {

leda_lock mtx;


/*
random_source& operator>>(char& x);
random_source& operator>>(unsigned char& x);
random_source& operator>>(int& x);
random_source& operator>>(long& x);
random_source& operator>>(unsigned int& x);
random_source& operator>>(unsigned long& x);
random_source& operator>>(double& x);
random_source& operator>>(float& x);
random_source& operator>>(bool& b);
*/

public:

random_source_mt() {}


template<class T>
 random_source_mt& operator>>(T& x)
{ mtx.lock(); 
  //random_source::operator>>(x);
  (random_source&)*this >> x;
  mtx.unlock(); 
  return *this;
}


void set_range(int l, int h)
{ mtx.lock(); 
  random_source::set_range(l,h);
  mtx.unlock(); 
}

int set_precision(int p)
{ mtx.lock(); 
  int x = random_source::set_precision(p);
  mtx.unlock(); 
  return x;
}

int get_precision() { return random_source::get_precision(); }

unsigned long get()
{ mtx.lock(); 
  unsigned long x = random_source::get();
  mtx.unlock(); 
  return x;
}

  
void set_seed(int s)
{ mtx.lock(); 
  random_source::set_seed(s);
  mtx.unlock(); 
}


int operator()()
{ mtx.lock();
  int x = random_source::operator()();
  mtx.unlock();
  return x;
}

int operator()(int p)
{ mtx.lock();
  int x = random_source::operator()(p);
  mtx.unlock();
  return x;
}

int operator()(int l, int h)
{ mtx.lock();
  int x = random_source::operator()(l,h);
  mtx.unlock();
  return x;
}

};


extern __exportD random_source_mt rand_int;   // standard random input stream


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600012
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
