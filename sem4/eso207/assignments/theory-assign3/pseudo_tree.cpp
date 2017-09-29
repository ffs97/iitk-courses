#include<iostream>
#include<vector>
#include<stdio.h>

using namespace std;

typedef vector<int> vec;
typedef vector< vector<int> > vvec;
typedef long long int ll;

#define fori(a, b) for( int i=a ; i<b ; i++ )
#define num 100005
#define pb push_back

int n, m;
bool val[num], vis[num], par[num];
vvec rel;

bool isRootedTree( int vertex, int parent){
	
	bool p = true;

	vis[vertex] = true;

	par[vertex] = parent;

	vec::iterator it;

	for( it = rel[vertex].begin() ; it != rel[vertex].end() ; it++ ){

		if( vis[ (*it) ] == true ){
			if( par[ (*it) ] == parent ){
				cout<<(*it)<<"\n";
				return false;
			}
		}
		else{
			p = p & isRootedTree( (*it), parent);
		}

	}

	// val[vertex] = p;

	return p;

}

int main(){

	cin>>n>>m;

	int u, v;
	bool x;

	rel.resize(n+2);

	fori(0, m){
		cin>>u>>v;
		rel[u].pb( v );
	}

	fori(0, n){
		vis[i] = false;
		par[i] = i;
	}

	fori(0, n){

		if( vis[i] == true ){
			continue;
		}

		x = isRootedTree(i, i);

		if( x == false ){
			cout<<"NO\n";
			return 0;
		}

	}

	cout<<"YES\n";

	return 0;
}