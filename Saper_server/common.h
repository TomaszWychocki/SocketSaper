#ifndef SAPER_SERVER_COMMON_H
#define SAPER_SERVER_COMMON_H

#define BOARD_HEIGHT 11
#define BOARD_WIDTH 21
#define BOARD_ELEMENTS_COUNT (BOARD_HEIGHT * BOARD_WIDTH)
#define NO_PLAYER -1


typedef struct pos
{
    int x;
    int y;
} pos;

enum MoveDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    WRONG_DIRECTION
};

enum MsgType
{
    NEW_PLAYER,
    BOARD,
    CURRENT_ROUND_INFO,
    NEXT_MOVE
};

enum BoardElementType
{
    FINISH = -2,
    MINE = -1,
    NO_MINE = 0
};

typedef struct gameBoardElement
{
    BoardElementType type : 4;
    int isVisible : 1;
    int player : 3;
    int minesAround : 8;
} gameBoardElement;

typedef struct basicMsg
{
    MsgType type;
    char payload[1024];
} basicMsg;

typedef struct newPlayerMsg
{
    char name[20];
} newPlayerMsg;

typedef struct boardMsg
{
    gameBoardElement board[BOARD_ELEMENTS_COUNT];
} boardMsg;

typedef struct currentRoundInfoMsg
{
    char playerName[20];
    int isMyMove;
} currentRoundInfoMsg;

typedef struct nextMoveMsg
{
    MoveDirection direction;
} nextMoveMsg;

#endif //SAPER_SERVER_COMMON_H
