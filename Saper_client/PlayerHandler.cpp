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

            boardMsg* boardMessage_p = nullptr;
            this->getMessagePayload<boardMsg*>(basicMessage, boardMessage_p);
            memcpy(this->board.getBoardPointer(), boardMessage_p->board,
                   sizeof(gameBoardElement) * BOARD_ELEMENTS_COUNT);

            this->board.showBoard();
        }
        else if (basicMessage.type == MsgType::CURRENT_ROUND_INFO)
        {
            //std::cout << "MsgType::CURRENT_ROUND_INFO received" << std::endl;

            currentRoundInfoMsg* currentRoundInfoMessage_p = nullptr;
            this->getMessagePayload<currentRoundInfoMsg*>(basicMessage, currentRoundInfoMessage_p);

            this->isMyMove = currentRoundInfoMessage_p->isMyMove;

            if (this->isMyMove)
            {
                std::cout << "Your move!" << std::endl;
            }
            else
            {
                std::cout << "Waiting for " << currentRoundInfoMessage_p->playerName << std::endl;
            }

//            std::cout << "====================" << std::endl;
//            std::cout << "CURRENT: " << currentRoundInfoMessage.playerName << std::endl;
//            std::cout << "isMyMove: " << currentRoundInfoMessage.isMyMove << std::endl;
//            std::cout << "====================" << std::endl;
        }
    }

    return bytesReceived;
}

void PlayerHandler::sendNextMove(MoveDirection direction)
{
    this->isMyMove = 0;

    basicMsg msg;
    nextMoveMsg nextMoveMessage;

    msg.type = MsgType::NEXT_MOVE;
    nextMoveMessage.direction = direction;
    memcpy(&msg.payload, &nextMoveMessage, sizeof(nextMoveMessage));

    this->send_message((void*)&msg, sizeof(basicMsg));
}

template<class T>
void PlayerHandler::getMessagePayload(basicMsg &input, T& output)
{
    output = reinterpret_cast<T>(input.payload);
}
