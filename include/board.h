#ifndef _BOARD_H_
#define _BOARD_H_

#include <d2d1_1.h>
#include <d2d1_1helper.h>

#include "enums.h"


// Determine what if any game square was clicked on
// by the player.
GAME_SQUARES ValidateClick(D2D1_POINT_2F point);


// Given a game square, returns the screen cooridates of the center
// of that square.
D2D1_POINT_2F CenterOfSquare(GAME_SQUARES square);

// Checks the state of the board and if it's still in play
bool ValidateBoard(char board[9]);

// Determine computer's move
bool PlayMove( char board[9]);

#endif // _BOARD_H_
