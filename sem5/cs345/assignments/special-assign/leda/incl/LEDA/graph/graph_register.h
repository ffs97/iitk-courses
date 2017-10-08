/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_register.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




LEDA_BEGIN_NAMESPACE

template<class graph_t>
class g_register {

typedef graph_map<graph_t>* map_ptr;

list<map_ptr> map_list[3];  //list of registered maps
list<int>     free_data[3]; //list of unused data slots


leda_lock reg_mutex1;
leda_lock reg_mutex2;

public:

g_register(int n0, int ns, int e0, int es)
{ int i;
  for(i=0; i<ns; i++) free_data[0].append(n0+i);
  for(i=0; i<es; i++) free_data[1].append(e0+i);
}

const list<map_ptr>& get_map_list(int i) const { return map_list[i]; }

list_item register_map(map_ptr M, int& slot_index, bool use_slot=false)
{ 
  int k = M->get_kind();

  reg_mutex1.lock();
  list_item it = map_list[k].push(M);
  reg_mutex1.unlock();

  if (!use_slot) 
  { slot_index = -1;
    return it;
   }

  if (!free_data[k].empty()) 
  { reg_mutex2.lock();
    slot_index  = free_data[k].pop(); 
    reg_mutex2.unlock();
    return it;
   }

  slot_index = -1;
  return it;
}


void unregister_map(map_ptr M)
{
  int k = M->get_kind();

  if (!map_list[k].empty())
  { 
    reg_mutex1.lock();
    map_list[k].del_item(M->get_loc());
    reg_mutex1.unlock();

    if (M->slot_index() > 0) 
    { reg_mutex2.lock();
      free_data[k].push(M->slot_index());
      reg_mutex2.unlock();
     }
   }
}


};


LEDA_END_NAMESPACE


