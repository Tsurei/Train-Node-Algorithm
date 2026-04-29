#pragma once
#include <vector>
#include "Graph.h"

/**
 * @brief Draws a directional arrow between two points.
 *
 * Used to visually represent a route between two cities.
 *
 * @param start Starting position (source city)
 * @param end Ending position (destination city)
 * @param color Color of the arrow
 */
void DrawArrow(Vector2 start, Vector2 end, Color color);

/**
 * @brief Renders the full map including cities, routes, and UI.
 *
 * This function draws:
 * - All routes (edges) between cities
 * - Highlighted shortest path
 * - City nodes with labels
 * - UI text such as selected cities and path information
 *
 * @param cities List of all cities (nodes)
 * @param routes List of visual routes (edges)
 * @param path Shortest path (sequence of city indices)
 * @param visibleEdges Number of edges currently revealed (animation)
 * @param startCity Index of selected start city
 * @param endCity Index of selected destination city
 * @param totalCost Total cost of the computed path
 */
void DrawMap(const std::vector<City>& cities,
    const std::vector<VisualRoute>& routes,
    const std::vector<int>& path,
    int visibleEdges,
    int startCity,
    int endCity,
    int totalCost);

/**
 * @brief Determines which city was clicked by the mouse.
 *
 * Checks if the mouse position overlaps any city node.
 *
 * @param cities List of all cities
 * @param mousePos Current mouse position
 * @return Index of clicked city, or -1 if none
 */
int GetClickedCity(const std::vector<City>& cities, Vector2 mousePos);