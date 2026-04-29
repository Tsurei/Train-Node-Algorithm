#pragma once
#include <vector>
#include <string>
#include "raylib.h"

/**
 * @brief Represents a city node in the graph.
 */
struct City {
    std::string name; ///< Name of the city
    Vector2 pos;      ///< Screen position for rendering
};

/**
 * @brief Represents a connection between cities (used for Dijkstra).
 */
struct Route {
    int to;    ///< Destination city index
    int cost;  ///< Cost of traveling to that city
};

/**
 * @brief Represents a route used only for visualization.
 */
struct VisualRoute {
    int from;  ///< Starting city index
    int to;    ///< Destination city index
    int cost;  ///< Cost of the route
};

/**
 * @brief Adds a bidirectional route between two cities.
 *
 * Updates both:
 * - The graph (for pathfinding)
 * - The visual route list (for rendering)
 *
 * @param graph Adjacency list representing the graph
 * @param routes List of visual routes
 * @param from Source city index
 * @param to Destination city index
 * @param cost Travel cost between cities
 */
void AddRoute(std::vector<std::vector<Route>>& graph,
    std::vector<VisualRoute>& routes,
    int from, int to, int cost);

/**
 * @brief Computes the shortest path between two cities using Dijkstra's algorithm.
 *
 * @param start Starting city index
 * @param destination Target city index
 * @param graph Adjacency list graph
 * @return Vector of city indices representing the shortest path
 */
std::vector<int> DijkstraPath(int start, int destination,
    const std::vector<std::vector<Route>>& graph);

/**
 * @brief Calculates the total cost of a path.
 *
 * Iterates through consecutive nodes in the path and sums edge weights.
 *
 * @param path Sequence of city indices
 * @param graph Adjacency list graph
 * @return Total cost of the path
 */
int GetPathCost(const std::vector<int>& path,
    const std::vector<std::vector<Route>>& graph);