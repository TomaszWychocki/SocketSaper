#include <iostream>
#include "GameBoard.h"
#include <random>


GameBoard::GameBoard()
{
    this->generateBoard();
}

void GameBoard::generateBoard()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1,100);

    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            int randVal = dist100(rng);

            if(randVal <= 10)
            {
                this->board[i][j] = BoardElement::MINE;
            }
            else
            {
                this->board[i][j] = BoardElement::NO_MINE;
            }
        }
    }

    this->board[0][0] = BoardElement::NO_MINE;
    this->board[BOARD_HEIGHT - 1][0] = BoardElement::NO_MINE;
    this->board[0][BOARD_WIDTH - 1] = BoardElement::NO_MINE;
    this->board[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] = BoardElement::NO_MINE;
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

void GameBoard::calculateFields()
{
    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            if (this->board[i][j] == BoardElement::MINE)
            {
                this->setBoardNumbersAroundMine(i, j);
            }
        }
    }
}

void GameBoard::setBoardNumbersAroundMine(int i, int j)
{
    int k = (i - 1) > 0 ? (i - 1) : 0;

    for (; k < BOARD_HEIGHT && k <= i + 1; ++k)
    {
        int l = (j - 1) > 0 ? (j - 1) : 0;
        for (; l < BOARD_WIDTH && l <= j + 1; ++l)
        {
            if (this->board[k][l] != BoardElement::MINE)
            {
                this->board[k][l]++;
            }
        }
    }
}

char *GameBoard::getBoardPointer()
{
    return &this->board[0][0];
}
