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
    . . . . * x * . . . . .
    . . . . . . * . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 1
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * . . . . . .
    . . . . . x . . . . . .
    . . . . * * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 2
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . * . . . . . . .
    . . . . * x * . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 3
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * * . . . . .
    . . . . . x . . . . . .
    . . . . . * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
*/

BlockJ::BlockJ(unsigned char clr, unsigned char **board) : Block(clr, board)
{
    board[0][3] = clr;
    board[0][4] = clr;
    board[0][5] = clr;
    board[1][5] = clr;
}

bool BlockJ::canMoveDown()
{
    unsigned char *nextRow = board[row + 1];
    unsigned char *secondNextRow = board[row + 2];
    switch(dir)
    {
    case 0:
        return row < CTetrisDlg::ROW - 2 && !nextRow[col - 1] && !nextRow[col] && !secondNextRow[col + 1];
    case 1:
        return row < CTetrisDlg::ROW - 2 && !secondNextRow[col - 1] && !secondNextRow[col];
    case 2:
        return row < CTetrisDlg::ROW - 1 && !nextRow[col - 1] && !nextRow[col] && !nextRow[col + 1];
    default:
        return row < CTetrisDlg::ROW - 2 && !secondNextRow[col] && !board[row][col + 1];
    }
}

bool BlockJ::canMoveLeft()
{
    switch(dir)
    {
    case 0:
        return col > 1 && !board[row][col - 2] && !board[row + 1][col];
    case 1:
        return col > 1 && !board[row - 1][col - 1] && !board[row][col - 1] && !board[row + 1][col - 2];
    case 2:
        return col > 1 && !board[row - 1][col - 2] && !board[row][col - 2];
    default:
        return col > 0 && !board[row - 1][col - 1] && !board[row][col - 1] && !board[row + 1][col - 1];
    }
}

bool BlockJ::canMoveRight()
{
    switch(dir)
    {
    case 0:
        return col < CTetrisDlg::COL - 2 && !board[row][col + 2] && !board[row + 1][col + 2];
    case 1:
        return col < CTetrisDlg::COL - 1 && !board[row - 1][col + 1] && !board[row][col + 1] && !board[row + 1][col + 1];
    case 2:
        return col < CTetrisDlg::COL - 2 && !board[row - 1][col] && !board[row][col + 2];
    default:
        return col < CTetrisDlg::COL - 2 && !board[row - 1][col + 2] && !board[row][col + 1] && !board[row + 1][col + 1];
    }
}

bool BlockJ::canTurn()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        return row > 0 && !prevRow[col] && !nextRow[col - 1] && !nextRow[col];
    case 1:
        return col < CTetrisDlg::COL - 1 && !prevRow[col - 1] && !thisRow[col - 1] && !thisRow[col + 1];
    case 2:
        return row < CTetrisDlg::ROW - 1 && !prevRow[col] && !prevRow[col + 1] && !nextRow[col];
    default:
        return col > 0 && !thisRow[col - 1] && !thisRow[col + 1] && !nextRow[col + 1];
    }
}

void BlockJ::moveDown()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        thisRow[col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col - 1] = clr;
        nextRow[col] = clr;
        ++row;
        board[row + 1][col + 1] = clr;
        return;
    case 1:
        prevRow[col] = 0;
        nextRow[col - 1] = 0;
        ++row;
        board[row + 1][col - 1] = clr;
        board[row + 1][col] = clr;
        return;
    case 2:
        prevRow[col - 1] = 0;
        thisRow[col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col - 1] = clr;
        nextRow[col] = clr;
        nextRow[col + 1] = clr;
        ++row;
        return;
    default:
        prevRow[col] = 0;
        prevRow[col + 1] = 0;
        thisRow[col + 1] = clr;
        ++row;
        board[row + 1][col] = clr;
    }
}

void BlockJ::moveLeft()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col + 1] = 0;
        nextRow[col + 1] = 0;
        nextRow[col] = clr;
        thisRow[col - 2] = clr;
        --col;
        return;
    case 1:
        prevRow[col] = 0;
        thisRow[col] = 0;
        nextRow[col] = 0;
        prevRow[col - 1] = clr;
        thisRow[col - 1] = clr;
        --col;
        nextRow[col - 1] = clr;
        return;
    case 2:
        prevRow[col - 1] = 0;
        thisRow[col + 1] = 0;
        --col;
        prevRow[col - 1] = clr;
        thisRow[col - 1] = clr;
        return;
    default:
        prevRow[col + 1] = 0;
        thisRow[col] = 0;
        nextRow[col] = 0;
        prevRow[col - 1] = clr;
        thisRow[col - 1] = clr;
        nextRow[col - 1] = clr;
        --col;
    }
}

void BlockJ::moveRight()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        nextRow[col + 1] = 0;
        ++col;
        thisRow[col + 1] = clr;
        nextRow[col + 1] = clr;
        return;
    case 1:
        prevRow[col] = 0;
        thisRow[col] = 0;
        nextRow[col - 1] = 0;
        prevRow[col + 1] = clr;
        thisRow[col + 1] = clr;
        nextRow[col + 1] = clr;
        ++col;
        return;
    case 2:
        prevRow[col - 1] = 0;
        thisRow[col - 1] = 0;
        prevRow[col] = clr;
        ++col;
        thisRow[col + 1] = clr;
        return;
    default:
        prevRow[col] = 0;
        thisRow[col] = 0;
        nextRow[col] = 0;
        thisRow[col + 1] = clr;
        nextRow[col + 1] = clr;
        ++col;
        prevRow[col + 1] = clr;
    }
}

void BlockJ::turn()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        thisRow[col + 1] = 0;
        nextRow[col + 1] = 0;
        prevRow[col] = clr;
        nextRow[col - 1] = clr;
        nextRow[col] = clr;
        break;
    case 1:
        prevRow[col] = 0;
        nextRow[col - 1] = 0;
        nextRow[col] = 0;
        prevRow[col - 1] = clr;
        thisRow[col - 1] = clr;
        thisRow[col + 1] = clr;
        break;
    case 2:
        prevRow[col - 1] = 0;
        thisRow[col - 1] = 0;
        thisRow[col + 1] = 0;
        prevRow[col] = clr;
        prevRow[col + 1] = clr;
        nextRow[col] = clr;
        break;
    default:
        prevRow[col] = 0;
        prevRow[col + 1] = 0;
        nextRow[col] = 0;
        thisRow[col - 1] = clr;
        thisRow[col + 1] = clr;
        nextRow[col + 1] = clr;
    }
    ++dir %= 4;
}