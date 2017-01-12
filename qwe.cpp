#include <bits/stdc++.h>

using namespace std;

// #define INT_GEOM
#include "Geometry.h"

#define itn int
#define LL long long
#define pb push_back
#define mp make_pair
#define x first
#define y second
#define all(x) (x).begin(), (x).end()

inline int nxt(){
	int x;
	scanf("%d", &x);
	return x;
}

int main(){

	vector<Point> pts = {{4, 0}, {4, 4}, {2, 2}, {0, 4}, {0, 0}};
	Point pt(2, 3);
	Polygon poly(pts);
	// cerr << typeid(poly).name() << "\n";
	// cout << poly.contains(pt) << "\n";
	// poly = ConvexPolygon(getConvexHull(pts));
	// cerr << typeid(poly).name() << "\n";
	// cout << poly.contains(pt) << "\n";
	// cout << getConvexHull(pts).contains(pt) << "\n";

	return 0;
}
