#include <LEDA/geo/point.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/geo/circle.h>
#include <LEDA/core/list.h>
#include <LEDA/system/timer.h>

#include <iostream>
#include <fstream>
#include <limits>

using std::cout; using std::cin; using std::endl;
using namespace leda;

int main() {
    int n;
    cin >> n;

    list<point> L; random_points_in_unit_square(n, 100000, L);

    for (list_item it = L.first(); it; it = L.succ(it)) {
        cout << L.contents(it).xcoord() << " " << L.contents(it).ycoord() << endl;
    }
}
