#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector < bool > visited;
vector < bool > direction;

vector < int > connected_component;

vector < vector <int> > adj_forward_list;
vector < vector <int> > adj_backward_list;

bool find_invalid_cycle(int i, int p, int count_high_points, bool forward) {
	visited[i] = 1;

	bool ret = false;

	direction[i] = 1;
	for (auto j = adj_forward_list[i].begin(); j != adj_forward_list[i].end(); j++) {
		if (*j != p) {
			if (visited[*j] && count_high_points <= 1) {
				return true;
			}
			else {
				if (i != p && !forward) {
					if (find_invalid_cycle(*j, i, count_high_points+1, 1)) {
						return true;
					}
				}
				else {
					if (find_invalid_cycle(*j, i, count_high_points, 1)) {
						return true;
					}
				}
			}
		}
	}

	direction[i] = 0;
	for (auto j = adj_backward_list[i].begin(); j != adj_backward_list[i].end(); j++) {
		if (*j != p) {
			if (visited[*j] && (count_high_points + direction[*j]) <= 1) {
				return true;
			}
			else {
				if (find_invalid_cycle(*j, i, count_high_points, 0)) {
					return true;
				}
			}
		}
	}

	visited[i] = 0;
	return false;
}

int main() {
	int n, m, v, u;
	cin >> n;

	adj_list.resize(n);
	visited.resize(n, 0);
	direction.resize(n, 0);

	cin >> m;
	for (int i = 0; i < m; i++) {
		cin >> v >> u;
		adj_list[v-1].push_back(u-1);
	}

	for (int i = 0; i < n; i++) {
		if (find_invalid_cycle(i, i, 0, 1)) {

		}
	}

	cout << "There is no cycle in this graph\n";	
}