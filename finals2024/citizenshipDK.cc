#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  vector<int> months{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  vector<int> cm{0};
  for (int i = 0; i < 12; i++) cm.push_back(cm.back() + months[i]);

  int N, Y, D;
  while (cin >> N >> Y >> D) {
    vector<int> out(365*7200), years(365*7200);
    int last = 0;
    for (int i = 0; i < N; i++) {
      int y1, m1, d1, y2, m2, d2;
      scanf("%d-%d-%d %d-%d-%d", &y1, &m1, &d1, &y2, &m2, &d2);
      int v1 = (y1+1100)*365 + cm[m1-1] + d1-1;
      int v2 = (y2+1100)*365 + cm[m2-1] + d2-1;
      for (int v = v1; v <= v2; v++) out[v] = 1;
      last = v2;
    }
    for (int v = 365, cur = 365; ; cur += !out[v] - !out[v-365], v++) if (cur >= D) {
      years[v] = years[v-365] + 1;
      if (v > last && years[v] >= Y) {
        int m = 0;
        while (cm[m+1] <= v%365) m++;
        printf("%04d-%02d-%02d\n", v/365-1100, m+1, v%365-cm[m]+1);
        break;
      }
    }
  }
}
