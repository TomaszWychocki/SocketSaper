#ifndef SAPER_CLIENT_PLAYERHANDLER_H
#define SAPER_CLIENT_PLAYERHANDLER_H

#include <string>
#include "TcpSocket.h"
#include "GameBoard.h"
#include "../Saper_server/common.h"


class PlayerHandler : public TcpSocket
{
public:
    PlayerHandler(int port, std::string name);
    void sendNextMove(MoveDirection direction);
    int isMyMove;

private:
    ssize_t recv_message() override;
    template <class T> void getMessagePayload(basicMsg& input, T& output);
    GameBoard board;
};


#endif //SAPER_CLIENT_PLAYERHANDLER_H
