#include "GameBoard.h"
#include <iostream>
#include "../Saper_server/common.h"
#include "colors.h"


GameBoard::GameBoard()
    : currentPlayerNumber(-1)
{

}

void GameBoard::showBoard()
{
    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            if (this->board[i][j].playerNumber != NO_PLAYER)
            {
                int minesAround = this->board[i][j].minesAround;
                if (this->board[i][j].playerNumber != this->currentPlayerNumber)
                {
                    std::cout << BREDFBLK << minesAround << RST;
                }
                else
                {
                    std::cout << BGRNFBLK << minesAround << RST;
                }
            }
            else if (this->board[i][j].isVisible)
            {
                std::cout << this->board[i][j].minesAround;
            }
            else
            {
                std::cout << "-";
            }
        }
        std::cout << std::endl;
    }
}

gameBoardElement *GameBoard::getBoardPointer()
{
    return &this->board[0][0];
}
