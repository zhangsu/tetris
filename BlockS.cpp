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
    . . . . * * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 1
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * . . . . . .
    . . . . . x * . . . . .
    . . . . . . * . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
*/

BlockS::BlockS(unsigned char clr, unsigned char **board) : Block(clr, board)
{
    board[0][4] = clr;
    board[0][5] = clr;
    board[1][3] = clr;
    board[1][4] = clr;
}

bool BlockS::canMoveDown()
{
    unsigned char *secondNextRow = board[row + 2];
    switch(dir)
    {
    case 0:
        return row < CTetrisDlg::ROW - 2 && !board[row + 1][col + 1] && !secondNextRow[col - 1] && !secondNextRow[col];
    default:
        return row < CTetrisDlg::ROW - 2 && !board[row + 1][col] && !secondNextRow[col + 1];
    }
}

bool BlockS::canMoveLeft()
{
    switch(dir)
    {
    case 0:
        return col > 1 && !board[row][col - 1] && !board[row + 1][col - 2];
    default:
        return col > 0 && !board[row - 1][col - 1] && !board[row][col - 1] && !board[row + 1][col];
    }
}

bool BlockS::canMoveRight()
{
    switch(dir)
    {
    case 0:
        return col < CTetrisDlg::COL - 2 && !board[row][col + 2] && !board[row + 1][col + 1];
    default:
        return col < CTetrisDlg::COL - 2 && !board[row - 1][col + 1] && !board[row][col + 2] && !board[row + 1][col + 2];
    }
}

bool BlockS::canTurn()
{
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        return row > 0 && !board[row - 1][col] && !nextRow[col + 1];
    default:
        return col > 0 && !nextRow[col - 1] && !nextRow[col];
    }
}

void BlockS::moveDown()
{
   unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    unsigned char *secondNextRow = board[row + 2];
    switch(dir)
    {
    case 0:
        thisRow[col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col - 1] = 0;
        nextRow[col + 1] = clr;
        secondNextRow[col - 1] = clr;
        secondNextRow[col] = clr;
        break;
    default:
        board[row - 1][col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col] = clr;
        secondNextRow[col + 1] = clr;
    }
    ++row;
}

void BlockS::moveLeft()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col + 1] = 0;
        nextRow[col] = 0;
        thisRow[col - 1] = clr;
        nextRow[col - 2] = clr;
        break;
    default:
        prevRow[col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col + 1] = 0;
        prevRow[col - 1] = clr;
        thisRow[col - 1] = clr;
        nextRow[col] = clr;
    }
    --col;
}

void BlockS::moveRight()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col] = 0;
        nextRow[col - 1] = 0;
        thisRow[col + 2] = clr;
        nextRow[col + 1] = clr;
        break;
    default:
        prevRow[col] = 0;
        thisRow[col] = 0;
        nextRow[col + 1] = 0;
        prevRow[col + 1] = clr;
        thisRow[col + 2] = clr;
        nextRow[col + 2] = clr;
    }
    ++col;
}

void BlockS::turn()
{
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        nextRow[col - 1] = 0;
        nextRow[col] = 0;
        board[row - 1][col] = clr;
        nextRow[col + 1] = clr;
        break;
    default:
        board[row - 1][col] = 0;
        nextRow[col + 1] = 0;
        nextRow[col - 1] = clr;
        nextRow[col] = clr;
    }
    dir ^= 1;
}