#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

// WARNING TO ANYONE "LEARNING" FROM THIS SOLUTION: I really don't understand
// LP and the Simplex method very well.  This code is probably utter crap.

#define EPS 1e-6

int OneInColumn(const vector<vector<double>>& T, int c, int sr) {
  int one = -1;
  for (int r = sr; r < T.size(); r++) {
    if (one == -1 && fabs(T[r][c]-1) < EPS) {
      one = r;
    } else if (fabs(T[r][c]) > EPS) {
      return -1;
    }
  }
  return one;
}

void Pivot(vector<vector<double>>& T, int r, int c) {
  double fact = T[r][c];
  for (int c2 = 0; c2 < T[r].size(); c2++) T[r][c2] /= fact;
  for (int r2 = 0; r2 < T.size(); r2++) if (r2 != r) {
    double fact = T[r2][c];
    if (fabs(fact) < EPS) continue;
    for (int c2 = 0; c2 < T[r2].size(); c2++) T[r2][c2] -= T[r][c2] * fact;
  }
}

// Minimize row 0 of the tableau T.
double SimplexMethod(vector<vector<double>>& T) {
  // Pricing out.
  vector<int> rtoc(T.size(), -1);
  for (int c = 1; c+1 < T[0].size(); c++) {
    int one = OneInColumn(T, c, 1);
    if (one == -1 || rtoc[one] != -1) continue;
    rtoc[one] = c;
    Pivot(T, one, c);
  }

  /*for (int r = 0; r < T.size(); r++) {
  for (int c = 0; c < T[0].size(); c++) printf("%8.3lf", T[r][c]);
  printf("\n");
  }
  cout << endl;*/
  for (;;) {
    // Use Bland's rule.
    int c;
    for (c = 1; c+1 < T[0].size(); c++) if (T[0][c] > EPS) break;
    if (c+1 >= T[0].size()) break;

    int bestr = -1, bestc = -1;
    double bestratio = 1e200;
    for (int r = 1; r < T.size(); r++) if (T[r][c] > EPS) {
      double ratio = T[r].back() / T[r][c];
      // Use second part of Bland's rule.
      if (ratio < bestratio-EPS || ratio < bestratio+EPS && rtoc[r] < bestc) {
        bestratio = ratio;
        bestr = r;
        bestc = rtoc[r];
      }
    }
    if (bestr == -1) return -1e200;  // Unbounded.

    Pivot(T, bestr, c);
    rtoc[bestr] = c;
    /*for (int r = 0; r < T.size(); r++) {
    for (int c = 0; c < T[0].size(); c++) printf("%8.3lf", T[r][c]);
    printf("\n");
    }
    cout << endl;*/
  }

  return T[0].back();
}

// Minimize c'x subject to Ax = b and x >= 0.  b must be >= 0.
double LinearProgramming(const vector<vector<double>>& A,
                         const vector<double>& b,
                         const vector<double>& c) {
  // Add artificial variables and solve for initial state.
  vector<vector<double>> T(1);
  T[0].push_back(1.0);
  for (int i = 0; i < c.size(); i++) T[0].push_back(0.0);
  for (int i = 0; i < A.size(); i++) T[0].push_back(-1.0);
  T[0].push_back(0.0);
  for (int i = 0; i < A.size(); i++) {
    T.push_back(vector<double>(T[0].size()));
    for (int j = 0; j < c.size(); j++) T[i+1][j+1] = A[i][j];
    T[i+1][c.size()+1+i] = 1.0;
    T[i+1].back() = b[i];
  }
  if (SimplexMethod(T) > EPS) return 1e200;  // No solution.

  // An artifical variable might not have pivoted out.  In this case it's
  // set to 0 anyway, so we can pick any non-zero column to pivot to.
  for (int i = 0; i < A.size(); i++) {
    int one = OneInColumn(T, c.size()+1+i, 0);
    if (one >= 0) {
      for (int j = 0; j < c.size(); j++) if (fabs(T[one][j+1]) > EPS) {
        Pivot(T, one, j+1);
        break;
      }
    }
  }

  // Remove artificial variables and solve for objective function.
  T[0].clear();
  T[0].push_back(1.0);
  for (int i = 0; i < c.size(); i++) T[0].push_back(-c[i]);
  T[0].push_back(0.0);
  for (int i = 1; i < T.size(); i++) {
    T[i].erase(T[i].begin()+c.size()+1, T[i].begin()+T[i].size()-1);
  }
  return SimplexMethod(T);
}

vector<int> GetRoute(const vector<vector<int>>& C,
                     const vector<vector<int>>& D, int s, int e) {
  vector<int> ret(1, s);
  while (s != e) {
    int next = -1;
    for (int i = 0; i < C.size(); i++) if (C[s][i] > 0) {
      if (C[s][i] + D[i][e] == D[s][e]) {
        assert(next == -1);
        next = i;
      }
    }
    assert(next != -1);
    ret.push_back(next);
    s = next;
  }
  return ret;
}

int main() {
  int NC, NE = 0, NR, NQ;
  cin >> NC;

  vector<vector<int>> C(NC, vector<int>(NC, -1));
  vector<vector<int>> D(NC, vector<int>(NC, -1));
  vector<vector<int>> E(NC, vector<int>(NC, -1));
  vector<int> EC;
  for (int x = 0; x < NC; x++)
  for (int y = 0; y < NC; y++) {
    cin >> C[x][y];
    if (C[x][y] > 0) {
      E[x][y] = NE++;
      EC.push_back(C[x][y]);
    }
  }
  D = C;
  for (int k = 0; k < NC; k++)
  for (int i = 0; i < NC; i++)
  for (int j = 0; j < NC; j++) {
    if (D[i][k] != -1 && D[k][j] != -1) {
      if (D[i][j] == -1 || D[i][k] + D[k][j] < D[i][j]) {
        D[i][j] = D[i][k] + D[k][j];
      }
    }
  }

  cin >> NR;
  vector<vector<double>> a(NE+NR, vector<double>(2*NE));
  vector<double> b(NE+NR);
  for (int i = 0; i < NE; i++) {
    a[i][i] = a[i][i+NE] = 1.0;
    b[i] = EC[i];
  }
  for (int i = 0; i < NR; i++) {
    int s, e, t;
    cin >> s >> e >> t;
    assert(D[s][e] != -1);
    vector<int> route = GetRoute(C, D, s, e);
    for (int j = 0; j+1 < route.size(); j++) {
      int e = E[route[j]][route[j+1]];
      a[NE+i][e] = 1.0;
    }
    b[NE+i] = t - D[s][e];
  }

  cin >> NQ;
  for (int i = 0; i < NQ; i++) {
    int s, e;
    cin >> s >> e;
    printf("%d %d ", s, e);
    vector<int> route = GetRoute(C, D, s, e);
    vector<double> c(2*NE);
    for (int j = 0; j+1 < route.size(); j++) {
      int e = E[route[j]][route[j+1]];
      c[e] = 1.0;
    }
    printf("%.10lf ", D[s][e] + LinearProgramming(a, b, c));
    for (int j = 0; j+1 < route.size(); j++) {
      int e = E[route[j]][route[j+1]];
      c[e] = -1.0;
    }
    printf("%.10lf\n", D[s][e] - LinearProgramming(a, b, c));
  }
}
