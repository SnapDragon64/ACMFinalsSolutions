#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<int> jf(N+1), lf(N+1);
    vector<vector<int>> jb(N+1);
    for (int i = 1, j, l; i <= N; i++) {
      cin >> j >> l;
      jf[i] = j;
      lf[i] = l;
      jb[j].push_back(i);
    }

    // We look for a path (starting in the jealous cycle) of mixed like/jealous edges with the property: if
    // "A ->like B ->jealous* C" is part, the B-C "chain" does not include A's jealous parent.  When this forms a
    // mixed cycle (at least one like/jealous), we can safely order it by breaking one jealous edge.
    // And if, in our search, we prioritize jealous edges, then the first time "A ->like B" escapes A's parent's
    // subtree, that will be the last chain in the cycle.  Thus, if there's a predecessor to the cycle
    // "A ->like [cycle]", the entire cycle will lie in A's parent's subtree and it's safe to do it after A.
    // DFS suffices since a cycle is guaranteed except in one non-repeatable case (recursing to brother's subtree).
    vector<int> path, kPos(N+1, -1);
    int loop;
    function<bool(int,int,int)> search = [&](int i, int parent, int lastj) {
      if (kPos[i] != -1 && kPos[i] >= lastj) return false;  // No jealous edge in this cycle.
      if (kPos[i] != -1) { loop = kPos[i]; return true; }
      kPos[i] = path.size();
      path.push_back(i);
      if (jf[i] != parent && search(jf[i], parent, path.size())) return true;
      if (search(lf[i], jf[i], lastj)) return true;
      path.pop_back();
      kPos[i] = -1;
      return false;
    };

    for (int start = 1, n = 0; !n || start != 1; start = jf[start], n++) {
      if (search(start, jf[start], 0)) {
        // Warning: the logic of this part is very finicky.
        int preloop = loop;
        while (preloop > 0 && jf[path[preloop-1]] == path[preloop]) preloop--;
        if (jf[path.back()] != path[loop]) {
          // Rotate the loop so we're breaking a jealous edge.
          for (int i = path.size()-2; ; i--) if (jf[path[i]] == path[i+1]) {
            rotate(path.begin()+loop, path.begin()+i+1, path.end());
            break;
          }
        }

        vector<int> kUsed(N+1), kQueue;
        bool first = true;
        auto visit = [&](int i) {
          if (kUsed[i]) return;
          kUsed[i] = 1;
          if (!first) cout << ' ';
          first = false;
          cout << i;
          for (auto j : jb[i]) kQueue.push_back(j);
        };

        // Follow path, reversing the jealous chains.
        for (int i = 0, j, k; i < path.size(); i = j+1) {
          while (i >= preloop && i < loop) i++;
          for (j = i; j+1 < path.size() && jf[path[j]] == path[j+1];) j++;
          for (k = j; k >= i; k--) visit(path[k]);
        }
        for (int i = loop-1; i >= preloop; i--) visit(path[i]);
        for (int i = 0; i < kQueue.size(); i++) visit(kQueue[i]);
        goto done;
      }
    }

    cout << "impossible";
done:
    cout << endl;
  }
}
