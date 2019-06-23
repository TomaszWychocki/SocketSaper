#include "PlayerHandler.h"
#include <iostream>
#include <string>
#include <cstring>
#include "../Saper_server/common.h"


PlayerHandler::PlayerHandler(int port, std::string name)
    : TcpSocket(port)
{
    basicMsg msg;
    newPlayerMsg payload;

    msg.type = MsgType::NEW_PLAYER;
    strcpy(payload.name, name.c_str());
    memcpy(&msg.payload, &payload, sizeof(newPlayerMsg));

    this->send_message((void*)&msg, sizeof(basicMsg));
}
