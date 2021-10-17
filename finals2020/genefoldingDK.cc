#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

typedef unsigned long long uint64;

#define HASH1 1000072921
#define HASH2 1000000181

// Returned values (x,y,g) satisfy x*a+y*b = g.
// |x|+|y| is minimized, breaking ties with |x| <= |y|.
// If g>0, all other solutions are of the form (x+n*(b/g))*a + (y-n*(a/g))*b = g.
template<typename T> tuple<T,T,T> ExtendedGcd(T a, T b) {
	if( b == 0 ) {
    if (a == 0) return {0, 0, 0};
    if (a < 0) return {-1, 0, -a};
    return {1, 0, a};
	}
  T d, x, y;
  tie(x, y, d) = ExtendedGcd(b, a%b);
  return {y, x-y*(a/b), d};
}

// Requires a coprime to m.
inline unsigned ModInv(unsigned a, unsigned m) {
  return (get<0>(ExtendedGcd(int64_t(a), int64_t(m))) + m) % m;
}

// Note: Division only works for values coprime to Mod.
template<int Mod> struct ModInt {
  int value;
  ModInt(long long v = 0) : value(v % Mod) {}
  ModInt Inverse() const { return get<0>(ExtendedGcd(value, Mod)); }
  ModInt operator+(const ModInt& m) const { return value+m.value; }
  ModInt operator-(const ModInt& m) const { return value-m.value; }
  ModInt operator*(const ModInt& m) const { return (long long)value*m.value; }
  bool operator==(const ModInt& m) const { return int(*this) == int(m); }
  explicit operator int() const { return (value < 0) ? value + Mod : value; }
};

int main() {
  string S;
  while (cin >> S) {
    for (int swp = 0; swp < 2; swp++) {
      restart:
      ModInt<HASH1> x1 = 1000, xi1 = x1.Inverse(), a1 = S[S.size()-1], b1 = S[S.size()-2], p1 = 1;
      ModInt<HASH2> x2 = 1001, xi2 = x2.Inverse(), a2 = S[S.size()-1], b2 = S[S.size()-2], p2 = 1;
      for (int i = S.size()-1, j = S.size()-2; i >= (S.size()+1)/2; i--, j -= 2) {
        if (i < S.size()-1) {
          a1 = a1*x1 + S[i];
          a2 = a2*x2 + S[i];
          b1 = (b1-S[i])*xi1 + p1*S[j+1] + p1*x1*S[j];
          b2 = (b2-S[i])*xi2 + p2*S[j+1] + p2*x2*S[j];
          p1 = p1*x1;
          p2 = p2*x2;
        }
        if (a1 == b1 && a2 == b2) {
          S.erase(i);
          goto restart;
        }
      }
      reverse(S.begin(), S.end());
    }
    cout << S.size() << endl;
  }
}
