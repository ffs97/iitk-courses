/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  tuple.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LEDA_TUPLE_H
#define _LEDA_TUPLE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600030
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE



/*{\Manpage {two_tuple} {A,B} {Two Tuples} {p}}*/

/*{\Mdefinition
An instance |p| of type |\Mname| is a two-tuple $(a,b)$ of variables  
of types $A$, and $B$, respectively. 

Related types are |two_tuple|, |three_tuple|, and |four_tuple|.
}*/


template <class A, class B>
class two_tuple
{ A a;
  B b;

public:

/*{\Mtypes 5}*/

typedef A first_type;
/*{\Mtypemember the type of the first component.}*/

typedef B second_type;
/*{\Mtypemember the type of the second component.}*/


/*{\Mcreation}*/ 

two_tuple() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. All components
are initialized to their default value.}*/

two_tuple(const A& u, const B& v):
a(u), b(v)
{ }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes
it with the value $(u,v)$.}*/

// default copy constructor, assignment operator, and destructor work


/*{\Moperations 2 4}*/

const A&  first() const { return a;} 

      A& first() { return a;} 
/*{\Mop      returns the $A$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $A$.}*/

const B&  second() const { return b;} 

      B& second() { return b;}
/*{\Mop      returns the $B$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $B$.}*/

LEDA_MEMORY(two_tuple);

};


template <class A, class B> inline
istream& operator>>(istream& is,two_tuple<A,B>& q)
{ is >> q.first();
  is >> q.second();
  return is;
}

template <class A, class B> inline
ostream& operator<<(ostream& os,const two_tuple<A,B>& q)
{ os << q.first()  << " ";
  os << q.second();
  return os;
}




template <class A, class B> inline
bool operator==(const two_tuple<A,B>& p, const two_tuple<A,B>& q)
{ return p.first() == q.first() && p.second() == q.second();
}
/*{\Mbinopfunc equality test for |two_tuples|. Each of the 
component types must have an equality operator. }*/

template <class A, class B> inline
bool operator!=(const two_tuple<A,B>& p, const two_tuple<A,B>& q)
{ return p.first() != q.first() || p.second() != q.second();
}



template <class A, class B> inline
int compare(const two_tuple<A,B>& p, const two_tuple<A,B>& q)
{ int s = compare(p.first(),q.first());
  if (s != 0) return s;
  return compare(p.second(),q.second());
}
/*{\Mfunc lexicographic ordering for |two_tuples|. Each of the 
component types must have a compare function. }*/


template <class A, class B> inline
int Hash(const two_tuple<A,B>& p)
{ return Hash(p.first()) & Hash(p.second());
}
/*{\Mfunc hash function for |two_tuples|. Each of the 
component types must have a Hash function.}*/


/*{\Mimplementation 
The obvious implementation is used.
}*/


/*{\Manpage {three_tuple} {A,B,C} {Three Tuples} {p}}*/

/*{\Mdefinition
An instance |p| of type |\Mname| is a three-tuple $(a,b,c)$ of variables  
of types $A$, $B$, and $C$, respectively. 

Related types are |two_tuple|, |three_tuple|, and |four_tuple|.
}*/


template <class A, class B, class C>
class three_tuple
{ A a;
  B b;
  C c;

public:

/*{\Mtypes 5}*/

typedef A first_type;
/*{\Mtypemember the type of the first component.}*/

typedef B second_type;
/*{\Mtypemember the type of the second component.}*/

typedef C third_type;
/*{\Mtypemember the type of the third component.}*/


/*{\Mcreation}*/ 

three_tuple() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. All components
are initialized to their default value.}*/

three_tuple(const A& u, const B& v, const C& w):
a(u), b(v), c(w)
{ }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes
it with the value $(u,v,w)$.}*/

// default copy constructor, assignment operator, and destructor work


/*{\Moperations 2 4}*/

const A&  first() const { return a;} 

      A& first() { return a;} 
/*{\Mop      returns the $A$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $A$.}*/

const B&  second() const { return b;} 

      B& second() { return b;}
/*{\Mop      returns the $B$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $B$.}*/

const C&  third() const { return c;}

      C& third() { return c;}
/*{\Mop      returns the $C$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $C$.}*/

LEDA_MEMORY(three_tuple);

};


template <class A, class B, class C> inline
istream& operator>>(istream& is,three_tuple<A,B,C>& q)
{ is >> q.first();
  is >> q.second();
  is >> q.third();
  return is;
}

template <class A, class B, class C> inline
ostream& operator<<(ostream& os,const three_tuple<A,B,C>& q)
{ os << q.first()  << " ";
  os << q.second() << " ";
  os << q.third();
  return os;
}




template <class A, class B, class C> inline
bool operator==(const three_tuple<A,B,C>& p, const three_tuple<A,B,C>& q)
{ return p.first() == q.first() && p.second() == q.second() &&
         p.third() == q.third();
}
/*{\Mbinopfunc equality test for |three_tuples|. Each of the 
component types must have an equality operator. }*/


template <class A, class B, class C> inline
bool operator!=(const three_tuple<A,B,C>& p, const three_tuple<A,B,C>& q)
{ return p.first() != q.first() || p.second() != q.second() ||
         p.third() != q.third();
}



template <class A, class B, class C> inline
int compare(const three_tuple<A,B,C>& p, const three_tuple<A,B,C>& q)
{ int s = compare(p.first(),q.first());
  if (s != 0) return s;
  s = compare(p.second(),q.second());
  if ( s!=0 ) return s;
  return compare(p.third(),q.third());
}
/*{\Mfunc lexicographic ordering for |three_tuples|. Each of the 
component types must have a compare function. }*/


template <class A, class B, class C> inline
int Hash(const three_tuple<A,B,C>& p)
{ return Hash(p.first()) & Hash(p.second()) & 
         Hash(p.third());
}
/*{\Mfunc hash function for |three_tuples|. Each of the 
component types must have a Hash function.}*/


/*{\Mimplementation 
The obvious implementation is used.
}*/



/*{\Manpage {four_tuple} {A,B,C,D} {Four Tuples} {p}}*/

/*{\Mdefinition
An instance |p| of type |\Mname| is a four-tuple $(a,b,c,d)$ of variables  
of types $A$, $B$, $C$, and $D$, respectively. 

Related types are |two_tuple|, |three_tuple|, and |four_tuple|.
}*/


template <class A, class B, class C, class D>
class four_tuple
{ A a;
  B b;
  C c;
  D d;

public:

/*{\Mtypes 5}*/

typedef A first_type;
/*{\Mtypemember the type of the first component.}*/

typedef B second_type;
/*{\Mtypemember the type of the second component.}*/

typedef C third_type;
/*{\Mtypemember the type of the third component.}*/

typedef D fourth_type;
/*{\Mtypemember the type of the fourth component.}*/

/*{\Mcreation}*/ 

four_tuple() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. All components
are initialized to their default value.}*/

four_tuple(const A& u, const B& v, const C& w, const D& x):
a(u), b(v), c(w), d(x)
{ }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes
it with the value $(u,v,w,x)$.}*/

// default copy constructor, assignment operator, and destructor work


/*{\Moperations 2 4}*/

const A&  first() const { return a;} 

      A& first() { return a;} 
/*{\Mop      returns the $A$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $A$.}*/

const B&  second() const { return b;} 

      B& second() { return b;}
/*{\Mop      returns the $B$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $B$.}*/

const C&  third() const { return c;}

      C& third() { return c;}
/*{\Mop      returns the $C$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $C$.}*/

const D& fourth() const { return d;}

      D& fourth() { return d;}
/*{\Mop      returns the $D$-component of |\Mvar|. If |\Mvar| is a 
const-object the return type is $D$.}*/

LEDA_MEMORY(four_tuple);

};


template <class A, class B, class C, class D> inline
istream& operator>>(istream& is,four_tuple<A,B,C,D>& q)
{ is >> q.first();
  is >> q.second();
  is >> q.third();
  is >> q.fourth();
  return is;
}

template <class A, class B, class C, class D> inline
ostream& operator<<(ostream& os,const four_tuple<A,B,C,D>& q)
{ os << q.first()  << " ";
  os << q.second() << " ";
  os << q.third()  << " ";
  os << q.fourth();
  return os;
}




template <class A, class B, class C, class D> inline
bool operator==(const four_tuple<A,B,C,D>& p, const four_tuple<A,B,C,D>& q)
{ return p.first() == q.first() && p.second() == q.second() &&
         p.third() == q.third() && p.fourth() == q.fourth();
}
/*{\Mbinopfunc equality test for |four_tuples|. Each of the 
component types must have an equality operator. }*/

template <class A, class B, class C, class D> inline
bool operator!=(const four_tuple<A,B,C,D>& p, const four_tuple<A,B,C,D>& q)
{ return p.first() != q.first() || p.second() != q.second() ||
         p.third() != q.third() || p.fourth() != q.fourth();
}


template <class A, class B, class C, class D> inline
int compare(const four_tuple<A,B,C,D>& p, const four_tuple<A,B,C,D>& q)
{ int s = compare(p.first(),q.first());
  if (s != 0) return s;
  s = compare(p.second(),q.second());
  if ( s!=0 ) return s;
  s = compare(p.third(),q.third());
  if ( s != 0 ) return s;
  return compare(p.fourth(),q.fourth());
}
/*{\Mfunc lexicographic ordering for |four_tuples|. Each of the 
component types must have a compare function. }*/


template <class A, class B, class C, class D> inline
int Hash(const four_tuple<A,B,C,D>& p)
{ return Hash(p.first()) & Hash(p.second()) & 
         Hash(p.third()) & Hash(p.fourth());
}
/*{\Mfunc hash function for |four_tuples|. Each of the 
component types must have a Hash function.}*/


/*{\Mimplementation 
The obvious implementation is used.
}*/

/*{\Mexample

We customize |four_tuples| and define a |h_array| for them.

\begin{verbatim}
#define prio() first()
#define inf()  second()
#define pq_item() third()
#define part_item() fourth()
typedef four_tuple<int,int,int,int> my_qu;

my_qu q; 
my_qu q1(2,2,0,0);
q.prio() = 5;

h_array<my_qu,int> M;
M[my_qu(2,2,nil,nil)] = 5;
\end{verbatim}

}*/



#if LEDA_ROOT_INCL_ID == 600030
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
