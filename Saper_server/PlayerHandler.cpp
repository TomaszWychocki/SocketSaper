#include "PlayerHandler.h"
#include "TcpSocket.h"
#include "Player.h"
#include "messages.h"
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
    basicMsg message;
    std::memset(&message, 0, sizeof(basicMsg));

    ssize_t received_bytes = recv(event_fd, &message, sizeof(basicMsg), 0);

    if(received_bytes > 0)
    {
        auto player =
            std::find_if(this->players.begin(), this->players.end(), [this, &event_fd](Player* player)
            {
                return player->get_socket_fd() == event_fd;
            });

        if (player != this->players.end())
        {
            std::cout << "New data from " << event_fd << std::endl;
            std::cout << message.payload << std::endl;
        }
        else if (player == this->players.end())
        {
            auto *new_player = new Player(event_fd);
            this->players.push_back(new_player);

            std::cout << "New player added: " << event_fd << std::endl;
            this->send_message(event_fd, (void*)"Hello\n\0", 7);
        }
    }

    return received_bytes;
}

int PlayerHandler::checkWin(const Player *player)
{
    return 0;
}
