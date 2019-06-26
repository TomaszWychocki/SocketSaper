#include <iostream>
#include "GameBoard.h"
#include "common.h"
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
            this->board[i][j].player = NO_PLAYER;
            this->board[i][j].minesAround = 0;
            this->board[i][j].isVisible = 0;

            int randVal = dist100(rng);

            if(randVal <= 10)
            {
                this->board[i][j].type = BoardElementType::MINE;
            }
            else
            {
                this->board[i][j].type = BoardElementType::NO_MINE;
            }
        }
    }

    this->board[0][0].type = BoardElementType::NO_MINE;
    this->board[BOARD_HEIGHT - 1][0].type = BoardElementType::NO_MINE;
    this->board[0][BOARD_WIDTH - 1].type = BoardElementType::NO_MINE;
    this->board[BOARD_HEIGHT - 1][BOARD_WIDTH - 1].type = BoardElementType::NO_MINE;
    this->board[(BOARD_HEIGHT - 1) / 2][(BOARD_WIDTH - 1) / 2].type = BoardElementType::FINISH;
}

void GameBoard::calculateFields()
{
    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            this->board[i][j].minesAround = 0;
        }
    }

    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            if (this->board[i][j].type == BoardElementType::MINE)
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
            if (this->board[k][l].type != BoardElementType::MINE)
            {
                this->board[k][l].minesAround++;
            }
        }
    }
}

gameBoardElement *GameBoard::getBoardPointer()
{
    return &this->board[0][0];
}

void GameBoard::removePlayerFromBoardElement(pos& position)
{
    this->board[position.y][position.x].player = NO_PLAYER;
}

int GameBoard::setPlayerPosition(pos &position, int playerNumber)
{
    this->board[position.y][position.x].isVisible = 1;

    if (this->board[position.y][position.x].type == BoardElementType::MINE)
    {
        return 0;
    }

    this->board[position.y][position.x].player = playerNumber;

    return 1;
}

void GameBoard::showBoard()
{
    for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for (int j = 0; j < BOARD_WIDTH; ++j)
        {
            if (this->board[i][j].type == BoardElementType::NO_MINE)
            {
                std::cout << this->board[i][j].minesAround << "\t";
            }
            else
            {
                std::cout << "M\t";
            }
        }
        std::cout << std::endl;
    }
}
