#include "Renderer.h"
#include "Graph.h"
#include <cmath>
#include <string>

// Draws a directional arrow between two points (cities)
void DrawArrow(Vector2 start, Vector2 end, Color color) {
    // Draw main line
    DrawLineEx(start, end, 4, color);

    // Calculate direction vector from start → end
    Vector2 dir = { end.x - start.x, end.y - start.y };

    // Compute length of vector (used for normalization)
    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0) return; // Prevent divide by zero

    // Normalize direction (make it length = 1)
    dir.x /= len;
    dir.y /= len;

    // Position the arrow tip slightly before the end point
    Vector2 tip = { end.x - dir.x * 35, end.y - dir.y * 35 };

    // Calculate left and right corners of the arrowhead triangle
    Vector2 left = {
        tip.x - dir.x * 18 - dir.y * 9,
        tip.y - dir.y * 18 + dir.x * 9
    };

    Vector2 right = {
        tip.x - dir.x * 18 + dir.y * 9,
        tip.y - dir.y * 18 - dir.x * 9
    };

    // Draw arrowhead
    DrawTriangle(tip, left, right, color);
}

// Checks if a route (edge) is part of the currently revealed path
bool IsInPath(int from, int to, const std::vector<int>& path, int visibleEdges) {

    // Only check edges that have been "revealed" by animation
    for (size_t i = 0; i < static_cast<size_t>(visibleEdges) && i + 1 < path.size(); i++) {

        // Forward and backward check because graph is undirected
        bool forward = path[i] == from && path[i + 1] == to;
        bool backward = path[i] == to && path[i + 1] == from;

        if (forward || backward)
            return true;
    }

    return false;
}

// Main rendering function: draws the entire map and UI
void DrawMap(const std::vector<City>& cities,
    const std::vector<VisualRoute>& routes,
    const std::vector<int>& path,
    int visibleEdges,
    int startCity,
    int endCity,
    int totalCost) {

    // Instruction text
    DrawText("Click a start city, then click a destination city", 30, 25, 24, BLACK);

    // =========================
    // DRAW ROUTES (EDGES)
    // =========================
    for (const auto& r : routes) {

        // Highlight route if it is part of the shortest path
        Color c = IsInPath(r.from, r.to, path, visibleEdges) ? GOLD : GRAY;

        Vector2 start = cities[r.from].pos;
        Vector2 end = cities[r.to].pos;

        DrawArrow(start, end, c);

        // Calculate midpoint of edge for cost display
        Vector2 mid = {
            (start.x + end.x) / 2.0f,
            (start.y + end.y) / 2.0f
        };

        // Draw cost bubble
        DrawCircleV(mid, 16, WHITE);

        // Draw cost number
        DrawText(TextFormat("%d", r.cost),
            static_cast<int>(mid.x - 6),
            static_cast<int>(mid.y - 9),
            20,
            BLACK);
    }

    // =========================
    // DRAW CITIES (NODES)
    // =========================
    for (size_t i = 0; i < cities.size(); i++) {

        // Default color
        Color c = SKYBLUE;

        // Highlight start and end cities
        if (static_cast<int>(i) == startCity)
            c = GREEN;
        else if (static_cast<int>(i) == endCity)
            c = RED;

        // Draw city circle
        DrawCircleV(cities[i].pos, 35, c);

        // Outline for visibility
        DrawCircleLines(
            static_cast<int>(cities[i].pos.x),
            static_cast<int>(cities[i].pos.y),
            35,
            DARKBLUE
        );

        // Draw city index (debug/visual clarity)
        DrawText(TextFormat("%d", static_cast<int>(i)),
            static_cast<int>(cities[i].pos.x - 6),
            static_cast<int>(cities[i].pos.y - 12),
            24,
            BLACK);

        // Draw city name
        DrawText(cities[i].name.c_str(),
            static_cast<int>(cities[i].pos.x - 55),
            static_cast<int>(cities[i].pos.y + 45),
            18,
            BLACK);
    }

    // =========================
    // UI: START CITY
    // =========================
    if (startCity >= 0 && startCity < static_cast<int>(cities.size())) {
        DrawText(TextFormat("Start: %s", cities[startCity].name.c_str()),
            30, 590, 20, DARKGREEN);
    }

    // =========================
    // UI: END CITY + PATH INFO
    // =========================
    if (endCity >= 0 && endCity < static_cast<int>(cities.size())) {

        DrawText(TextFormat("End: %s", cities[endCity].name.c_str()),
            30, 620, 20, MAROON);

        // If no path exists
        if (path.empty()) {
            DrawText("No path found.", 30, 650, 20, RED);
        }
        else {
            // Build path string (e.g., Winnipeg -> Regina -> Edmonton)
            std::string pathText = "Path: ";

            for (size_t i = 0; i < path.size(); i++) {
                pathText += cities[path[i]].name;

                if (i + 1 < path.size())
                    pathText += " -> ";
            }

            // Draw path string
            DrawText(pathText.c_str(), 250, 590, 18, BLACK);

            // Draw total cost (computed in main.cpp)
            DrawText(TextFormat("Total Cost: %d trains", totalCost),
                250, 620, 20, BLACK);

            // Show animation progress
            DrawText(TextFormat("Animating step %d of %d",
                visibleEdges,
                static_cast<int>(path.size()) - 1),
                250, 650, 18, DARKGRAY);
        }
    }
}

// Detects which city (if any) was clicked by the mouse
int GetClickedCity(const std::vector<City>& cities, Vector2 mousePos) {

    for (size_t i = 0; i < cities.size(); i++) {

        // Check if mouse is inside city circle
        if (CheckCollisionPointCircle(mousePos, cities[i].pos, 35)) {
            return static_cast<int>(i);
        }
    }

    return -1; // No city clicked
}