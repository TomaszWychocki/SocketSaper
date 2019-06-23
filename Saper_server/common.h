#ifndef SAPER_SERVER_COMMON_H
#define SAPER_SERVER_COMMON_H

enum MsgType
{
    NEW_PLAYER,
    BOARD
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

#endif //SAPER_SERVER_COMMON_H
