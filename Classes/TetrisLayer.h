#pragma once

#include "cocos2d.h"

#include "Block.h"

class TetrisLayer : public cocos2d::Layer
{
private:
    int _time;
    Block curBlock;
    bool isCurBlock;
    int board[22][12];
    int border;
    
public:
    void update(float dt);
    void gameStart();
    void gameEnd();
    
    void drawBlock();
    void drawSprite(int x, int y);
    void drawBoard();
    void addBlock();
    void clearBlock(int x, int y);
    bool moveBlock(int key);
    void rotateBlock();
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
public:
    static Layer* create();
    virtual bool init();
};
