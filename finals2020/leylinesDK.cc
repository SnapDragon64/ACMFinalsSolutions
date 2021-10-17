#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#define PI 3.1415926535897932384626

struct Point {
  long double x, y;
  Point(long double x = 0.0, long double y = 0.0) : x(x), y(y) {}
  Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
  long double Len() const { return hypot(x, y); }
};

struct Event {
  long double th;
  int n;
  bool added;

  bool operator<(const Event& e) { return th < e.th; }
};

int main() {
  int N, T;
  while (cin >> N >> T) {
    vector<Point> P(N);
    for (auto& p : P) cin >> p.x >> p.y;

    int ret = 0;
    for (int i = 0; i < P.size(); i++) {
      vector<Event> ev;
      for (int j = 0; j < P.size(); j++) if (j != i) {
        Point v = P[j]-P[i];
        long double th = atan2(v.y, v.x);
        long double ln = v.Len();
        if (ln <= T) {
          ev.push_back(Event{fmod(3*PI+th, 2*PI), j, true });
          ev.push_back(Event{fmod(2*PI+th, 2*PI), j, false});
        } else {
          long double ph = asin(T / ln);
          ev.push_back(Event{fmod(2*PI+th-ph, 2*PI), j, true });
          ev.push_back(Event{fmod(2*PI+th   , 2*PI), j, false});
          ev.push_back(Event{fmod(3*PI+th   , 2*PI), j, true });
          ev.push_back(Event{fmod(3*PI+th+ph, 2*PI), j, false});
        }
      }

      sort(ev.begin(), ev.end());
      vector<bool> used(N);
      int cur = 1;
      for (int rep = 0; rep < 2; rep++)
      for (auto const& e : ev) {
        if (e.added && !used[e.n]) {
          used[e.n] = true;
          cur++;
          ret = max(ret, cur);
        }
        if (!e.added && used[e.n]) {
          used[e.n] = false;
          cur--;
        }
      }
    }
    cout << ret << endl;
  }
}


