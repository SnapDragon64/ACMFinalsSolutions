#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> h1, h2;
vector<string> history;
int lidx;
string s;
bool doit(int mv) {
  if (mv == 0) {
    int i;
    for (i = 0; i < s.size(); i++) if (s[i] != '.') break;
    for (; i < s.size(); i++) if (s[i] != 'A') break;
    for (; i < s.size(); i++) if (s[i] != 'B') break;
    for (; i < s.size(); i++) if (s[i] != '.') break;
    if (i == s.size()) {
      for (int j = 0; j < history.size(); j++) {
        cout << history[j] << "  " << h1[j] << " to " << h2[j] << endl;
      }
      cout << s << endl << endl;
      return true;
    }
    return false;
  }

  bool ret = false;
  history.push_back(s);
  for (int i = 0; i+1 < s.size(); i++) if (s[i] != '.' && s[i+1] != '.') {
    char c1 = s[i], c2 = s[i+1];
    s[i] = '.'; s[i+1] = '.';
    for (int j = 0; j+1 < s.size(); j++)
      if (j != i && s[j] == '.' && s[j+1] == '.') {
        s[j] = c1; s[j+1] = c2;
        h1.push_back(i+lidx); h2.push_back(j+lidx);
        ret |= doit(mv-1);
        h1.pop_back(); h2.pop_back();
        s[j] = '.'; s[j+1] = '.';
      }
    s[i] = c1; s[i+1] = c2;
  }
  history.pop_back();
  return ret;
}

void doit(int N, int add) {
  if (N == 4) {
    cout <<  6+add << " to " << -1+add << endl;
    cout <<  3+add << " to " <<  6+add << endl;
    cout <<  0+add << " to " <<  3+add << endl;
    cout <<  7+add << " to " <<  0+add << endl;
  } else if (N == 5) {
    cout <<  8+add << " to " << -1+add << endl;
    cout <<  3+add << " to " <<  8+add << endl;
    cout <<  6+add << " to " <<  3+add << endl;
    cout <<  0+add << " to " <<  6+add << endl;
    cout <<  9+add << " to " <<  0+add << endl;
  } else if (N == 6) {
    cout << 10+add << " to " << -1+add << endl;
    cout <<  7+add << " to " << 10+add << endl;
    cout <<  2+add << " to " <<  7+add << endl;
    cout <<  6+add << " to " <<  2+add << endl;
    cout <<  0+add << " to " <<  6+add << endl;
    cout << 11+add << " to " <<  0+add << endl;
  } else if (N == 7) {
    cout <<  8+add << " to " << -1+add << endl;
    cout <<  5+add << " to " <<  8+add << endl;
    cout << 12+add << " to " <<  5+add << endl;
    cout <<  3+add << " to " << 12+add << endl;
    cout <<  9+add << " to " <<  3+add << endl;
    cout <<  0+add << " to " <<  9+add << endl;
    cout << 13+add << " to " <<  0+add << endl;
  } else {
    cout << 2*N-2+add << " to " << -1+add << endl;
    cout << 3+add << " to " << 2*N-2+add << endl;
    doit(N-4, 4+add);
    cout << 0+add << " to " << 2*N-5+add << endl;
    cout << 2*N-1+add << " to " << 0+add << endl;
  }
}

main() {
  /*s = "....BABABA";       lidx = -3; doit(3);
  s = "..BABABABA";       lidx = -1; doit(4);
  s = "..BABABABABA";     lidx = -1; doit(5);
  s = "..BABABABABABA";   lidx = -1; doit(6);
  s = "..BABABABABABABA"; lidx = -1; doit(7);
  s = "..BABABABABABABABA"; lidx=-1; doit(8);*/

  int N;
  while (cin >> N) {
    if (N == 3) {
      cout << "2 to -1" << endl;
      cout << "5 to 2" << endl;
      cout << "3 to -3" << endl;
    } else {
      doit(N, 0);
    }
  }
}
