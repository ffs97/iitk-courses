#include <LEDA/geo/geo_alg.h>

using namespace leda;

int main(int argc, char** argv)
{
  int seg_num;
  if (argc < 2)
     seg_num = read_int("#segments = ");
  else
     seg_num = atoi(argv[1]);

/*
  for (int i = 0; i < seg_num; i++)
  { leda::rat_point p, q;
    leda::random_point_in_unit_square(p);
    leda::random_point_in_unit_square(q);
    if (p != q) std::cout << leda::rat_segment(p, q) << std::endl;
  }
*/
      int k = 10;
      integer size = integer(1) << k;

      for (int i = 0; i < seg_num; i++)
      { integer x1 = integer::random(k)-size;
        integer y1 = integer::random(k)-size;
        integer w1 = 1;
        integer x2 = integer::random(k)-size;
        integer y2 = integer::random(k)-size;
        integer w2 = 1;
        rat_segment s(rat_point(x1,y1,w1),rat_point(x2,y2,w2));
        std::cout << s << std::endl;
       }

  return 0;
}
