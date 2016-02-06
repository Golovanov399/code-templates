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
