#ifndef SAPER_SERVER_PLAYER_H
#define SAPER_SERVER_PLAYER_H


#include <string>

class Player
{
public:
    Player(int socketFd);

    int getSocketFd() const;
    void setSocketFd(int socketFd);
    const std::string &getName() const;
    void setName(const std::string &name);

private:
    int socket_fd;
    std::string name;
};


#endif //SAPER_SERVER_PLAYER_H
