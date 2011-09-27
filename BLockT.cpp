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
    . . . . . * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 1
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * . . . . . .
    . . . . . x * . . . . .
    . . . . . * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 2
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * . . . . . .
    . . . . * x * . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 3
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * . . . . . .
    . . . . * x . . . . . .
    . . . . . * . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
*/

BlockT::BlockT(unsigned char clr, unsigned char **board) : Block(clr, board)
{
    board[0][3] = clr;
    board[0][4] = clr;
    board[0][5] = clr;
    board[1][4] = clr;
}

bool BlockT::canMoveDown()
{
    unsigned char *nextRow = board[row + 1];
    unsigned char *secondNextRow = board[row + 2];
    switch(dir)
    {
    case 0:
        return row < CTetrisDlg::ROW - 2 && !nextRow[col - 1] && !nextRow[col + 1] && !secondNextRow[col];
    case 1:
        return row < CTetrisDlg::ROW - 2 && !secondNextRow[col] && !nextRow[col + 1];
    case 2:
        return row < CTetrisDlg::ROW - 1 && !nextRow[col - 1] && !nextRow[col] && !nextRow[col + 1];
    default:
        return row < CTetrisDlg::ROW - 2 && !secondNextRow[col] && !nextRow[col - 1];
    }
}

bool BlockT::canMoveLeft()
{
    switch(dir)
    {
    case 0:
        return col > 1 && !board[row][col - 2] && !board[row + 1][col - 1];
    case 1:
        return col > 0 && !board[row - 1][col - 1] && !board[row][col - 1] && !board[row + 1][col - 1];
    case 2:
        return col > 1 && !board[row - 1][col - 1] && !board[row][col - 2];
    default:
        return col > 1 && !board[row - 1][col - 1] && !board[row][col - 2] && !board[row + 1][col - 1];
    }
}

bool BlockT::canMoveRight()
{
    switch(dir)
    {
    case 0:
        return col < CTetrisDlg::COL - 2 && !board[row][col + 2] && !board[row + 1][col + 1];
    case 1:
        return col < CTetrisDlg::COL - 2 && !board[row - 1][col + 1] && !board[row][col + 2] && !board[row + 1][col + 1];
    case 2:
        return col < CTetrisDlg::COL - 2 && !board[row - 1][col + 1] && !board[row][col + 2];
    default:
        return col < CTetrisDlg::COL - 1 && !board[row - 1][col + 1] && !board[row][col + 1] && !board[row + 1][col + 1];
    }
}

bool BlockT::canTurn()
{
    switch(dir)
    {
    case 0:
        return row > 0 && !board[row - 1][col];
    case 1:
        return col > 0 && !board[row][col - 1];
    case 2:
        return row < CTetrisDlg::ROW - 1 && !board[row + 1][col];
    default:
        return col < CTetrisDlg::COL - 1 && !board[row][col + 1];
    }
}

void BlockT::moveDown()
{
   unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        thisRow[col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col - 1] = clr;
        nextRow[col + 1] = clr;
        ++row;
        board[row + 1][col] = clr;
        return;
    case 1:
        board[row - 1][col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col + 1] = clr;
        ++row;
        board[row + 1][col] = clr;
        return;
    case 2:
        board[row - 1][col] = 0;
        thisRow[col - 1] = 0;
        thisRow[col + 1] = 0;
        nextRow[col - 1] = clr;
        nextRow[col] = clr;
        nextRow[col + 1] = clr;
        ++row;
        return;
    default:
        board[row - 1][col] = 0;
        thisRow[col - 1] = 0;
        nextRow[col - 1] = clr;
        ++row;
        board[row + 1][col] = clr;
    }
}

void BlockT::moveLeft()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col + 1] = 0;
        nextRow[col] = 0;
        nextRow[col - 1] = clr;
        --col;
        thisRow[col - 1] = clr;
        return;
    case 1:
        prevRow[col] = 0;
        thisRow[col + 1] = 0;
        nextRow[col] = 0;
        prevRow[col - 1] = clr;
        thisRow[col - 1] = clr;
        nextRow[col - 1] = clr;
        --col;
        return;
    case 2:
        prevRow[col] = 0;
        thisRow[col + 1] = 0;
		prevRow[col - 1] = clr;
        --col;
        thisRow[col - 1] = clr;
        return;
    default:
        prevRow[col] = 0;
        thisRow[col] = 0;
        nextRow[col] = 0;
        prevRow[col - 1] = clr;
        nextRow[col - 1] = clr;
        --col;
		thisRow[col - 1] = clr;
    }
}

void BlockT::moveRight()
{
    unsigned char *prevRow = board[row - 1];
    unsigned char *thisRow = board[row];
    unsigned char *nextRow = board[row + 1];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        nextRow[col] = 0;
        nextRow[col + 1] = clr;
        ++col;
        thisRow[col + 1] = clr;
        return;
    case 1:
        prevRow[col] = 0;
        thisRow[col] = 0;
        nextRow[col] = 0;
        prevRow[col + 1] = clr;
		nextRow[col + 1] = clr;
        ++col;
        thisRow[col + 1] = clr;
        return;
    case 2:
        prevRow[col] = 0;
        thisRow[col - 1] = 0;
        prevRow[col + 1] = clr;
        ++col;
        thisRow[col + 1] = clr;
        return;
    default:
        prevRow[col] = 0;
        thisRow[col - 1] = 0;
        nextRow[col] = 0;
        prevRow[col + 1] = clr;
        thisRow[col + 1] = clr;
        nextRow[col + 1] = clr;
        ++col;
    }
}

void BlockT::turn()
{
    switch(dir)
    {
    case 0:
        board[row][col - 1] = 0;
        board[row - 1][col] = clr;
        break;
    case 1:
        board[row + 1][col] = 0;
        board[row][col - 1] = clr;
        break;
    case 2:
        board[row][col + 1] = 0;
        board[row + 1][col] = clr;
        break;
    default:
        board[row - 1][col] = 0;
        board[row][col + 1] = clr;
    }
    ++dir %= 4;
}