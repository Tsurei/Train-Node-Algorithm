#include "raylib.h"
#include "Graph.h"
#include "Renderer.h"

int main() {
    constexpr int SCREEN_WIDTH = 1000;
    constexpr int SCREEN_HEIGHT = 700;
    constexpr float ANIMATION_DELAY = 0.6f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dijkstra Visual");
    SetTargetFPS(60);

    std::vector<City> cities = {
        {"Winnipeg",     {150, 250}},
        {"Saskatchewan", {430, 120}},
        {"Regina",       {430, 400}},
        {"Toronto",      {750, 180}},
        {"Edmonton",     {750, 480}}
    };

    std::vector<std::vector<Route>> graph(cities.size());
    std::vector<VisualRoute> routes;

    AddRoute(graph, routes, 0, 1, 10);
    AddRoute(graph, routes, 0, 2, 3);
    AddRoute(graph, routes, 1, 2, 4);
    AddRoute(graph, routes, 1, 3, 2);
    AddRoute(graph, routes, 2, 3, 8);
    AddRoute(graph, routes, 2, 4, 2);
    AddRoute(graph, routes, 3, 4, 9);

    int startCity = -1;
    int endCity = -1;
    int totalCost = 0;

    std::vector<int> path;

    int visibleEdges = 0;
    float animationTimer = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Select start and end cities
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int clickedCity = GetClickedCity(cities, GetMousePosition());

            if (clickedCity != -1) {
                bool startingNewPath = startCity == -1 || endCity != -1;

                if (startingNewPath) {
                    startCity = clickedCity;
                    endCity = -1;
                    path.clear();
                    totalCost = 0;
                }
                else {
                    endCity = clickedCity;
                    path = DijkstraPath(startCity, endCity, graph);
                    totalCost = GetPathCost(path, graph);
                }

                visibleEdges = 0;
                animationTimer = 0.0f;
            }
        }

        // Animate path reveal
        if (!path.empty()) {
            int maxVisibleEdges = static_cast<int>(path.size()) - 1;

            animationTimer += deltaTime;

            if (animationTimer >= ANIMATION_DELAY && visibleEdges < maxVisibleEdges) {
                visibleEdges++;
                animationTimer = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMap(cities, routes, path, visibleEdges, startCity, endCity, totalCost);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}