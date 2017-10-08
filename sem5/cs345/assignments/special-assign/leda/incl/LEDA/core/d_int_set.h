/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d_int_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D_INT_SET_H
#define LEDA_D_INT_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600034
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

typedef unsigned long  word;

/*{\Manpage {d_int_set} {} {Dynamic Integer Sets}}*/

class __exportC d_int_set {
public:
  typedef int value_type;
  typedef value_type element_type;

private:
/*{\Mdefinition
An instance $S$ of the data type |\Mname| is a subset of the integers. }*/

// number of bits per word

//static const int SIZE_OF_ULONG = 8 * sizeof(unsigned long); // msc6 problem 
  enum { SIZE_OF_ULONG = 8 * sizeof(unsigned long) };

  unsigned long* V;    // array of ulongs
  short NoOfElements;  // number of entries
  short Offset;        // number of ulongs between zero and min ulong
  short NoOfWords;     // number of used ulongs
	// NOTE: bit j in V[i] corresponds to element CalcVal(j, Offset+i)

  short CalcOffset(int x) const { 
  /* calculates the offset in which word the int is stored */
    int w = x/SIZE_OF_ULONG;
    if ( x < 0 && (x%SIZE_OF_ULONG) ) w--;
    return w;
  }
  short CalcBit(int x) const {
  /* calculates the position/bit i where the int is stored in a
     suitable offsetted word */
    short i = x%SIZE_OF_ULONG;
    if ( x < 0 ) return (SIZE_OF_ULONG+i)%SIZE_OF_ULONG;
    else         return i;
  }
  int CalcVal(int i, int offset) const {
  /* calculates the int value which is represented by bit i
     i in the word at offset; where 0 <= i < SIZE_OF_ULONG;
     if (offset >= 0) val = offset*SIZE_OF_ULONG + i
     if (offset <  0) val = -((-offset-1)*SIZE_OF_ULONG + SIZE_OF_ULONG-i);
     which results in the above! */
   return offset*SIZE_OF_ULONG + i;
  }
 
public:

  LEDA_MEMORY(d_int_set)

/*{\Mcreation S }*/

 d_int_set();
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|
            initializes it to the empty set.}*/
            
 d_int_set(const d_int_set&);  

~d_int_set() { if(V != nil) delete[] V; }  

 d_int_set& operator=(const d_int_set& S1);   

/*{\Moperations 1.8 5 }*/

 int first_element() const { return V != nil ? min() : stop_element(); }
 int next_element(int x) const;
 int stop_element() const { return MAXINT; }

int min() const;
/*{\Mop      returns the smallest element in |\Mvar|.\\
             \precond |\Mvar| is not empty.}*/

int max() const;
/*{\Mop      returns the largest element in |\Mvar|.\\
             \precond |\Mvar| is not empty.}*/

void insert(int x);  
/*{\Mop adds $x$ to |\Mvar|. As the sets range is expanding
        dynamically during insertion for the range |[\Mvar.min(),\Mvar.max()]|
        inserting the extrema early saves repeated reallocation time.}*/

void del(int x);            
/*{\Mop     deletes $x$ from |\Mvar|.}*/

bool member(int x) const;    
/*{\Mop      returns true if $x$ in |\Mvar|, false otherwise.}*/

int  choose() const;
/*{\Mop  returns a random element of |\Mvar|.\\
         \precond |\Mvar| is not empty.}*/

bool subset(const d_int_set& S1) const; 
bool proper_subset(const d_int_set& S1) const ;

bool empty() const; 
/*{\Mop        returns true if |\Mvar| is empty, false otherwise.}*/
           
int  size() const;
/*{\Mop        returns the size of |\Mvar|.}*/

void clear();                
/*{\Mop      makes |\Mvar| the empty set.}*/

void _join(const d_int_set& T);
void _intersect(const d_int_set& T);
void _diff(const d_int_set& T);
void _symdiff(const d_int_set& T);

d_int_set join(const d_int_set& T) const
{ d_int_set res(*this); res._join(T); return res; }
/*{\Mop    returns |\Mvar| $\cup$ |T|. }*/
 
d_int_set intersect(const d_int_set& T) const
{ d_int_set res(*this); res._intersect(T); return res; }
/*{\Mop    returns |\Mvar| $\cap$ |T|. }*/

d_int_set diff(const d_int_set& T) const
{ d_int_set res(*this); res._diff(T); return res; }
/*{\Mop    returns |\Mvar| $-$ |T|. }*/
    
d_int_set symdiff(const d_int_set& T) const
{ d_int_set res(*this); res._symdiff(T); return res; }
/*{\Mop    returns the symmectric difference of |\Mvar| and |T|. }*/

d_int_set  operator+(const d_int_set& T) const
{ return join(T); }
/*{\Mbinop    returns the union |\Mvar.join(T)|. }*/

d_int_set  operator-(const d_int_set& T) const
{ return diff(T); }
/*{\Mbinop    returns the difference |\Mvar.diff(T)|. }*/

d_int_set  operator&(const d_int_set& T) const
{ return intersect(T); }
/*{\Mbinop    returns the intersection of |\Mvar| and |T|. }*/

d_int_set  operator|(const d_int_set& T) const
{ return join (T); }
/*{\Mbinop    returns the union |\Mvar.join(T)|.}*/
 
d_int_set  operator%(const d_int_set& T) const
{ return symdiff(T); }
/*{\Mbinop    returns the symmetric difference |\Mvar.symdiff(T)|.}*/
 
d_int_set& operator+=(const d_int_set& T)
{ this->_join(T); return *this; }
/*{\Mbinop    assigns |\Mvar.join(T)| to |\Mvar| and returns |\Mvar|. }*/

d_int_set& operator-=(const d_int_set& T)
{ this->_diff(T); return *this; }
/*{\Mbinop    assigns |\Mvar.diff(T)| to |\Mvar| and returns |\Mvar|. }*/

d_int_set& operator&=(const d_int_set& T)
{ this->_intersect(T); return *this; }
/*{\Mbinop    assigns |\Mvar.intersect(T)| to |\Mvar| and returns |\Mvar|. }*/

d_int_set& operator|=(const d_int_set& T)
{ this->_join(T); return *this; }
/*{\Mbinop    assigns |\Mvar.join(T)| to |\Mvar| and returns |\Mvar|. }*/

d_int_set& operator%=(const d_int_set& T)
{ this->_symdiff(T); return *this; }
/*{\Mbinop    assigns |\Mvar.symdiff(T)| to |\Mvar| and returns |\Mvar|. }*/

bool operator!=(const d_int_set& T) const;  
/*{\Mbinop    returns true if $\Mvar \neq T$, false otherwise. }*/

bool operator==(const d_int_set& T) const;  
/*{\Mbinop    returns true if $\Mvar = T$, false otherwise. }*/

bool operator>=(const d_int_set& T) const; 
/*{\Mbinop    returns true if $T \subseteq \Mvar$, false otherwise. }*/

bool operator<=(const d_int_set& T) const;  
/*{\Mbinop    returns true if $\Mvar \subseteq T$, false otherwise. }*/

bool operator<(const d_int_set& T) const; 
/*{\Mbinop    returns true if $\Mvar \subset T$, false otherwise. }*/

bool operator>(const d_int_set& T) const;   
/*{\Mbinop    returns true if $T \subset \Mvar$, false otherwise. }*/


void get_element_list(list<int>& L) const;
/*{\Mop  fills |L| with all elements stored in the set in increasing
         order.}*/

friend __exportF ostream& operator<<(ostream& O, const d_int_set& C); 
friend __exportF istream& operator>>(istream& I, d_int_set& C); 

};

/*{\Mtext \headerline{Iteration}

{\bf forall\_elements}($x$,$S$)       
$\{$ ``the elements of $S$ are successively assigned to $x$'' $\}$

}*/

#define forall_elements(x, S) \
LEDA_FORALL_PREAMBLE \
for (int loop_var = S.first_element(); \
     x=loop_var, loop_var != S.stop_element(); \
	 loop_var = S.next_element(loop_var))


/*{\Mimplementation
Dynamic integer sets are implemented by (dynamic) bit vectors. 
Operations member, empty, size, min and max take constant time. 
The operations clear, intersection, union and complement take time 
$O(b-a+1)$, where $a=|max()|$ and $b=|min()|$.
The operations insert and del also take time $O(b-a+1)$, if the bit vector has
to be reallocated. Otherwise they take constant time.
Iterating over all elements (with the iteration macro) requires time $O(b-a+1)$
plus the time spent in the body of the loop.
}*/

#if LEDA_ROOT_INCL_ID == 600034
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
