#include <raylib-cpp.hpp>
#include <iostream>
#include <deque>
#include <raymath.h>

Vector2 resolution = {1280, 720};
int cellSize = 30;
int cellCount = 25;
int offset = 75;

int main()
{
    std::cout << "Starting the game..." << std::endl;
    InitWindow(2* offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "myrrhSnake");
    SetTargetFPS(60);

   

    while(WindowShouldClose() == false)
    {
        //Logic
        BeginDrawing();

        ClearBackground(BLACK);
    
        EndDrawing();
    }
   
    CloseWindow();
    return 0;
}