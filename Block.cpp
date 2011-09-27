#include "StdAfx.h"
#include "Block.h"
#include "Tetris.h"

Block::Block(unsigned char clr, unsigned char **board)
{
    row = 0;
    col = 4;
    dir = 0;
    this->clr = clr;
    this->board = board;
}