#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>
#include <iostream>


using namespace leda;
using std::cout; using std::endl;


void min_enclosing_circle(list<point> *L, circle *C);


int main() 
{
    list<point> L; random_points_in_unit_square(10, 100, L);

    circle C;
    min_enclosing_circle(&L, &C);

    cout << C.center() << endl
         << C.radius() << endl;
}


void min_enclosing_circle(list <point> *L, circle *C) {
    point p, p1, p2;
    bool in;
    list_item i, j, k;

    *C = circle(0.0, 0.0, 1024);
    circle C_t;

    for (i = L->first(); i; i = L->succ(i)) {
        for (j = L->first(); j != i; j = L->succ(j)) {
            for (k = L->first(); k != j; k = L->succ(k)) {
                if (collinear(L->contents(i), L->contents(j), L->contents(k)))
                    continue;

                C_t = circle(L->contents(i), L->contents(j), L->contents(k));
                
                in = true;
                forall(p, (*L)) {
                    if (C_t.outside(p)) {
                        in = false;
                        break;
                    }
                }

                if (in && C_t.radius() < (*C).radius()) {
                    *C = C_t;
                }
            }

            p1 = L->contents(i);
            p2 = L->contents(j);

            C_t = circle((p1.xcoord() + p2.xcoord()) / 2.0, (p1.ycoord() + p2.ycoord()) / 2.0, p1.distance(p2) / 2.0);
            
            in = true;
            forall(p, (*L)) {
                if (C_t.outside(p)) {
                    in = false;
                    break;
                }
            }

            if (in && C_t.radius() < (*C).radius()) {
                *C = C_t;
            }
        }
    }
}
