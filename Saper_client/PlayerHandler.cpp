#include "PlayerHandler.h"
#include <iostream>
#include <string>
#include <cstring>
#include "../Saper_server/common.h"


PlayerHandler::PlayerHandler(int port, std::string name)
    : TcpSocket(port)
    , isMyMove(0)
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
        //std::cout << "Received " << bytesReceived << " bytes" << std::endl;

        if (basicMessage.type == MsgType::BOARD)
        {
            std::cout << "MsgType::BOARD received" << std::endl;

            boardMsg boardMessage;
            memcpy(&boardMessage, basicMessage.payload, sizeof(boardMessage));
            memcpy(this->board.getBoardPointer(), boardMessage.board, sizeof(char) * BOARD_WIDTH * BOARD_HEIGHT);

            this->board.showBoard();
        }
        else if (basicMessage.type == MsgType::CURRENT_ROUND_INFO)
        {
            std::cout << "MsgType::CURRENT_ROUND_INFO received" << std::endl;

            currentRoundInfoMsg currentRoundInfoMessage;
            memcpy(&currentRoundInfoMessage, basicMessage.payload, sizeof(currentRoundInfoMessage));

            this->isMyMove = currentRoundInfoMessage.isMyMove;

            if (this->isMyMove)
            {
                std::cout << "Your move!" << std::endl;
            }
            else
            {
                std::cout << "Waiting for " << currentRoundInfoMessage.playerName << std::endl;
            }
        }
    }

    return bytesReceived;
}
