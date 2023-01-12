#include <iostream>
#include <thread>
#include <cmath>
#include "Model3D.h"
#include "raylib.h"

using namespace std;

double magnitude(Vector3 vector) {
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}

double distance3D(Vector3 v1, Vector3 v2) {
    return magnitude({v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
}

Vector2 to2D(Vector3 vertex, const Camera3D &camera, float scale) {
    Vector3 cameraNormal{camera.target.x - camera.position.x,
                         camera.target.y - camera.position.y,
                         camera.target.z - camera.position.z};

    double dt = magnitude(cameraNormal);

    double t1 = camera.fovy / dt;

    double t2 = t1 * (pow(dt, 2)) / (
            cameraNormal.x * (vertex.x - camera.position.x) +
            cameraNormal.y * (vertex.y - camera.position.y) +
            cameraNormal.z * (vertex.z - camera.position.z)
    );

    Vector3 planePoint{
            static_cast<float>(camera.position.x + t2 * (vertex.x - camera.position.x)),
            static_cast<float>(camera.position.y + t2 * (vertex.y - camera.position.y)),
            static_cast<float>(camera.position.z + t2 * (vertex.z - camera.position.z))
    };

    Vector3 planeOrigin{
            static_cast<float>(camera.position.x + t1 * cameraNormal.x),
            static_cast<float>(camera.position.y + t1 * cameraNormal.y),
            static_cast<float>(camera.position.z + t1 * cameraNormal.z)
    };

    Vector3 vertexPlaneLine{planePoint.x - planeOrigin.x,
                            planePoint.y - planeOrigin.y,
                            planePoint.z - planeOrigin.z};

    float dXY = magnitude({cameraNormal.x, 0, cameraNormal.z});
    float t3 = -(cameraNormal.y) / dXY;

    Vector3 y_axis{static_cast<float>(cameraNormal.x * t3 / dt),
                   static_cast<float>(-t3 * pow(dXY, 2) / (cameraNormal.y * dt)),
                   static_cast<float>(cameraNormal.z * t3 / dt)};

    float y = (vertexPlaneLine.x * y_axis.x + vertexPlaneLine.y * y_axis.y + vertexPlaneLine.z * y_axis.z) /
              magnitude(y_axis);

    Vector3 x_axis{-y_axis.y * cameraNormal.z + y_axis.z * cameraNormal.y,
                   -y_axis.z * cameraNormal.x + y_axis.x * cameraNormal.z,
                   -y_axis.x * cameraNormal.y + y_axis.y * cameraNormal.x};

    float x = (vertexPlaneLine.x * x_axis.x + vertexPlaneLine.y * x_axis.y + vertexPlaneLine.z * x_axis.z) /
              magnitude(x_axis);

    return {GetScreenWidth() / 2 + x * scale, GetScreenHeight() / 2 - y * scale};
}

void draw(const Model3D &model, const Camera3D *camera, float *scale, float *timeFrame) {
    InitWindow(800, 800, "DEMO");
    SetTargetFPS(60);

    list<Edge> axises = {{{10, 0,  0},  {0, 0, 0}},
                         {{0,  10, 0},  {0, 0, 0}},
                         {{0,  0,  10}, {0, 0, 0}}};

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    while (!WindowShouldClose()) {
        *timeFrame += GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(GetScreenWidth() - 100, 0);

        for (float i = -4; i <= 4; i += 1) {
            auto vx1 = to2D({10, 0, i * 2}, *camera, *scale);
            auto vx2 = to2D({-10, 0, i * 2}, *camera, *scale);
            auto vz1 = to2D({i * 2, 0, 10}, *camera, *scale);
            auto vz2 = to2D({i * 2, 0, -10}, *camera, *scale);

            DrawLineEx(vx1, vx2, 2, GRAY);
            DrawLineEx(vz1, vz2, 2, GRAY);
        }

        for (auto axis: axises) {
            auto v1 = to2D(axis.v1.vertex, *camera, *scale);
            auto v2 = to2D(axis.v2.vertex, *camera, *scale);
            DrawLineEx(v1, v2, 2, BLACK);
        }

        for (auto face: model.getFaces()) {
            auto v1 = to2D(face.v1.vertex, *camera, *scale);
            auto v2 = to2D(face.v2.vertex, *camera, *scale);
            auto v3 = to2D(face.v3.vertex, *camera, *scale);
            DrawTriangle(v1, v2, v3, YELLOW);
        }

        for (auto edge: model.getEdges()) {
            auto v1 = to2D(edge.v1.vertex, *camera, *scale);
            auto v2 = to2D(edge.v2.vertex, *camera, *scale);
            DrawLineEx(v1, v2, 2, RED);
        }

        int i = 0;
        for (auto vertex: model.getVertices()) {
            auto v = to2D(vertex.vertex, *camera, *scale);
            DrawText(to_string(++i).c_str(), v.x, v.y, 32, BLACK);
        }

        auto targetV = to2D(camera->target, *camera, *scale);
        DrawCircle(targetV.x, targetV.y, 5, BLUE);

        DrawRectangle(0, 0, 500, 150, {64, 64, 64, 192});
        DrawText("POSITION", 5, 0, 24, BLACK);
        DrawText(to_string(camera->position.x).c_str(), 150, 0, 20, BLACK);
        DrawText(to_string(camera->position.y).c_str(), 275, 0, 20, BLACK);
        DrawText(to_string(camera->position.z).c_str(), 400, 0, 20, BLACK);

        DrawText("TARGET", 5, 30, 24, BLACK);
        DrawText(to_string(camera->target.x).c_str(), 150, 30, 20, BLACK);
        DrawText(to_string(camera->target.y).c_str(), 275, 30, 20, BLACK);
        DrawText(to_string(camera->target.z).c_str(), 400, 30, 20, BLACK);

        DrawText("UP", 5, 60, 24, BLACK);
        DrawText(to_string(camera->up.x).c_str(), 150, 60, 20, BLACK);
        DrawText(to_string(camera->up.y).c_str(), 275, 60, 20, BLACK);
        DrawText(to_string(camera->up.z).c_str(), 400, 60, 20, BLACK);

        DrawText("FOV", 5, 90, 24, BLACK);
        DrawText(to_string(camera->fovy).c_str(), 150, 90, 20, BLACK);

        DrawText("SCALE", 5, 120, 24, BLACK);
        DrawText(to_string(*scale).c_str(), 150, 120, 20, BLACK);

        EndDrawing();
    }
}

int main() {
    Vertex v1{1, 1, 1};
    Vertex v2{-1, 1, 1};
    Vertex v3{-1, -1, 1};
    Vertex v4{1, -1, 1};
    Vertex v5{1, 1, -1};
    Vertex v6{-1, 1, -1};
    Vertex v7{-1, -1, -1};
    Vertex v8{1, -1, -1};

    Model3D square{
            {v1,           v2, v3,           v4, v5,           v6, v7,           v8},

            {{v1, v2},
                           {v2, v3},
                               {v3, v4},
                                             {v4, v1},
                                                 {v5, v6},
                                                               {v6, v7},
                                                                   {v7, v8},
                                                                                 {v8, v5},
                    {v1, v5},
                                  {v2, v6},
                    {v3, v7},
                                  {v4, v8}
            },

            {{v1, v2, v3}, {v1, v3, v4},
                               {v5, v6, v7}, {v5, v7, v8},
                                                 {v1, v2, v6}, {v1, v5, v6},
                                                                   {v1, v4, v8}, {v1, v7, v8},
                    {v2, v3, v7}, {v2, v6, v7},
                    {v3, v4, v8}, {v3, v7, v8}}
    };

    Camera3D camera{{10, 10, 10}, {0, 0, 0}, {0, 1, 0}, 10, 0};

    float scale = 100;
    float timeFrame = 0;

    thread window{draw, square, &camera, &scale, &timeFrame};

    while (!IsWindowReady());

    while (true) {
        int wheel = GetMouseWheelMove();
        if (IsKeyDown(KEY_KP_6)) {
            camera.position.x += 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_4)) {
            camera.position.x -= 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_8)) {
            camera.position.y += 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_2)) {
            camera.position.y -= 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_9)) {
            camera.position.z += 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_1)) {
            camera.position.z -= 10 * timeFrame;
        } else if (IsKeyDown(KEY_UP)) {
            camera.target.y += 10 * timeFrame;
        } else if (IsKeyDown(KEY_DOWN)) {
            camera.target.y -= 10 * timeFrame;
        } else if (IsKeyDown(KEY_D)) {
            camera.target.x += 10 * timeFrame;
        } else if (IsKeyDown(KEY_A)) {
            camera.target.x -= 10 * timeFrame;
        } else if (IsKeyDown(KEY_W)) {
            camera.target.z += 10 * timeFrame;
        } else if (IsKeyDown(KEY_S)) {
            camera.target.z -= 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_7)) {
            int m = distance3D(camera.position, camera.target);
            if (m < 1000) {
                camera.position.x += ((camera.position.x - camera.target.x) / m) * 10 * timeFrame;
                camera.position.y += ((camera.position.y - camera.target.y) / m) * 10 * timeFrame;
                camera.position.z += ((camera.position.z - camera.target.z) / m) * 10 * timeFrame;
            }
        } else if (IsKeyDown(KEY_KP_3)) {
            int m = distance3D(camera.position, camera.target);
            if (m > 1) {
                camera.position.x -= ((camera.position.x - camera.target.x) / m) * 10 * timeFrame;
                camera.position.y -= ((camera.position.y - camera.target.y) / m) * 10 * timeFrame;
                camera.position.z -= ((camera.position.z - camera.target.z) / m) * 10 * timeFrame;
            }
        } else if (IsKeyDown(KEY_KP_ADD)) {
            scale += 10 * timeFrame;
        } else if (IsKeyDown(KEY_KP_SUBTRACT)) {
            scale -= 10 * timeFrame;
            if (scale < 0.1)
                scale = 0.1;
        } else if (IsKeyDown(KEY_KP_MULTIPLY)) {
            camera.fovy += 1 * timeFrame;
        } else if (IsKeyDown(KEY_KP_DIVIDE)) {
            camera.fovy -= 1 * timeFrame;
            if (camera.fovy < 1)
                camera.fovy = 1;
        } else if (IsKeyDown(KEY_KP_DECIMAL)) {
            camera.position = {10, 10, 10};
            camera.target = {0, 0, 0};
            camera.up = {0, 1, 0};
            camera.fovy = 10;
            scale = 100;
        } else if (IsKeyDown(KEY_ESCAPE)) {
            break;
        }
        if (wheel) {
            int m = distance3D(camera.position, camera.target);
            if (m < 1000 && wheel < 0 || m > 1 && wheel > 0) {
                camera.position.x -= 10 * (camera.position.x / m) * wheel * timeFrame;
                camera.position.y -= 10 * (camera.position.y / m) * wheel * timeFrame;
                camera.position.z -= 10 * (camera.position.z / m) * wheel * timeFrame;
            }
        }
        if (timeFrame != 0) {
            timeFrame = 0;
        }
    }
    window.join();
    return 0;
}
