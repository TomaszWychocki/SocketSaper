#include "PlayerHandler.h"
#include "TcpSocket.h"
#include "Player.h"
#include "common.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>


PlayerHandler::PlayerHandler(int port, std::vector<Player*> &players)
    : TcpSocket(port, players)
{

}

ssize_t PlayerHandler::recv_message(int event_fd)
{
    basicMsg basicMessage;
    std::memset(&basicMessage, 0, sizeof(basicMsg));

    ssize_t received_bytes = recv(event_fd, &basicMessage, sizeof(basicMsg), 0);

    if(received_bytes > 0)
    {
        if (basicMessage.type == MsgType::NEW_PLAYER)
        {
            newPlayerMsg msg;
            memcpy(&msg, basicMessage.payload, sizeof(newPlayerMsg));

            auto *new_player = new Player(event_fd);
            new_player->setName(msg.name);
            this->players.push_back(new_player);

            std::cout << "New player added: " << new_player->getName() << std::endl;
            this->send_message(event_fd, (void*)"Hello\n\0", 7);
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

    return received_bytes;
}

int PlayerHandler::checkWin(const Player *player)
{
    return 0;
}
