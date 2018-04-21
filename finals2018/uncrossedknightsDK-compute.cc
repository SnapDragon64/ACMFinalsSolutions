// This was code I wrote while investigating the original math problem, so it
// does a little more than the problem asked for.  In particular, it outputs
// .svg diagrams, it proves the cycle is valid (something contestants can just
// assume), and it can also handle open tours.

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Globals that define the search.
bool OpenPaths = false;
int NCol = 8, NRow = 8;

struct State {
  // (v, nends, compGone) represents the partial state for one row.
  // Positions 0, 3, 6, ... store 0 for an empty square, -1 for an unavailable square (because two edges joined
  //   there from above), or [component #] for an open edge coming from above.
  // Positions 2, 5, ... store [component #] for an open edge, going right, halfway between one square and the next.
  // Positions 1, 4, ... store [component #] for an open edge, going left, halfway between one square and the next.
  // Note that for closed paths, component #s will always show up exactly 2 times.  For open paths, up to two
  //   component #s can show up only 1 time.
  vector<char> v;

  // For open paths, stores how many path endpoints we've created so far, from 0 up to 2.
  char nends;

  // Whether a component was unrecoverably closed off (or its edges abandoned) at or before this row.
  // This will happen exactly once for a proper path, open or closed.
  bool compGone;

  // Whether we've generated all successors of this state yet.
  bool succGenerated;

  // For convenience, store number of open components in this state.
  char ncomp;

  // Each distinguished state gets a unique ID.
  // State 0 is the "end" state with no squares visited.  All compGone states have this as a successor.
  // State 1 is the "start" state with no squares visited (but no component closed off yet).
  int id;

  // Successor state IDs, and how many new knight edges are added in the transition.  Generated lazily.
  vector<pair<int, int>> succ;


  static vector<State*> States;

  // Takes ownership of st; returns a normalized copy with an ID.
  static const State* StoreState(State* st) {
    struct StateCmp {
      bool operator()(const State* st1, const State* st2) const {
        if (st1->nends != st2->nends) return st1->nends < st2->nends;
        if (st1->compGone != st2->compGone) return st1->compGone < st2->compGone;
        return st1->v < st2->v;
      }
    };
    static set<const State*, StateCmp> StateSet;

    pair<decltype(StateSet)::const_iterator, bool> res = StateSet.insert(st);
    const State* newst = *res.first;
    if (res.second) {
      st->id = States.size();
      States.push_back(st);
      if (st->id % 1000 == 0) {
        cout << "  Generated " << st->id << " states." << endl;
      }
    } else {
      delete st;
    }
    return newst;
  }


  // Runs along the row v (at x1) making choices of edges to connect to row v2 (at >= x2).
  // Recursively makes all possible choices to generate all possible successor states.
  // Note that edges will always be added in increasing order in both x1 and x2, since they can't cross.
  // So x2 is always the rightmost such coordinate of any edge added so far.
  // v2 is modified in place, where possible, for efficiency.
  static void genSucc(vector<char>& v, int x1, vector<char>& v2, int x2,
                      int nends, int compgone, int nsq, 
                      pair<int, int>* lines, function<void(const State*, int)> found_func) {
    if (x1 == v.size()) {
      State* nst = new State();
      nst->nends = nends;
      nst->compGone = compgone;

      // Normalize components so they're ordered by leftmost edge.
      nst->ncomp = 0;
      nst->v = v2;
      for (int i = 0; i < nst->v.size(); i++) {
        if (nst->v[i] > nst->ncomp) {
          nst->ncomp++;
          int n1 = nst->v[i], n2 = nst->ncomp;
          if (n1 != n2) for (int j = i; j < nst->v.size(); j++) {
            if (nst->v[j] == n1) nst->v[j] = n2;
            else if (nst->v[j] == n2) nst->v[j] = n1;
          }
        }
      }

      // If a component disappeared without connecting to anything else, we'd
      // better only have done it once, and there can't be any loose ends.
      if (compgone > 0) {
        assert(compgone == 1);
        if (nst->ncomp > 0 || nends < 2) return;
      }

      found_func(StoreState(nst), nsq);
      return;
    }

    if (v[x1] == 0 && x1%3 == 0) {
      // We're at an empty square.
      // Option 1: do nothing.
      genSucc(v, x1+1, v2, x2, nends, compgone, nsq, lines, found_func);
      // Option 2: open a new endpoint.
      if (OpenPaths && nends < 2) {
        for (int nx = x1-6; nx <= x1+6; nx += 4) {
          tryEdge(v, x1, v2, x2, nx, -1, nends+1, compgone, nsq+1, lines, found_func);
        }
      }
      // Option 3: start two new edges together.
      for (int nx = x1-6; nx < x1+6; nx += 4)
      for (int nx2 = nx+4; nx2 <= x1+6; nx2 += 4) {
        tryEdge(v, x1, v2, x2, nx, nx2, nends, compgone, nsq+1, lines, found_func);
      }
    } else if (v[x1] > 0 && x1%3 == 0) {
      // We're at a square with an edge coming in from above.
      // Option 1: don't continue, let this be an endpoint.
      if (nends < 2) {
        // Check if the component is disappearing.
        bool seen = false;
        for (int i = 0; i <= x2; i++) if (v2[i] == v[x1]) seen = true;
        for (int i = x1+1; i < v.size(); i++) if (v[i] == v[x1]) seen = true;
        if (compgone+!seen <= 1) {
          genSucc(v, x1+1, v2, x2, nends+1, compgone+!seen, nsq+1, lines, found_func);
        }
      }
      // Option 2: add new edge going down.
      for (int nx = x1-6; nx <= x1+6; nx += 4) {
        tryEdge(v, x1, v2, x2, nx, -1, nends, compgone, nsq+1, lines, found_func);
      }
    } else if (v[x1] > 0 && x1%3 == 1) {
      // Must continue half-edge leftwards.
      tryEdge(v, x1, v2, x2, x1-1, -1, nends, compgone, nsq, lines, found_func);
    } else if (v[x1] > 0 && x1%3 == 2) {
      // Must continue half-edge rightwards.
      tryEdge(v, x1, v2, x2, x1+1, -1, nends, compgone, nsq, lines, found_func);
    } else {
      // Can't do anything at this position.
      genSucc(v, x1+1, v2, x2, nends, compgone, nsq+(v[x1]==-1), lines, found_func);
    }
  }

  static void tryEdge(vector<char>& v, int x1, vector<char>& v2, int x2, int nx, int nx2,
                      int nends, int compgone, int nsq,
                      pair<int, int>* lines, function<void(const State*, int)> found_func) {
    if (nx < 0 || nx >= v.size()) return;
    if (compgone > 0) return;  // Can't have any other edges if a component was closed off.
    int c1 = v[x1];
    if (c1 == 0) c1 = v.size()+1+x1;  // New component starts here.
    if (nx < x2) {
      return;  // Edges cross.
    } else if (nx == x2) {
      // Edges meet at this point.
      int c2 = v2[x2];
      if (c2 == -1) return;  // Already used square.
      // Join up the two components that are now one.
      char* c2p = NULL;
      bool seen = (nx2 != -1);
      for (int i = 0; i < x2; i++) {
        if (v2[i] == c1) seen = true;
        else if (v2[i] == c2) { c2p = &v2[i]; seen = true; }
      }
      for (int i = x1+1; i < v.size(); i++) {
        if (v[i] == c1) seen = true;
        else if (v[i] == c2) { c2p = &v[i]; seen = true; }
      }
      if (c2p) *c2p = c1;
      v2[x2] = -1;
      if (lines) { lines->first = x1; lines->second = nx; lines++; }
      if (nx2 != -1) {
        tryEdge(v, x1, v2, nx, nx2, -1, nends, compgone+!seen, nsq, lines, found_func);
      } else {
        genSucc(v, x1+1, v2, nx, nends, compgone+!seen, nsq, lines, found_func);
      }
      if (lines) { lines->first = lines->second = 0; lines--; }
      v2[x2] = c2;
      if (c2p) *c2p = c2;
    } else {
      v2[nx] = c1;
      if (lines) { lines->first = x1; lines->second = nx; lines++; }
      if (nx2 != -1) {
        tryEdge(v, x1, v2, nx, nx2, -1, nends, compgone, nsq, lines, found_func);
      } else {
        genSucc(v, x1+1, v2, nx, nends, compgone, nsq, lines, found_func);
      }
      if (lines) { lines->first = lines->second = 0; lines--; }
      v2[nx] = 0;
    }
  }

  // Generates succ if necessary.
  void GenerateSucc() {
    if (succGenerated) return;
    succGenerated = true;
    vector<char> vc = v, v2(v.size());
    genSucc(vc, 0, v2, -1, nends, compGone, 0, NULL, [&] (const State* st, int nsq) {
      succ.emplace_back(st->id, nsq);
    });
  }

  // Returns x indices of all lines going from this state's row to the given successor state's row.
  vector<pair<int, int>> GetLines(int succ_idx) const {
    vector<pair<int, int>> lines(2*v.size()), ret;
    vector<char> vc = v, v2(v.size());
    genSucc(vc, 0, v2, -1, nends, compGone, 0, &lines[0], [&] (const State* st, int nsq) {
      if (succ_idx-- == 0) ret = lines;
    });
    while (ret.size() && ret.back().first == 0 && ret.back().second == 0) ret.pop_back();
    return ret;
  }


  State() : id(0), ncomp(0), succGenerated(false) {}

  static void Init() {
    assert(States.size() == 0);
    State* end = new State();
    end->v.resize(NCol*3-2);
    end->nends = 2;
    end->compGone = true;
    StoreState(end);
    State* start = new State();
    start->v.resize(NCol*3-2);
    start->nends = OpenPaths ? 0 : 2;
    start->compGone = false;
    StoreState(start);
  }

  // Returns 3 strings with an ASCII representation of the board between this row and the successor's row.
  // The first 2 strings are fully filled-in, but note that the successor's string may be incomplete, because
  // its state doesn't include lines going further down.
  // '.': unused chessboard square
  // '`': one line coming in from above
  // ',': one line coming in from below
  // 'v': two lines coming in from above
  // '^': two lines coming in from below
  // '|': one line above, one below
  // '/': midpoint of a line, going (roughly) this direction
  // '\': midpoint of a line, going (roughly) this direction
  vector<string> AsciiRow(int succ_idx) const {
    vector<pair<int, int>> lines = GetLines(succ_idx);
    vector<string> ret(3, string(NCol*2-1, ' '));
    for (int i = 0; i < ret[0].size(); i += 2) ret[0][i] = ret[2][i] = '.';
    for (int i = 0; i < v.size(); i += 3) {
      if (v[i] == -1) ret[0][i/3*2] = 'v';
      else if (v[i]) ret[0][i/3*2] = '`';
    }
    for (int i = 0; i < lines.size(); i++) {
      int x = lines[i].first, x2 = lines[i].second;
      if (x%3 == 0) {
        if (ret[0][x/3*2] == '`') ret[0][x/3*2] = '|';
        else if (ret[0][x/3*2] == ',') ret[0][x/3*2] = '^';
        else ret[0][x/3*2] = ',';
      } else {
        if (x%3 == 1) ret[0][x/3*2+1] = '/';
        else ret[0][x/3*2+1] = '\\';
      }
      if (x2%3 == 0) {
        if (ret[2][x2/3*2] == '`') ret[2][x2/3*2] = 'v';
        else ret[2][x2/3*2] = '`';
      } else {
        if (x2%3 == 1) ret[2][x2/3*2+1] = '/';
        else ret[2][x2/3*2+1] = '\\';
      }
      if (x2 == x+6) ret[1][(x+3)/3*2] = '\\';
      if (x2 == x-6) ret[1][(x-3)/3*2] = '/';
    }
    return ret;
  }

  string ToString() const {
    ostringstream sout;
    sout << '{';
    for (int i = 0; i < v.size(); i += 3) {
      sout << (int)v[i];
      if (i+1 < v.size()) {
        sout << '<' << (int)v[i+1] << ',' << (int)v[i+2] << '>';
      }
    }
    sout << " ends=" << (int)nends;
    if (compGone) sout << " done";
    sout << '}';
    return sout.str();
  }

  friend ostream& operator<<(ostream& out, const State& st) {
    out << st.ToString();
    return out;
  }
};

vector<State*> State::States;


void GenerateAllStates() {
  for (int i = 0; i < State::States.size(); i++) State::States[i]->GenerateSucc();
  cout << "Total: " << State::States.size() << " states." << endl;
}

void PrintAllSucc() {
  for (int i = 0; i < State::States.size(); i++) {
    cout << "Successors for #" << i << " " << *State::States[i] << ":" << endl << endl;
    for (int j = 0; j < State::States[i]->succ.size(); j++) {
      vector<string> v = State::States[i]->AsciiRow(j);
      cout << '|' << v[0] << '|' << endl;
      cout << '|' << v[1] << "| +" << State::States[i]->succ[j].second;
      if (State::States[State::States[i]->succ[j].first]->compGone) cout << " done";
      cout << endl;
      cout << '|' << v[2] << '|' << endl;
      cout << endl;
    }
  }
}

// Prints the chessboard with the given solution.
// States should satisfy id[0] == 1, id[nrows] == 0, with each succ[idx[i]] pointing to id[i+1].
void PrintOneSolution(const vector<int>& id, const vector<int>& idx) {
  cout << ' ' << string(NCol*2-1, '-') << ' ' << endl;
  for (int i = 0; i+1 < id.size(); i++) {
    vector<string> v = State::States[id[i]]->AsciiRow(idx[i]);
    cout << '|' << v[0] << '|' << endl;
    if (i+2 < id.size()) cout << '|' << v[1] << '|' << endl;
  }
  cout << ' ' << string(NCol*2-1, '-') << ' ' << endl;
}

// Outputs the given solution to the file "open[col]x[row].svg".
// If specified, highlights rows minHL to maxHL.
void WriteSolutionSVG(const vector<int>& id, const vector<int>& idx, int minHL = -1, int maxHL = -1) {
  int x = NCol, y = id.size()-1;
  int sq = 15;  // Size of a square, in pixels

  char fn[100];
  sprintf(fn, "%s%dx%d.svg", OpenPaths ? "open" : "closed", x, y);
  ofstream fout(fn);

  fout << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
  fout << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";
  fout << "  <pattern id=\"checkerboard\" x=\"5\" y=\"5\"";
  fout <<           " width=\"" << sq*2 << "\" height=\"" << sq*2 << "\" patternUnits=\"userSpaceOnUse\">\n";
  fout << "    <rect fill=\"rgba(159, 188, 191, 0.4)\" x=\"0\" y=\"0\"";
  fout <<          " width=\"" << sq << "\" height=\"" << sq << "\"/>\n";
  fout << "    <rect fill=\"rgba(159, 188, 191, 0.4)\" x=\"" << sq << "\" y=\"" << sq << "\"";
  fout <<          " width=\"" << sq << "\" height=\"" << sq << "\"/>\n";
  fout << "  </pattern>\n";
  fout << "  <rect fill=\"url(#checkerboard)\" x=\"5\" y=\"5\"";
  fout <<        " width=\"" << sq*x << "\" height=\"" << sq*y << "\"";
  fout <<        " style=\"stroke-width:2;stroke:rgb(150,150,230)\"/>\n";

  vector<vector<bool>> used(y, vector<bool>(x));
  for (int r = 0; r+1 < y; r++) {
    vector<pair<int, int>> lines = State::States[id[r]]->GetLines(idx[r]);
    for (auto line : lines) {
      double x1 = (line.first / 3) + (line.first % 3 != 0) * 0.5;
      double x2 = (line.second / 3) + (line.second % 3 != 0) * 0.5;
      if (x1 == (int)x1) used[r][x1] = true;
      if (x2 == (int)x2) used[r+1][x2] = true;
      fout << "  <line x1=\"" << 5 + x1*sq + sq/2.0 << "\" y1=\"" << 5 + r*sq + sq/2.0 << "\"";
      fout <<        " x2=\"" << 5 + x2*sq + sq/2.0 << "\" y2=\"" << 5 + (r+1)*sq + sq/2.0 << "\"";
      fout <<        " style=\"stroke:rgb(0,0,0);stroke-width:1\"/>\n";
    }
  }
  for (int r = 0; r < y; r++)
  for (int c = 0; c < x; c++)
  if (!used[r][c]) {
      fout << "  <circle fill=\"rgb(255,0,0)\""
           <<          " cx=\"" << 5 + c*sq + sq/2.0 << "\" cy=\"" << 5 + r*sq + sq/2.0 << "\" r=\"1\"/>\n";
  }

  if (minHL != -1) {
    fout << "  <rect fill=\"none\" x=\"2\" y=\"" << 2 + minHL*sq + sq/2.0 << "\"";
    fout <<        " width=\"" << 6 + x*sq << "\" height=\"" << (maxHL-minHL)*sq << "\"";
    fout <<        " stroke-dasharray=\"7, 7\" style=\"stroke-width:2;stroke:rgb(250,150,250)\"/>\n";
  }

  fout << "</svg>\n";
}

void FindOneSolutionAndPeriod() {
  vector<vector<int>> max_path(NRow+1, vector<int>(State::States.size(), -1));
  vector<vector<int>> pred(NRow+1, vector<int>(State::States.size(), -1));
  vector<vector<int>> pred_idx(NRow+1, vector<int>(State::States.size(), -1));
  vector<vector<int>> nsol(NRow+1, vector<int>(State::States.size(), 0));
  max_path[0][1] = 0;
  nsol[0][1] = 1;
  int found_period = -1, found_added = -1, found_n = -1;
  for (int row = 1; row <= NRow; row++) {
    for (int i = 0; i < State::States.size(); i++) if (max_path[row-1][i] >= 0)
    for (int j = 0; j < State::States[i]->succ.size(); j++) {
      int nst = State::States[i]->succ[j].first;
      int ne = State::States[i]->succ[j].second;
      if (max_path[row-1][i] + ne > max_path[row][nst]) {
        max_path[row][nst] = max_path[row-1][i] + ne;
        pred[row][nst] = i;
        pred_idx[row][nst] = j;
        nsol[row][nst] = nsol[row-1][i];
      } else if (max_path[row-1][i] + ne == max_path[row][nst]) {
        nsol[row][nst] += nsol[row-1][i];
      }
    }

    cout << endl;
    cout << NCol << " x " << row << ": " << max_path[row][0];
    cout << " (" << nsol[row][0] << " solutions)" << endl;
    if (max_path[row][0] != -1) {
      vector<int> st_id(row+1), st_idx(row+1);
      for (int r = row, st = 0, idx = -1; r >= 0; idx = pred_idx[r][st], st = pred[r][st], r--) {
        st_id[r] = st;
        st_idx[r] = idx;
      }
      PrintOneSolution(st_id, st_idx);

      // Try a naive way of finding periodicity.
      int period, added, start;
      for (period = 1; period < st_id.size(); period++) {
        added = -1;
        for (int i = 0; i+period < st_id.size(); i++)
        if (st_id[i] == st_id[i+period]) {
          int cur_added = 0;
          for (int j = 0; j < period; j++) {
            cur_added += State::States[st_id[i+j]]->succ[st_idx[i+j]].second;
          }
          if (cur_added > added) { added = cur_added; start = i; }
        }

        if (added != -1) {
          // We have a potential pattern: every "period" rows we can add "added".
          // We can prove this is optimal by looking back "period" rows to see if there
          // are no states that gain more than "added" in score.
          bool optimal = true;
          for (int i = 0; i < max_path[row].size(); i++) {
            if ((max_path[row][i] != -1) ^ (max_path[row-period][i] != -1)) { optimal = false; break; }
            if (max_path[row][i] == -1) continue;
            if (max_path[row-period][i] + added < max_path[row][i]) { optimal = false; break; }
          }
          if (optimal) break;
        }
      }

      if (period < st_id.size()) {
        WriteSolutionSVG(st_id, st_idx, start, start+period);
      } else {
        WriteSolutionSVG(st_id, st_idx);
      }

      if (period < st_id.size()) {
        cout << NCol << " x " << row << ": Repeated optimal pattern, period " << period;
        cout << ", avg score " << (double)added/period << endl;
        cout << "Rows " << start << " to " << start+period << " can be repeated, adding " << added << "." << endl;
        if (found_period != -1 && found_period % period == 0 && added * (found_period / period) == found_added) {
          found_n++;
        } else {
          found_period = period;
          found_added = added;
          found_n = 1;
        }
        if (found_n == found_period) {
          // Simplify the periodic portion as much as we can
          period = found_period;
          added = found_added;
          vector<int> v;
          for (int r = 0; r <= row; r++) v.push_back(max_path[r][0]);
          for (int i = 0; i < period; i++) v.push_back(v[v.size()-period] + added);
          for (int div = 2; div <= period; div++) if (period%div == 0 && added%div == 0) {
            int i;
            for (i = 0; i < period-period/div; i++) {
              if (v[v.size()-period + i] + added/div != v[v.size()-period + i+period/div]) break;
            }
            if (i == period-period/div) {
              period /= div;
              added /= div;
              div--;
            }
          }
          while (v.size() > 2*period &&
                 v[v.size()-1-period] == v[v.size()-1] - added &&
                 v[v.size()-1-2*period] == v[v.size()-1] - 2*added) {
            v.pop_back();  // compress periodic portion as much as possible
          }
          cout << "Found complete rotation of " << period << " periods.  Terminating search." << endl;
          cout << "C=" << NCol << ":";
          for (int r = 1; r < v.size(); r++) {
            cout << ' ';
            if (r == v.size()-2*period || r == v.size()-period) cout << '[';
            cout << v[r];
            if (r == v.size()-1-period || r == v.size()-1) cout << ']';
            cout << ',';
          }
          cout << " ..." << endl;
          return;
        }
      } else {
        found_period = -1;
      }
    }
  }
}


int main(int argc, char* argv[]) {
  if (argc > 1) sscanf(argv[1], "%d", &NCol);
  if (argc > 2) sscanf(argv[2], "%d", &NRow);
  if (argc > 3) OpenPaths = !strcmp(argv[3], "open");
  cout << "Solving for " << NCol << " columns, ";
  cout << (OpenPaths ? "open" : "closed") << " paths." << endl;

  State::Init();

  GenerateAllStates();
  FindOneSolutionAndPeriod();
}
