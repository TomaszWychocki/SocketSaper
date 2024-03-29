#ifndef SAPER_SERVER_PLAYERHANDLER_H
#define SAPER_SERVER_PLAYERHANDLER_H

#include "TcpSocket.h"
#include "Player.h"
#include "GameBoard.h"
#include "common.h"
#include <vector>


class PlayerHandler : public TcpSocket
{
public:
    PlayerHandler(int port, std::vector<Player*> &players);
    void nextRound();

private:
    ssize_t recv_message(int event_fd) override;
    int checkWin(const Player *player);
    void sendBoard(const Player *player);
    void sendCurrentRoundInfo(int playerIndex, int playerJoined);
    template <class T> void getMessagePayload(basicMsg& input, T& output);
    void sendWelcomeMessage(const Player *player);
    void sendPlayerDead(const Player *player);
    void sendPlayerWin(const Player *player);

    int moveRequestSent;
    int currentRound;
    GameBoard board;

protected:
    void onCloseConnection(Player *player) override;
};


#endif //SAPER_SERVER_PLAYERHANDLER_H