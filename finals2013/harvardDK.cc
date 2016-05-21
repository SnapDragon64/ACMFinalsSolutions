#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int B, S;
vector<char> ttype;
vector<int> tval, tend;
int freev[14];
long long con[15][15], totv, cur, best;

void doit(int a, int b, long long mul, int& firstv, int& lastv) {
  if (a == b) return;
  firstv = lastv = -1;
  for (int i = a; i < b; i++) {
    if (ttype[i] == 'V') {
      totv += mul;
      if (!freev[tval[i]]) {
        if (firstv == -1) firstv = tval[i];
        if (lastv != -1) con[lastv][tval[i]] += mul;
        lastv = tval[i];
      }
    } else if (ttype[i] == 'R') {
      int fv, lv;
      doit(i+1, tend[i], mul*tval[i], fv, lv);
      if (fv != -1) {
        con[lv][fv] += mul*(tval[i]-1);
        if (firstv == -1) fv = tval[i];
        if (lastv != -1) con[lastv][fv] += mul;
        lastv = lv;
      }
      i = tend[i];
    }
  }
}

main() {
  for(;;) {
    string line;
    if (!getline(cin, line)) break;
    istringstream line1(line);
    if (!(line1 >> B >> S)) break;
    if (!getline(cin, line)) break;
    istringstream line2(line);
    vector<int> rstack;
    char ch;
    ttype.clear(); tval.clear(); tend.clear();
    while (line2 >> ch) {
      ttype.push_back(ch);
      tval.push_back(0);
      tend.push_back(0);
      if (ch == 'E') {
        tend[rstack.back()] = tend.size()-1;
        rstack.pop_back();
      } else {
        line2 >> tval.back();
        if (ch == 'R') rstack.push_back(tend.size()-1);
        if (ch == 'V') tval.back()--;
      }
    }

    best = 1e18;
    for (int i = 0; i < 14; i++) freev[i] = (i >= 14-S);
    do {
      memset(con, 0, sizeof(con));
      totv = 0;
      int firstv, lastv;
      doit(0, ttype.size(), 1, firstv, lastv);

      cur = totv + (firstv != -1);
      for (int i = 0; i < 14; i++)
      for (int j = 0; j < 14; j++) if (i != j)
        cur += con[i][j];

      if (S == 1) {best = cur; continue;}
      vector<int> cliques;
      for (int i = 0; i < 14-S || (i%S); i++) cliques.push_back(i/S+1);
      do {
        bool ordered = true;
        for (int j = 0, cc = 0; j < cliques.size(); j++)
          if (cliques[j] > cc) {
            if (cliques[j] > cc+1) {ordered = false; break;}
            cc++;
          }
        if (!ordered) continue;

        long long saved = 0;
        for (int ii = 0, i = 0; ii < 14; ii++) if (!freev[ii]) {
          for (int ji = ii+1, j = i+1; ji < 14; ji++) if (!freev[ji]) {
            if (cliques[j] == cliques[i])
              saved += con[ii][ji] + con[ji][ii];
            j++;
          }
          i++;
        }
        best = min(best, cur-saved);
      } while (next_permutation(cliques.begin(), cliques.end()));
    } while (next_permutation(freev, freev+14));

    cout << best << endl;
  }
}
