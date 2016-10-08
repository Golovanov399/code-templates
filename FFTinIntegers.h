#include <vector>
#include <iostream>

using namespace std;

int rev(int n, int cnt){
	int res = 0;
	for (int i = 0; i < cnt; i++){
		res = (res << 1) | (n & 1);
		n >>= 1;
	}
	return res;
}
 
const int mod = 924844033;
const int g = 5;
 
long long pw(long long a, long long b) {
	long long res = 1;
	while (b) {
		if (b & 1ll) {
			res = res * a % mod;
		}
		b >>= 1;
		a = a * a % mod;
	}
	return res;
}
 
void fft(vector<long long>& a, const vector<long long>& angles, bool inv = false){
	int n = a.size();
	int inv_n = pw(n, mod - 2);
 
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
		long long u = a[j], v = a[j + 1];
		a[j] = (u + v + mod) % mod;
		a[j + 1] = (u - v + mod) % mod;
	}
 
	if (n >= 4){
		for (int j = 0; j < n; j += 4){
			long long u = a[j], v = a[j + 2];
			a[j] = (u + v + mod) % mod;
			a[j + 2] = (u - v + mod) % mod;
 
			long long w = inv ? angles[n / 4 * 3] : angles[n / 4];
			u = a[j + 1], v = a[j + 3] * w % mod;
			a[j + 1] = (u + v) % mod;
			a[j + 3] = (u - v + mod) % mod;
		}
	}
 
	for (int len = 8; len <= n; len <<= 1){
		for (int j = 0; j < n; j += len){
			auto uptr = a.begin() + j, vptr = a.begin() + j + len / 2;
			for (int i = 0; i < len / 2; i++){
				int index = n / len * i;
				if (inv && index)
					index = n - index;
				long long w = angles[index];
				long long u = *uptr, v = (*vptr) * w % mod;
				*uptr = (u + v) % mod;
				*vptr = (u - v + mod) % mod;
				++uptr, ++vptr;
			}
		}
	}
 
	if (inv){
		for (int i = 0; i < n; i++)
			a[i] = a[i] * inv_n % mod;
	}
}
 
vector<int> fft_mult(const vector<int>& a, const vector<int>& b){
	int n = 1;
	while (n < (int)a.size() || n < (int)b.size())
		n *= 2;
	n *= 2;
	vector<long long> ar(n), br(n);
	for (int i = 0; i < (int)a.size(); i++)
		ar[i] = a[i];
	for (int i = 0; i < (int)b.size(); i++)
		br[i] = b[i];
	vector<long long> angles(n);
	for (int i = 0; i < n; i++){
		angles[i] = pw(g, (mod - 1) / n * i);
	}
	fft(ar, angles);
	fft(br, angles);
	for (int i = 0; i < n; i++)
		ar[i] = ar[i] * br[i] % mod;
	fft(ar, angles, true);
	vector<int> res(n);
	for (int i = 0; i < n; i++){
		res[i] = ar[i];
	}
	while (res.size() > 1 && res.back() == 0)
		res.pop_back();
	return res;
}
 
void test_fft(int n){
	vector<int> a(n), b(n);
	for (int i = 0; i < n; i++){
		a[i] = rand() % 200 - 100;
		b[i] = rand() % 200 - 100;
	}
	clock_t start = clock();
	vector<int> c = fft_mult(a, b);
	clock_t finish = clock();
	cerr << "n = " << n << ": ";
	if (n <= 1000){
		vector<int> res;
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				if (i + j >= (int)res.size()) {
					res.push_back(0);
				}
				res[i + j] += a[i] * b[j];
			}
		}
		for (int &x : res) {
			x %= mod;
			if (x < 0) {
				x += mod;
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

