/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dp_hash.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DP_HASH_H
#define LEDA_DP_HASH_H

//------------------------------------------------------------------------------
// Dynamic Perfect Hashing
//
// Michael Wenzel          ( 1990/91 )
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE


// ---------------------------------------------------------------
// declarations
// ---------------------------------------------------------------


class headertable;
class subtable;
class dp_hash;

typedef headertable* htp;
typedef subtable* stp;

#define EMPTY (GenPtr(2147483648UL))
#define maxuni 2147483647
                                // #define maxprim 2147483659
				// aber kleiner wegen random-Funktion
#define maxprim 2147483647
#define wursechs 2.44948974278




// --------------------------------------------------------------
// class headertable
//
// Items werden auf Headertables gehasht,
// die sie dann injektiv auf Subtables verteilen

class __exportC headertable {

  unsigned kj;                     // Multiplikator
  int wj;                          // Anzahl Elemente in Tafel
  int mj;                          // max. Anzahl Elemente
  stp tj;                          // Startpunkt der Subtables

  friend class __exportC b_dict;
  friend class __exportC dp_hash;
  friend class __exportC subtable;

  stp lookup(GenPtr );

  bool insert(GenPtr ,GenPtr ,stp& ,int& ,bool& ,stp ,stp );
  int dinsert(GenPtr ,GenPtr ,stp ,stp& ,stp& );

  bool del(GenPtr ,stp ,stp ); 
  int give_elements(stp& ,stp ,stp );  


  headertable()
    { 
      kj = wj = mj=0; 
      tj=0;   
     }

};

// --------------------------------------------------------------
// class subtable
//
// Jedes Subtableelement ist leer,
// oder von der Headertable wird genau ein Element    
// auf eine Position gehasht

class __exportC subtable {

  GenPtr key;
  GenPtr inf;

  friend class __exportC b_dict;
  friend class __exportC dp_hash;
  friend class __exportC headertable;

  void set_s(GenPtr a,GenPtr b)
    {
       key=a; inf=b; }

  void clear()
    { 
       key=EMPTY; inf=0; }

  subtable()
    {  
       key=EMPTY; inf=0; }

  subtable(GenPtr a ,GenPtr b )
    {  
       key=a; inf=b; }

  subtable(subtable& s)
    {  
       key=s.key;
       inf=s.inf; }

   subtable& operator=(subtable& s)
    {
       key=s.key;
       inf=s.inf;
       return *this; }

};

// ------------------------------------------------------------------
// class dp_hash
//
// alle Informationen fuer das Dictionary
// Elemente werden auf Headertables gehasht, 
// die dann die Elemente weitergeben
// Der Platzverbrauch der Headertables wird kontrolliert


class __exportC dp_hash {

  htp* htablep;      // Feld von Zeigern auf Headertables
		     // leere Headertables werden nicht angelegt

  unsigned k;        // Verteilungsfunktion (k*x)%p%sM

  int n;             // Anzahl Elemente im Dictionary

  int M;             // Anzahl Elemente, die momentan bzgl.
		     // Platzverbrauch mit Wahrscheinlichkeit >= 0.5
		     // abgespeichert werden koennen ( ohne Rehash )

  int sM;            // Anzahl Headertables

  int bed;           // Kontrolle des Platzverbrauchs

  stp anf,wo,ende;   // zur Speicherverwaltung beim Rehash



  stp  rehash_all(GenPtr=EMPTY ,GenPtr=0);


  virtual void clear_key(GenPtr&)const {}
  virtual void clear_inf(GenPtr&)const {}

  virtual void copy_key(GenPtr&) const {}
  virtual void copy_inf(GenPtr&) const {}

public:

  typedef stp item;

  stp first_item() const
  { LEDA_EXCEPTION(1,"sorry, dp_hash::first_item not implemented");
    return nil;
   }

  stp next_item(stp) const
  { LEDA_EXCEPTION(1,"sorry, dp_hash::next_item not implemented");
    return nil;
   }

  dp_hash& operator=(const dp_hash&) 
  { LEDA_EXCEPTION(1,"sorry, dp_hash::operator= not implemented");
    return *this;
   }

  GenPtr  key(stp it) const { return it->key; }
  GenPtr& inf(stp it) const { return it->inf; }

  stp  insert(GenPtr,GenPtr );
  void del(GenPtr);

  void del_item(stp it) { del(it->key); }

  stp  lookup(GenPtr ) const;

  stp  change_inf(GenPtr ,GenPtr );

  int  size() const { return n; }

  void clear();


  dp_hash();
  dp_hash(int ,GenPtr* ,GenPtr* );
  virtual ~dp_hash();


  friend class __exportC b_dict;

};

LEDA_END_NAMESPACE

#endif
