#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

int main() {
  string S;
  while (cin >> S) {
    S.erase(unique(S.begin(), S.end()), S.end());
    S = S.substr(0, 2+S.size()%2);
    cout << S << endl;
  }
}
