#include <bits/stdc++.h>

#define itn int

using namespace std;

struct BigInteger{
	vector<int> digits;
	bool neg;

	BigInteger(long long n = 0){
		neg = 0;
		if (n < 0){
			neg = 1;
			n = -n;
		}
		while (n){
			digits.push_back(n % 10);
			n /= 10;
		}
		if (digits.empty())
			digits.push_back(0);
	}

	BigInteger(const vector<int>& a, bool neg):neg(neg){
		digits = a;
	}

	BigInteger(const string& s){
		int l = 0, r = s.length();
		neg = 0;
		if (s[l] == '-'){
			l++;
			neg = 1;
		}
		for (int i = r; i >= l; i--){
			digits.push_back(s[i] - '0');
		}
	}

	int & operator [](int index){
		return digits[index];
	}

	const int & operator [](int index) const {
		return digits[index];
	}

	inline int size() const {
		return digits.size();
	}

	void push_back(int x){
		digits.push_back(x);
	}

	bool operator <(const BigInteger& a) const {
		if (neg != a.neg)
			return neg;
		if (neg){
			if (digits.size() != a.size())
				return digits.size() > a.size();
			int i = digits.size() - 1;
			while (i > 0){
				if (digits[i] != a[i])
					return digits[i] > a[i];
				i--;
			}
			return false;
		} else {
			if (digits.size() != a.size())
				return digits.size() < a.size();
			int i = digits.size() - 1;
			while (i > 0){
				if (digits[i] != a[i])
					return digits[i] < a[i];
				i--;
			}
			return false;
		}
	}

	bool abs_lower(const BigInteger& a) const {
		if (digits.size() != a.size())
			return digits.size() < a.size();
		int i = digits.size() - 1;
		while (i > 0){
			if (digits[i] != a[i])
				return digits[i] < a[i];
			i--;
		}
		return false;
	}

	bool operator ==(const BigInteger& a) const {
		return neg == a.neg && digits == a.digits;
	}

	BigInteger operator +(const BigInteger& a) const {
		if (neg == a.neg){
			int k = 0;
			int i = 0;
			vector<int> res;
			while (i < size() || i < a.size() || k){
				res.push_back(k);
				if (i < digits.size())
					res.back() += digits[i];
				if (i < a.size())
					res.back() += a[i];
				k = res.back() / 10;
				res.back() %= 10;
				i += 1;
			}
			return BigInteger(res, neg);
		} else {
			bool newneg = (abs_lower(a) ? a.neg : neg);
			int coeff = newneg == neg ? 1 : -1;
			int k = 0;
			int i = 0;
			vector<int> res;
			while (i < size() || i < a.size() || k){
				res.push_back(k);
				if (i < digits.size())
					res.back() += coeff * digits[i];
				if (i < a.size())
					res.back() -= coeff * a[i];
				if (res.back() < 0){
					res.back() += 10;
					k = -1;
				} else {
					k = 0;
				}
				i += 1;
			}
			while (!res.empty() && res.back() == 0)
				res.pop_back();
			if (res.empty()){
				newneg = 0;
				res.resize(1, 0);
			}
			return BigInteger(res, newneg);
		}
	}

	BigInteger operator -(const BigInteger& a) const {
		if (neg != a.neg){
			int k = 0;
			int i = 0;
			vector<int> res;
			while (i < size() || i < a.size() || k){
				res.push_back(k);
				if (i < digits.size())
					res.back() += digits[i];
				if (i < a.size())
					res.back() += a[i];
				k = res.back() / 10;
				res.back() %= 10;
				i += 1;
			}
			return BigInteger(res, neg);
		} else {
			bool newneg = (abs_lower(a) ? !a.neg : neg);
			int coeff = newneg == neg ? 1 : -1;
			int k = 0;
			int i = 0;
			vector<int> res;
			while (i < size() || i < a.size() || k){
				res.push_back(k);
				if (i < digits.size())
					res.back() += coeff * digits[i];
				if (i < a.size())
					res.back() -= coeff * a[i];
				if (res.back() < 0){
					res.back() += 10;
					k = -1;
				} else {
					k = 0;
				}
				i += 1;
			}
			while (!res.empty() && res.back() == 0)
				res.pop_back();
			if (res.empty()){
				newneg = 0;
				res.resize(1, 0);
			}
			return BigInteger(res, newneg);
		}
	}

	BigInteger operator *(const BigInteger& a) const {
		int n = digits.size(), m = a.size();
		vector<int> res(n + m - 1);
		for (int i = 0; i < n; i++){
			for (int j = 0; j < m; j++){
				res[i + j] += digits[i] * a[j];
			}
		}
		for (int i = 0; i < n + m - 2; i++){
			res[i + 1] += res[i] / 10;
			res[i] %= 10;
		}
		while (res.back() > 9){
			res.push_back(res.back() / 10);
			res[(int)res.size() - 2] %= 10;
		}
		while (!res.empty() && res.back() == 0)
			res.pop_back();
		if (res.empty()){
			return BigInteger(0);
		} else 
			return BigInteger(res, neg ^ a.neg);
	}

	void zero(){
		neg = 0;
		digits.resize(1);
		digits[0] = 0;
	}

	string toString(){
		string res = "";
		if (neg)
			res = "-";
		for (int i = (int)digits.size() - 1; i >= 0; i--){
			res += (char)('0' + digits[i]);
		}
		return res;
	}

	void print(){
		if (neg)
			printf("-");
		for (int i = (int)digits.size() - 1; i >= 0; i--){
			printf("%d", digits[i]);
		}
	}
};
