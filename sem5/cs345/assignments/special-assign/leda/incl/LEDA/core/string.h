/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  string.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>

#ifndef LEDA_STRING_H
#define LEDA_STRING_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600040
#include <LEDA/internal/PREAMBLE.h>
#endif

LEDA_BEGIN_NAMESPACE

class string;

//LEDA_BEGIN_NAMESPACE

class __exportC string_rep : public handle_rep {

      char*   s;
    //int     len;
      size_t  len;

 string_rep(const char*);
 string_rep(const char*,const char*); //concat
 string_rep(char);

public:

~string_rep() 
 { delete[] s; 
   //std_memory.deallocate_bytes(s,len);
  }

friend class __exportC string;
};




/*{\Manpage {string} {} {Strings} {s}}*/

class __exportC string  : public HANDLE_BASE(string_rep)
{

/*{\Mdefinition An instance $s$ of the data type $string$ is a
sequence of characters (type $char$).  The number of characters in the
sequence is called the length of $s$. A string of length zero is
called the empty string.  Strings can be used wherever a \CC{}
|const char*| string can be used. 

$Strings$ differ from the \CC{} type $char*$ in several aspects:
parameter passing by value and assignment works properly (i.e., the
value is passed or assigned and not a pointer to the value) and
$strings$ offer many additional operations.  }*/

 friend class __exportC string_rep;
 friend class __exportC window;

 static char* str_dup(const char*);
 static char* str_cat(const char*,const char*);
 static char* str_ncat(int, char**);

 string(string_rep* rep) { PTR = rep; }  // for private use only

 string_rep* ptr() const { return (string_rep*)PTR; }
public:

/*{\Mtypes 5}*/ 

 typedef int size_type;
/*{\Mtypemember the size type.}*/ 

 static int cmp(const char*, const char*);

/*{\Mcreation s }*/

string();
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized 
            with the empty string.}*/


string(const char* p);
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized 
            with a copy of the \CC{} string $p$.}*/

explicit // sn (november 2002)

string(char c);
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized 
            with the one-character string ``$c$''.}*/


string(int argc, char** argv);


// string(const char*, ...); // printf-like constructor
//
// That's what we want, but then (ARM page 326) a call string("xyz") is
// ambiguous. We first tried to use a dummy class "format_string" with
// a format_string(const char*) constructor to resolve  the  ambiguity:
// string(format_string, ...);
// However, only g++ seems to be able to handle the case where the 
// first argument is a class object (like format_string). For this reason
// we now provide a version of the string constructor for every possible 
// second argument (this seems to work with all compilers).

 string(const char*, char, ...);
 string(const char*, unsigned char, ...);
 string(const char*, short,...);
 string(const char*, unsigned short, ...);
 string(const char*, int, ...);
 string(const char*, unsigned int, ...);
 string(const char*, long, ...);
 string(const char*, unsigned long, ...);
 string(const char*, float, ...);
 string(const char*, double, ...);
 string(const char*, const char*, ...);
 string(const char*, void*, ...);

/*{\Moptions nextwarning=no}*/
/* manual:
string(const char* format, ...); 
*/
/*{\Mcreate introduces a variable $s$ of type $string$. $s$ is initialized 
            with the string produced by printf($format$,\dots). }*/



 string(const string& x) : HANDLE_BASE(string_rep)(x)  {}
~string() {}

string& operator=(const string& x) 
{ HANDLE_BASE(string_rep)::operator=(x); return *this; }


/*{\Moperations 2. 4.7 }*/

// conversion
const char* operator~() const { return ptr()->s; }
operator const char*()  const { return ptr()->s; }
char* cstring()         const { return ptr()->s; }
const char* c_str()     const { return ptr()->s; }

int length() const  { return (int)ptr()->len; }
/*{\Mop        returns the length of string $s$.}*/

bool empty() const  { return length() == 0; }
/*{\Mop        returns whether |\Mvar| is the empty string.}*/

char  get(int i) const  { return ptr()->s[i]; }
/*{\Mop     returns the character at position $i$.\\ 
	    \precond{$0 \le i \le s$.length()-1.} }*/

char  operator[](int i) const { return ptr()->s[i]; }

char& operator[](int i)
{ if (refs() > 1) *this = cstring();    //disconnect
  return ptr()->s[i];
}
/*{\Marrop     returns a reference to the character at position $i$.\\ 
	       \precond{$0 \le i \le s$.length()-1.} }*/


string operator()(int i, int j)  const { return sub(i,j); }
/*{\Mfunop    returns the substring of $s$ starting at 
	      position $\max(0,i)$ and ending at 
              position $\min(j,s$.length()$-1)$.\\
              If $\min(j,s$.length()$-1) < \max(0,i)$
              then the empty string is returned.  
	     }*/

string head(int i)               const { return sub(0,i-1); }
/*{\Mop       returns the first $i$ characters of $s$. }*/

string tail(int i)               const { return sub(length()-i,length()-1); }
/*{\Mop       returns the last $i$ characters of $s$. }*/

string sub(int,int)      const;



int pos(string s1, int i) const;
/*{\Mop       returns the minimum $j$ such that $j \ge i$ and $s_1$
              is a substring of $s$ starting at position $j$
	      (returns -1 if no such $j$ exists).}*/

int pos(const string& s1) const { return pos(s1,0); }
/*{\Mop       returns pos$(s1,0)$.}*/

int pos(char c, int i) const { return pos(string(c),i); }
int pos(char c) const { return pos(string(c)); }

/*{\Moptions nextwarning=no}*/
bool contains(const string& s1) const { return pos(s1,0) >= 0; }
/*{\Mop       true iff $s1$ is a substring of |\Mvar|.}*/

/*{\Moptions nextwarning=no}*/
string insert(string s1, int i =0)     const;
string insert(int i, string s1)       const;
/*{\Mop       returns $s(0,i-1)$ + $s_1$ + $s(i,s$.length()$-1)$.}*/


string insert(char c, int i=0) const { return insert(string(c),i); }
string insert(int i, char c)   const { return insert(i,string(c)); }



string replace(const string& s1, const string& s2, int i=1) const;
/*{\Mopl      returns the string created from $s$ by replacing 
	      the $i$-th occurrence of $s_1$ in $s$ by $s_2$. \\
		  Remark: The occurences of $s_1$ in $s$ are counted in a 
		  non-overlapping manner, for instance the string $sasas$ 
		  contains only one occurence of the string $sas$.}*/

string replace(int i, int j , const string& s1) const;
/*{\Mopl     returns the string created from $s$ by replacing 
	      $s(i,j)$ by $s_1$.\\ \precond{$i \leq j$.} }*/

string replace(int i, const string& s1) const { return replace(i,i,s1);  }
/*{\Mopl      returns the string created from $s$ by replacing
	      $s[i]$ by $s_1$. }*/

string replace_all(const string& s1, const string& s2) const 
{ return replace(s1,s2,0); }
/*{\Mopl      returns the string created from $s$ by replacing  
	      all occurrences of $s_1$ in $s$ by $s_2$.\\
              \precond{The occurrences of $s_1$ in $s$ 
               do not overlap (it's hard to say what the function returns
               if the precondition is violated.).} }*/

string replace(char c, const string& s2, int i=1) const
{ return replace(string(c),s2,i); }

string replace_all(char c, const string& s2) const 
{ return replace(c,s2,0); }


string to_lower() const;
/*\Mop        returns a low-character copy of |\Mvar|. }*/ 

string utf_to_iso8859() const;
/*\Mop        returns an iso8859 encoded copy of |\Mvar|. }*/ 

string iso8859_to_utf() const;
/*\Mop        returns an utf encoded copy of |\Mvar|. }*/ 

string del(const string& s1 , int i=1) const;
/*{\Mopl       returns $s$.replace($s_1,"",i$). }*/

string del(int i , int j) const;
/*{\Mop       returns $s$.replace($i,j,""$). }*/

string del(int i) const  { return del(i,i); }
/*{\Mop       returns $s$.replace($i,""$). }*/

string del_all(const string& s1) const  { return del(s1,0); }
/*{\Mop       returns $s$.replace\_all($s_1,""$). }*/

string trim() const;
/*/Mop        returns a copy of |\Mvar| where all leading and trailing 
              whitespace is removed. }*/ 


void   read(istream& I, char delim = ' ');
/*{\Mopl      reads characters from input stream $I$ into $s$ 
              until the first occurrence of character $delim$.
			  (If |delim| is '$\backslash$ $n$' it is extracted 
			  from the stream, otherwise it remains there.)}*/

void   read(char delim = ' ')           { read(cin,delim); }
/*{\Mop       same as $s$.read($cin$,$delim$). }*/

void   read_line(istream& I)    { read(I,'\n'); }
/*{\Mop       same as $s$.read($I$,'$\backslash$ $n$'). }*/

void   read_line() { read_line(cin); }
/*{\Mop       same as $s$.read\_line($cin$). }*/


void   read_file(istream& I)    { read(I,(char)EOF); }
/*{\Mop       same as $s$.read($I$,'EOF'). }*/

void   read_file() { read_file(cin); }
/*{\Mop       same as $s$.read\_file($cin$). }*/


string format(string) const;

string& operator+=(const string& x);
/*{\Mbinop     appends $x$  to $s$ and returns a reference to $s$.}*/

string& operator+=(char c) { return operator+=(string(c)); }



friend string operator+(const string& x ,const string& y)
{ string s=x; s+=y; return s; }
/*{\Mbinopfunc     returns the concatenation of $x$ and $y$.}*/


friend string operator+(const string& x, char c)
{ string s=x; s+=c; return s; }



friend bool operator==(const string& x, const string& y)
{ return string::cmp(x.cstring(),y.cstring())==0; }
/*{\Mbinopfunc     true iff $x$ and $y$ are equal.}*/

friend bool operator==(const string& x, const char* y)
{ return string::cmp(x.cstring(),y)==0; }

friend bool operator==(const char* x, const string& y)
{ return string::cmp(x,y.cstring())==0; }



friend bool operator!=(const string& x, const string& y)
{ return string::cmp(x.cstring(),y.cstring())!=0; }
/*{\Mbinopfunc     true iff $x$ and $y$ are not equal.}*/

friend bool operator!=(const string& x, const char* y)
{ return string::cmp(x.cstring(),y)!=0; }

friend bool operator!=(const char* x, const string& y)
{ return string::cmp(x,y.cstring())!=0; }



friend bool operator< (const string& x, const string& y)
{ return string::cmp(x.cstring(),y.cstring())<0; }
/*{\Mbinopfunc     true iff $x$ is lexicographically smaller than $y$.}*/

friend bool operator> (const string& x, const string& y)
{ return string::cmp(x.cstring(),y.cstring())>0; }
/*{\Mbinopfunc     true iff $x$ is lexicographically greater than $y$.}*/

friend bool operator<=(const string& x, const string& y)
{ return string::cmp(x.cstring(),y.cstring())<=0; }
/*{\Mbinopfunc     returns $(x < y)\ \Lvert \ (x == y)$.}*/

friend bool operator>=(const string& x, const string& y)
{ return string::cmp(x.cstring(),y.cstring())>=0; }
/*{\Mbinopfunc     returns $(x > y)\ \Lvert \ (x == y)$.}*/


friend __exportF istream& operator>>(istream& I, string& s);
/*{\Mbinopfunc     same as $s$.read($I$,' '). }*/

friend __exportF ostream& operator<<(ostream& O, const string& s) ;
/*{\Mbinopfunc     writes string $s$ to the output stream $O$. }*/

};



// strings with a line by line input operator

class string_line : public string {
friend istream& operator>>(istream& is, string_line& s) 
{ s.read_line(is); return is; }
};



/*
LEDA_END_NAMESPACE

#if defined(LEDA_NAMESPACE)
typedef leda::string     string;
typedef leda::string_rep string_rep;
#endif
*/


COMPARE_DECL_PREFIX
inline int  DEFAULT_COMPARE(const string& x, const string& y) 
{ return string::cmp(x.cstring(),y.cstring()); }

unsigned long ID_Number(const string& x);

inline const char* leda_tname(const string*) { return "string"; }

inline int Hash(const string& s)
{
    int size = s.length();
    int step = size/10;
    ++step;
    int i=0,j;
    for(j=0;j<size;j+=step)
        i = 131*i + s[j];
    
    return i;
}
        
  


/*{\Mimplementation 
Strings are implemented by \CC{} character vectors. All operations involving
the search for a pattern $s1$ in a string $s$ take time $O(s.lenght() * s1.length())$, $[\ ]$ takes constant time and all other operations on a 
string $s$ take time $O(s.length())$.}*/



/*
// stream output into strings

template <class T>
string& operator<<(string& str, const T& x)
{ ostringstream os;
  os << x << ends;
  return str += os.str();
 }
*/



#if LEDA_ROOT_INCL_ID == 600040
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
