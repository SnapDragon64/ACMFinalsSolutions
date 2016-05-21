#include <cmath>
#include <iostream>
#include <string>
using namespace std;

long long findb(string s, long long Y) {
  long long lo = 10, hi = 1.1e18;
  while (lo < hi) {
    long long b = (hi+lo+1)/2, cur = 0;
    if (pow(b, s.size()-1)*(s[0]-'0') > Y+1e9) {hi = b-1; continue;}
    for (int i = 0; i < s.size(); i++) cur = (cur*b) + s[i]-'0';
    if (cur > Y) hi = b-1; else lo = b;
  }
  return lo;
}

int main() {
  long long Y, b;
  string L;
  while (cin >> Y >> L) {
    for (int lp = L.size()-1;;) {
      long long b1 = findb(L.substr(0, lp+1) + string(L.size()-lp-1, '9'), Y);
      long long b2 = findb(L.substr(0, lp+1) + string(L.size()-lp-1, '0'), Y);
//cout << L << ' ' << b1 << ' ' << b2 << endl;
      for (b = b2; b >= b1; b--) {
        long long pb = 1, y;
        for (int i = 0; i < L.size()-1; i++) pb *= b;
        for (y = Y; pb; pb /= b) {
          long long dig = y/pb;
          if (dig > 9) break;
          y -= dig*pb;
        }
        if (y == 0) goto done;
      }
      while (L[lp]++ == '9') {
        if (lp == 0) {
          L = '0' + L;
        } else {
          lp--;
        }
      }
    }

done:
    cout << b << endl;
  }
}
