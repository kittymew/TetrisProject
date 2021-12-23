#include "TetrisLayer.h"
#include "ConstValue.h"

using namespace cocos2d;

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
    _time -= dt;
    
    int backupX = 0;
    int backupY = 0;

    if(isCurBlock == false)
    {
        TetrisLayer::addBlock();
    }
    else
    {
        backupX = curBlock.getPositionX();
        backupY = curBlock.getPositionY();
        
        bool moveDown = TetrisLayer::moveBlock(Constant::down);
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
    arrPt block = curBlock.getBlock();
    
    if(key == Constant::down)
    {
        curBlock.goDown();
    }
    else if(key == Constant::left)
    {
        curBlock.goLeft();
    }
    else if(key == Constant::right)
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
                if(key == Constant::down) // 내려가서 겹친 경우 해당 블록 움직임 종료
                {
                    isCurBlock = false;
                }
                return false;
            }
        }
    }
    
    drawSprite(curBlock.getPositionX(), curBlock.getPositionY());
    return true;
}

void TetrisLayer::rotateBlock() // rotation이 아니라 shape가 변함
{
    clearBlock(curBlock.getPositionX(), curBlock.getPositionY());
    // 회전실패하면 회전 안 되게 해야하면 백업 rotationCounter가 필요
    curBlock.rotate();
    // FIXME 충돌 확인, 충돌하면 좌 또는 우로 이동 (아래, 위로도 이동해야할 때가 있을지도 -> 회전실패로 간주?)
    drawBlock();
}

void TetrisLayer::clearBlock(int x, int y)
{
    arrPt block = curBlock.getBlock();
    
    for(int xidx = x; xidx < x + 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx - x][yidx - y] >= 1)
            {
                board[xidx][yidx] = 0;
                drawSprite(xidx, yidx);
            }
        }
    }
}

void TetrisLayer::drawBlock()
{
    int x = curBlock.getPositionX();
    int y = curBlock.getPositionY();
    arrPt block = curBlock.getBlock();
    
    for(int xidx = x; xidx < x + 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx - x][yidx - y] >= 1)
            {
                board[xidx][yidx] = block[curBlock.getRotationCount()][xidx - x][yidx - y];
                drawSprite(xidx, yidx);
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
            drawSprite(x, y);
        }
    }
}

void TetrisLayer::drawSprite(int x, int y)
{
    Sprite* sprite = Sprite::create("square_white.png");
    switch(board[x][y])
    {
        case 1:
            sprite = Sprite::create("square_red.png");
            break;
        case 2:
            sprite = Sprite::create("square_blue.png");
            break;
        case 3:
            sprite = Sprite::create("square_green.png");
            break;
        case 4:
            sprite = Sprite::create("square_pink.png");
            break;
        case 5:
            sprite = Sprite::create("square_sky.png");
            break;
        case 6:
            sprite = Sprite::create("square_yellow.png");
            break;
        case 7:
            sprite = Sprite::create("square_gray.png");
            break;
    }
    
    int originWidth = sprite->getContentSize().width;
    int originHeight = sprite->getContentSize().height;
    sprite->setScale((double)Constant::blockSize / (double)originWidth, (double)Constant::blockSize / (double)originHeight);
    sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setPosition(Vec2(y * Constant::blockSize + Constant::blockGap,
                             (Constant::mapHeight - x) * Constant::blockSize + Constant::blockGap));
    this->addChild(sprite);
//    delete sprite; error
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
        bool moveLeft = TetrisLayer::moveBlock(Constant::left);
        if(moveLeft == false)
        {
            curBlock.setPositionX(backupX);
            curBlock.setPositionY(backupY);
        }
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        bool moveRight = TetrisLayer::moveBlock(Constant::right);
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
