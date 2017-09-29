#include <iostream>
#include <vector>

using namespace std;

vector < vector <int> > adj_list;
vector < bool > visited;

void DFS(int i) {
	visited[i] = 1;
	cout << i+1 << " ";
	for (auto j = adj_list[i].cbegin(); j != adj_list[i].cend(); j++) {
		if (!visited[*j]) {
			DFS(*j);
		}
	}
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

	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			cout << "\n";
			DFS(i);
		}
	}
}