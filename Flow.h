#include <vector>

using namespace std;

struct Network{

	struct Edge {
		int from, to, cap, flow;

		Edge(int fr, int to, int cap, int fl = 0):
			from(fr),to(to),cap(cap),flow(fl){}
	};

	int n;
	int s, t;
	vector<Edge> edges;
	vector<vector<int>> a;
	vector<int> ptr;

	Network(int n, int s, int t):n(n),s(s),t(t){
		a.resize(n);
		ptr.resize(n);
	}

	void add_edge(int from, int to, int cap){
		int num = edges.size();
		edges.push_back(Edge(from, to, cap));
		edges.push_back(Edge(to, from, 0));
		a[from].push_back(num);
		a[to].push_back(num + 1);
	}

	int push(int v, int limit){
		if (v == t)
			return limit;
		if (limit == 0)
			return 0;
		int res = 0;
		while (ptr[v] < a[v].size() && limit){
			int pos = a[v][ptr[v]];
			int to = edges[pos].to;
			int lm = min(limit, edges[pos].cap - edges[pos].flow);
			ptr[v]++;
			int flow = push(to, lm);
			limit -= flow;
			res += flow;
			edges[pos].flow += flow;
			edges[pos ^ 1].flow -= flow;
		}
		return res;
	}

	int get_flow(){
		int res = 0;
		while (true){
			for (int i = 0; i < n; i++)
				ptr[i] = 0;
			int flow = push(s, INT_MAX / 10);
			if (flow == 0)
				break;
			res += flow;
		}
		return res;
	}

	void show_flow() const {
		vector<vector<int>> out(n);
		for (int i = 0; i < edges.size(); i += 2){
			if (edges[i].flow == 0)
				continue;
			int j = i;
			if (edges[i].flow < 0)
				j++;
			out[edges[j].from].push_back(j);
		}
		for (int i = 0; i < n; i++){
			for (int j : out[i]){
				cerr << i << " -> " << edges[j].to << " (" << edges[j].flow << "/" << edges[j].cap << ")\n";
			}
		}
	}
};

vector<int> find_matching(vector<vector<int>> a, int n, int m){
	Network network(n + m + 2, 0, n + m + 1);
	for (int i = 0; i < n; i++)
		network.add_edge(0, i + 1, 1);
	for (int i = 0; i < m; i++)
		network.add_edge(i + n + 1, n + m + 1, 1);
	for (int i = 0; i < n; i++){
		for (int j : a[i]){
			network.add_edge(i + 1, j + n + 1, 1);
		}
	}

	int sz = network.get_flow();
	vector<int> res(n);
	for (int i = 0; i < n; i++){
		int j = 0;
		while (j < network.a[i + 1].size() && network.edges[network.a[i + 1][j]].flow < 1)
			j++;
		if (j == network.a[i + 1].size())
			res[i] = -1;
		else
			res[i] = network.edges[network.a[i + 1][j]].to - n - 1;
	}

	// network.show_flow();

	return res;
}

void test_perfect_matching(int n){
	vector<int> perm(n);
	for (int i = 0; i < n; i++)
		perm[i] = i;
	random_shuffle(perm.begin(), perm.end());
	vector<vector<int>> a(n);
	for (int i = 0; i < n; i++)
		a[i].push_back(perm[i]);
	int m = min(100000ll, 1ll * n * n / 2) - n;
	for (int i = 0; i < n; i++){
		int j = min(m, m / (n - i)/* + rand() % 100 - 50*/);
		if (j < 0)
			j = 0;
		m -= j;
		while (j--){
			a[i].push_back(rand() % n);
		}
	}
	for (int i = 0; i < n; i++){
		sort(a[i].begin(), a[i].end());
		a[i].resize(unique(a[i].begin(), a[i].end()) - a[i].begin());
		random_shuffle(a[i].begin(), a[i].end());
	}

	// for (int i = 0; i < n; i++){
	// 	cerr << i << ":";
	// 	for (auto x : a[i])
	// 		cerr << " " << x;
	// 	cerr << "\n";
	// }

	cerr << "n = " << n << ": ";
	clock_t start = clock();
	vector<int> res = find_matching(a, n, n);
	clock_t finish = clock();

	// for (auto x : res)
	// 	cerr << x << " ";
	// cerr << "\n";

	for (int i = 0; i < n; i++){
		if (res[i] == -1){
			cerr << "incorrect -- matching is not perfect\n";
			return;
		}
		int j = 0;
		while (j < a[i].size() && a[i][j] != res[i])
			j++;
		if (j == a[i].size()){
			cerr << "incorrect -- an edge does not exist\n";
			return;
		}
	}

	sort(res.begin(), res.end());
	for (int i = 0; i < n; i++){
		if (res[i] != i){
			cerr << "incorrect -- neighbors are not unique\n";
			return;
		}
	}

	cerr << setprecision(6) << fixed;
	cerr << "correct for " << 1.0 * (finish - start) / CLOCKS_PER_SEC << " secs.\n";
}

void check_perfect_matching(){
	for (int n = 1; n <= 100000; n *= 10){
		test_perfect_matching(n);
	}
	// test_perfect_matching(12);
}