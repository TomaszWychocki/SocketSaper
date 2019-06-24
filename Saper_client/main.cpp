#include <iostream>
#include <string>
#include <signal.h>
#include <thread>
#include <chrono>
#include "../Saper_server/common.h"
#include "PlayerHandler.h"


void sigint_handler(int sig_num);
MoveDirection getDirection(char direction);

int doLoop = 1;

int main()
{
    signal(SIGTERM, sigint_handler);
    signal(SIGINT, sigint_handler);

    std::string playerName;
    std::cout << "Name: ";
    std::cin >> playerName;
    while ((getchar()) != '\n');

    PlayerHandler playerHandler(45189, playerName);

    while (doLoop)
    {
        playerHandler.handleConnections();

        if (playerHandler.isMyMove)
        {
            MoveDirection direction = MoveDirection::WRONG_DIRECTION;

            while(direction == MoveDirection::WRONG_DIRECTION)
            {
                std::cout << "Direction [W/A/S/D]: ";
                direction = getDirection(getchar());
                while ((getchar()) != '\n');
            }

            playerHandler.sendNextMove(direction);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

void sigint_handler(int sig_num)
{
    std::cout << "Signal received\n";
    doLoop = 0;
}

MoveDirection getDirection(char direction)
{
    switch (toupper(direction))
    {
        case 'W':
            return MoveDirection::UP;
        case 'S':
            return MoveDirection::DOWN;
        case 'A':
            return MoveDirection::LEFT;
        case 'D':
            return MoveDirection::RIGHT;
        default:
            return MoveDirection::WRONG_DIRECTION;
    }
}