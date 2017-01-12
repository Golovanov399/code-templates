#ifdef SUPER_PRECISE
	using ld = long double;
	#define sqrt sqrtl
	#define fabs fabsl
	#define cos cosl
	#define sin sinl
	#define acos acosl
	#define asin asinl
#else
	using ld = double;
#endif

const ld eps = 1e-9;
const ld PI = acosl(-1);

#ifdef INT_GEOM
	using ptype = long long;
	#define ptype_zero(x) ((x) == 0)
	#define abs llabs
#else
	using ptype = ld;
	#define ptype_zero(x) (fabs((x)) < eps)
	#define abs fabs
#endif

int sign(ptype x) {
	if (ptype_zero(x)) {
		return 0;
	} else if (x > 0) {
		return 1;
	} else {
		return -1;
	}
}

class Point {
public:
	Point() {}
	Point(ptype _x, ptype _y): x(_x), y(_y) {}

	ptype getX() const {
		return x;
	}

	ptype getY() const {
		return y;
	}

	Point operator +(const Point& ot) const {
		return Point(x + ot.x, y + ot.y);
	}

	Point operator -(const Point& ot) const {
		return Point(x - ot.x, y - ot.y);
	}

	Point operator *(ptype k) const {
		return Point(x * k, y * k);
	}

	void operator +=(const Point& ot) {
		x += ot.x;
		y += ot.y;
	}

	void operator -=(const Point& ot) {
		x -= ot.x;
		y -= ot.y;
	}

	void operator *=(ptype k) {
		x *= k;
		y *= k;
	}

#ifndef INT_GEOM
	Point operator /(ptype k) const {
		return Point(x / k, y / k);
	}

	void operator /=(ptype k) {
		x /= k;
		y /= k;
	}
#endif	

	ptype operator %(const Point& ot) const {
		return x * ot.x + y * ot.y;
	}

	ptype operator *(const Point& ot) const {
		return x * ot.y - y * ot.x;
	}

	ptype getSqrDist() const {
		return x * x + y * y;
	}

	ptype getSqrDist(const Point& ot) const {
		return (ot - *this) % (ot - *this);
	}

	ld getDist() const {
		return sqrt(getSqrDist());
	}

	ld getDist(const Point& pt) const {
		return sqrt(getSqrDist(pt));
	}

	void rot90() {
		swap(x, y);
		x = -x;
	}

#ifndef INT_GEOM
	void rot(ld angle) {
		ptype nx = x * cos(angle) + y * sin(angle);
		ptype ny = x * sin(angle) - y * cos(angle);
		x = nx, y = ny;
	}

	void inverse(ptype radius = 1) {
		ptype dist2 = getSqrDist();
		x = x * radius * radius / dist2;
		y = y * radius * radius / dist2;
	}

	Point getInversed(ptype radius = 1) const {
		ptype dist2 = getSqrDist();
		return Point(x * radius * radius / dist2, y * radius * radius / dist2);
	}
#endif

	bool operator <(const Point& ot) const {
		int sx = sign(x - ot.x);
		int sy = sign(y - ot.y);
		if (sx) {
			return sx == -1;
		} else {
			return sy == -1;
		}
	}

	void scan() {
		cin >> x >> y;
	}
// private:
	ptype x, y;
};

istream& operator >>(istream& in, Point& pt) {
	ptype x, y;
	in >> x >> y;
	pt = Point(x, y);
	return in;
}

ostream& operator <<(ostream& out, const Point& pt) {
	out << "(" << pt.getX() << ", " << pt.getY() << ")";
	return out;
}

class Line {
public:
	Line() {}
	Line(const Point& fst, const Point& snd) {
		p = fst;
		n = snd - fst;
		n.rot90();
	}

	bool intersects(const Line& ot) const {
		return !ptype_zero(n * ot.n);
	}

	bool contains(const Point& pt) const {
		return ptype_zero((pt - p) % n);
	}

	bool is_parallel(const Line& ot) const {
		return ptype_zero(n * ot.n);
	}

	ptype apply(const Point& pt) const {
		return (pt - p) % n;
	}

#ifndef INT_GEOM
	Point intersect(const Line& ot) const {
		if (!intersects(ot)) {
			return Point();
		}
		ptype c1 = p % n, c2 = ot.p % ot.n;
		return (n * (c1 * ot.n.getSqrDist() - c2 * (n % ot.n)) +
			   ot.n * (c2 * n.getSqrDist() - c1 * (n % ot.n))) /
			   (n.getSqrDist() * ot.n.getSqrDist() - (n % ot.n) * (n % ot.n));
	}
#endif
// private:
	Point p, n;
};

class Segment {
public:
	Segment() {}
	Segment(const Point& _p, const Point& _q): p(_p), q(_q) {
		l = Line(_p, _q);
	}

	bool intersectsInside(const Segment& ot) const {
		if (ptype_zero(l.n * ot.l.n)) {
			return false;
		}

		return (sign(l.apply(ot.p)) * sign(l.apply(ot.q)) == -1) &&
			   (sign(ot.l.apply(p)) * sign(ot.l.apply(q)) == -1);
	}

#ifndef INT_GEOM
	Point intersectInside(const Segment& ot) const {
		if (!intersectsInside(ot)) {
			return Point();
		} else {
			return l.intersect(ot.l);
		}
	}
#endif

	bool contains(const Point& pt) const {
		return (sign((pt - p) % (pt - q)) == -1) && ptype_zero((pt - p) * (pt - q));
	}
// private:
	Point p, q;
	Line l;
};

class Polygon {
public:
	Polygon() {}
	Polygon(const vector<Point>& _pts): pts(_pts) {}

	ptype getDoubleSquare() const {
		ptype result = 0;
		int n = pts.size();
		for (int i = 1; i < n - 1; ++i) {
			result += (pts[i] - pts[0]) * (pts[i + 1] - pts[0]);
		}
		return abs(result);
	}

	virtual bool contains(const Point& pt) const {
		int n = pts.size();
		for (int i = 0; i < n; ++i) {
			int j = i + 1;
			if (j == n) {
				j = 0;
			}
			if (Segment(pts[i], pts[j]).contains(pt)) {
				return true;
			}
		}

		Point v(rand(), rand());
		ptype maxx = 0, maxy = 0;
		for (const Point& p : pts) {
			maxx = max(maxx, abs(p.x - pt.x));
			maxy = max(maxy, abs(p.y - pt.y));
		}
		while (ptype_zero(v.x) || ptype_zero(v.y)) {
			v = Point(rand(), rand());
		}
		ptype k = min(maxx / abs(v.x), maxy / abs(v.y)) + 1;
		v *= k;
		Segment ray(pt, pt + v);
		bool result = false;
		for (int i = 0; i < n; ++i) {
			int j = i + 1;
			if (j == n) {
				j = 0;
			}
			if (ray.intersectsInside(Segment(pts[i], pts[j]))) {
				result ^= 1;
			}
		}

		return result;
	}
// private:
	vector<Point> pts;
};

/***************************************
	Convex polygon should have its
	vertices in the CCW order.
	Moreover, the 0th vertex should
	be the lowest among the left ones.

	However, it may be not necessary
***************************************/
class ConvexPolygon : public Polygon {
public:
	ConvexPolygon(const vector<Point>& _pts): pts(_pts) {}

	bool contains(const Point& pt) const {
		int n = pts.size();
		int l = 1, r = n - 1;
		while (r > l + 1) {
			int mid = (l + r) / 2;
			int sgn = sign((pt - pts[0]) * (pts[mid] - pts[0]));
			if (sgn == 0) {
				return Segment(pts[0], pts[mid]).contains(pt);
			} else if (sgn == 1) {
				r = mid;
			} else {
				l = mid;
			}
		}

		Segment side(pts[l], pts[r]);
		if (Segment(pts[0], pts[l]).contains(pt) || Segment(pts[0], pts[r]).contains(pt)) {
			return true;
		}
		return !side.intersectsInside(Segment(pts[0], pt));
	}
// private:
	vector<Point> pts;
};

ConvexPolygon getConvexHull(vector<Point> pts) {
	int index = min_element(pts.begin(), pts.end()) - pts.begin();
	swap(pts[index], pts[0]);
	Point origin = pts[0];
	sort(pts.begin() + 1, pts.end(), [&origin](const Point& fi, const Point& se) {
		int sgn = sign((fi - origin) * (se - origin));
		if (sgn) {
			return sgn == 1;
		} else {
			return (fi - origin).getSqrDist() < (se - origin).getSqrDist();
		}
	});

	vector<Point> result = {pts[0]};
	for (int i = 1; i < (int)pts.size(); ++i) {
		while (result.size() > 1 && sign((result.back() - result[result.size() - 2]) * (pts[i] - result.back())) != 1) {
			result.pop_back();
		}
		result.push_back(pts[i]);
	}
	return ConvexPolygon(result);
}