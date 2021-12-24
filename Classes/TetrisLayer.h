#pragma once

#include "cocos2d.h"

#include "Block.h"
#include "ConstValue.h"

class TetrisLayer : public cocos2d::Layer
{
private:
    int _time;
    int score; // not using
    Block curBlock;
    bool isCurBlock;
    int board[Constant::mapHeight][Constant::mapWidth];
    cocos2d::Sprite* boardSprite[Constant::mapHeight][Constant::mapWidth];
    
public:
    void update(float dt);
    void gameStart();
    void gameEnd();
    
    void drawBlock();
    void createSprite(int x, int y);
    void changeSprite(int x, int y);
    void drawBoard();
    void clearBlock(int x, int y);
    
    bool isGround();
    bool enableAddBlock();
    bool enableGround();
    bool moveBlock(int key);
    void addBlock();
    void rotateBlock();
    void checkFullRow();
    int checkCollision();
    int getDirectionMaxCollision(); // 회전시 블록간 충돌을 위한 함수
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
public:
    static Layer* create();
    virtual bool init();
};
