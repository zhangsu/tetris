// Blcok.cpp : 实现文件
//

#include "StdAfx.h"
#include "Block.h"
#include "Tetris.h"

Block::Block(unsigned char clr, unsigned char **board)
{
    row = 0;                                                            // 第一行
    col = 4;                                                            // 第五列
    dir = 0;                                                            // 第一种方向
    this->clr = clr;
    this->board = board;
}