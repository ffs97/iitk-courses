#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>

#include <iostream>
#include <limits>


using namespace leda;
using std::cout; using std::endl; using std::cin;


bool point_outside_circle(circle C, point p);
void min_enclosing_circle(list<point> *L, circle *C);


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
    min_enclosing_circle(&L, &C);

    cout << C.center() << endl
         << C.radius() << endl;
}


// Using this function as Circle::Outside does not work for trivial circles
bool point_outside_circle(circle C, point p) {
    // This allows for some error as using floating point numbers introduces some amount of error, which does not generate correct circles
    return (p.distance(C.center()) - C.radius()) > 1e-14;
}


void min_enclosing_circle(list <point> *L, circle *C) {
    point p, p1, p2;
    bool in;
    list_item i, j, k;

    *C = circle(0.0, 0.0, std::numeric_limits<float>::infinity());
    circle C_t;

    for (i = L->first(); i; i = L->succ(i)) {
        for (j = L->first(); j != i; j = L->succ(j)) {
            for (k = L->first(); k != j; k = L->succ(k)) {
                if (collinear(L->contents(i), L->contents(j), L->contents(k)))
                    continue;

                C_t = circle(L->contents(i), L->contents(j), L->contents(k));
                
                in = true;
                forall(p, (*L)) {
                    if (point_outside_circle(C_t, p)) {
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
                if (point_outside_circle(C_t, p)) {
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
