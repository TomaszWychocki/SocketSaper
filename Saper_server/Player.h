#ifndef SAPER_SERVER_PLAYER_H
#define SAPER_SERVER_PLAYER_H


#include <string>
#include "common.h"


class Player
{
public:
    Player(int socketFd, int x, int y);
    int getSocketFd() const;
    void setSocketFd(int socketFd);
    const std::string &getName() const;
    void setName(const std::string &name);

    pos position;

private:
    int socket_fd;
    std::string name;
};


#endif //SAPER_SERVER_PLAYER_H
