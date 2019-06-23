#ifndef SAPER_CLIENT_PLAYERHANDLER_H
#define SAPER_CLIENT_PLAYERHANDLER_H

#include <string>
#include "TcpSocket.h"
#include "GameBoard.h"


class PlayerHandler : public TcpSocket
{
public:
    PlayerHandler(int port, std::string name);

private:
    ssize_t recv_message() override;
    GameBoard board;
};


#endif //SAPER_CLIENT_PLAYERHANDLER_H
