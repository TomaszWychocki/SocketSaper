//
// Created by tomasz on 21.06.19.
//

#include "Player.h"

int Player::get_socket_fd() const
{
    return socket_fd;
}

void Player::set_socket_fd(int socketFd)
{
    socket_fd = socketFd;
}

Player::Player(int socketFd)
    : socket_fd(socketFd)
{

}
