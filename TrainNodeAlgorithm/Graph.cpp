#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>

// Represents "infinity" for unreachable nodes
const int INF = std::numeric_limits<int>::max();

void AddRoute(std::vector<std::vector<Route>>& graph,
    std::vector<VisualRoute>& routes,
    int from, int to, int cost)
{
    // Add edge in both directions -> undirected graph
    graph[from].push_back({ to, cost });
    graph[to].push_back({ from, cost });

    // Only store one visual route to avoid duplicate rendering
    routes.push_back({ from, to, cost });
}

std::vector<int> DijkstraPath(int start, int destination,
    const std::vector<std::vector<Route>>& graph)
{
    int n = static_cast<int>(graph.size());

    // Distance to each node (initialized to "infinity")
    std::vector<int> distance(n, INF);

    // Tracks previous node to rebuild path later
    std::vector<int> previous(n, -1);

    // Min-heap storing (distance, node)
    std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        std::greater<std::pair<int, int>>
    > pq;

    // Start node setup
    distance[start] = 0;
    pq.push({ 0, start });

    // Process nodes in order of shortest known distance
    while (!pq.empty()) {

        int dist = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        // Skip if this entry is outdated
        if (dist > distance[node]) {
            continue;
        }

        // Check all neighbors of current node
        for (const Route& r : graph[node]) {

            int newDist = distance[node] + r.cost;

            // Relaxation step: update if shorter path is found
            if (newDist < distance[r.to]) {
                distance[r.to] = newDist;
                previous[r.to] = node;

                // Push updated distance to queue
                pq.push({ newDist, r.to });
            }
        }
    }

    std::vector<int> path;

    // If destination is invalid or unreachable, return empty
    if (destination < 0 || destination >= n || distance[destination] == INF) {
        return path;
    }

    // Reconstruct path by walking backwards from destination
    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }

    // Reverse to get correct order (start → destination)
    std::reverse(path.begin(), path.end());

    return path;
}

int GetPathCost(const std::vector<int>& path,
    const std::vector<std::vector<Route>>& graph)
{
    int total = 0;

    // Sum cost of each consecutive edge in the path
    for (size_t i = 0; i + 1 < path.size(); i++) {

        int from = path[i];
        int to = path[i + 1];

        // Find matching edge and add its weight
        for (const Route& r : graph[from]) {
            if (r.to == to) {
                total += r.cost;
                break; // Stop once edge is found
            }
        }
    }

    return total;
}