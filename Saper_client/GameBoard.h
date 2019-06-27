#ifndef SAPER_CLIENT_GAMEBOARD_H
#define SAPER_CLIENT_GAMEBOARD_H

#include "../Saper_server/common.h"


class GameBoard
{
public:
    GameBoard();
    void showBoard();
    gameBoardElement *getBoardPointer();

    int currentPlayerNumber;

private:
    gameBoardElement board[BOARD_HEIGHT][BOARD_WIDTH];
};


#endif //SAPER_CLIENT_GAMEBOARD_H
