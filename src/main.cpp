#include <raylib-cpp.hpp>
#include <iostream>
#include <algorithm>


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


struct Position {
    int x;
    int y;
};
struct Block {
  Color color = BLACK;
};

struct TetrominoShape {

    std::vector<Position> offsets;
    Color color;
};

const TetrominoShape TBlock = {{{0, 0}, {-1, 0}, {1, 0}, {0, -1}}, PINK};
const TetrominoShape SquareBlock = {{{0,0}, {1,0}, {1,-1}, {0,-1}}, YELLOW};

class Tetromino {
    TetrominoShape shape;
    std::vector<Position> currentOffsets;
    Position position;
public:
    Tetromino(TetrominoShape shape, Position position): shape(shape), position(position)
    {
        currentOffsets = shape.offsets;
    }

    void RotateCW()
    {
        // if(shape == SquareBlock) return;
        // for(Position& pos : currentOffsets)
        // {
        //     // pos.y = pos.y > 0 ? pos.y - pos.y : pos.y + pos.y;
        // }
    }

    ~Tetromino()
    {
        
    }
};

struct Tetramino {
    std::vector<Position> offsets;
    Color color;
    Position position;
};

const Tetramino t = {{{0, 0}, {-1, 0}, {1, 0}, {0, -1}}, GREEN, {1,8}};




class Well {
    std::vector<Color> wellCol;
    std::vector<std::vector<Color>> well;
    Rectangle wellRect = {padding.x, padding.y, gridSize* WellSizeX, gridSize * WellSizeY};
    Tetramino activePiece;

    void SetActivePiece(Tetramino newPiece) 
    {
        activePiece = newPiece;
        for(auto it = activePiece.offsets.begin(); it != activePiece.offsets.end(); ++it)
        {
            well[activePiece.position.x + it->x][activePiece.position.y + it->y] = activePiece.color;
        }
    }

    bool BlockIsEmpty(const int x, const int y)
    {
        if(x >= well.size() || y >= well[0].size()) return false;
        return well[x][y].r == 0 && well[x][y].g == 0 && well[x][y].b == 0;
    }

    bool BlockIsActive(const int x, const int y)
    {
        std::cout << "Block check: " << x << " " << y << std::endl;
        for(auto it = activePiece.offsets.begin(); it != activePiece.offsets.end(); ++it)
        {
            if(it->x + activePiece.position.x == x && it->y + activePiece.position.y == y)
            {
                std::cout << "block is ACTIVE" << std::endl;
                return true;
            }
        }
        std::cout << "block is INACTIVE" << std::endl;
        return false;
    }

public: 

    Well(const int height, const int width): wellCol(height, {BLACK}), well(width, wellCol)
    {
        SetActivePiece(t);
    }

    ~Well()
    {

    }

    void UpdateWell()
    {
        std::vector<Position> moveSpots;
        // for(int i = 0; i < well.size(); i++)
        // {
        //     for(int j = well[i].size()-2; j > 0; j--)
        //     {
        //         if(!BlockIsEmpty(i, j) && BlockIsEmpty(i, j+1) && !BlockIsActive(i, j))
        //         {
        //             moveSpots.push_back({i, j});
        //         }
                
        //     }
        // }

        bool canMove = true;
        for(auto it = activePiece.offsets.begin(); it != activePiece.offsets.end(); ++it)
        {
            const int x = it->x + activePiece.position.x;
            const int y = it->y + activePiece.position.y;
            std::cout << "Current Pos: " << x << " " << y << std::endl;
            if(!BlockIsActive(x, y+1) && !BlockIsEmpty(x, y+1))
            {
                std::cout << "BLOCK IS FULL " << x << " " << y+1 << std::endl;
                canMove = false;
            }
        }
        if(canMove)
        {
            std::cout << "PIECE CAN MOVE!" << std::endl;
            for(auto it = activePiece.offsets.rbegin(); it != activePiece.offsets.rend(); ++it)
            {
                moveSpots.push_back({it->x + activePiece.position.x, it->y + activePiece.position.y});
            }
            activePiece.position = {activePiece.position.x, activePiece.position.y+1};
        }

        std::for_each(moveSpots.rbegin(), moveSpots.rend(), [this](const Position p){
            std::cout << p.x << " " << p.y << std::endl;
            well[p.x][p.y+1] = well[p.x][p.y];
            well[p.x][p.y] = well[p.x][p.y-1];
        });
    }

    void DrawWell()
    {
        DrawRectangleLinesEx(wellRect, 3, WHITE);
        for(int i = 0; i < well.size(); i++)
        {
            for(int j = 0; j < well[i].size(); j++)
            {
                if(!BlockIsEmpty(i, j))
                {
                    DrawGridSpot(i, j, well[i][j]);
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

// Rotate by 90 deg: 
// Transpose, then reverse rows
Matrix RotateCW(Matrix mat)
{
    Matrix t = MatrixTranspose(mat);
    return Matrix{t.m12, t.m8, t.m4, t.m0, t.m13, t.m9, t.m5, t.m1, t.m14, t.m10, t.m6, t.m2, t.m15, t.m11, t.m7, t.m3};
}

// Rotate by -90 deg:
// Transpose, then reverse columns
Matrix RotateCCW(Matrix mat)
{
    Matrix t = MatrixTranspose(mat);
    return Matrix{t.m3, t.m7, t.m11, t.m15, t.m2, t.m6, t.m10, t.m14, t.m1, t.m5, t.m9, t.m13, t.m0, t.m4, t.m8, t.m12};
}

int main()
{
    std::cout << "Starting the game..." << std::endl;
    InitWindow(resolution.x, resolution.y, "myyrtris");
    SetTargetFPS(60);
    Well well(WellSizeY, WellSizeX);
    const Matrix t = {
        0, 0, 0, 0, 
        0, 1, 1, 1, 
        0, 0, 1, 0, 
        0, 0, 0, 0
    };
    while(WindowShouldClose() == false)
    {
        // EVENTS

        // UPDATES 

        // DRAW
        BeginDrawing();

        ClearBackground(BLACK);
        well.DrawWell();
        well.DrawGrid();

        if(TimeToDrop())
        {
            well.UpdateWell();
            std::cout << "UPDATE" << std::endl;
            PrintMatrix(t);
            std::cout << std::endl;
            PrintMatrix(RotateCW(t));
            std::cout << std::endl;
            PrintMatrix(RotateCCW(t));
        }
    
        EndDrawing();
    }
   
    CloseWindow();
    return 0;
}