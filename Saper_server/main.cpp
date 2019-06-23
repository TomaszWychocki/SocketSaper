#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "PlayerHandler.h"
#include "Player.h"
#include <vector>
#include <thread>
#include <chrono>
#include <signal.h>
#include "GameBoard.h"


void sigint_handler(int sig_num);

int doLoop = 1;

int main(int argc, char *argv[])
{
    signal(SIGTERM, sigint_handler);
    signal(SIGINT, sigint_handler);

    std::vector<Player*> players;
    PlayerHandler playerHandler(45189, players);

    while (doLoop)
    {
        playerHandler.handle_connections();
        playerHandler.nextRound();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

void sigint_handler(int sig_num)
{
    std::cout << "Signal received\n";
    doLoop = 0;
}
