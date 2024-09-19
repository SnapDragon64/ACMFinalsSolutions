#include <algorithm>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template<typename T> constexpr T Gcd(const T& a, const T& b) { return b != 0 ? Gcd(b, a%b) : a < 0 ? -a : a; }

struct Fract {
  int64_t num, den;
  Fract(int64_t n = 0) : num(n), den(1) {};
  Fract(int64_t n, int64_t d) { int64_t g = Gcd(n, d); if (d < 0) g = -g; num = n/g; den = d/g; }
  Fract operator+(const Fract& f) const { return Fract(num*f.den + f.num*den, den*f.den); }
  Fract& operator+=(const Fract& f) { return *this = *this + f; }
  Fract operator-(const Fract& f) const { return Fract(num*f.den - f.num*den, den*f.den); }
  Fract operator*(const Fract& f) const { return Fract(num*f.num, den*f.den); }
  Fract operator%(int64_t m) { m *= den; return Fract((num%m+m)%m, den); }
  bool operator==(const Fract& f) const { return num == f.num && den == f.den; }
  bool operator!=(const Fract& f) const { return num != f.num || den != f.den; }
  bool operator<(const Fract& f) const { return num*f.den < f.num*den; }
  bool operator>(const Fract& f) const { return num*f.den > f.num*den; }
  explicit operator double() const { return double(num)/den; }
};

int main() {
  int W, H, D, R, B;
  while (cin >> W >> H >> D >> R >> B) {
    bitset<200> state;
    int sb = 0;
    vector<int> PX(R+B), PY(R+B), VX(R+B), VY(R+B);
    vector<Fract> t1(R+B, 1e9), t2(R+B, 1e9), tp(R+B, 1e9);
    for (int i = 0; i < R+B; i++) {
      cin >> PX[i] >> PY[i] >> VX[i] >> VY[i];
      if ((i < R) ^ (PX[i] < 0)) state.set(i);
      if (PX[i] < 0) { PX[i] = -PX[i]; VX[i] = -VX[i]; }
      if (VX[i] == 0) continue;
      Fract t = VX[i] < 0 ? Fract(PX[i], -VX[i]) : Fract(2*W-PX[i], VX[i]), st = t;
      Fract y = (t * VY[i] + PY[i]) % (2*H), sy = y;
      Fract dt = Fract(2*W, abs(VX[i]));
      Fract dy = dt * VY[i];
      do {
        if (y == D) t1[i] = t;
        if (y == 2*H-D) t2[i] = t;
        t += dt;
        y = (y+dy) % (2*H);
      } while (y != sy);
      tp[i] = t-st;
    }
    if (state.none()) { cout << 0 << endl; continue; }

    vector<bitset<200>> mat(R+B);
    priority_queue<pair<Fract, int>, vector<pair<Fract, int>>, greater<pair<Fract, int>>> q;
    for (int i = 0; i < R+B; i++) if (min(t1[i], t2[i]) < 1e9) q.push({min(t1[i], t2[i]), i});
    while (!q.empty()) {
      bitset<200> cur;
      auto [t, i] = q.top();
      if (t > 100000) break;  // What's the max?
      for(;;) {
        q.pop();
        if (t1[i] == t) t1[i] += tp[i];
        if (t2[i] == t) t2[i] += tp[i];
        q.push({min(t1[i], t2[i]), i});
        cur.set(i);
        auto [t2, j] = q.top();
        if (t2 != t) break;
        i = j;
      }
      for (int b = 0; b < R+B; b++) if (cur[b]) {
        if (mat[b][b]) cur ^= mat[b]; else { mat[b] = cur; break; }
      }
      for (; sb < R+B; sb++) if (state[sb]) {
        if (mat[sb][sb]) state ^= mat[sb]; else break;
      }
      if (sb == R+B) {
        cout << fixed << setprecision(10) << double(t) << endl;
        goto done;
      }
    }

    cout << "impossible" << endl;
    continue;
done:;
  }
}
