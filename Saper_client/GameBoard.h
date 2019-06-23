#ifndef SAPER_CLIENT_GAMEBOARD_H
#define SAPER_CLIENT_GAMEBOARD_H
#define BOARD_HEIGHT 11
#define BOARD_WIDTH 21


enum BoardElement
{
    BLOWN_UP = -2,
    MINE = -1,
    NO_MINE = 0
};

class GameBoard
{
public:
    GameBoard();
    void showBoard();
    char *getBoardPointer();

private:
    char board[BOARD_HEIGHT][BOARD_WIDTH];
};


#endif //SAPER_CLIENT_GAMEBOARD_H
