#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

const int HAND_SIZE[5] = {5, 5, 4, 4, 3};

struct State {
  int valid[5];
  char cp[21], nc[5];

  State() {
    for (int i = 0; i < 5; i++) valid[i] = (1<<21)-1;
    for (int i = 0; i < 21; i++) cp[i] = -1;
    for (int i = 0; i < 5; i++) nc[i] = 0;
  }

  void invalid(int p, int c) {
    valid[p] &= ~(1<<c);
  }
  bool select(int p, int c) {
    if ((valid[p] & (1<<c)) == 0) return false;
    if (cp[c] != -1 && cp[c] != p) return false;
    if (cp[c] == -1) {
      if (nc[p] == HAND_SIZE[p]) return false;
      nc[p]++;
    }
    cp[c] = p;
    return true;
  }
};

int seen[3], choices[100];
vector<vector<int>> options;

// Brute force beats bipartite match any day, right? :P
bool doit2(const State& st, int c) {
  if (c == 21) {
    int x = options.size();
    seen[0] |= (1<<options[x-3][choices[x-3]]);
    seen[1] |= (1<<options[x-2][choices[x-2]]);
    seen[2] |= (1<<options[x-1][choices[x-1]]);
    return true;
  }
  if (st.cp[c] != -1) return doit2(st, c+1);
  for (int p = 1; p < 5; p++) {
    State st2 = st;
    if (st2.select(p, c) && doit2(st2, c+1)) return true;
  }
  return false;
}

void doit(const State& st, int x) {
  if (x == options.size()) {
    doit2(st, 0);
    return;
  }

  int p = options[x][0];
  int& i = choices[x];
  for (i = 1; i < options[x].size(); i++) {
    int c = options[x][i];
    if (st.cp[c] == p) {
      doit(st, x+1);
      return;
    }
  }
  for (i = 1; i < options[x].size(); i++) {
    int c = options[x][i];
    State st2 = st;
    if (st2.select(p, c)) doit(st2, x+1);
  }
}

char chosen(int b) {
  if (b == 0) return 'X';
  for (int i = 0; i < 21; i++) if (b == (1<<i)) return 'A'+i;
  return '?';
}

int main() {
  int M;
  while (cin >> M) {
    State st;
    seen[0] = seen[1] = seen[2] = 0;
    options.clear();

    for (int i = 0; i < 5; i++) {
      char ch;
      cin >> ch;
      if (!st.select(0, ch-'A')) cout << "CONFLICT!" << endl;
    }
    for (int i = 0; i < M; i++) {
      int p1 = i%4;
      char ch1, ch2, ch3, ch;
      cin >> ch1 >> ch2 >> ch3;
      for (int p2 = (p1+1)%4; p2 != p1; p2 = (p2+1)%4) {
        cin >> ch;
        if (ch == '-') {
          st.invalid(p2, ch1-'A');
          st.invalid(p2, ch2-'A');
          st.invalid(p2, ch3-'A');
        } else if (ch == '*') {
          options.push_back({p2, ch1-'A', ch2-'A', ch3-'A'});
          break;
        } else {
          if (!st.select(p2, ch-'A')) cout << "CONFLICT!" << endl;
          break;
        }
      }
    }
    options.push_back({4, 0, 1, 2, 3, 4, 5});
    options.push_back({4, 6, 7, 8, 9, 10, 11});
    options.push_back({4, 12, 13, 14, 15, 16, 17, 18, 19, 20});

    doit(st, 0);
    cout << chosen(seen[0]) << chosen(seen[1]) << chosen(seen[2]) << endl;
  }
}
