// NOTE: The meat of this problem is in the precomputation.  See other source file.

#include <iostream>
#include <vector>
using namespace std;

vector<int> sol[8] = {
  {0},
  {0},
  {0, 0, 0, 0, 4,
     6, 6, 6, 6},
  {0, 0, 0,
     4},
  {0, 0, 4, 6,
     8, 12, 14, 18, 20, 22, 24, 28, 30, 34},
  {0, 0, 6, 8, 12, 12, 18, 22, 24, 28, 32, 36,
     38},
  {0, 0, 6, 10, 14, 18, 24, 26, 32, 36, 42, 44, 48, 54, 58, 62, 66, 72,
   74, 80, 84, 88, 94, 98, 100, 106, 112, 114, 118, 124, 128, 130,
     136, 140, 144, 148, 154, 158, 162, 166, 170, 176, 180, 184, 188, 192, 196, 200, 204,
     210, 214, 218, 222, 226, 232, 236, 240, 244, 248, 254, 256, 260, 266, 270, 274},
  {0, 0, 6, 12, 18, 22, 26, 32, 36, 42, 46, 52, 58,
     64, 70, 76, 80, 88, 92},
};

int rowPeriod[8] =    {1, 1, 4, 1, 10, 1,  33, 6};
int squarePeriod[8] = {0, 0, 4, 2, 26, 4, 142, 36};

int main() {
  long long M, N;
  while (cin >> M >> N) {
    M--; N--;
    if (N >= sol[M].size()) {
      long long rep = (N-sol[M].size()) / rowPeriod[M] + 1;
      cout << sol[M][N-rep*rowPeriod[M]] + rep*squarePeriod[M] << endl;
    } else {
      cout << sol[M][N] << endl;
    }
  }
}
