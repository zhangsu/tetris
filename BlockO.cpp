#include "StdAfx.h"
#include "Tetris.h"
#include "TetrisDlg.h"

/*
    case dir
    when 0
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . x * . . . . .
    . . . . . * * . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
*/

BlockO::BlockO(unsigned char clr, unsigned char **board) : Block(clr, board)
{
    board[0][4] = clr;
    board[0][5] = clr;
    board[1][4] = clr;
    board[1][5] = clr;
}

bool BlockO::canMoveDown()
{
    return row < CTetrisDlg::ROW - 2 && !board[row + 2][col] && !board[row + 2][col + 1];
}

bool BlockO::canMoveLeft()
{
    return col > 0 && !board[row][col - 1] && !board[row + 1][col - 1];
}

bool BlockO::canMoveRight()
{
    return col < CTetrisDlg::COL - 2 && !board[row][col + 2] && !board[row + 1][col + 2];
}

bool BlockO::canTurn()
{
    return true;
}

void BlockO::moveDown()
{
    board[row][col] = 0;
    board[row][col + 1] = 0;
    board[row + 2][col] = clr;
    board[row + 2][col + 1] = clr;
    ++row;
}

void BlockO::moveLeft()
{
    board[row][col + 1] = 0;
    board[row + 1][col + 1] = 0;
    board[row][col - 1] = clr;
    board[row + 1][col - 1] = clr;
    --col;
}

void BlockO::moveRight()
{
    board[row][col] = 0;
    board[row + 1][col] = 0;
    board[row][col + 2] = clr;
    board[row + 1][col + 2] = clr;
    ++col;
}

void BlockO::turn()
{
}