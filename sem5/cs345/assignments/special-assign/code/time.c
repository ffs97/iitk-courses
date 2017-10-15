#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>
#include <LEDA/system/timer.h>

#include <iostream>
#include <fstream>
#include <limits>


using namespace leda;
using std::cout; using std::endl; using std::cin;
using std::ofstream;


bool point_outside_circle(circle C, point p);
void min_enclosing_circle_welzl(list<point> *L, circle *C, list<point> *I, list_item end_it);
void min_enclosing_circle_brute(list<point> *L, circle *C);


int main() {
    timer t;

    ofstream welzl_file, brute_file;
    welzl_file.open("welzl.dat");
    brute_file.open("brute.dat");

    circle C;

    cout << "Using Welzl Algorithm:" << endl << endl;
    cout << "# Points\tTime" << endl << endl;

    int N_welzl[] = {1000, 10000, 50000, 75000, 100000, 250000, 375000, 500000, 625000, 750000, 1000000};
    int n;
    double time;

    for (int i = 0; i < *(&N_welzl + 1) - N_welzl; i++) {
        n = 100;

        t.reset();
        t.start();

        time = 0.0;

        welzl_file << N_welzl[i] << "\t";

        while (n--) {
            list<point> L; random_points_in_unit_square(N_welzl[i], 100000, L);
            list <point> I;

            t.reset();
            t.start();
            min_enclosing_circle_welzl(&L, &C, &I, NULL);
            t.stop();

            welzl_file << t.elapsed_time() << "\t";

            time += t.elapsed_time();
        }

        welzl_file << endl;

        t.stop();
        cout << N_welzl[i] << "\t\t";
        cout << time * 10 << "ms" << endl;
    }

    cout << endl << endl;

    cout << "Using Brute Force:" << endl << endl;
    cout << "# Points\tTime" << endl << endl;

    int N_brute[] = {50, 100, 200, 500, 625, 750, 875, 1000, 1125, 1250};
    for (int i = 0; i < *(&N_brute + 1) - N_brute; i++) {
        list<point> L; random_points_in_unit_square(N_brute[i], 10000, L);

        t.reset();
        t.start();
        min_enclosing_circle_brute(&L, &C);
        t.stop();

        brute_file << N_brute[i] << "\t" << t.elapsed_time() << endl;

        cout << N_brute[i] << "\t\t";
        cout << t.elapsed_time() << "s" << endl;
    }

    welzl_file.close();
    brute_file.close();
}


// Using this function as Circle::Outside does not work for trivial circles
bool point_outside_circle(circle C, point p) {
    return (p.distance(C.center()) - C.radius()) > 1e-14;
}


void min_enclosing_circle_welzl(list <point> *L, circle *C, list<point> *I, list_item end_it) {
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
            min_enclosing_circle_welzl(L, C, I, it);
            I->pop();
        }
    }
}


void min_enclosing_circle_brute(list <point> *L, circle *C) {
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
