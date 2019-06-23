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

ssize_t PlayerHandler::recv_message()
{
    basicMsg basicMessage;
    std::memset(&basicMessage, 0, sizeof(basicMsg));

    ssize_t bytesReceived = recv(this->client_socket_fd, &basicMessage, sizeof(basicMsg), 0);

    if (bytesReceived > 0)
    {
        std::cout << "Received " << bytesReceived << " bytes" << std::endl;

        if (basicMessage.type == MsgType::BOARD)
        {
            boardMsg boardMessage;
            memcpy(&boardMessage, basicMessage.payload, sizeof(boardMessage));
            memcpy(this->board.getBoardPointer(), boardMessage.board, sizeof(char) * BOARD_WIDTH * BOARD_HEIGHT);

            this->board.showBoard();
        }

        std::cout << basicMessage.payload << std::endl;
    }

    return bytesReceived;
}
