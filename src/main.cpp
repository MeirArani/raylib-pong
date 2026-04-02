#include <raylib-cpp.hpp>
#include <iostream>
#include <algorithm>
#include <stdexcept>


const int WellSizeX = 10;
const int WellSizeY = 20;
const int gridSize = 40;
const Vector2 padding = {80, 80};

const Vector2 resolution = {padding.x * 2 + gridSize * WellSizeX, padding.y * 2 + gridSize * WellSizeY};
int offset = 75;

const double timeToDrop = 1;
double timeSinceLastDrop = 0;

struct Position {
    int x;
    int y;

    Position operator+(const Position &pos)
    {
        return {pos.x + x, pos.y + y};
    }

    Position operator-(const Position &pos)
    {
        return {x - pos.x, y - pos.y};
    }
};


std::ostream& operator<<(std::ostream& os, const Position &pos)
{
    os << "(" << pos.x << ", " << pos.y << ")" << std::endl;
    return os;
}

struct Block {
  Color color = BLACK;
};

std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec)
{
    for(int i = 0; i<vec.size(); i++)
    {
        os << vec[i] << " ";
        if((i+1) % 5 == 0)
        {
            os << std::endl;
        }
    }
    return os;
}




class Tetromino {

    void Transpose()
    {
        std::vector<int> newArr(25, 0);
        newArr[0] = piece[0];
        newArr[1] = piece[5];
        newArr[2] = piece[10];
        newArr[3] = piece[15];
        newArr[4] = piece[20];
        newArr[5] = piece[1];
        newArr[6] = piece[6];
        newArr[7] = piece[11];
        newArr[8] = piece[16];
        newArr[9] = piece[21];
        newArr[10] = piece[2];
        newArr[11] = piece[7];
        newArr[12] = piece[12];
        newArr[13] = piece[17];
        newArr[14] = piece[22];
        newArr[15] = piece[3];
        newArr[16] = piece[8];
        newArr[17] = piece[13];
        newArr[18] = piece[18];
        newArr[19] = piece[23];
        newArr[20] = piece[4];
        newArr[21] = piece[9];
        newArr[22] = piece[14];
        newArr[23] = piece[20];
        newArr[24] = piece[24];
        piece = newArr;
        
        //std::cout<< "TRANSPOSE" << std::endl << piece << std::endl;
    }

    void ReverseRows()
    {
        std::vector<int> newArr(25, 0);
        newArr[0] = piece[4];
        newArr[1] = piece[3];
        newArr[2] = piece[2];
        newArr[3] = piece[1];
        newArr[4] = piece[0];
    
        newArr[5] = piece[9];
        newArr[6] = piece[8];
        newArr[7] = piece[7];
        newArr[8] = piece[6];
        newArr[9] = piece[5];
        
        newArr[10] = piece[14];
        newArr[11] = piece[13];
        newArr[12] = piece[12];
        newArr[13] = piece[11];
        newArr[14] = piece[10];
        
        newArr[15] = piece[19];
        newArr[16] = piece[18];
        newArr[17] = piece[17];
        newArr[18] = piece[16];
        newArr[19] = piece[15];
        
        newArr[20] = piece[24];
        newArr[21] = piece[23];
        newArr[22] = piece[22];
        newArr[23] = piece[21];
        newArr[24] = piece[20];
        piece = newArr;
       // std::cout<< "COL REVERSE: " << std::endl << piece << std::endl;

    }

    void ReverseCols()
    {
        std::vector<int> newArr(25, 0);
        newArr[0] = piece[20];
        newArr[1] = piece[15];
        newArr[2] = piece[10];
        newArr[3] = piece[5];
        newArr[4] = piece[0];
    
        newArr[5] = piece[21];
        newArr[6] = piece[16];
        newArr[7] = piece[11];
        newArr[8] = piece[6];
        newArr[9] = piece[1];
        
        newArr[10] = piece[22];
        newArr[11] = piece[17];
        newArr[12] = piece[12];
        newArr[13] = piece[7];
        newArr[14] = piece[2];
        
        newArr[15] = piece[23];
        newArr[16] = piece[18];
        newArr[17] = piece[13];
        newArr[18] = piece[8];
        newArr[19] = piece[3];
        
        newArr[20] = piece[24];
        newArr[21] = piece[19];
        newArr[22] = piece[14];
        newArr[23] = piece[9];
        newArr[24] = piece[4];
        piece = newArr; 
        
        //std::cout << "ROW REVERSE: " << std::endl << piece << std::endl;
    }

public: 
    Color color;
    std::vector<int> piece;   
    Tetromino(): color(BLACK), piece(25, 0) {}
    Tetromino(std::vector<Position> offsets, Color color): color(color), piece(25, 0)
    {
        piece[12] = 1;
        for(const Position& offset : offsets)
        {
            piece[(offset.x + 2) + (offset.y + 2) * 5] = 1;
        }
    }

    void RotateCW()
    {
        Transpose();
        ReverseRows();
        std::cout << "ROTATE CW:" << std::endl << piece << std::endl;
    }

    void RotateCCW()
    {
        ReverseRows();
        Transpose();
        std::cout << "ROTATE CCW:" << std::endl << piece << std::endl;
    }

    std::vector<Position> GetActiveOffsets() 
    {
        std::vector<Position> offsets;
        for(int i = 0; i<piece.size(); i++)
        {
            if(i == 12) continue; // Always occupied (pivot)
            if(piece[i] != 0)
            {
                offsets.push_back({(i % 5) -2 , (i/5) - 2});
            }
        }
        return offsets;
    }

    std::vector<Position> GetExtremes()
    {
        
    }
};

Tetromino TBlock({{-1, 0}, {1, 0}, {0, -1}}, GREEN);

std::ostream& operator<<(std::ostream& os, const Tetromino& tet)
{
    os << tet.piece << std::endl;
    return os;
}

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
class Well {
    std::vector<Color> wellCol;
    std::vector<std::vector<Color>> well;
    Rectangle wellRect = {padding.x, padding.y, gridSize* WellSizeX, gridSize * WellSizeY};
    Tetromino activePiece;
    Position activePiecePosition;

    void SetActivePiece(Tetromino newPiece)
    {
        activePiece = newPiece;
        activePiecePosition = {2,2};
    }

    bool BlockIsEmpty(const int x, const int y)
    {
        if(x >= well.size() || y >= well[0].size()) return false;
        return well[x][y].r == 0 && well[x][y].g == 0 && well[x][y].b == 0;
    }

    bool BlockIsActive(const int x, const int y)
    {

    }

public: 

    Well(const int height, const int width): wellCol(height, {BLACK}), well(width, wellCol)
    {
        SetActivePiece(TBlock);
    }

    ~Well()
    {

    }

    void UpdateWell()
    {
        if(IsKeyDown(KEY_F))
        {
            activePiece.RotateCW();
        }
        else if(IsKeyDown(KEY_D))
        {
            activePiece.RotateCCW();
        }
        else if(IsKeyDown(KEY_LEFT))
        {
            if(activePiecePosition.x > 0)
            {
                activePiecePosition = {activePiecePosition.x-1, activePiecePosition.y};
            }
        }
        else if(IsKeyDown(KEY_RIGHT))
        {
            if(activePiecePosition.x < well.size())
            {
                activePiecePosition = {activePiecePosition.x + 1, activePiecePosition.y};
            }
        }
        if(CanMove())
        {
                    activePiecePosition = {activePiecePosition.x, activePiecePosition.y + 1};
        }
    }

    bool CanMove()
    {
        return true;
    }

    void DrawWell()
    {
        DrawRectangleLinesEx(wellRect, 3, WHITE);
        // Draw dead blocks
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

        DrawGridSpot(activePiecePosition.x, activePiecePosition.y, activePiece.color);
        for(Position &pos : activePiece.GetActiveOffsets()) 
        {
            DrawGridSpot(pos.x + activePiecePosition.x, pos.y + activePiecePosition.y, activePiece.color);
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



int main()
{
    std::cout << "Starting the game..." << std::endl;
    InitWindow(resolution.x, resolution.y, "myyrtris");
    SetTargetFPS(60);
    Well well(WellSizeY, WellSizeX);

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
        }
    
        EndDrawing();
    }
   
    CloseWindow();
    return 0;
}