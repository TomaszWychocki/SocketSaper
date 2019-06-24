#ifndef SAPER_SERVER_PLAYERHANDLER_H
#define SAPER_SERVER_PLAYERHANDLER_H

#include "TcpSocket.h"
#include "Player.h"
#include "GameBoard.h"
#include <vector>


class PlayerHandler : public TcpSocket
{
public:
    PlayerHandler(int port, std::vector<Player*> &players);
    void nextRound();

private:
    ssize_t recv_message(int event_fd) override;
    int checkWin(const Player *player);
    int moveRequestSent;
    int currentRound;
    void sendBoard(const Player *player);
    void sendCurrentRoundInfo(int playerIndex, int playerJoined);

protected:
    void onCloseConnection(Player *player) override;

private:

    GameBoard board;
};


#endif //SAPER_SERVER_PLAYERHANDLER_H