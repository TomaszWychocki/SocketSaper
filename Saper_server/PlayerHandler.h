#ifndef SAPER_SERVER_PLAYERHANDLER_H
#define SAPER_SERVER_PLAYERHANDLER_H

#include "TcpSocket.h"
#include "Player.h"
#include <vector>


class PlayerHandler : public TcpSocket
{
public:
    PlayerHandler(int port, std::vector<Player*> &players);

private:
    ssize_t recv_message(int event_fd) override;
    int checkWin(const Player *player);
};


#endif //SAPER_SERVER_PLAYERHANDLER_H