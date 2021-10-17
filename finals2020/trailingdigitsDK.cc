#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

template<typename T> constexpr T Gcd(const T& a, const T& b) { return b != 0 ? Gcd(b, a%b) : a < 0 ? -a : a; }

int main() {
  int B, D;
  string A;
  while (cin >> B >> D >> A) {
    int ret = 0;
    A = "000000000000"+A;
    vector<int> p10(A.size()), dd(A.size());
    p10[0] = 1;
    for (int i = 1; i < A.size(); i++) p10[i] = (p10[i-1] * 10LL) % B;
    for (int i = 1; i < A.size(); i++) dd[i] = (dd[i-1] * 10LL + D) % B;
    for (ret = A.size()-12; ret >= 0; ret--) {
      if (Gcd(dd[ret], B) % Gcd(p10[ret], B) != 0) continue;
      int n;
      for (n = (D == 0); (dd[ret] + 1LL*n*p10[ret]) % B; n++)
        ;
      char buf[20];
      sprintf(buf, "%d", n);
      string s = buf + string(ret, char(D+'0'));
      s = string(A.size()-s.size(), '0') + s;
      if (s <= A) goto done;
    }
done:
    cout << ret << endl;
  }
}
