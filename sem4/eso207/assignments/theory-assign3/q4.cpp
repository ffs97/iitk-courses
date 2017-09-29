#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector < bool > visited;
vector < bool > rooted;
vector < int > connected_component;
vector < vector <int> > adj_list;

bool is_acyclic(int i) {
	visited[i] = 1;
	connected_component.push_back(i);

	for (auto j = adj_list[i].begin(); j != adj_list[i].end(); j++) {
		if (visited[*j]) {
			return false;
		}
		else {
			if (!is_acyclic(*j)) return false;
		}
	}

	rooted[i] = 1;
	return true;
}

int main() {
	int n, m, v, u;
	cin >> n;

	adj_list.resize(n);
	visited.resize(n, 0);
	rooted.resize(n, 0);

	cin >> m;
	for (int i = 0; i < m; i++) {
		cin >> v >> u;
		adj_list[v-1].push_back(u-1);
	}

	for (int i = 0; i < n; i++) {
		if (!rooted[i]) {
			if (!is_acyclic(i)) {
				cout << "There is a cycle in this graph\n";
				return 0;
			}

			for (auto j = connected_component.begin(); j != connected_component.end(); j++) {
				visited[*j] = 0;
			}
			connected_component.clear();
		}
	}

	cout << "There is no cycle in this graph\n";	
}