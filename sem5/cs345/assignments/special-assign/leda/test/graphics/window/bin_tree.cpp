/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bin_tree.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/graphwin.h>
#include <LEDA/core/map.h>

#include <LEDA/core/impl/bin_tree.h>
#include <LEDA/core/impl/avl_tree.h>
#include <LEDA/core/impl/bb_tree.h>
#include <LEDA/core/impl/rb_tree.h>
#include <LEDA/core/impl/rs_tree.h>

using namespace leda;

enum { BIN_TREE, AVL_TREE, BB_TREE, RB_TREE, RS_TREE };


static window W(540,600);

static map<bin_tree_node*,node> NODE;
static GraphWin* gwp;

static int kind = BIN_TREE;  // bin_tree

static bin_tree* T;



node build_graph(graph& G, const bin_tree& T, bin_tree_node* r) 
{ node v  = G.new_node();
  NODE[r] = v;
  bin_tree_node* left  = T.l_child(r);
  bin_tree_node* right = T.r_child(r);
  if (left)  G.new_edge(v,build_graph(G,T,left));
  if (right) G.new_edge(v,build_graph(G,T,right));
  return v;
 }

   

void gw_draw_node(bin_tree_node* it, double x, double y)
{ node v = NODE[it];
  gwp->set_position(v,point(x,y)); 
  if (it->is_leaf())
  { gwp->set_label_type(v,user_label); 
    gwp->set_user_label(v,string("%d",it->get_key())); 
    gwp->set_shape(v,rectangle_node); 
    gwp->set_width(v,20); 
    gwp->set_height(v,15); 
   }
}


void gw_draw_edge(double, double, double, double) {  }




void draw_node(bin_tree_node* v, double x, double y)
{ 
   if (v->is_leaf())
   { //W.draw_rectangle(x-1.4,y-1.4,x+1.4,y+1.4,black);
     W.draw_ctext(x,y,string("%d",v->get_key()),black); 
     return;
    }

  int bal = v->get_bal();

  switch (kind) {

    case  BIN_TREE:
             W.draw_filled_node(x,y, yellow);
             break;

    case  AVL_TREE:
             W.draw_filled_node(x,y, yellow);
             switch (bal) {
               case  0: W.draw_ctext(x,y,"=",blue);
                        break;
               case -1: W.draw_ctext(x,y,">",blue);
                        break;
               case  1: W.draw_ctext(x,y,"<",blue);
                        break;
              }
             break;

    case  RB_TREE: 
             W.draw_filled_node(x,y, (bal == 0) ? red : black);
             break;

    default: W.draw_filled_node(x,y,yellow);
             W.draw_ctext(x,y,string("%d",bal),blue); 
             break;
   }
}




void draw_edge(double x0, double y0, double x1, double y1)
{ W.draw_edge(point(x0,y0),point(x1,y1),blue); }


void draw_tree()
{ double dy = (W.ymax()-W.ymin())/10;
  T->draw(draw_node,draw_edge, W.xmin(),W.xmax(),W.ymax()-2*dy,dy);
}


int main()
{
  bin_tree* TREE[5];
  string    NAME[5];

  TREE[0] = new bin_tree;  NAME[0] = "BINARY TREE";
  TREE[1] = new avl_tree;  NAME[1] = "AVL TREE";
  TREE[2] = new bb_tree;  NAME[2] = "BB[ALPHA] TREE";
  TREE[3] = new rb_tree;   NAME[3] = "RED/BLACK TREE";
  TREE[4] = new rs_tree;   NAME[4] = "RANDOMIZED SEARCH TREE";


  panel P("BINARY TREES");

  int n = 20;
  int mode = 0;

  P.choice_item("TREE",kind,
                "bin_tree","avl_tree","bb_tree","rb_tree","rs_tree");

  P.choice_item("INPUT",mode,"random", "1 2 3 ...");

  P.int_item("# INSERTS",n,0,50);

  //W.set_node_width(10);
  //W.set_line_width(2);

  W.set_node_width(9);
  W.set_line_width(2);

  W.set_redraw(draw_tree);

  W.open();

  for(;;)
  {
    P.open(W);

    W.clear();
    W.message(NAME[kind]);

    T = TREE[kind];
  
    T->clear();
  
    int i;
  
    if (mode==0)
      for(i=0;i<n;i++) T->insert((void*)rand_int(0,99),0);
    else
      for(i=0;i<n;i++) T->insert((void*)i,0);
  
    draw_tree();

    graph G;
    build_graph(G,*T,T->root());

    GraphWin gw(G);
    gw.set_node_width(15);
    gw.set_node_height(15);
    gw.set_node_label_type(no_label);
    gw.set_node_label_font(roman_font,6);
    gw.set_edge_direction(undirected_edge);

    gwp = &gw;
    double dy = (gw.get_ymax()-gw.get_ymin())/10;
    T->draw(gw_draw_node,gw_draw_edge,gw.get_xmin(),gw.get_xmax(),
                                                    gw.get_ymax()-2*dy,dy);

    gw.display(0,0);
    //gw.fill_window();
    gw.edit();
  }
  
  return 0;
}
