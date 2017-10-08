
#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>
#include <LEDA/system/timer.h>
#include <iostream>

#include <fstream>


using namespace leda;
using std::cout; using std::endl;
using std::ofstream;

void min_enclosing_circle(list<point> *L, circle *C);
void min_enclosing_circle_brute(list<point> *L, circle *C);
void min_enclosing_circle_through_points(list<point> *L, circle *C, list<point> *I, list_item end_it);


int main() 
{
    timer t;

    circle C;

    int N_welzl[7] = {10, 100, 1000, 10000, 50000, 1000000, 5000000};
    int n;
    for (int i = 0; i < 7; i++) {
        n = 1000;
        t.reset();
        t.start();
        while (n--) {
            list<point> L; random_points_in_unit_square(N_welzl[i], 100000, L);

            min_enclosing_circle(&L, &C);
        }

        cout << N_welzl[i] << "\t";
        t.stop();
        cout << t << endl;
    }

    /*cout << endl;*/

    /*int N_brute[7] = {5, 10, 20, 40, 80, 100};*/
    /*for (int i = 0; i < 6; i++) {*/
        /*cout << N_brute[i] << "\t";*/

        /*list<point> L; random_points_in_unit_square(N_welzl[i], 1000, L);*/

        /*t.reset();*/
        /*t.start();*/
        /*min_enclosing_circle_brute(&L, C);*/
        /*t.stop();*/
        /*cout << t << endl;*/
    /*}*/
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

    point p, pt;
    for (list_item it = L->first(); it != end_it; it = L->succ(it)) {
        p = L->contents(it);
        if (!(*C).inside(p)) {
            I->push(p);
            min_enclosing_circle_through_points(L, C, I, it);
            I->pop();
        }
    }
}


void min_enclosing_circle_brute(list <point> *L, circle *C) {
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
