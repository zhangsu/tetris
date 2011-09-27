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
    . . . . * x * * . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    when 1
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . . . . . . . .
    . . . . . * . . . . . . 
    . . . . . x . . . . . . 
    . . . . . * . . . . . . 
    . . . . . * . . . . . . 
    . . . . . . . . . . . .
    . . . . . . . . . . . .
*/

BlockI::BlockI(unsigned char clr, unsigned char **board) : Block(clr, board)
{
    board[0][3] = clr;
    board[0][4] = clr;
    board[0][5] = clr;
    board[0][6] = clr;
}

bool BlockI::canMoveDown()
{
    switch(dir)
    {
    case 0:
        return passable(row + 1, col);
    default:
        return row < CTetrisDlg::ROW - 3 && !board[row + 3][col];
    }
}

bool BlockI::canMoveLeft()
{
    switch(dir)
    {
    case 0:
        return col > 1 && !board[row][col - 2];
    default:
        return passable(row, col - 1);
    }
}

bool BlockI::canMoveRight()
{
    switch(dir)
    {
    case 0:
        return col < CTetrisDlg::COL - 3 && !board[row][col + 3];
    default:
        return passable(row, col + 1);
    }
}

bool BlockI::canTurn()
{
    switch(dir)
    {
    case 0:
        return row > 0 && row < CTetrisDlg::ROW - 2 && !board[row - 1][col] && !board[row + 1][col] && !board[row + 2][col];
    default:
        if(col > 0 && col < CTetrisDlg::COL - 2)
        {
            unsigned char *thisRow = board[row];
            return !thisRow[col - 1] && !thisRow[col + 1] && !thisRow[col + 2];
        }
        return false;
    }
}

void BlockI::moveDown()
{
    switch(dir)
    {
    case 0:
        clear();
        ++row;
        show();
        return;
    default:
        board[row - 1][col] = 0;
        ++row;
        board[row + 2][col] = clr;
    }
}

void BlockI::moveLeft()
{
    unsigned char *thisRow = board[row];
    switch(dir)
    {
    case 0:
        thisRow[col + 2] = 0;
        --col;
        thisRow[col - 1] = clr;
        return;
    default:
        clear();
        --col;
        show();
    }
}

void BlockI::moveRight()
{
    unsigned char *thisRow = board[row];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        ++col;
        thisRow[col + 2] = clr;
        return;
    default:
        clear();
        ++col;
        show();
    }
}

void BlockI::turn()
{
    clear();
    dir ^= 1;
    show();
}

bool BlockI::passable(char row, char col)
{
    switch(dir)
    {
    case 0:
        if(row < CTetrisDlg::ROW && col > 0 && col < CTetrisDlg::COL - 2)
        {
            unsigned char *thisRow = board[row];
            return !thisRow[col - 1] && !thisRow[col] && !thisRow[col + 1] && !thisRow[col + 2];
        }
        return false;
    default:
        if(row > 0 && row < CTetrisDlg::ROW - 2 && col > -1 && col < CTetrisDlg::COL)
        {
            return !board[row - 1][col] && !board[row][col] && !board[row + 1][col] && !board[row + 2][col];
        }
        return false;
    }
}

void BlockI::show()
{
    unsigned char *thisRow = board[row];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = clr;
        thisRow[col] = clr;
        thisRow[col + 1] = clr;
        thisRow[col + 2] = clr;
        return;
    default:
        board[row - 1][col] = clr;
        board[row][col] = clr;
        board[row + 1][col] = clr;
        board[row + 2][col] = clr;
    }
}

void BlockI::clear()
{
    unsigned char *thisRow = board[row];
    switch(dir)
    {
    case 0:
        thisRow[col - 1] = 0;
        thisRow[col] = 0;
        thisRow[col + 1] = 0;
        thisRow[col + 2] = 0;
        return;
    default:
        board[row - 1][col] = 0;
        board[row][col] = 0;
        board[row + 1][col] = 0;
        board[row + 2][col] = 0;
    }
}