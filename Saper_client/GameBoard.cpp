#include "GameBoard.h"
#include <iostream>
#include "../Saper_server/common.h"


GameBoard::GameBoard()
{

}

void GameBoard::showBoard()
{
    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            if (this->board[i][j].player != NO_PLAYER)
            {
                std::cout << "P" << this->board[i][j].player << "\t";
            }
            else if (this->board[i][j].isVisible)
            {
                std::cout << this->board[i][j].minesAround << "\t";
            }
            else
            {
                std::cout << "+\t";
            }
        }
        std::cout << std::endl;
    }
}

gameBoardElement *GameBoard::getBoardPointer()
{
    return &this->board[0][0];
}
