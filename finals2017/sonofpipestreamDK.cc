#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

#define MAXN 200

int N, P, cap[MAXN+1][MAXN+1], cap2[MAXN+1][MAXN+1], cap3[MAXN+1][MAXN+1];
double V, A, B, wflow[MAXN+1][MAXN+1], fflow[MAXN+1][MAXN+1];

int seen[MAXN+1];
int flow_dfs(int c[MAXN+1][MAXN+1], int x, int e, int flow) {
  if (x == e) return flow;
  if (seen[x]) return 0;
  seen[x] = true;
  for (int y = 1; y <= N; y++) if (c[x][y]) {
    int f = flow_dfs(c, y, e, min(flow, c[x][y]));
    if (f) {
      c[x][y] -= f;
      c[y][x] += f;
      return f;
    }
  }
  return 0;
}

int flow(int c[MAXN+1][MAXN+1], int x, int e) {
  memset(seen, 0, sizeof(seen));
  return flow_dfs(c, x, e, 1000000000);
}

double flubber_dfs(int x, int e, double flubber) {
  seen[x] = true;
  if (x == e) return flubber;
  for (int y = 1; y <= N && flubber > 1e-7; y++) {
    if (!seen[y] && wflow[x][y] > 1e-7) {
      double f = flubber_dfs(y, e, min(wflow[x][y], flubber));
      if (f > 0) {
        wflow[x][y] -= f; wflow[y][x] += f;
        fflow[x][y] += f; fflow[y][x] -= f;
        return f;
      }
    }
  }
  return 0;
}

int main() {
  while (cin >> N >> P >> V >> A) {
    B = 1.0-A;
    memset(cap, 0, sizeof(cap));
    vector<int> X(P), Y(P), C(P);
    for (int i = 0; i < P; i++) {
      cin >> X[i] >> Y[i] >> C[i];
      cap[X[i]][Y[i]] = cap[Y[i]][X[i]] = C[i];
    }

    memcpy(cap2, cap, sizeof(cap));
    memcpy(cap3, cap, sizeof(cap));
    int mnF = 0, mnW = 0, mxF = 0, mxW = 0, f;
    while ((f = flow(cap, 1, 3))) mxF += f;
    while ((f = flow(cap, 2, 3))) mnW += f;
    while ((f = flow(cap2, 2, 3))) mxW += f;
    while ((f = flow(cap2, 1, 3))) mnF += f;

    f = mxF + mnW;
    // Maximum of (x/V)^A * (f-x)^B lies at A/V * (f-x) - B * x/V = 0, or
    // F = Af / (A+B)
    double F = max((double)mnF, min((double)mxF, A*f / (double)(A+B)));
    double v1, v2;
    if (mnF == mxF) {
      v1 = v2 = 0.5;
    } else {
      v1 = (F-mnF)/(mxF-mnF);
      v2 = (mxF-F)/(mxF-mnF);
    }
    for (int x = 1; x <= N; x++)
    for (int y = 1; y <= N; y++) {
      wflow[x][y] = cap3[x][y] - (v1*cap[x][y] + v2*cap2[x][y]);
    }
    memset(fflow, 0, sizeof(fflow));
    for (double rem = F; rem > 1e-7; ) {
      memset(seen, 0, sizeof(seen));
      rem -= flubber_dfs(1, 3, rem);
    }

    for (int i = 0; i < P; i++) {
      printf("%.9lf ",  fflow[X[i]][Y[i]] / V);
      printf("%.9lf\n", wflow[X[i]][Y[i]]);
    }
    printf("%.9lf\n", pow(F/V, A) * pow(f-F, B));
  }
}
