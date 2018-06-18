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

struct Solution {

    typedef long long DistType;

    struct Edge;

    static const int N = 202;
    const DistType INF = (DistType) 1e18 + 7ll;

    DistType previous[N], dist[N];

    Edge* previousEdge[N];
    std::vector<Edge*> edges;
    std::vector<std::pair<int, Edge*>> graph[N];

    size_t s, t;

    struct Edge {
        Solution* solution;
        size_t id;
        size_t from, to;
        DistType weight;
        int flow = 0, capacity = 1;
        Edge* paired;

        inline bool able() {
            return (flow < capacity);
        }

        inline DistType getWeight() {
            return (weight + solution->previous[from] - solution->previous[to]);
        }

        inline Edge getReversed() {
            Edge res = (*this);
            std::swap(res.from, res.to);
            res.capacity = 0;
            res.weight = -weight;
            return res;
        }

        inline void pushFlow() {
            flow++;
            paired->flow--;
        }
    };

    inline void dijkstra() {
        dist[s] = 0;
        for (size_t i = 0; i < s; i++)
            dist[i] = INF;

        std::set<std::pair<DistType, int>> q;
        q.insert({dist[s], s});
        while (!q.empty()) {
            int v = q.begin()->second;
            q.erase(q.begin());
            for (auto &u : graph[v]) {
                if (u.second->able() && dist[v] + u.second->getWeight() < dist[u.second->to]) {
                    q.erase({dist[u.second->to], u.second->to});
                    dist[u.second->to] = dist[v] + u.second->getWeight();
                    previousEdge[u.second->to] = u.second;
                    q.insert({dist[u.second->to], u.second->to});
                }
            }
        }

        for (size_t i = 0; i < s; i++)
            previous[i] += dist[i];
    }

    inline void addEdge(Edge* edge) {
        edge->solution = this;
        edges.push_back(edge);
        graph[edge->from].emplace_back(edge->to, edge);
        Edge *rev = new Edge(edge->getReversed());
        rev->paired = edge;
        edge->paired = rev;
        edges.push_back(rev);
        graph[edge->to].emplace_back(edge->from, rev);
    }

    size_t n, m, k;

    void read() {
        std::cin >> n >> m >> k;
        for (size_t i = 1; i <= m; i++) {
            Edge* edge = new Edge();
            edge->id = i;
            std::cin >> edge->from >> edge->to >> edge->weight, edge->from--, edge->to--;
            addEdge(edge);
            Edge* rev = new Edge(*edge);
            std::swap(rev->from, rev->to);
            addEdge(rev);
        }

        s = n;
        t = n - 1;
        Edge* edge = new Edge();
        edge->from = n;
        edge->to = 0;
        edge->weight = 0;
        edge->capacity = (int)k;
        addEdge(edge);
    }

    bool solve(DistType& cost, std::vector<std::vector<int>>& ans) {
        memset(previous, 0, sizeof(previous));
        dijkstra();

        if (dist[t] == INF)
            return false;

        cost = 0;
        size_t cnt = 0;
        do {
            cnt++;
            size_t cur = t;
            while (cur != s) {
                previousEdge[cur]->pushFlow();
                cur = previousEdge[cur]->from;
            }
            dijkstra();
        } while (dist[t] != INF);

        if (cnt < k)
            return false;

        ans.resize(k);
        std::vector<std::pair<int, int>> tmp[N];
        for (auto i : edges)
            if (i->flow > 0) {
                tmp[i->to].emplace_back(i->from, i->id);
                cost += (i->flow) * (i->weight);
            }

        for (size_t i = 0; i < k; i++) {
            size_t cur = t;
            while (cur != 0) {
                std::pair<size_t , int> v(tmp[cur][(int)tmp[cur].size() - 1]);
                tmp[cur].pop_back();
                ans[i].push_back(v.second);
                cur = v.first;
            }
        }
        return true;
    }

    void write(DistType cost, std::vector<std::vector<int>>& ans) {
        std::cout.precision(6);
        std::cout << std::fixed << (double) cost / (double) k << std::endl;
        for (auto &i : ans) {
            reverse(i.begin(), i.end());
            std::cout << i.size() << ' ';
            for (auto &j : i)
                std::cout << j << ' ';
            std::cout << std::endl;
        }
    }

    void writeNoSolution() {
        std::cout << -1 << std::endl;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    Solution solution;
    solution.read();
    Solution::DistType cost = 0;
    std::vector<std::vector<int>> ans;
    if (solution.solve(cost, ans))
        solution.write(cost, ans);
    else
        solution.writeNoSolution();

    return 0;
}

