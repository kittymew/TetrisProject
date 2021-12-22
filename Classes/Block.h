#pragma once

typedef const int(*arrPt)[4][4];
//typedef const int(*blockPt)[4][4][4];

class Block
{
private:
    arrPt pBlock;
    int rotationCount = 0;
    int x = 0;
    int y = 0;
    
public:
    arrPt getBlock();
    int getRotationCount();
    int getPositionX();
    int getPositionY();
    void rotate();
    void goDown();
    void goLeft();
    void goRight();
    void setPositionX(int _x);
    void setPositionY(int _y);
    void setBlock(int shape);

public:
    Block() = default;
    Block(int shape);
};
