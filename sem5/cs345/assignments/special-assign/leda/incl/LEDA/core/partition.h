/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  partition.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_Partition_H
#define LEDA_Partition_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600047
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// partition   (union find)
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {partition} {} {Partitions} }*/

class __exportC partition_node {
/*{\Mdefinition
An instance  $P$ of the data type |partition| consists of a finite set of
items (|partition_item|) and a partition of this set
into blocks.}*/

friend class __exportC partition;

partition_node* next;
partition_node* father;
int size;

public:

         partition_node() : father(0), size(1) {}
virtual ~partition_node() {}

LEDA_MEMORY(partition_node)

};


// a partition item is a pointer to a partition node:

typedef partition_node* partition_item;



class __exportC partition {

partition_item used_items;                 // List of used partition items

protected:
int block_count;

public:

/*{\Mcreation P }*/

partition() { used_items = 0; block_count = 0; }  
/*{\Mcreate creates an instance $P$ of type |partition| and initializes it to 
            the empty partition.}*/

virtual ~partition() { clear(); }

/*{\Moperations 3 4}*/


void add_item(partition_item p)
{ p->next = used_items;
  used_items = p;
}

partition_item make_block() 
{ block_count++;
  partition_item p = new partition_node;
  add_item(p);
  return p;
}
/*{\Mop      returns a new |partition_item| $it$ and adds
	     the block |{it}| to partition $P$.}*/

partition_item find(partition_item p) const;
/*{\Mopl     returns a canonical item of the block that
	     contains item $p$, i.e., iff |P.same_block(p,q)|
	     then |P.find(p)| and |P.find(q)| return the same item.\\
	     \precond $p$ is an item in $P$. }*/

int size(partition_item p) const
{ return find(p)->size; }
/*{\Mopl returns the size of the block containing $p$.}*/

int number_of_blocks() const { return block_count; }
/*{\Mopl returns the number of blocks in |P|.}*/

bool  same_block(partition_item p, partition_item q) const
{ return find(p) == find(q); }
/*{\Mopl      returns true if $p$ and $q$ belong to the same
	      block of partition $P$.\\
	      \precond $p$ and $q$ are items in $P$.}*/

void  union_blocks(partition_item p, partition_item q);
/*{\Mopl      unites the blocks of partition $P$ containing
	     items $p$ and $q$.\\
	     \precond $p$ and $q$ are items in $P$.}*/

void split(const list<partition_item>& L);
/*{\Mop     turns all items in $L$ to singleton blocks.\\
            \precond     $L$ is a union of blocks. }*/



void clear();                      // deletes all used items


partition_item first_item() const  { return used_items;  }
partition_item next_item(partition_item it) const  { return it ? it->next : 0; }

void reset(partition_item p);

};


/*{\Mimplementation
Partitions are implemented by the union find algorithm with weighted union
and path compression (cf.~\cite{T83}).  Any sequence of $n$ make\_block and 
$m \ge n$ other operations (except for |split|) takes time $O(m\ \alpha(m,n))$. 
The cost of a split is proportional to the size
of the blocks dismantled.  }*/


/*{\Mexample
    Spanning Tree Algorithms (cf. section \ref{Graph Algorithms}).}*/


/*{\Mtext \newpage}*/

//------------------------------------------------------------------------------
// Partition  (parameterized partitions)
//-----------------------------------------------------------------------------

template <class E>
class Partition : private partition {

/*{\Manpage {Partition} {E} {Parameterized Partitions} }*/

/*{\Mdefinition
An instance  $P$ of the data type |\Mname| consists of a finite set of
items (|partition_item|) and a partition of this set
into blocks. Each item has an associated information of type |E|.}*/


class Pnode : public partition_node {
friend class Partition<E>;
E inf;
Pnode(const E& i) : inf(i) {}
LEDA_MEMORY(Pnode)
};

public:

typedef Pnode* item;

/*{\Mcreation P }*/

Partition()  {}  
/*{\Mcreate creates an instance $P$ of type |\Mname| and initializes it to 
            the empty partition.}*/

~Partition() { clear(); }


/*{\Moperations 3 4}*/

partition_item make_block(const E& x) 
{ block_count++;
  Pnode* p = new Pnode(x);
  partition::add_item(p);
  return p;
}
/*{\Mop      returns a new |partition_item| $it$, adds
	     the block |{it}| to partition $P$, and associates |x| with |it|.}*/

partition_item find(partition_item p) const
{ return partition::find(p); }
/*{\Mopl     returns a canonical item of the block that
	     contains item $p$, i.e., iff |P.same_block(p,q)| 
	     then |P.find(p)| and |P.find(q)| return the same item.\\
	     \precond $p$ is an item in $P$. }*/

int size(partition_item p) const
{ return partition::size(p); }
/*{\Mopl returns the size of the block containing $p$.}*/


int number_of_blocks() const { return partition::number_of_blocks(); }
/*{\Mopl returns the number of blocks in |P|.}*/

bool  same_block(partition_item p, partition_item q) const
{ return partition::same_block(p,q); }
/*{\Mopl      returns true if $p$ and $q$ belong to the same
	      block of partition $P$.\\
	      \precond $p$ and $q$ are items in $P$.}*/

void  union_blocks(partition_item p, partition_item q)
{ partition::union_blocks(p,q); }
/*{\Mopl      unites the blocks of partition $P$ containing
	     items $p$ and $q$.\\
	     \precond $p$ and $q$ are items in $P$.}*/


void  split(const list<partition_item> & L) { partition::split(L); }
/*{\Mopl     turns all items in $L$ to singleton blocks. \\
	     \precond $L$ is a union of blocks}*/


const E& inf(partition_item it)  const { return (item(it))->inf; }
/*{\Mop      returns the information associated with |it|.}*/



void  change_inf(partition_item it, const E& x) { (item(it))->inf = x; }
/*{\Mop      changes the information associates with |it| to |x|.}*/

void clear() { partition::clear(); }


partition_item first_item() const  
{ return partition::first_item();  }

partition_item next_item(partition_item it) const
{ return partition::next_item(it);  }

void reset(partition_item it) { return partition::reset(it);  }
};



#if LEDA_ROOT_INCL_ID == 600047
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
