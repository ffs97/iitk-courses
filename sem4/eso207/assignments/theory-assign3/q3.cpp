#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector < bool > visited;
vector < vector <int> > adj_list;

int max_path_length = 0;

int set_max_path_length(int i) {
	visited[i] = 1;

	int first_max = 0, second_max = 0, temp;

	for (auto j = adj_list[i].begin(); j != adj_list[i].end(); j++) {
		if (!visited[*j]) {
			temp = set_max_path_length(*j);
			if (temp > first_max) first_max = temp;
			else if (temp > second_max) second_max = temp;
		}
	}

	max_path_length = max(max_path_length, first_max + second_max + 1);
	return first_max + 1;
}

int main() {
	int n, m, v, u;
	cin >> n;

	adj_list.resize(n);
	visited.resize(n, 0);

	cin >> m;
	for (int i = 0; i < m; i++) {
		cin >> v >> u;
		adj_list[v-1].push_back(u-1);
		adj_list[u-1].push_back(v-1);
	}

	set_max_path_length(0);
	max_path_length = max_path_length - 1;
	cout << max_path_length;
}