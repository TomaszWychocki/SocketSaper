//
// Created by tomasz on 21.06.19.
//

#include "Player.h"

int Player::getSocketFd() const
{
    return socket_fd;
}

void Player::setSocketFd(int socketFd)
{
    socket_fd = socketFd;
}

Player::Player(int socketFd)
    : socket_fd(socketFd)
{

}

const std::string &Player::getName() const
{
    return this->name;
}

void Player::setName(const std::string &name)
{
    this->name = name;
}
