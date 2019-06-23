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
{
    this->board.calculateFields();
    this->board.showBoard();
}

ssize_t PlayerHandler::recv_message(int event_fd)
{
    basicMsg basicMessage;
    std::memset(&basicMessage, 0, sizeof(basicMsg));

    ssize_t bytesReceived = recv(event_fd, &basicMessage, sizeof(basicMsg), 0);

    if(bytesReceived > 0)
    {
        if (basicMessage.type == MsgType::NEW_PLAYER)
        {
            newPlayerMsg msg;
            memcpy(&msg, basicMessage.payload, sizeof(newPlayerMsg));

            auto *new_player = new Player(event_fd);
            new_player->setName(msg.name);
            this->players.push_back(new_player);

            std::cout << "New player added: " << new_player->getName() << std::endl;
            this->sendBoard(new_player);
        }
        else
        {
            auto player =
                    std::find_if(this->players.begin(), this->players.end(), [this, &event_fd](Player *player)
                    {
                        return player->getSocketFd() == event_fd;
                    });

            if (player != this->players.end())
            {
                std::cout << "New data from " << event_fd << std::endl;
                std::cout << basicMessage.payload << std::endl;
            }
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
