#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "PlayerHandler.h"
#include "Player.h"
#include <vector>
#include <signal.h>
#include "GameBoard.h"


void sigint_handler(int sig_num);


int doLoop = 1;

int main(int argc, char *argv[])
{
    signal(SIGTERM, sigint_handler);

    std::vector<Player*> players;
    PlayerHandler playerHandler(45189, players);
    GameBoard board;

    board.calculateFields();
    board.showBoard();

    while (doLoop)
    {
        playerHandler.handle_connections();
    }

    return 0;
}

void sigint_handler(int sig_num)
{
    std::cout << "SIGTERM handled\n";
    doLoop = 0;
}