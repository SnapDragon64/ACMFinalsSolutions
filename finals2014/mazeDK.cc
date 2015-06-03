#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#define RND1 0x195a82bb394827f9LL
#define RND2 0x4829b9a829471ac3LL
#define RND3 0x1528de2948920182LL
#define RND4 0x11a39b8f30abbc51LL
#define RND5 0xa59b4110247e8ff3LL
#define RND6 0x498d73f8100294ebLL
#define DEPTH_MUL 5

main() {
  int N;
  while (cin >> N) {
    vector<vector<int> > c(N+1), cidx(N+1);
    for (int i = 1; i <= N; i++) {
      int K, x;
      cin >> K;
      for (int j = 0; j < K; j++) {
        cin >> x;
        c[i].push_back(x);
      }
    }
    for (int i = 1; i <= N; i++)
    for (int j = 0; j < c[i].size(); j++)
    for (int k = 0; k < c[c[i][j]].size(); k++) {
      if (c[c[i][j]][k] == i) cidx[i].push_back(k);
    }

    // Seed with a depth-0 hash value based on the corridor count.
    vector<vector<long long> > hash(N+1);
    for (int i = 1; i <= N; i++) {
      long long h = RND1;
      for (int j = 0; j <= c[i].size(); j++) h = (h * RND2) ^ RND3;
      for (int j = 0; j < c[i].size(); j++) hash[i].push_back(h);
    }

    // Compute higher-depth hashes up to a multiple of N.
    for (int depth = 0; depth < DEPTH_MUL * N; depth++) {
/*cout << "Level " << depth << ":" << endl;
for (int i = 1; i <= N; i++) {
  cout << i << " -";
  for (int j = 0; j < hash[i].size(); j++) cout << ' ' << hash[i][j];
  cout << endl;
}
cout << endl;*/
      vector<vector<long long> > newhash(N+1);
      for (int i = 1; i <= N; i++) if (c[i].size()) {
        // Use a rolling hash-of-hashes.
        long long pow = RND4, curhh = 0;
        for (int j = c[i].size()-1; j >= 0; j--) {
          curhh += pow * (hash[c[i][j]][cidx[i][j]]%RND6);
          pow *= RND5;
        }
        int baseidx = 0, basehh = curhh;
        for (int j = 0; j < c[i].size(); j++) {
          curhh = curhh * RND5 + (RND4-pow) * (hash[c[i][j]][cidx[i][j]]%RND6);
          newhash[i].push_back(curhh);
        }
      }
      hash.swap(newhash);
    }

    // Sort the hashes for each room and look for matches.
    for (int i = 1; i <= N; i++) {
      sort(hash[i].begin(), hash[i].end());
      if (hash[i].size() == 0) hash[i].push_back(RND1);
    }
    vector<int> seen(N+1);
    bool sets = false;
    for (int i = 1; i <= N; i++) if (!seen[i]) {
      bool foundset = false;
      for (int j = i+1; j <= N; j++) if (!seen[j]) {
        int x = 0, y = 0;
        while (x < hash[i].size() && y < hash[j].size()) {
          if (hash[i][x] == hash[j][y]) {
            if (!foundset) cout << i;
            foundset = sets = true;
            seen[j] = true;
            cout << ' ' << j;
            break;
          }
          if (hash[i][x] < hash[j][y]) x++; else y++;
        }
      }
      if (foundset) cout << endl;
    }
    if (!sets) cout << "none" << endl;
  }
}
