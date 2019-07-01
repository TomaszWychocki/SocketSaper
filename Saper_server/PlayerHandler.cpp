#include "PlayerHandler.h"
#include "TcpSocket.h"
#include "Player.h"
#include "common.h"
#include "GameBoard.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>


PlayerHandler::PlayerHandler(int port, std::vector<Player*> &players)
    : TcpSocket(port, players)
    , currentRound(-1)
    , moveRequestSent(0)
{
    this->board.calculateFields();
    this->board.showBoard();
}

ssize_t PlayerHandler::recv_message(int event_fd)
{
    basicMsg basicMessage;
    std::memset(&basicMessage, 0, sizeof(basicMsg));

    ssize_t bytesReceived = recv(event_fd, &basicMessage, sizeof(basicMsg), 0);

    if(bytesReceived > 0)
    {
        auto player =
            std::find_if(this->players.begin(), this->players.end(), [this, &event_fd](Player *player)
            {
                return player->getSocketFd() == event_fd;
            });

        if (basicMessage.type == MsgType::NEW_PLAYER)
        {
            newPlayerMsg* msg_p = nullptr;
            this->getMessagePayload<newPlayerMsg*>(basicMessage, msg_p);

            int posX = (this->players.size() & 0x01) * (BOARD_WIDTH - 1);
            int posY = ((this->players.size() >> 1) & 0x01) * (BOARD_HEIGHT - 1);

            auto *new_player = new Player(event_fd, posX, posY);
            new_player->setName(msg_p->name);
            this->players.push_back(new_player);

            this->board.setPlayerPosition(new_player->position, new_player->playerNumber);

            std::cout << "New playerNumber added [" << posX << "," << posY << "]: " << new_player->getName() << std::endl;

            this->sendWelcomeMessage(new_player);

            if (this->moveRequestSent)
            {
                // New playerNumber joined after round start
                this->sendBoard(new_player);
            }

            if(this->players.size() > 1)
            {
                this->sendCurrentRoundInfo(this->players.size() - 1, 1);
            }
        }
        else if (basicMessage.type == MsgType::NEXT_MOVE)
        {
            nextMoveMsg* nextMoveMessage_p = nullptr;
            this->getMessagePayload<nextMoveMsg*>(basicMessage, nextMoveMessage_p);

            std::cout << (*player)->getName() << " [" << (*player)->position.x << "," << (*player)->position.y <<"]->";
            this->board.removePlayerFromBoardElement((*player)->position);

            if (nextMoveMessage_p->direction == MoveDirection::UP)
            {
                (*player)->position.y--;
                if ((*player)->position.y < 0) (*player)->position.y = 0;
            }
            else if (nextMoveMessage_p->direction == MoveDirection::DOWN)
            {
                (*player)->position.y++;
                if ((*player)->position.y > BOARD_HEIGHT - 1) (*player)->position.y = BOARD_HEIGHT - 1;
            }
            else if (nextMoveMessage_p->direction == MoveDirection::LEFT)
            {
                (*player)->position.x--;
                if ((*player)->position.x < 0) (*player)->position.x = 0;
            }
            else if (nextMoveMessage_p->direction == MoveDirection::RIGHT)
            {
                (*player)->position.x++;
                if ((*player)->position.x > BOARD_WIDTH - 1) (*player)->position.x = BOARD_WIDTH - 1;
            }

            std::cout << "[" << (*player)->position.x << "," << (*player)->position.y <<"]" << std::endl;

            int result = this->board.setPlayerPosition((*player)->position, (*player)->playerNumber);

            if (!result)
            {
                std::cout << (*player)->getName() << " is dead" << std::endl;
                this->sendPlayerDead(*player);
            }

            if ((*player)->position.x == (BOARD_WIDTH + 1) / 2 && (*player)->position.y == (BOARD_HEIGHT + 1) / 2)
            {
                this->sendPlayerWin(*player);
                exit(0);
            }

            this->moveRequestSent = 0;
        }
    }

    return bytesReceived;
}

int PlayerHandler::checkWin(const Player *player)
{
    return 0;
}

void PlayerHandler::sendBoard(const Player *player)
{
    basicMsg basicMessage;
    boardMsg boardMessage;

    basicMessage.type = MsgType::BOARD;

    memcpy(boardMessage.board, this->board.getBoardPointer(), sizeof(gameBoardElement) * BOARD_ELEMENTS_COUNT);
    memcpy(basicMessage.payload, &boardMessage, sizeof(boardMessage));

    this->send_message(player->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
}

void PlayerHandler::nextRound()
{
    if (this->players.empty() || this->moveRequestSent)
    {
        return;
    }

    this->currentRound++;
    this->board.calculateFields();

    for (int i = 0; i < this->players.size(); i++)
    {
        this->sendBoard(this->players[i]);
        this->sendCurrentRoundInfo(i, 0);
    }

    this->moveRequestSent = 1;
}

void PlayerHandler::sendCurrentRoundInfo(int playerIndex, int playerJoined)
{
    int currentPlayerMove = this->currentRound % this->players.size();
    int isMyMove = currentPlayerMove == playerIndex;

    basicMsg basicMessage;
    currentRoundInfoMsg currentRoundInfoMessage;

    basicMessage.type = MsgType::CURRENT_ROUND_INFO;

    strcpy(currentRoundInfoMessage.playerName, this->players[currentPlayerMove]->getName().c_str());

    if (isMyMove && !playerJoined)
    {
        currentRoundInfoMessage.isMyMove = 1;
    }
    else
    {
        currentRoundInfoMessage.isMyMove = 0;
    }

    memcpy(basicMessage.payload, &currentRoundInfoMessage, sizeof(currentRoundInfoMessage));
    this->send_message(this->players[playerIndex]->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
}

void PlayerHandler::onCloseConnection(Player *player)
{
    auto itr = std::find(this->players.cbegin(), this->players.cend(), player);

    if (itr != this->players.cend())
    {
        int currentPlayerMove = this->currentRound % this->players.size();
        int playerIndex = std::distance(this->players.cbegin(), itr);

        if (currentPlayerMove == playerIndex)
        {
            this->currentRound--;
            this->moveRequestSent = 0;
        }

        this->board.removePlayerFromBoardElement(player->position);
    }
}

template<class T>
void PlayerHandler::getMessagePayload(basicMsg& input, T& output)
{
    output = reinterpret_cast<T>(input.payload);
}

void PlayerHandler::sendWelcomeMessage(const Player *player)
{
    basicMsg basicMessage;
    serverWelcomeMessage welcomeMessage;

    basicMessage.type = MsgType::WELCOME_MESSAGE;
    welcomeMessage.playerNumber = player->playerNumber;
    memcpy(basicMessage.payload, &welcomeMessage, sizeof(serverWelcomeMessage));

    this->send_message(player->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
}

void PlayerHandler::sendPlayerDead(const Player *player)
{
    basicMsg basicMessage;
    playerDeadMsg deadMessage;

    basicMessage.type = MsgType::PLAYER_DEAD;
    deadMessage.playerNumber = player->playerNumber;
    strcpy(deadMessage.playerName, player->getName().c_str());
    memcpy(basicMessage.payload, &deadMessage, sizeof(playerDeadMsg));

    for (const Player *pl : this->players)
    {
        this->send_message(pl->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
    }
}

void PlayerHandler::sendPlayerWin(const Player *player)
{
    basicMsg basicMessage;
    playerWinMsg winMessage;

    basicMessage.type = MsgType::PLAYER_WIN;
    winMessage.playerNumber = player->playerNumber;
    strcpy(winMessage.playerName, player->getName().c_str());
    memcpy(basicMessage.payload, &winMessage, sizeof(playerWinMsg));

    for (const Player *pl : this->players)
    {
        this->send_message(pl->getSocketFd(), (void*)&basicMessage, sizeof(basicMessage));
    }
}
