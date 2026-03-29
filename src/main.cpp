#include <raylib-cpp.hpp>
#include <iostream>


const int WellSizeX = 10;
const int WellSizeY = 20;
const int gridSize = 40;
const Vector2 padding = {80, 80};

const Vector2 resolution = {padding.x * 2 + gridSize * WellSizeX, padding.y * 2 + gridSize * WellSizeY};
int offset = 75;

const double timeToDrop = 1;
double timeSinceLastDrop = 0;

bool TimeToDrop()
{
    timeSinceLastDrop += GetFrameTime();
    if(timeSinceLastDrop > timeToDrop) 
    {
        timeSinceLastDrop = 0;
        return true;
    }
    return false;
}


struct Tetramino {
    
};

struct Block {
  bool active = false;
  Color color = BLACK;  
};

bool BlockIsEmpty(Block block)
{
    return block.color.r == 0 && block.color.g == 0 && block.color.b == 0;
}

class Well {
    std::vector<Block> wellRow;
    std::vector<std::vector<Block>> well;
    Rectangle wellRect = {padding.x, padding.y, gridSize* WellSizeX, gridSize * WellSizeY};
public: 

    Well(int width, int height): wellRow(width, {false, RED}), well(height, wellRow)
    {
        well[2][3].active = true;
        well[2][3].color = RED;
    }

    ~Well()
    {

    }

    void UpdateWell()
    {
        for(int i = 0; i < well.size()-1; i++)
        {
            for(int j = 0; j < well[i].size()-1; j++)
            {
                if(well[i][j].active)
                {
                    std::cout << "well " << i << " " << j << " is active!" << std::endl; 
                    well[i][j+1].color = well[i][j].color;
                    well[i][j+1].active = true;
                    well[i][j].color = BLACK;
                    well[i][j].active = false;
                    return;
                }
            }
        }
    }
    void PrintWell()
    {
        for(int i = 0; i< well.size(); i++)
        {
            for(int j = 0; j<well[i].size(); j++)
            {
                if(BlockIsEmpty(well[i][j]))
                {
                    std::cout << 0 << " ";
                }
                else 
                {
                    std::cout << 1 << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    void DrawWell()
    {
        DrawRectangleLinesEx(wellRect, 3, WHITE);
        for(int i = 0; i < well.size(); i++)
        {
            for(int j = 0; j < well[i].size(); j++)
            {
                if(well[i][j].active)
                {
                    DrawGridSpot(i, j, well[i][j].color);
                }
            }
        }
    }

    void DrawGrid()
    {
        // Col
        for(int i = 0; i< WellSizeX; i++)
        {
            float xVal = wellRect.x + i * wellRect.width / WellSizeX;
            DrawLineEx({xVal, wellRect.y}, {xVal, wellRect.height + wellRect.y}, 2, WHITE);
        }

        //Row
        for(int i = 0; i< WellSizeY; i++)
        {
            float yVal = wellRect.y + i * wellRect.height / WellSizeY;
            DrawLineEx({wellRect.x, yVal}, {wellRect.x + wellRect.width, yVal}, 2, WHITE);
        }
    }

    void DrawGridSpot(const int x, const int y, Color color)
    {
        if(x < 0 || y < 0 || x > WellSizeX -1 || y > WellSizeY -1) return;

        DrawRectangle(padding.x + gridSize * x, padding.y + gridSize * y, gridSize, gridSize, color);

    }

};


void PrintMatrix(Matrix mat)
{
    std::cout << mat.m0 << mat.m4 << mat.m8 << mat.m12 << std::endl;
    std::cout << mat.m1 << mat.m5 << mat.m9 << mat.m13 << std::endl;
    std::cout << mat.m2 << mat.m6 << mat.m10 << mat.m14 << std::endl;
    std::cout << mat.m3 << mat.m7 << mat.m11 << mat.m15 << std::endl;
}


int main()
{
    std::cout << "Starting the game..." << std::endl;
    InitWindow(resolution.x, resolution.y, "myyrtris");
    SetTargetFPS(60);
    Well well(WellSizeX, WellSizeY);
    while(WindowShouldClose() == false)
    {
        //Logic
        BeginDrawing();

        ClearBackground(BLACK);
        well.DrawWell();
        well.DrawGrid();

        if(TimeToDrop())
        {
            well.UpdateWell();
            std::cout << "UPDATE" << std::endl;
        }
    
        EndDrawing();
    }
   
    CloseWindow();
    return 0;
}