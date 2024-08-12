#ifndef _ENUMS_H_
#define _ENUMS_H_

// All hits to track within the window client area
typedef enum
{
    HIT_NONE,
    HIT_SQUARE_1,
    HIT_SQUARE_2,
    HIT_SQUARE_3,
    HIT_SQUARE_4,
    HIT_SQUARE_5,
    HIT_SQUARE_6,
    HIT_SQUARE_7,
    HIT_SQUARE_8,
    HIT_SQUARE_9,
    HIT_RESET,
    HIT_ERROR = 0xff
} GAME_SQUARES;

// All game states of the play board
typedef enum
{
    BOARD_IS_VALID,
    BOARD_PLAYER_WON,
    BOARD_COMPUTER_WON,
    BOARD_DRAW

}VALIDATE_BOARD;

#endif // _ENUMS_H_
