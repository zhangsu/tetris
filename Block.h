#pragma once

class Block
{
public:
    Block(unsigned char clr, unsigned char **board);
    virtual bool canMoveDown() = 0;
    virtual bool canMoveLeft() = 0;
    virtual bool canMoveRight() = 0;
    virtual bool canTurn() = 0;
    virtual void moveDown() = 0;
    virtual void moveLeft() = 0;
    virtual void moveRight() = 0;
    virtual void turn() = 0;

protected:
    char row;
    char col;
    unsigned char dir;
    unsigned char clr;
    unsigned char **board;
};

class BlockI : public Block
{
public:
    BlockI(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
private:
    bool passable(char row, char col);
    void show();
    void clear();
};

class BlockJ : public Block
{
public:
    BlockJ(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
};

class BlockL : public Block
{
public:
    BlockL(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
};

class BlockO : public Block
{
public:
    BlockO(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
};

class BlockS : public Block
{
public:
    BlockS(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
private:
    void show();
    void clear();
};

class BlockT : public Block
{
public:
    BlockT(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
private:
    void show();
    void clear();
};

class BlockZ : public Block
{
public:
    BlockZ(unsigned char clr, unsigned char **board);
    void moveDown();
    void moveLeft();
    void moveRight();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void turn();
    bool canTurn();
private:
    void show();
    void clear();
};