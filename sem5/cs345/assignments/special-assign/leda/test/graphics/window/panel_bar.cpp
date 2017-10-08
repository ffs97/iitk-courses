#include <LEDA/graphics/window.h>
#include <LEDA/graphics/bitmaps/button21.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif

static unsigned char* bm_bits [] = {
  triang_21_bits,
  voro_21_bits,
  empty_circle_21_bits,
  encl_circle_21_bits,
  grid_21_bits,
  hull_21_bits,
};


void file_handler(int) { }

void redraw(window* wp, double x0, double y0, double x1, double y1)
{
cout << string("redraw: %.2f %.2f %.2f %.2f",x0,y0,x1,y1);
cout << endl;

  wp->draw_vline(800,blue);
  wp->draw_vline(wp->pix_to_real(300));
}


int main()
{
  menu file_menu;
  file_menu.button("Load File",0,file_handler);
  file_menu.button("Save File",1,file_handler);
  file_menu.button("Load/Save",2, file_handler);
  file_menu.button("Exit",3);

  menu dummy_menu;
  dummy_menu.button("button1");
  dummy_menu.button("button2");
  dummy_menu.button("button3");

  window W(500,600);

  W.set_item_width(130);

  W.button("File", file_menu);
  W.button("Help", dummy_menu);
  W.button("Exit", 99);


  int x = 0;
  W.choice_item("bitmap",x,6,21,21,bm_bits);
  //W.choice_item("",x,"1","2","3","4","5");


  string str = "hallo";
  W.string_item("",str);

  W.make_menu_bar(3);

  W.set_clear_on_resize(false);

  W.set_redraw(redraw);

  W.init(0,1000,0);
  W.display(window::center,window::center);

  redraw(&W,0,1000,0,1000);

  W.draw_vline(800,blue);
  W.draw_vline(W.pix_to_real(300));

  W.read_mouse();

  return 0;

}
