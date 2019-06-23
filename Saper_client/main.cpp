#include <iostream>
#include <string>
#include <signal.h>
#include <thread>
#include <chrono>
#include "PlayerHandler.h"


void sigint_handler(int sig_num);

int doLoop = 1;

int main()
{
    signal(SIGTERM, sigint_handler);
    signal(SIGINT, sigint_handler);

    std::string playerName;
    std::cout << "Name: ";
    std::cin >> playerName;

    PlayerHandler playerHandler(45189, playerName);

    while (doLoop)
    {
        playerHandler.handleConnections();

        if (playerHandler.isMyMove)
        {
            char direction;
            std::cout << "Direction [W/A/S/D]: ";
            std::cin >> direction;
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