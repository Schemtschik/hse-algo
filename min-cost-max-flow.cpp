#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>

typedef long long DistType;

struct Edge;

static const int N = 202;
const DistType INF = (DistType) 1e18 + 7ll;

struct Edge {
    size_t id;
    size_t from, to;
    DistType weight;
    int flow = 0, capacity = 1;
    Edge* paired;

    inline bool able() {
        return (flow < capacity);
    }

    inline DistType getWeight(DistType* potentials) {
        return (weight + potentials[from] - potentials[to]);
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

struct Graph {
    std::vector<Edge*> edges;
    std::vector<std::pair<int, Edge*>> graph[N];

    inline void addEdge(Edge* edge) {
        edges.push_back(edge);
        graph[edge->from].emplace_back(edge->to, edge);
        Edge* rev = new Edge(edge->getReversed());
        rev->paired = edge;
        edge->paired = rev;
        edges.push_back(rev);
        graph[edge->to].emplace_back(edge->from, rev);
    }
};

inline void dijkstra(const Graph& graph, DistType* potentials, DistType* dist, Edge** previousEdge, size_t s) {
    dist[s] = 0;
    for (size_t i = 0; i < s; i++)
        dist[i] = INF;

    std::set<std::pair<DistType, int>> q;
    q.insert({dist[s], s});
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());
        for (auto &u : graph.graph[v]) {
            if (u.second->able() && dist[v] + u.second->getWeight(potentials) < dist[u.second->to]) {
                q.erase({dist[u.second->to], u.second->to});
                dist[u.second->to] = dist[v] + u.second->getWeight(potentials);
                previousEdge[u.second->to] = u.second;
                q.insert({dist[u.second->to], u.second->to});
            }
        }
    }

    for (size_t i = 0; i < s; i++)
        potentials[i] += dist[i];
}

void read(Graph& graph, size_t& n, size_t& m, size_t& k, size_t& s, size_t& t) {
    std::cin >> n >> m >> k;
    for (size_t i = 1; i <= m; i++) {
        Edge* edge = new Edge();
        edge->id = i;
        std::cin >> edge->from >> edge->to >> edge->weight, edge->from--, edge->to--;
        graph.addEdge(edge);
        Edge* rev = new Edge(*edge);
        std::swap(rev->from, rev->to);
        graph.addEdge(rev);
    }

    s = n;
    t = n - 1;
    Edge* edge = new Edge();
    edge->from = n;
    edge->to = 0;
    edge->weight = 0;
    edge->capacity = (int) k;
    graph.addEdge(edge);
}

bool solve(Graph& graph, DistType &cost, std::vector<std::vector<int>> &ans, size_t s, size_t t, size_t k) {
    DistType potentials[N], dist[N];
    Edge* previousEdge[N];

    memset(potentials, 0, sizeof(potentials));
    dijkstra(graph, potentials, dist, previousEdge, s);

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
        dijkstra(graph, potentials, dist, previousEdge, s);
    } while (dist[t] != INF);

    if (cnt < k)
        return false;

    ans.resize(k);
    std::vector<std::pair<int, int>> tmp[N];
    for (auto i : graph.edges)
        if (i->flow > 0) {
            tmp[i->to].emplace_back(i->from, i->id);
            cost += (i->flow) * (i->weight);
        }

    for (size_t i = 0; i < k; i++) {
        size_t cur = t;
        while (cur != 0) {
            std::pair<size_t, int> v(tmp[cur][(int) tmp[cur].size() - 1]);
            tmp[cur].pop_back();
            ans[i].push_back(v.second);
            cur = v.first;
        }
    }
    return true;
}

void write(DistType cost, std::vector<std::vector<int>> &ans, size_t k) {
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

int main() {
    std::ios_base::sync_with_stdio(false);

    size_t s, t, n, m, k;
    Graph graph;

    read(graph, n, m, k, s, t);
    DistType cost = 0;
    std::vector<std::vector<int>> ans;
    if (solve(graph, cost, ans, s, t, k))
        write(cost, ans, k);
    else
        writeNoSolution();

    return 0;
}
