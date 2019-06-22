#ifndef SAPER_SERVER_MESSAGES_H
#define SAPER_SERVER_MESSAGES_H

enum msgType
{
    NEW_PLAYER
};

typedef struct basicMsg
{
    msgType type;
    int sender;
    char payload[1000];
} basicMsg;

typedef struct newPlayerMsg
{
    char name[20];
} newPlayerMsg;

typedef struct boardMsg
{
    char board[250];
} boardMsg;

#endif //SAPER_SERVER_MESSAGES_H
