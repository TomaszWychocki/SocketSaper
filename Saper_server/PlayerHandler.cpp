#include "PlayerHandler.h"
#include "TcpSocket.h"
#include "Player.h"
#include "common.h"
#include "GameBoard.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>


PlayerHandler::PlayerHandler(int port, std::vector<Player*> &players)
    : TcpSocket(port, players)
    , currentRound(-1)
    , moveRequestSent(0)
{
    this->board.calculateFields();
}

ssize_t PlayerHandler::recv_message(int event_fd)
{
    basicMsg basicMessage;
    std::memset(&basicMessage, 0, sizeof(basicMsg));

    ssize_t bytesReceived = recv(event_fd, &basicMessage, sizeof(basicMsg), 0);

    if(bytesReceived > 0)
    {
        auto player =
            std::find_if(this->players.begin(), this->players.end(), [this, &event_fd](Player *player)
            {
                return player->getSocketFd() == event_fd;
            });

        if (basicMessage.type == MsgType::NEW_PLAYER)
        {
            newPlayerMsg msg;
            memcpy(&msg, basicMessage.payload, sizeof(newPlayerMsg));

            int posX = (this->players.size() & 0x01) * (BOARD_WIDTH - 1);
            int posY = ((this->players.size() >> 1) & 0x01) * (BOARD_HEIGHT - 1);

            auto *new_player = new Player(event_fd, posX, posY);
            new_player->setName(msg.name);
            this->players.push_back(new_player);

            std::cout << "New player added [" << posX << "," << posY <<"]: " << new_player->getName() << std::endl;
            this->sendBoard(new_player);
            this->sendCurrentRoundInfo(this->players.size() - 1);
        }
        else if (basicMessage.type == MsgType::NEXT_MOVE)
        {
            nextMoveMsg nextMoveMessage;
            memcpy(&nextMoveMessage, basicMessage.payload, sizeof(nextMoveMsg));

            std::cout << nextMoveMessage.direction << std::endl;

            this->moveRequestSent = 0;
        }
    }

    return bytesReceived;
}

int PlayerHandler::checkWin(const Player *player)
{
    return 0;
}

void PlayerHandler::sendBoard(const Player *player)
{
    basicMsg basicMessage;
    boardMsg boardMessage;

    basicMessage.type = MsgType::BOARD;

    memcpy(boardMessage.board, this->board.getBoardPointer(), sizeof(char) * BOARD_WIDTH * BOARD_HEIGHT);
    memcpy(basicMessage.payload, &boardMessage, sizeof(boardMessage));

    this->send_message(player->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
}

void PlayerHandler::nextRound()
{
    if (this->players.empty() || this->moveRequestSent)
    {
        return;
    }

    this->currentRound++;

    for (int i = 0; i < this->players.size(); i++)
    {
        this->sendCurrentRoundInfo(i);
    }

    this->moveRequestSent = 1;
}

void PlayerHandler::sendCurrentRoundInfo(int playerIndex)
{
    int currentPlayerMove = this->currentRound % this->players.size();
    int isMyMove = currentPlayerMove == playerIndex;

    basicMsg basicMessage;
    currentRoundInfoMsg currentRoundInfoMessage;

    basicMessage.type = MsgType::CURRENT_ROUND_INFO;

    strcpy(currentRoundInfoMessage.playerName, this->players[currentPlayerMove]->getName().c_str());

    if (isMyMove)
    {
        currentRoundInfoMessage.isMyMove = 1;
    }
    else
    {
        currentRoundInfoMessage.isMyMove = 0;
    }

    memcpy(basicMessage.payload, &currentRoundInfoMessage, sizeof(currentRoundInfoMessage));

    this->send_message(this->players[playerIndex]->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
}

void PlayerHandler::onCloseConnection(Player *player)
{
    auto itr = std::find(this->players.cbegin(), this->players.cend(), player);

    if (itr != this->players.cend())
    {
        int currentPlayerMove = this->currentRound % this->players.size();
        int playerIndex = std::distance(this->players.cbegin(), itr);

        if (currentPlayerMove == playerIndex)
        {
            this->moveRequestSent = 0;
        }
    }
}
