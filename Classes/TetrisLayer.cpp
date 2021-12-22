#include "TetrisLayer.h"

using namespace cocos2d;

const int down = 80;
const int left = 75;
const int right = 77;
const int up = 72;

Layer* TetrisLayer::create()
{
    Layer *pRet = new(std::nothrow) TetrisLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool TetrisLayer::init()
{
    if(!Layer::init())
        return false;
    
    isCurBlock = false;
    border = 2;
    
    for(int x = 0; x < 22; ++x)
    {
        for(int y = 0; y < 12; ++y)
        {
            board[x][y] = 0;
        }
    }
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TetrisLayer::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    TetrisLayer::drawBoard();
    gameStart();
    
    return true;
}

void TetrisLayer::gameStart()
{
    _time = 1000;
    schedule(CC_SCHEDULE_SELECTOR(TetrisLayer::update), 1.0f); // 1초마다 호출되는 스케줄러 시작
}

void TetrisLayer::update(float dt)
{
//    CCLOG("update start");
    _time -= dt;
    
    auto backupX = 0;
    auto backupY = 0;

    if(isCurBlock == false)
    {
        TetrisLayer::addBlock();
    }
    else
    {
        backupX = curBlock.getPositionX();
        backupY = curBlock.getPositionY();
        
        bool moveDown = TetrisLayer::moveBlock(down);
        if(moveDown == false)
        {
            curBlock.setPositionX(backupX);
            curBlock.setPositionY(backupY);
        }
    }
    
    TetrisLayer::drawBlock();
    
    // 줄 완성되었는지 체크
    // 한칸씩 아래로 내리기

    if(_time <= 0)
    {
        TetrisLayer::gameEnd();
    }
//    CCLOG("update end");
}

void TetrisLayer::gameEnd()
{
    unschedule(CC_SCHEDULE_SELECTOR(TetrisLayer::update)); // 스케줄러 중지
}

void TetrisLayer::addBlock()
{
    int shape = RandomHelper::random_int(0, 6);
    curBlock = Block(shape);
    isCurBlock = true;
}

bool TetrisLayer::moveBlock(int key)
{
    TetrisLayer::clearBlock(curBlock.getPositionX(), curBlock.getPositionY());
    auto block = curBlock.getBlock();
    
    if(key == down)
    {
        curBlock.goDown();
    }
    else if(key == left)
    {
        curBlock.goLeft();
    }
    else if(key == right)
    {
        curBlock.goRight();
    }
    
    for(int xidx = 0; xidx < 4; ++xidx)
    {
        for(int yidx = 0; yidx < 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx][yidx] <= 0) // 빈 공간이면 다음 칸 체크
                continue;
            if(xidx + curBlock.getPositionX() > 21) // 바닥과 충돌
            {
                isCurBlock = false;
                return false;
            }
            if(yidx + curBlock.getPositionY() > 11 || yidx + curBlock.getPositionY() < 0) // 벽면 충돌
            {
                return false;
            }
            if(board[xidx + curBlock.getPositionX()][yidx + curBlock.getPositionY()] >= 1) // 다른 블록과 겹친 상태, 이동 불가
            {
                if(key == down) // 내려가서 겹친 경우 해당 블록 움직임 종료
                {
                    isCurBlock = false;
                }
                return false;
            }
        }
    }
    
    drawRect(curBlock.getPositionX(), curBlock.getPositionY());
    return true;
}

void TetrisLayer::rotateBlock() // rotation이 아니라 shape가 변함
{
    clearBlock(curBlock.getPositionX(), curBlock.getPositionY());
    curBlock.rotate();
    // FIXME 충돌 확인, 충돌하면 좌 또는 우로 이동 (아래, 위로도 이동해야할 때가 있을지도 -> 회전실패로 간주?)
    drawBlock();
}

void TetrisLayer::clearBlock(int x, int y)
{
    auto block = curBlock.getBlock();
    
    for(int xidx = x; xidx < x + 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx - x][yidx - y] >= 1)
            {
                board[xidx][yidx] = 0;
                drawRect(xidx, yidx);
            }
        }
    }
}

void TetrisLayer::drawBlock()
{
    auto x = curBlock.getPositionX();
    auto y = curBlock.getPositionY();
    auto block = curBlock.getBlock();
    
    for(int xidx = x; xidx < x + 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx - x][yidx - y] >= 1)
            {
                board[xidx][yidx] = block[curBlock.getRotationCount()][xidx - x][yidx - y];
                drawRect(xidx, yidx);
            }
        }
    }
}

void TetrisLayer::drawBoard()
{
    for(int x = 0; x < 22; ++x)
    {
        for(int y = 0; y < 12; ++y)
        {
            drawRect(x, y);
        }
    }
}

void TetrisLayer::drawRect(int x, int y)
{
    auto rectNode = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(y * 40 + border, (22 - x) * 40 - border);
    rectangle[1] = Vec2((y + 1) * 40 - border, (22 - x) * 40 - border);
    rectangle[2] = Vec2((y + 1) * 40 - border, (21 - x) * 40 + border);
    rectangle[3] = Vec2(y * 40 + border, (21 - x) * 40 + border);
    
    Color4F color = Color4F(1, 1, 1, 1);
    switch(board[x][y])
    {
        case 1:
            color = Color4F(0, 1, 0, 1);
            break;
        case 2:
            color = Color4F(1, 0, 0, 1);
            break;
        case 3:
            color = Color4F(0, 0, 1, 1);
            break;
        case 4:
            color = Color4F(1, 1, 0, 1);
            break;
        case 5:
            color = Color4F(0, 1, 1, 1);
            break;
        case 6:
            color = Color4F(1, 0, 1, 1);
            break;
        case 7:
            color = Color4F(0.5, 0.5, 0.5, 1);
            break;
    }
    rectNode->drawPolygon(rectangle, 4, color, 1, color);
    this->addChild(rectNode);
}

void TetrisLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(isCurBlock == false)
        return;
    
    int backupX = curBlock.getPositionX();
    int backupY = curBlock.getPositionY();
    
    if(keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
        TetrisLayer::rotateBlock();
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
    {
        bool moveLeft = TetrisLayer::moveBlock(left);
        if(moveLeft == false)
        {
            curBlock.setPositionX(backupX);
            curBlock.setPositionY(backupY);
        }
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        bool moveRight = TetrisLayer::moveBlock(right);
        if(moveRight == false)
        {
            curBlock.setPositionX(backupX);
            curBlock.setPositionY(backupY);
        }
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        // move down or not doing
    }
}
