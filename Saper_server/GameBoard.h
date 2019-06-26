#ifndef SAPER_SERVER_GAMEBOARD_H
#define SAPER_SERVER_GAMEBOARD_H

#include "common.h"

class GameBoard
{
public:
    GameBoard();
    void calculateFields();
    gameBoardElement *getBoardPointer();
    void removePlayerFromBoardElement(pos& position);
    int setPlayerPosition(pos& position, int playerNumber);
    void showBoard();

private:
    gameBoardElement board[BOARD_HEIGHT][BOARD_WIDTH];
    void generateBoard();
    void setBoardNumbersAroundMine(int i, int j);
};


#endif //SAPER_SERVER_GAMEBOARD_H
