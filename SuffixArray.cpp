#include <bits/stdc++.h>

#define itn int
#define all(x) (x).begin(), (x).end()

using namespace std;

inline int nxt() {
	int x;
	cin >> x;
	return x;
}

ostream& operator <<(ostream& out, const pair<int, int>& p) {
	out << "(" << p.first << ", " << p.second << ")";
	return out;
}

template <typename T>
ostream& operator <<(ostream& out, const vector<T>& v) {
	if (v.empty()) {
		out << "{}";
	} else {
		out << "{" << v[0];
		for (int i = 1; i < (int)v.size(); ++i) {
			out << ", " << v[i];
		}
		out << "}";
	}
	return out;
}

template <class T = string>
struct SuffixArray {
	vector<int> sa;
	vector<int> pos;
	vector<int> lcp;
	vector<vector<int>> sparse_lcp;

	SuffixArray(const T& s) {
		int n = s.size() + 1;
		sa.resize(n);
		vector<int> cls(n);
		vector<pair<int, int>> tmp(n);
		for (int i = 0; i < n - 1; ++i) {
			tmp[i] = {s[i], i};
		}
		tmp[n - 1] = {-1e9, n - 1};
		sort(all(tmp));
		for (int i = 0; i < n; ++i) {
			sa[i] = tmp[i].second;
			if (i > 0) {
				cls[sa[i]] = cls[sa[i - 1]] + (tmp[i].first == tmp[i - 1].first ? 0 : 1);
			}
		}

		for (int k = 1; k < n; k *= 2) {
			int cl_cnt = cls[sa[n - 1]] + 1;
			if (cl_cnt == n) {
				break;
			}
			vector<int> new_sa(n);
			vector<int> cur(cl_cnt);
			for (int i = 0; i < n; ++i) {
				if (i == 0 || cls[sa[i]] != cls[sa[i - 1]]) {
					cur[cls[sa[i]]] = i;
				}
			}

			for (int i = 0; i < n; ++i) {
				int index = sa[i] - k;
				if (index < 0) {
					index += n;
				}
				int cl = cls[index];
				new_sa[cur[cl]++] = index;
			}

			vector<int> new_cls(n);
			sa.swap(new_sa);
			for (int i = 1; i < n; ++i) {
				int i1 = sa[i] + k;
				int i2 = sa[i - 1] + k;
				if (i1 >= n) {
					i1 -= n;
				}
				if (i2 >= n) {
					i2 -= n;
				}
				if (cls[sa[i]] == cls[sa[i - 1]] && cls[i1] == cls[i2]) {
					new_cls[sa[i]] = new_cls[sa[i - 1]];
				} else {
					new_cls[sa[i]] = new_cls[sa[i - 1]] + 1;
				}
			}
			cls.swap(new_cls);
		}

		pos.resize(n);
		lcp.resize(n);
		for (int i = 0; i < n; ++i) {
			pos[sa[i]] = i;
		}
		int lastLcp = 0;
		if (pos[0] + 1 != n) {
			while (lastLcp < n - 1 && sa[pos[0] + 1] + lastLcp < n - 1) {
				if (s[lastLcp] == s[sa[pos[0] + 1] + lastLcp]) {
					++lastLcp;
				} else {
					break;
				}
			}
			lcp[pos[0]] = lastLcp;
		}
		for (int i = 1; i < n - 1; ++i) {
			--lastLcp;
			if (lastLcp < 0) {
				lastLcp = 0;
			}
			int index = pos[i] + 1;
			if (index == n) {
				lastLcp = 0;
				continue;
			} else {
				index = sa[index];
			}
			while (i + lastLcp < n - 1 && index + lastLcp < n - 1) {
				if (s[i + lastLcp] == s[index + lastLcp]) {
					++lastLcp;
				} else {
					break;
				}
			}
			lcp[pos[i]] = lastLcp;
		}

		buildSparseLcp();
	}

	void show() {
		// cerr << "array: ";
		for (int x : sa) {
			// cerr << x << " ";
		}
		// cerr << "\npos: ";
		for (int x : pos) {
			// cerr << x << " ";
		}
		// cerr << "\nlcp: ";
		for (int x : lcp) {
			// cerr << x << " ";
		}
		// cerr << "\n";
	}

	void buildSparseLcp() {
		int n = lcp.size();
		sparse_lcp.resize(n);
		for (itn i = 0; i < n; ++i) {
			sparse_lcp[i].push_back(lcp[i]);
		}
		for (int j = 1; j + j <= n; j *= 2) {
			for (int i = 0; i + j + j <= n; ++i) {
				sparse_lcp[i].push_back(min(sparse_lcp[i].back(), sparse_lcp[i + j].back()));
			}
		}
	}

	int getMin(int l, int r) {	// [l..r)
		int k = 31 - __builtin_clz(r - l);
		return min(sparse_lcp[l][k], sparse_lcp[r - (1 << k)][k]);
	}

	int getLcp(int l, int r) {	// LCP(s[l:], s[r:])
		l = pos[l];
		r = pos[r];
		if (l > r) {
			swap(l, r);
		}
		return getMin(l, r);
	}

	pair<int, int> findBoundsForLcp(int suf, int size) {	// [*, *)
		suf = pos[suf];
		if (size == 0) {
			return {0, pos.size()};
		}
		// otherwise 0 doesn't meet the constraint
		pair<int, int> result;
		int l = 0, r = suf;
		while (r > l + 1) {
			int mid = (l + r) / 2;
			if (getMin(mid, suf) < size) {
				l = mid;
			} else {
				r = mid;
			}
		}
		result.first = r;
		l = suf + 1, r = pos.size() + 1;
		while (r > l + 1) {
			int mid = (l + r) / 2;
			if (getMin(suf, mid) >= size) {
				l = mid;
			} else {
				r = mid;
			}
		}
		result.second = r;
		if (getMin(suf, l) < size) {
			result.second = l;
		}

		// cerr << "result for " << suf << " and " << size << " is " << result << "\n";

		return result;
	}
};

struct Query {
	int x, y;	// [-oo..x][-oo..y]
	int *upd;
	int mult;
};

struct Fenwick {
	vector<int> a;
	int n;

	Fenwick(int _n): n(_n) {
		a.resize(n);
	}

	void add(int pos, int x) {
		while (pos < n) {
			a[pos] += x;
			pos |= pos + 1;
		}
	}

	int get(int pos) {
		int res = 0;
		while (pos >= 0) {
			res += a[pos];
			pos = (pos & (pos + 1)) - 1;
		}
		return res;
	}
};

struct Rectsum {
	vector<pair<int, int>> pts;
	vector<Query> qrs;

	Rectsum(const vector<pair<int, int>>& _pts): pts(_pts) {}
	Rectsum() {}

	void addPoint(int x, int y) {
		pts.emplace_back(x, y);
	}

	void addRect(int l1, int l2, int r1, int r2, int* val, int cnt = 1) {
		qrs.push_back({l2, r2, val, cnt});
		qrs.push_back({l1 - 1, r2, val, -cnt});
		qrs.push_back({l2, r1 - 1, val, -cnt});
		qrs.push_back({l1 - 1, r1 - 1, val, cnt});
	}

	void perform() {
		// cerr << "So what we have here\n";
		// cerr << "pts = " << pts << "\nnow queries\n";
		for (const auto q : qrs) {
			// cerr << "(-oo, " << q.x << "] x (-oo, " << q.y << "]\n";
		}
		vector<pair<int, int>> evs;
		vector<int> ys;
		for (int i = 0; i < (int)pts.size(); ++i) {
			evs.push_back({pts[i].first, -i - 1});
			ys.push_back(pts[i].second);
		}
		for (int i = 0; i < (int)qrs.size(); ++i) {
			evs.push_back({qrs[i].x, i});
			ys.push_back(qrs[i].y);
		}
		sort(all(evs));
		sort(all(ys));
		ys.resize(unique(all(ys)) - ys.begin());

		Fenwick fenw(ys.size());
		for (int i = 0; i < (int)evs.size(); ++i) {
			if (evs[i].second >= 0) {
				const Query& query = qrs[evs[i].second];
				int y = lower_bound(all(ys), query.y) - ys.begin();
				*(query.upd) += query.mult * fenw.get(y);
				// cerr << "answer for (-oo.." << query.x << "] x (-oo.." << query.y << "]: " << fenw.get(y) << "\n";
			} else {
				int y = lower_bound(all(ys), pts[-1 - evs[i].second].second) - ys.begin();
				fenw.add(y, 1);
				// cerr << "added " << pts[-1 - evs[i].second] << "\n";
			}
		}
	}
};

void solve() {
	string s;
	cin >> s;
	int q = nxt();
	vector<string> queries(q);
	for (int i = 0; i < q; ++i) {
		cin >> queries[i];
	}

	vector<int> a;
	for (char c : s) {
		a.push_back(c);
	}
	for (int i = 0; i < q; ++i) {
		a.push_back(-i);
		for (char c : queries[i]) {
			a.push_back(c);
		}
	}

	auto b = a;
	reverse(all(b));

	SuffixArray<vector<int>> sa(a), sb(b);
	Rectsum rectsum;
	for (int i = 0; i < (int)s.length(); ++i) {
		rectsum.addPoint(sa.pos[i + 1], sb.pos[(int)b.size() - i]);
	}

	vector<int> origin_positions;
	for (int i = 0; i < (int)s.length(); ++i) {
		origin_positions.push_back(sa.pos[i]);
	}
	sort(all(origin_positions));

	// cerr << s;
	for (int i = 0; i < q; ++i) {
		// cerr << "#" << queries[i];
	}
	// cerr << "\n";
	// cerr << a << "\n" << b << "\n" << sa.sa << ", " << sa.pos << ", " << sa.lcp << "\n" << sb.sa << ", " << sb.pos << ", " << sb.lcp << "\n";

	vector<int> ans(q);
	int cur = (int)s.length() + 1;
	for (int i = 0; i < q; ++i) {
		for (int j = 0; j < (int)queries[i].length(); ++j) {
			auto bnd = sa.findBoundsForLcp(cur + j + 1, (int)queries[i].length() - 1 - j);
			auto rbnd = sb.findBoundsForLcp((int)b.size() - cur - j, j);
			rectsum.addRect(bnd.first, bnd.second - 1, rbnd.first, rbnd.second - 1, &ans[i]);
		}
		auto bnd = sa.findBoundsForLcp(cur, (int)queries[i].length());
		int cnt = lower_bound(all(origin_positions), bnd.second) - 
				  lower_bound(all(origin_positions), bnd.first);
		// cerr << "whole string occurs " << cnt << " times\n";
		ans[i] = cnt * (1 - (int)queries[i].length());
		cur += queries[i].length() + 1;
	}

	rectsum.perform();

	for (int x : ans) {
		cout << x << "\n";
	}
}

int main() {
	ios_base::sync_with_stdio(false);

	int T = nxt();
	while (T--) {
		solve();
	}

	return 0;
}