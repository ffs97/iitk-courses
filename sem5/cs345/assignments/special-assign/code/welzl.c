#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>

#include <iostream>
#include <limits>

using namespace leda;
using std::cout; using std::endl; using std::cin;


bool point_outside_circle(circle C, point p);
void min_enclosing_circle(list<point> *L, circle *C, list<point> *I, list_item end_it);


int main() {
    int n;
    double x, y;

    cin >> n;
    
    list <point> L;
    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        L.append(point(x, y));
    }

    circle C;
    list <point> I;
    min_enclosing_circle(&L, &C, &I, NULL);

    cout << C.center().xcoord() << ","
         << C.center().ycoord() << endl
         << C.radius() << endl;
}


// Using this function as Circle::Outside does not work for trivial circles
bool point_outside_circle(circle C, point p) {
    // This allows for some error as using floating point numbers introduces some amount of error, which does not generate correct circles
    return p.distance(C.center()) > C.radius() + C.radius() * 1e-14;
}


void min_enclosing_circle(list <point> *L, circle *C, list<point> *I, list_item end_it) {
    if (L->length() == 0) {
        return;
    }

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
            *C = circle(point(0.0, 0.0));
            } break;
    }

    point p;
    for (list_item it = L->first(); it != end_it; it = L->succ(it)) {
        p = L->contents(it);
        if (point_outside_circle(*C, p)) {
            I->push(p);
            min_enclosing_circle(L, C, I, it);
            I->pop();
        }
    }
}
