#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

string dir = "enws";
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, -1, 0, 1};

int R, C, D, E;
vector<string> G, P(27);
vector<vector<int>> next1(27), next2(27);

void donext(int p) {
  next1[p] = next2[p] = vector<int>(P[p].size(), -1);
  for (int i = 0; i < P[p].size(); i++) if (next1[p][i] == -1) {
    if (P[p][i] == 'u') {
      int j, par;
      for (j = i+3, par = 1; par; j++) par += (P[p][j]=='(')-(P[p][j]==')');
      next2[p][i] = i+3;
      next1[p][i] = j;
      next1[p][j-1] = i;
    } else if (P[p][i] == 'i') {
      int j, k, par;
      for (j = i+3, par = 1; par; j++) par += (P[p][j]=='(')-(P[p][j]==')');
      for (k = j+1, par = 1; par; k++) par += (P[p][k]=='(')-(P[p][k]==')');
      next1[p][i] = i+3;
      next2[p][i] = j;
      next1[p][j-1] = k;
    } else {
      next1[p][i] = i+1;
    }
  }
}

struct State {
  char p, x, y, d;
  int hash() const { return (int(p)<<24)+(int(x)<<16)+(int(y)<<8)+d; }
  bool operator<(const State& s) const { return hash() < s.hash(); }
  bool operator==(const State& s) const { return hash() == s.hash(); }
};
State Inf{-1,-1,-1,-1};

map<State, State> memo;
State doit(const State& os) {
  if (os.p != 26 && memo.count(os)) return memo[os];
  memo[os] = Inf;
  State s = os;
  vector<set<State>> seen(P[s.p].size());
  for (int i = 0; i < P[s.p].size(); ) {
    if (P[s.p][i] == 'm') {
      int x2 = s.x + dx[s.d], y2 = s.y + dy[s.d];
      if (G[y2][x2] != '#') { s.x = x2; s.y = y2; }
      i = next1[s.p][i];
    } else if (P[s.p][i] == 'l') {
      s.d = ((s.d+1)&3);
      i = next1[s.p][i];
    } else if (P[s.p][i] == 'u' || P[s.p][i] == 'i') {
      if (P[s.p][i] == 'u' && !seen[i].insert(s).second) return Inf;
      if (P[s.p][i+1] == 'b') {
        i = ((G[s.y+dy[s.d]][s.x+dx[s.d]] == '#') ? next1 : next2)[s.p][i];
      } else {
        i = ((P[s.p][i+1] == dir[s.d]) ? next1 : next2)[s.p][i];
      }
    } else if (P[s.p][i] >= 'A' && P[s.p][i] <= 'Z') {
      s.p = P[s.p][i]-'A';
      s = doit(s);
      if (s == Inf) return s;
      s.p = os.p;
      i = next1[s.p][i];
    } else {
      i = next1[s.p][i];
    }
  }
  if (s.p != 26) memo[os] = s;
  return s;
}

int main() {
  while (cin >> R >> C >> D >> E) {
    G = vector<string>(R+2, string(C+2, '#'));
    memo.clear();
    for (int y = 1; y <= R; y++) for (int x = 1; x <= C; x++) cin >> G[y][x];
    for (int i = 0; i < D; i++) {
      string s;
      cin >> s;
      P[s[0]-'A'] = s.substr(2);
      donext(s[0]-'A');
    }
    for (int i = 0; i < E; i++) {
      char ch;
      int x, y;
      cin >> y >> x >> ch >> P[26];
      donext(26);
      State s = doit({26, char(x), char(y), char(dir.find(ch))});
      if (s == Inf) {
        cout << "inf" << endl;
      } else {
        cout << int(s.y) << ' ' << int(s.x) << ' ' << dir[s.d] << endl;
      }
    }
  }
}
