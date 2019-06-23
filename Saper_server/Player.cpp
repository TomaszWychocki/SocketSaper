#include "Player.h"


Player::Player(int socketFd, int x, int y)
    : socket_fd(socketFd)
{
    this->position.x = x;
    this->position.y = y;
}

int Player::getSocketFd() const
{
    return socket_fd;
}

void Player::setSocketFd(int socketFd)
{
    socket_fd = socketFd;
}

const std::string &Player::getName() const
{
    return this->name;
}

void Player::setName(const std::string &name)
{
    this->name = name;
}
