#include "GameBoard.h"
#include <iostream>


GameBoard::GameBoard()
{

}

void GameBoard::showBoard()
{
    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            std::cout << (int)this->board[i][j] << "\t";
//            switch (this->board[i][j])
//            {
//                case BoardElement::MINE:
//                    std::cout << "I";
//                    break;
//                case BoardElement::NO_MINE:
//                    std::cout << "O";
//                    break;
//                default:
//                    std::cout << " ";
//                    break;
//            }
        }
        std::cout << std::endl;
    }
}

char *GameBoard::getBoardPointer()
{
    return &this->board[0][0];
}
