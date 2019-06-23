#ifndef SAPER_CLIENT_PLAYERHANDLER_H
#define SAPER_CLIENT_PLAYERHANDLER_H

#include <string>
#include "TcpSocket.h"


class PlayerHandler : public TcpSocket
{
public:
    PlayerHandler(int port, std::string name);
};


#endif //SAPER_CLIENT_PLAYERHANDLER_H
