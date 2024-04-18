#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int64_t memo1[51];
int64_t count1(int64_t n) {
  if (n == 0) return 1;
  int64_t& ret = memo1[n];
  if (ret) return ret;
  for (int64_t a = 1; a <= n; a++)
  for (int64_t c = 1; a*c <= n; c++) {
    ret += count1(n - a*c);
  }
  return ret;
}

// Returns # of possible next elements for generated sequences matching "seq".
map<pair<vector<int64_t>, vector<int64_t>>, map<int64_t,int64_t>> memo;
vector<int64_t> curc, cura;
vector<tuple<vector<int64_t>, vector<int64_t>, vector<int64_t>>> saved;
const map<int64_t,int64_t>& count(vector<int64_t> seq, vector<int64_t> prev, bool save) {
  static map<int64_t,int64_t> empty{}, base{{0,1}};
  if (seq[0] == 0) {
    for (int i = 1; i < seq.size(); i++) if (seq[i]) return empty;
    if (save) {
      vector<int64_t> curs = cura;
      while (curs.size() < curc.size()+30) {
        int64_t x = 0;  // There may be some overflow, but this shouldn't affect relative sorting.
        for (int i = 0; i < curc.size(); i++) x += curs[curs.size()-curc.size()+i] * curc[i];
        curs.push_back(x);
      }
      curs.erase(curs.begin(), curs.begin()+curc.size());
      saved.push_back({curs, curc, cura});
    }
    return base;
  }
  for (auto x : seq) if (x <= 0) return empty;

  if (seq.size() >= 2) {
    vector<int64_t> seq2 = seq, prev2 = prev;
    seq2.pop_back(); prev2.pop_back();
    auto it = memo.find({seq2, prev2});
    if (it == memo.end() || !it->second.count(seq.back())) return empty;
  }

  auto [it, inserted] = memo.insert({{seq, prev}, empty});
  map<int64_t,int64_t>& ret = it->second;
  if (save) { ret.clear(); inserted = true; }
  if (!inserted) return ret;

  prev.insert(prev.begin(), 0);
  for (int64_t c = 1;   c <= seq[0]; c++)
  for (int64_t a = 1; a*c <= seq[0]; a++) {
    prev[0] = a;
    for (int i = 0; i < seq.size(); i++) seq[i] -= prev[i]*c;
    int64_t tmp = prev.back();
    prev.pop_back();

    if (save) { curc.insert(curc.begin(), c); cura.insert(cura.begin(), a); }
    for (auto [v, n] : count(seq, prev, save)) ret[v + tmp*c] += n;
    if (save) { curc.erase(curc.begin()); cura.erase(cura.begin()); }

    prev.push_back(tmp);
    for (int i = 0; i < seq.size(); i++) seq[i] += prev[i]*c;
  }
  return ret;
}

int main() {
  int64_t N;
  while (cin >> N) {
    memo.clear(); cura.clear(); curc.clear(); saved.clear();

    vector<int64_t> seq;
    for (int64_t n = 1; ; n++) {
      if (count1(n) < N) N -= count1(n); else { seq.push_back(n); break; }
    }
    while (seq.size() < 30 && seq.back() < 1e16) {
      auto m = count(seq, seq, false);
      int64_t tot = 0;
      for (auto [v, n] : m) {
        if (n < N) {
          N -= n;
        } else {
          seq.push_back(v);
          if (n <= 20) goto done;  // Small enough to brute force.
          break;
        }
      }
    }
done:

    count(seq, seq, true);
    sort(saved.begin(), saved.end());
    auto [sv, cv, av] = saved[N-1];
    cout << cv.size() << endl;
    for (int i = 0; i < cv.size(); i++) { if (i) cout << ' '; cout << cv[i]; }
    cout << endl;
    for (int i = 0; i < av.size(); i++) { if (i) cout << ' '; cout << av[i]; }
    cout << endl;
    for (int i = 0; i < 10; i++) { if (i) cout << ' '; cout << sv[i]; }
    cout << endl;
  }
}
