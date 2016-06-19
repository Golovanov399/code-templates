#include <vector>
#include <iostream>

using namespace std;

// typedef complex<double> base;

struct base {
	double re, im;

	base(double x = 0, double y = 0):re(x),im(y){}

	base operator +(const base& z) const {
		return base(re + z.re, im + z.im);
	}

	base operator -(const base& z) const {
		return base(re - z.re, im - z.im);
	}

	base operator *(const base& z) const {
		return base(re * z.re - im * z.im, re * z.im + im * z.re);
	}

	void operator *=(const base& z){
		double tmp = re * z.re - im * z.im;
		im = re * z.im + im * z.re;
		re = tmp;
	}

	void operator /=(const double& z){
		re /= z;
		im /= z;
	}

	inline double real() const {
		return re;
	}

	inline double imag() const {
		return im;
	}
};

const long double PI = acosl(-1);

int rev(int n, int cnt){
	int res = 0;
	for (int i = 0; i < cnt; i++){
		res = (res << 1) | (n & 1);
		n >>= 1;
	}
	return res;
}

void fft(vector<base>& a, const vector<base>& angles, bool inv = false){
	int n = a.size();
	int cnt = __builtin_ctz(n);

	for (int i = 1, j = 0; i < n; i++){
		int bit = n >> 1;
		while (j >= bit){
			j -= bit;
			bit >>= 1;
		}
		j += bit;
		if (i < j)
			swap(a[i], a[j]);
	}

	for (int j = 0; j < n; j += 2){
		base u = a[j], v = a[j + 1];
		a[j] = u + v;
		a[j + 1] = u - v;
	}

	if (n >= 4){
		for (int j = 0; j < n; j += 4){
			base u = a[j], v = a[j + 2];
			a[j] = u + v;
			a[j + 2] = u - v;

			base w = inv ? angles[n / 4 * 3] : angles[n / 4];
			u = a[j + 1], v = a[j + 3] * w;
			a[j + 1] = u + v;
			a[j + 3] = u - v;
		}
	}

	for (int len = 8; len <= n; len <<= 1){
		for (int j = 0; j < n; j += len){
			auto uptr = a.begin() + j, vptr = a.begin() + j + len / 2;
			for (int i = 0; i < len / 2; i++){
				int index = n / len * i;
				if (inv && index)
					index = n - index;
				base w = angles[index];
				base u = *uptr, v = (*vptr) * w;
				*uptr = u + v;
				*vptr = u - v;
				++uptr, ++vptr;
			}
		}
	}

	if (inv){
		for (int i = 0; i < n; i++)
			a[i] /= n;
	}
}

vector<int> fft_mult(const vector<int>& a, const vector<int>& b){
	int n = 1;
	while (n < a.size() || n < b.size())
		n *= 2;
	n *= 2;
	vector<base> ar(n), br(n);
	for (int i = 0; i < a.size(); i++)
		ar[i] = a[i];
	for (int i = 0; i < b.size(); i++)
		br[i] = b[i];
	vector<base> angles(n);
	for (int i = 0; i < n; i++){
		angles[i] = base(cos(2 * PI * i / n), sin(2 * PI * i / n));
	}
	fft(ar, angles);
	fft(br, angles);
	for (int i = 0; i < n; i++)
		ar[i] *= br[i];
	fft(ar, angles, true);
	vector<int> res(n);
	for (int i = 0; i < n; i++){
		res[i] = round(ar[i].real());
	}
	while (res.size() > 1 && res.back() == 0)
		res.pop_back();
	return res;
}

void test_fft(int n){
	vector<int> a(n), b(n);
	for (int i = 0; i < n; i++){
		a[i] = rand() % 2000 - 1000;
		b[i] = rand() % 2000 - 1000;
	}
	clock_t start = clock();
	vector<int> c = fft_mult(a, b);
	clock_t finish = clock();
	cerr << "n = " << n << ": ";
	if (n <= 1000){
		vector<int> res;
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				if (i + j >= res.size())
					res.push_back(0);
				res[i + j] += a[i] * b[j];
			}
		}
		if (res != c){
			cerr << "incorrect fft\n";
			return;
		}
	}
	cerr << setprecision(6) << fixed;
	cerr << 1.0 * (finish - start) / CLOCKS_PER_SEC << " secs.\n";
}

void run_fft(){
	test_fft(1);
	test_fft(2);
	test_fft(3);
	test_fft(4);
	test_fft(100);
	test_fft(200000);
}

