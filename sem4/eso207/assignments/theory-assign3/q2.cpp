#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector < bool > visited;
vector < int > connected_component;
int digits_count[10] = {0};

vector < vector <int> > adj_list;
vector < int > digits;

void DFS(int i) {
	visited[i] = 1;
	digits_count[digits[i]]++;
	connected_component.push_back(i);

	for (auto j = adj_list[i].begin(); j != adj_list[i].end(); j++) {
		if (!visited[*j]) {
			DFS(*j);
		}
	}
}

int main() {
	int n, m, v, u;
	char d;
	cin >> n;

	for (int i = 0; i < n; i++) {
		cin >> d;
		digits.push_back(d - '0');
	}

	adj_list.resize(n);
	visited.resize(n, 0);

	cin >> m;
	for (int i = 0; i < m; i++) {
		cin >> v >> u;
		adj_list[v-1].push_back(u-1);
		adj_list[u-1].push_back(v-1);
	}

	int index;
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			DFS(i);

			sort(connected_component.begin(), connected_component.end());

			index = 9;
			for (auto j = connected_component.begin(); j != connected_component.end(); j++) {
				while (!digits_count[index]) {
					index--;
				}
				digits[*j] = index;
				digits_count[index]--;
			}

			connected_component.clear();
		}
	}

	for (int i = 0; i < n; i++) {
		cout << digits[i];
	}
}