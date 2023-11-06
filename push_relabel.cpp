#include <queue>
#include <iostream>
#include <chrono>

const int inf = 1000000000;

int n;
std::vector<std::vector<int>> capacity, flow;
std::vector<int> height, excess, seen;
std::queue<int> excess_vertices;

void push(int u, int v)
{
    int d = std::min(excess[u], capacity[u][v] - flow[u][v]);
    flow[u][v] += d;
    flow[v][u] -= d;
    excess[u] -= d;
    excess[v] += d;
    if (d && excess[v] == d)
        excess_vertices.push(v);
}

void relabel(int u)
{
    int d = inf;
    for (int i = 0; i < n; i++) {
        if (capacity[u][i] - flow[u][i] > 0)
            d = std::min(d, height[i]);
    }
    if (d < inf)
        height[u] = d + 1;
}

void discharge(int u)
{
    while (excess[u] > 0) {
        if (seen[u] < n) {
            int v = seen[u];
            if (capacity[u][v] - flow[u][v] > 0 && height[u] > height[v])
                push(u, v);
            seen[u]++;
        } else {
            relabel(u);
            seen[u] = 0;
        }
    }
}

int max_flow(int s, int t)
{
    height.assign(n, 0);
    height[s] = n;
    flow.assign(n, std::vector<int>(n, 0));
    excess.assign(n, 0);
    excess[s] = inf;
    for (int i = 0; i < n; i++) {
        if (i != s)
            push(s, i);
    }
    seen.assign(n, 0);

    while (!excess_vertices.empty()) {
        int u = excess_vertices.front();
        excess_vertices.pop();
        if (u != s && u != t)
            discharge(u);
    }

    int max_flow = 0;
    for (int i = 0; i < n; i++)
        max_flow += flow[i][t];
    return max_flow;
}

int main() {
    int m, s, t;
    std::cin >> n >> m >> s >> t;

    capacity.assign(n, std::vector<int>(n));

    for (int i = 0; i < m; i++) {
        int u, v, c;
        std::cin >> u >> v >> c;
        capacity[u][v] = c;
    }

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    auto res = max_flow(s, t);
    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << ms_int.count() << "ms\n";
    std::cout << "Max flow: " << res << '\n';
    return 0;
}
