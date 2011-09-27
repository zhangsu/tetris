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
    . . . . * x . . . . . .
    . . . . . * * . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 1
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . * . . . . .
    . . . . . x * . . . . .
    . . . . . * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
*/

BlockZ::BlockZ(unsigned char clr, unsigned char **board) : Block(clr, board)
{
    board[0][3] = clr;
    board[0][4] = clr;
    board[1][4] = clr;
    board[1][5] = clr;
}

bool BlockZ::canMoveDown()
{
    unsigned char *secondNextRow = board[row + 2];
    switch(dir)
    {
    case 0:
        return row < CTetrisDlg::ROW - 2 && !board[row + 1][col - 1] && !secondNextRow[col] && !secondNextRow[col + 1];
    default:
        return row < CTetrisDlg::ROW - 2 && !board[row + 1][col + 1] && !secondNextRow[col];
    }
}

bool BlockZ::canMoveLeft()
{
    switch(dir)
    {
    case 0:
        return col > 1 && !board[row][col - 2] && !board[row + 1][col - 1];
    default:
        return col > 0 && !board[row - 1][col] && !board[row][col - 1] && !board[row + 1][col - 1];
    }
}

bool BlockZ::canMoveRight()
{
    switch(dir)
    {
    case 0:
        return col < CTetrisDlg::COL - 2 && !board[row][col + 1] && !board[row + 1][col + 2];
    default:
        return col < CTetrisDlg::COL - 2 && !board[row - 1][col + 2] && !board[row][col + 2] && !board[row + 1][col + 1];
    }
}

bool BlockZ::canTurn()
{
    switch(dir)
    {
    case 0:
        return row > 0 && !board[row - 1][col + 1] && !board[row][col + 1];
    default:
        return col > 0 && !board[row][col - 1] && !board[row + 1][col + 1];
    }
}

void BlockZ::moveDown()
{
   unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    unsigned char *secondNextRow = board[row + 2];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        thisRow[col] = 0;
        nextRow[col + 1] = 0;
        nextRow[col - 1] = clr;
        secondNextRow[col] = clr;
        secondNextRow[col + 1] = clr;
        break;
    default:
        board[row - 1][col + 1] = 0;
        thisRow[col] = 0;
        nextRow[col + 1] = clr;
        secondNextRow[col] = clr;
    }
    ++row;
}

void BlockZ::moveLeft()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col] = 0;
        nextRow[col + 1] = 0;
        thisRow[col - 2] = clr;
        nextRow[col - 1] = clr;
        break;
    default:
        prevRow[col + 1] = 0;
        thisRow[col + 1] = 0;
        nextRow[col] = 0;
        prevRow[col] = clr;
        thisRow[col - 1] = clr;
        nextRow[col - 1] = clr;
    }
    --col;
}

void BlockZ::moveRight()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        nextRow[col] = 0;
        thisRow[col + 1] = clr;
        nextRow[col + 2] = clr;
        break;
    default:
        prevRow[col + 1] = 0;
        thisRow[col] = 0;
        nextRow[col] = 0;
        prevRow[col + 2] = clr;
        thisRow[col + 2] = clr;
        nextRow[col + 1] = clr;
    }
    ++col;
}

void BlockZ::turn()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        board[row + 1][col + 1] = 0;
        prevRow[col + 1] = clr;
        thisRow[col + 1] = clr;
        break;
    default:
        prevRow[col + 1] = 0;
        thisRow[col + 1] = 0;
        thisRow[col - 1] = clr;
        board[row + 1][col + 1] = clr;
    }
    dir ^= 1;
}