#include <bits/stdc++.h>

using namespace std;

inline int nxt(){
	int x;
	scanf("%d", &x);
	return x;
}

const int mod = 1000000007;

struct Tree{
	int n;
	vector<long long> a, b, sum;

	Tree(int sz = 0){
		n = 1;
		while (n < sz)
			n *= 2;
		a.resize(n + n);
		b.resize(n + n);
		sum.resize(n + n);
	}

	void relax(int v, int l, int r){
		if (v >= n)
			return;
		a[v + v] += a[v];
		a[v + v + 1] += a[v];
		b[v + v] += b[v];
		b[v + v + 1] += b[v];
		int mid = (l + r) / 2;
		sum[v + v] += a[v] * (mid - l) * (mid + l - 1) / 2 + b[v] * (mid - l);
		sum[v + v + 1] += a[v] * (r - mid) * (mid + r - 1) / 2 + b[v] * (r - mid);
		a[v] = b[v] = 0;
		a[v + v] %= mod;
		b[v + v] %= mod;
		sum[v + v] %= mod;
		a[v + v + 1] %= mod;
		b[v + v + 1] %= mod;
		sum[v + v + 1] %= mod;
	}

	void _add(int v, int l1, int r1, int l, int r, int x, int y){
		if (r1 <= l || r <= l1)
			return;
		if (l < l1)
			l = l1;
		if (r > r1)
			r = r1;
		sum[v] = (sum[v] + 1ll * y * (r - l) + 1ll * x * (r - l) * (r + l - 1) / 2) % mod;
		if (l == l1 && r == r1){
			a[v] += x;
			b[v] += y;
			return;
		}
		int mid = (l1 + r1) / 2;
		relax(v, l1, r1);
		_add(v + v, l1, mid, l, r, x, y);
		_add(v + v + 1, mid, r1, l, r, x, y);
	}

	void add(int l, int r, int x, int y){
		_add(1, 0, n, l, r, x, y);
	}

	int _get(int v, int l1, int r1, int l, int r){
		if (r1 <= l || r <= l1)
			return 0;
		if (l < l1)
			l = l1;
		if (r > r1)
			r = r1;
		if (l == l1 && r == r1){
			return sum[v];
		}
		long long res = a[v] * (r - l) * (r + l - 1) / 2 + b[v] * (r - l);
		int mid = (l1 + r1) / 2;
		res += _get(v + v, l1, mid, l, r) + _get(v + v + 1, mid, r1, l, r);
		return res % mod;
	}

	int get(int l, int r){
		return _get(1, 0, n, l, r);
	}
};

#define next ljshflgjsf
const int N = 444444;
int top[N], size[N], level[N], next[N], pr[N];
char used[N];
vector<int> a[N];
Tree tree[N];

void dfs(int v){
	used[v] = 1;
	size[v] = 1;
	top[v] = v;
	int cur = -1;
	for (auto x : a[v]){
		if (!used[x]){
			level[x] = level[v] + 1;
			pr[x] = v;
			dfs(x);
			size[v] += size[x];
			if (cur == -1 || size[x] > size[cur])
				cur = x;
		}
	}
	next[v] = cur;
}

void dfs1(int v){
	used[v] = 2;
	if (next[v] > -1)
		top[next[v]] = top[v];
	for (auto x : a[v])
		if (used[x] != 2)
			dfs1(x);
}

int get(int u, int v){
	long long ans = 0;
	while (top[u] != top[v]){
		if (level[top[u]] > level[top[v]]){
			swap(u, v);
		}
		int w = top[v];
		ans += tree[w].get(0, level[v] - level[w] + 1);
		v = pr[w];
	}
	int w = top[u];
	if (level[u] > level[v])
		swap(u, v);
	ans += tree[w].get(level[u] - level[w], level[v] - level[w] + 1);
	return ans % mod;
}

void update(int u, int v, int x){
	vector<int> to_add;
	int cur = 0;
	while (top[u] != top[v]){
		if (level[top[u]] < level[top[v]]){
			to_add.push_back(v);
			v = pr[top[v]];
		} else {
			int w = top[u];
			tree[w].add(0, level[u] - level[w] + 1, mod - x, 1ll * (1 - level[w] + level[u] + cur) * x % mod);
			cur += (level[u] - level[w] + 1);
			u = pr[w];
		}
	}
	int w = top[u];
	if (level[u] > level[v])
		tree[w].add(level[v] - level[w], level[u] - level[w] + 1, mod - x, 1ll * (1 - level[w] + level[u] + cur) * x % mod);
	else
		tree[w].add(level[u] - level[w], level[v] - level[w] + 1, x, 1ll * (cur + 1 - level[u] + level[w]) * x % mod);
	cur += abs(level[u] - level[v]) + 1;
	while (!to_add.empty()){
		u = to_add.back();
		w = top[u];
		tree[w].add(0, level[u] - level[w] + 1, x, 1ll * (1 + cur) * x % mod);
		to_add.pop_back();
		cur += level[u] - level[w] + 1;
	}
}

int main(){

	int n = nxt(), q = nxt();
	for (int i = 0; i < n - 1; i++){
		int u = nxt(), v = nxt();
		assert(u != v);
		a[u].push_back(v);
		a[v].push_back(u);
	}

	pr[0] = -1;
	for (int i = 0; i < n; i++)
		next[0] = -1;
	dfs(0);
	dfs1(0);
	for (int i = 0; i < n; i++)
		assert(used[i]);

	vector<int> len(n);
	for (int i = 0; i < n; i++){
		len[top[i]] = max(len[top[i]], level[i] - level[top[i]] + 1);
	}
	for (int i = 0; i < n; i++){
		tree[i] = Tree(len[i]);
	}

	while (q--){
		int t = nxt();
		if (t == 1){
			int u = nxt(), v = nxt(), x = nxt();
			update(u, v, x);
		} else {
			int u = nxt(), v = nxt();
			cout << get(u, v) << "\n";
		}
	}

	return 0;
}