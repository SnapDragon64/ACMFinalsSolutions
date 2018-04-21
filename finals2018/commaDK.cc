#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

vector<bool> comma, period;
vector<string> text;
map<string, vector<int>> wpos;

void donext(const string& w);
void doprev(const string& w) {
  static set<string> Done;
  if (Done.count(w)) return;
  Done.insert(w);
  for (auto i : wpos[w]) {
    if (i == 0 || period[i-1] || comma[i-1]) continue;
    comma[i-1] = true;
    donext(text[i-1]);
  }
}

void donext(const string& w) {
  static set<string> Done;
  if (Done.count(w)) return;
  Done.insert(w);
  for (auto i : wpos[w]) {
    if (period[i] || comma[i]) continue;
    comma[i] = true;
    if (i+1 < text.size()) doprev(text[i+1]);
  }
}

int main() {
  string s;
  while (cin >> s) {
    comma.push_back(s[s.size()-1] == ',');
    period.push_back(s[s.size()-1] == '.');
    if (comma.back() || period.back()) s = s.substr(0, s.size()-1);
    wpos[s].push_back(text.size());
    text.push_back(s);
  }
  for (int i = 0; i < text.size(); i++) if (comma[i]) {
    donext(text[i]);
    if (i+1 < text.size()) doprev(text[i+1]);
  }
  for (int i = 0; i < text.size(); i++) {
    if (i) cout << ' ';
    cout << text[i];
    if (comma[i]) cout << ',';
    if (period[i]) cout << '.';
  }
  cout << endl;
}
