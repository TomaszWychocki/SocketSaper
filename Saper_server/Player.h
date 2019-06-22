//
// Created by tomasz on 21.06.19.
//

#ifndef SAPER_SERVER_PLAYER_H
#define SAPER_SERVER_PLAYER_H


class Player
{
public:
    Player(int socketFd);

    int get_socket_fd() const;
    void set_socket_fd(int socketFd);

private:
    int socket_fd;
};


#endif //SAPER_SERVER_PLAYER_H
