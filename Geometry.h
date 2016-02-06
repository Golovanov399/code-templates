const long double PI = acosl(-1);
const long double eps = 1e-9;

inline bool eq(long double x, long double y = 0){
	return fabsl(x - y) < eps;
}

struct pt{
	long double x, y;

	pt(){x = y = 0;}
	pt(long double x, long double y):x(x),y(y){}
	pt (const pt& p){
		x = p.x;
		y = p.y;
	}

	void rot(const long double& ang){
		long double nx = x * cosl(ang) - y * sinl(ang);
		long double ny = x * sinl(ang) + y * cosl(ang);
		x = nx, y = ny;
	}

	pt operator +(const pt& p) const {
		return pt(x + p.x, y + p.y);
	}

	void operator +=(const pt& p){
		x += p.x;
		y += p.y;
	}

	pt operator -(const pt& p) const {
		return pt(x - p.x, y - p.y);
	}

	void operator -=(const pt& p){
		x -= p.x;
		y -= p.y;
	}

	pt operator *(long double k) const {
		return pt(x * k, y * k);
	}

	void operator *=(long double k){
		x *= k;
		y *= k;
	}

	long double len(){
		return sqrtl(x * x + y * y);
	}

	void norm(long double k = 1){
		long double d = len();
		x = x * k / d;
		y = y * k / d;
	}

	inline bool operator ==(const pt& p) const {
		return eq(x, p.x) && eq(y, p.y);
	}

	inline bool operator !=(const pt& p) const {
		return !eq(x, p.x) || !eq(y, p.y);
	}

	void in(){
		cin >> x >> y;
	}
};

long double vec(const pt& p, const pt& q){
	return p.x * q.y - p.y * q.x;
}

long double scal(const pt& p, const pt& q){
	return p.x * q.x + p.y * q.y;
}

long double S(const pt& p, const pt& q, const pt& r){
	return fabsl(vec(q - p, r - p)) / 2;
}

struct line{
	pt p;	// point of a line
	pt n;	// normal vector

	line(){}
	line(const pt& p, const pt& q):p(p){
		// if (p == q)
		// 	cerr << p.x << " " << p.y << "\n" << q.x << " " << q.y << "\n";
		assert(p != q);
		n = q - p;
		swap(n.x, n.y);
		n.y *= -1;
	}
	line(const line& l){
		p = l.p;
		n = l.n;
	}

	inline bool parallel(const line& l) const {
		return eq(vec(n, l.n), 0);
	}

	pt intersect(const line& l) const {
		pt v = n;
		v.rot(PI / 2);
		if (parallel(l)){
			// cerr << "Intersection of two parallel lines\n";
			assert(false);
			return p;
		} else {
			return p + v * (scal(l.p - p, l.n) / scal(v, l.n));
		}
	}

	pt proj(const pt& r) const {
		return r + n * (scal(p - r, n) / scal(n, n));
	}
};

struct segment{
	pt p, q;

	segment(){}
	segment(const pt& p, const pt& q):p(p),q(q){}
	segment(const segment& l){
		p = l.p;
		q = l.q;
	}

	inline bool has(const pt& r) const {
		return eq(vec(r - p, q - p)) && scal(r - p, r - q) <= eps;
	}

	bool parallel(const segment& s){
		return line(p, q).parallel(line(s.p, s.q));
	}

	bool intersect(const segment& s, pt& res) const {
		line l1(p, q);
		line l2(s.p, s.q);
		if (l1.parallel(l2)){
			if (s.has(p) || s.has(q) || has(s.p) || has(s.q))
				return true;
			else
				return false;
		}
		pt p = l1.intersect(l2);
		if (has(p) && s.has(p)){
			res = p;
			return true;
		} else {
			return false;
		}
	}

	bool intersect(const vector<segment>& a) const {
		pt res;
		for (const segment& x : a){
			if (intersect(x, res) && ((x.p != p || x.q != q) && (x.p != q || x.q != p))){
				return true;
			}
		}
		return false;
	}

	bool occur(const vector<segment>& a) const {
		for (const segment& x : a){
			if ((x.p == p && x.q == q) || (x.p == q && x.q == p))
				return true;
		}
		return false;
	}

	bool on_the_border(const vector<segment>& a) const {
		for (const segment& x : a){
			if (x.has(p) && x.has(q))
				return true;
		}
		return false;
	}

	inline long double len() const {
		return (q - p).len();
	}

	pt proj(const pt& r) const {
		return line(p, q).proj(r);
	}

	void make_interval(){
		long double d = len();
		pt v = q - p;
		v.norm(d - 2 * eps);
		q = p + v;
		v = p - q;
		v.norm(d - 4 * eps);
		p = q + v;
	}
};

bool inside(const vector<segment>& poly, const pt& p){
	for (const auto& x : poly)
		if (x.has(p))
			return true;
	int res = 0;
	line l(p, pt(p.x + 1, p.y + 0.36345144353635));
	for (const segment& s : poly){
		line l1(s.p, s.q);
		if (l.parallel(l1))
			continue;
		pt q = l.intersect(l1);
		if (q.x < p.x)	// assume that p != q
			continue;
		if (q == s.q)
			continue;
		if (s.has(q))
			res ^= 1;
	}
	return res;
}
