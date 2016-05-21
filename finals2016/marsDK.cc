#include <iostream>
#include <vector>
using namespace std;

int main() {
  int T, R, S, N;
  while (cin >> T >> R) {
    vector<int> tbp(T+1), ts(T+1), tip(T+1), rpc(R+1), tdone(T+1, -1);
    vector<vector<pair<char, int>>> ti(T+1);
    for (int i = 1; i <= T; i++) {
      cin >> ts[i] >> tbp[i] >> N;
      ti[i].resize(N);
      for (int j = 0; j < N; j++) {
        cin >> ti[i][j].first >> ti[i][j].second;
        if (ti[i][j].first == 'L') {
          rpc[ti[i][j].second] = max(rpc[ti[i][j].second], tbp[i]);
        }
      }
    }

    vector<int> held_by(R+1);
    for (int curtime = 0, ndone = 0; ndone < T;) {
      vector<int> curp = tbp;
      int bestp, besti, bestambig;
      bool change;
      do {
        change = false;
        bestp = 0;
        for (int i = 1; i <= T; i++)
        if (curtime >= ts[i] && tip[i] < ti[i].size()) {
          pair<char, int>& inst = ti[i][tip[i]];
          bool blocked = false;
          if (inst.first == 'L') {
            if (held_by[inst.second]) {
              blocked = true;
              if (curp[held_by[inst.second]] < curp[i]) {
                curp[held_by[inst.second]] = curp[i];
                change = true;
              }
            }
            for (int j = 1; j <= R; j++) {
              if (held_by[j] && held_by[j] != i && rpc[j] >= curp[i]) {
                blocked = true;
                if (curp[held_by[j]] < curp[i]) {
                  curp[held_by[j]] = curp[i];
                  change = true;
                }
              }
            }
          }
          if (!blocked && curp[i] > bestp) {
            bestp = curp[i];
            besti = i;
            bestambig = false;
          } else if (!blocked && curp[i] == bestp) {
            bestambig = true;
          }
        }
      } while (change);

      if (bestp == 0) {curtime++; continue;}
      if (bestambig) cout << "AMBIGUOUS!" << endl;

      pair<char, int>& inst = ti[besti][tip[besti]];
//cout << curtime << ' ' << bestp << ' ' << besti << ' ' << inst.first << inst.second << ' ' << held_by[1] << endl;
      if (inst.first == 'C' && inst.second > 0) {
        inst.second--;
        curtime++;
      } else {
        if (inst.first == 'L') {
          held_by[inst.second] = besti;
        } else if (inst.first == 'U') {
          held_by[inst.second] = 0;
        }
        if (++tip[besti] == ti[besti].size()) {
          tdone[besti] = curtime;
          ndone++;
        }
      }
    }

    for (int i = 1; i <= T; i++) {
      cout << tdone[i] << endl;
    }
  }
}
