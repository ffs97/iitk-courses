#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>
#include <iostream>


using namespace leda;
using std::cout; using std::endl;


void min_enclosing_circle(list<point> *L, circle *C);
void min_enclosing_circle_through_points(list<point> *L, circle *C, list<point> *I, list_item end_it);


int main() 
{
    list<point> L; random_points_in_unit_square(1000, 1000, L);
    
    circle C;
    min_enclosing_circle(&L, &C);

    cout << C.center() << endl
         << C.radius() << endl;
}


void min_enclosing_circle(list<point> *L, circle *C) {
    if (L->length() == 0) {
        return;
    }

    list_item it = L->first();
    point p = L->contents(it);

    *C = circle(p);

    list <point> I;

    for (it = L->succ(it); it; it = L->succ(it)) {
        p = L->contents(it);
        if (!(*C).inside(p)) {
            I.push(p);
            min_enclosing_circle_through_points(L, C, &I, it);
            I.pop();
        }
    }
    
    return;
}


void min_enclosing_circle_through_points(list <point> *L, circle *C, list<point> *I, list_item end_it) {
    switch (I->length()) {
        case 3:
            {
            *C = circle(I->contents(I->get_item(0)), I->contents(I->get_item(1)), I->contents(I->get_item(2)));
            return;
            } break;
        case 2:
            {
            point p1 = I->contents(I->get_item(0));
            point p2 = I->contents(I->get_item(1));

            *C = circle((p1.xcoord() + p2.xcoord()) / 2.0, (p1.ycoord() + p2.ycoord()) / 2.0, p1.distance(p2) / 2.0);

            } break;
        case 1:
            {
            *C = circle(I->contents(I->first()));
            } break;
        case 0:
            {
            return;
            } break;
    }

    point p;
    for (list_item it = L->first(); it != end_it; it = L->succ(it)) {
        p = L->contents(it);
        if (!(*C).inside(p)) {
            I->push(p);
            min_enclosing_circle_through_points(L, C, I, it);
            I->pop();
        }
    }
}
