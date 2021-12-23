#include "Block.h"
#include "ConstValue.h"

arrPt Block::getBlock()
{
    return pBlock;
}

int Block::getRotationCount()
{
    return rotationCount;
}

int Block::getPositionX()
{
    return x;
}

int Block::getPositionY()
{
    return y;
}

void Block::rotate()
{
    rotationCount = (rotationCount + 1) % 4;
}

void Block::goDown()
{
    ++x;
}

void Block::goLeft()
{
    --y;
}

void Block::goRight()
{
    ++y;
}

void Block::setPositionX(int _x)
{
    x = _x;
}

void Block::setPositionY(int _y)
{
    y = _y;
}

void Block::setBlock(int shape)
{
    pBlock = &Constant::tetrisBlock[shape][0];
}

Block::Block(int shape) : x(0), y(Constant::mapWidth / 2 - 2), rotationCount(0)
{
    pBlock = &Constant::tetrisBlock[shape][rotationCount];
}
