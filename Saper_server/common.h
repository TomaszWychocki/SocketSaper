#ifndef SAPER_SERVER_COMMON_H
#define SAPER_SERVER_COMMON_H


typedef struct pos
{
    int x;
    int y;
} pos;

enum MsgType
{
    NEW_PLAYER,
    BOARD,
    CURRENT_ROUND_INFO,
    NEXT_MOVE
};

typedef struct basicMsg
{
    MsgType type;
    char payload[500];
} basicMsg;

typedef struct newPlayerMsg
{
    char name[20];
} newPlayerMsg;

typedef struct boardMsg
{
    char board[250];
} boardMsg;

typedef struct currentRoundInfoMsg
{
    char playerName[20];
    int isMyMove;
} currentRoundInfoMsg;

typedef struct nextMoveMsg
{
    char direction;
} nextMoveMsg;

#endif //SAPER_SERVER_COMMON_H
