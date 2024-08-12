
#include "board.h"

GAME_SQUARES ValidateClick(D2D1_POINT_2F point)
{
    // Case: Click is outside the rectangle play area
    if ((0.f < point.x && point.x < 100.f) || (1000.f < point.x) || (0.f < point.y && point.y < 50.f) || (850.f < point.y))
    {
        if (1030.f <= point.x && point.x <= 1250.f && 150.f <= point.y && point.y <= 200.f) return HIT_RESET;
        return HIT_NONE;
    }

    // Handle every other case of the 9 squares
    // For first row:
    if (point.y < 50.f + 800.f / 3.f)
    {
        if (point.x < 100.f + 900.f / 3) return HIT_SQUARE_1;
        else if (point.x < 100.f + 2.f * 900.f / 3) return HIT_SQUARE_2;
        else return HIT_SQUARE_3;
    }

    // For second row:
    if (point.y < 50.f + 2.f * 800.f / 3.f)
    {
        if (point.x < 100.f + 900.f / 3) return HIT_SQUARE_4;
        else if (point.x < 100.f + 2.f * 900.f / 3) return HIT_SQUARE_5;
        else return HIT_SQUARE_6;
    }

    // For third row:
    if (point.y < 50.f + 800.f)
    {
        if (point.x < 100.f + 900.f / 3) return HIT_SQUARE_7;
        else if (point.x < 100.f + 2.f * 900.f / 3) return HIT_SQUARE_8;
        else return HIT_SQUARE_9;
    }

    // Should never arrive in this case, but compiler complains
    return HIT_ERROR;
}

D2D1_POINT_2F CenterOfSquare(GAME_SQUARES square)
{
    float x = 100.f + 900.f / 6.f + static_cast<float>(square % 3) * 900.f / 3.f;
    float y = 50.f + 800.f / 6.f + static_cast<float>(square / 3) * 800.f / 3.f;
    return D2D1::Point2F(x, y);
}

bool ValidateBoard(char board[9])
{
    for (int i = 0; i < 3; i++)
    {
        if (i >= 3) break;

        // Horizontal lines
        if (abs(board[i * 3] + board[i * 3 + 1] + board[i * 3 + 2]) == 3)
        {
            return false;
        }
        // Vertical lines
        if (abs(board[i] + board[i + 3] + board[i + 6]) == 3)
        {
            return false;
        }
    }
    if (abs(board[0] + board[4] + board[8]) == 3 || abs(board[2] + board[4] + board[6]) == 3) return false;
    return true;
}

bool PlayMove( char board[9])
{
    for (int i = 0; i < 9; i++)
    {

        if (board[i] == 0 && ValidateBoard(board))
        {
            board[i] = -1;
            return true;
        }
    }
    return false;
}
