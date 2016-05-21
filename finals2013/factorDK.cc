#include <iostream>
#include <map>
#include <vector>
using namespace std;

vector<int> primes;
map<long long, long long> best;
int totexp;
vector<int> exp;

void addbest(long long n) {
  //cout << n << ":";
  //for (int i = 0; i < exp.size(); i++)
  //  cout << ' ' << primes[i] << '^' << exp[i];
  //cout << endl;

  if (n == 1) return;
  vector<int> fact(totexp+1);
  for (int f = 2; f <= totexp; f++) {
    for (int x = 2, f2 = f; x <= f2; x++)
      while (f2%x == 0) {
        fact[x]++;
        f2 /= x;
      }
  }
  for (int i = 0; i < exp.size(); i++) {
    for (int f = 2; f <= exp[i]; f++) {
      for (int x = 2, f2 = f; x <= f2; x++)
        while (f2%x == 0) {
          fact[x]--;
          f2 /= x;
        }
    }
  }
  long long comb = 1;
  for (int i = 2; i <= totexp; i++)
  for (int j = 0; j < fact[i]; j++) {
    if ((double)comb*i > 1e19 || comb*i < 0) return;
    comb *= i;
  }
  long long& b = best[comb];
  if (!b || b > n) b = n;
}

void doit(int p, int e, long long n) {
  addbest(n);
  for (int i = 1; i <= e; i++) {
    if ((double)n*primes[p] > 1e19 || n*primes[p] < 0) break;
    n *= primes[p];
    exp.push_back(i); totexp += i;
    doit(p+1, i, n);
    exp.pop_back(); totexp -= i;
  }
}

main() {
  for (int x = 2; x <= 1000; x++) {
    int y;
    for (y = 2; y*y <= x; y++) if (x%y == 0) break;
    if (y*y > x) primes.push_back(x);
  }
  doit(0, 62, 1);

  long long b;
  while (cin >> b) {
    cout << b << ' ' << best[b] << endl;
  }
}
