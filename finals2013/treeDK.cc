#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Node;
vector<Node*> allnodes;

struct Node {
  char ch;
  Node* l, * r;
  Node(char ch, Node* l, Node* r) : ch(ch), l(l), r(r) {
    allnodes.push_back(this);
  }
  Node() : ch(0), l(NULL), r(NULL) {}
};
Node BadNode;

string prePrint(Node* nd) {
  if (nd == NULL) return "";
  return nd->ch + prePrint(nd->l) + prePrint(nd->r);
}
string inPrint(Node* nd) {
  if (nd == NULL) return "";
  return inPrint(nd->l) + nd->ch + inPrint(nd->r);
}
string postPrint(Node* nd) {
  if (nd == NULL) return "";
  return postPrint(nd->l) + postPrint(nd->r) + nd->ch;
}

string func[3] = {"Pre", "In", "Post"};
int fperm[6] = {0, 0, 1, 1, 2, 2};

map<pair<pair<string, string>, string>, Node*> memo;
Node* doit(int f1, const string& s1,
           int f2, const string& s2,
           int f3, const string& s3) {
  if (s1 == "" && s2 == "" && s3 == "") {
    return NULL;
  }

  Node* ret = &BadNode;
  if (s1 == "") f1 = 3;
  if (s2 == "") f2 = 3;
  if (s3 == "") f3 = 3;
  if (f1 == f2 && s1 != s2) return ret;
  if (f1 == f3 && s1 != s3) return ret;
  if (f2 == f3 && s2 != s3) return ret;
  int sz = s1.size();
  if (sz == 0) sz = s2.size();
  if (sz == 0) sz = s3.size();
  if (s2.size() && sz != s2.size()) return ret;
  if (s3.size() && sz != s3.size()) return ret;

  string s[4];
  s[f1] = s1; s[f2] = s2; s[f3] = s3;
  Node*& m = memo[make_pair(make_pair(s[0], s[1]), s[2])];
  if (m != NULL) return m;

  for (int ls = 0; ls <= sz-1; ls++) {
    char ch = 0;
    if (s[0] != "") ch = s[0][0];
    if (s[1] != "") {
      char ch2 = s[1][ls];
      if (ch && ch != ch2) continue;
      ch = ch2;
    }
    if (s[2] != "") {
      char ch2 = s[2][sz-1];
      if (ch && ch != ch2) continue;
      ch = ch2;
    }
    Node* lv = doit(fperm[0], s[0] == "" ? "" : s[0].substr(1, ls),
                    fperm[2], s[1] == "" ? "" : s[1].substr(0, ls),
                    fperm[4], s[2] == "" ? "" : s[2].substr(0, ls));
    if (lv == &BadNode) continue;
    Node* rv = doit(fperm[1], s[0] == "" ? "" : s[0].substr(1+ls),
                    fperm[3], s[1] == "" ? "" : s[1].substr(1+ls),
                    fperm[5], s[2] == "" ? "" : s[2].substr(ls, sz-ls-1));
    if (rv == &BadNode) continue;

    Node* cur = new Node(ch, lv, rv);
    if (ret == &BadNode || prePrint(cur) < prePrint(ret) ||
        prePrint(cur) == prePrint(ret) && inPrint(cur) < inPrint(ret)) {
      ret = cur;
    }
  }

  return m = ret;
}

main() {
  string s0, s1, s2;
  while (cin >> s0 >> s1 >> s2) {
    do {
      memo.clear();
      Node* ret = doit(0, s0, 1, s1, 2, s2);
      if (ret != &BadNode) {
        for (int j = 0; j < 6; j++) {
          cout << func[fperm[j]];
          if (j < 5) cout << ' '; else cout << endl;
        }
        cout << prePrint(ret) << endl;
        cout << inPrint(ret) << endl;
        cout << postPrint(ret) << endl;
      }
      for (int i = 0; i < allnodes.size(); i++) delete allnodes[i];
      allnodes.clear();
    } while (next_permutation(fperm, fperm+6));
  }
}
