#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <stack>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>

using namespace std;

typedef long long DistType;

const int N = 202;
const DistType INF = (DistType)1e18 + 7ll;

DistType p[N], d[N];

struct Edge;

struct Edge {
    int id;
    int f, t;
    DistType w;
    int flow = 0, cap = 1;
    Edge* paired;

    inline bool able() {
        return (flow < cap);
    }

    inline DistType getWeight() {
        return (w + p[f] - p[t]);
    }

    inline Edge rev() {
        Edge res = (*this);
        swap(res.f, res.t);
        res.cap = 0;
        res.w = -w;
        return res;
    }

    inline void pushFlow() {
        flow++;
        paired->flow--;
    }
};

Edge* e[N];
vector<Edge*> edges;
vector<pair<int, Edge*>> g[N];

int s, t;

inline void dijkstra() {
    d[s] = 0;
    for (int i = 0; i < s; i++)
        d[i] = INF;

    set<pair<DistType, int>> q;
    q.insert({d[s], s});
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());
        for (auto& u : g[v]) {
            if (u.second->able() && d[v] + u.second->getWeight() < d[u.second->t]) {
                q.erase({d[u.second->t], u.second->t});
                d[u.second->t] = d[v] + u.second->getWeight();
                e[u.second->t] = u.second;
                q.insert({d[u.second->t], u.second->t});
            }
        }
    }

    for (int i = 0; i < s; i++)
        p[i] += d[i];
}

inline void addEdge(Edge* edge) {
    edges.push_back(edge);
    g[edge->f].emplace_back(edge->t, edge);
    Edge* rev = new Edge(edge->rev());
    rev->paired = edge;
    edge->paired = rev;
    edges.push_back(rev);
    g[edge->t].emplace_back(edge->f, rev);
}

int main() {
    ios_base::sync_with_stdio(false);

    int n, m, k;
    cin >> n >> m >> k;
    for (int i = 1; i <= m; i++) {
        Edge* edge = new Edge();
        edge->id = i;
        cin >> edge->f >> edge->t >> edge->w, edge->f--, edge->t--;
        addEdge(edge);
        Edge* rev = new Edge(*edge);
        swap(rev->f, rev->t);
        addEdge(rev);
    }

    s = n;
    t = n - 1;
    Edge* edge = new Edge();
    edge->f = n;
    edge->t = 0;
    edge->w = 0;
    edge->cap = k;
    addEdge(edge);

    memset(p, 0, sizeof(p));
    dijkstra();

    if (d[t] == INF) {
        cout << -1 << endl;
        return 0;
    }

    DistType cost = 0;
    int cnt = 0;
    do {
        cnt++;
        int cur = t;
        while (cur != s) {
            e[cur]->pushFlow();
            cur = e[cur]->f;
        }
        dijkstra();
    } while (d[t] != INF);

    if (cnt < k) {
        cout << -1 << endl;
        return 0;
    }

    vector<vector<int>> ans(k);
    vector<pair<int, int>> tmp[N];
    for (auto i : edges)
        if (i->flow > 0) {
            tmp[i->t].emplace_back(i->f, i->id);
            cost += (i->flow) * (i->w);
        }

    for (int i = 0; i < k; i++) {
        int cur = t;
        while (cur != 0) {
            pair<int, int> v(tmp[cur][(int)tmp[cur].size() - 1]);
            tmp[cur].pop_back();
            ans[i].push_back(v.second);
            cur = v.first;
        }
    }

    cout.precision(6);
    cout << fixed << (double)cost / (double)k << endl;
    for (auto& i : ans) {
        reverse(i.begin(), i.end());
        cout << i.size() << ' ';
        for (auto& j : i)
            cout << j << ' ';
        cout << endl;
    }

    return 0;
}