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
    } else if(key == Constant::up)
    {
        curBlock.goUp();
    }
    
    int check = checkCollision();
    switch(check)
    {
        case Constant::collision_no:
            drawSprite(curBlock.getPositionX(), curBlock.getPositionY());
            return true;
        case Constant::collision_ground:
            isCurBlock = false; // 다른 곳에서 하기
        case Constant::collision_lwall:
        case Constant::collision_rwall:
            return false;
        case Constant::collision_block:
            if(key == Constant::down) // 내려가서 겹친 경우 해당 블록 움직임 종료 -> 여기서 말고 다른 곳에서
            {
                isCurBlock = false;
            }
            return false;
        case Constant::collision_error: // FIXME error 처리
        default:
            return false; // FIXME, 여기에 걸리는 상황이 오면 안 됨, error 처리?
    }
}

void TetrisLayer::rotateBlock() // rotation이 아니라 shape가 변함
{
    clearBlock(curBlock.getPositionX(), curBlock.getPositionY());
    // 회전실패하면 회전 안 되게 해야하면 백업 rotationCounter가 필요 -> 최대 3칸 이동해보고 안 되면 실패
    int backupRotation = curBlock.getRotationCount();
    int backupX = curBlock.getPositionX();
    int backupY = curBlock.getPositionY();
    
    curBlock.rotate();
    // FIXME 충돌 확인, 충돌하면 좌 또는 우로 이동 (아래, 위로도 이동해야할 때가 있을지도 -> 회전실패로 간주?)
    arrPt block = curBlock.getBlock();
    
    bool successRotation = false;
    for(int idx = 0; idx < 3; ++idx) // 최대 3칸 이동해보고 불가능하면 회전 실패로 간주
    {
        int check = checkCollision();
        switch(check)
        {
            case Constant::collision_no:
                successRotation = true;
                break; // 충돌 없음 회전 성공
            case Constant::collision_ground:
                curBlock.goUp();
                break;
            case Constant::collision_lwall:
                curBlock.goRight();
                break;
            case Constant::collision_rwall:
                curBlock.goLeft();
                break;
            case Constant::collision_block:
                // FIXME 추가해야함.. 어떻게 할까
                break;
            case Constant::collision_error:
                break; // FIXME error 처리
            default:
                break; // FIXME, 여기에 걸리는 상황이 오면 안 됨, error 처리?
        }
        
        if(successRotation)
            break;
    }
    
    if(successRotation == false)
    {
        curBlock.setPositionX(backupX);
        curBlock.setPositionY(backupY);
        curBlock.setRotationCount(backupRotation);
    }
    drawBlock();
}

int TetrisLayer::checkCollision()
{
    if(isCurBlock == false)
        return Constant::collision_error;
    
    arrPt block = curBlock.getBlock();
    
    for(int xidx = 0; xidx < 4; ++xidx)
    {
        for(int yidx = 0; yidx < 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx][yidx] <= 0) // 빈 공간이면 다음 칸 체크
                continue;
            if(xidx + curBlock.getPositionX() > Constant::mapHeight - 1) // 바닥과 충돌
                return Constant::collision_ground;
            if(yidx + curBlock.getPositionY() > Constant::mapWidth - 1) // 벽면 충돌
                return Constant::collision_rwall;
            if(yidx + curBlock.getPositionY() < 0)
                return Constant::collision_lwall;
            if(board[xidx + curBlock.getPositionX()][yidx + curBlock.getPositionY()] >= 1) // 다른 블록과 겹친 상태, 이동 불가
                return Constant::collision_block;
        }
    }
    return Constant::collision_no;
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
    if(x < 0 || x > Constant::mapHeight - 1)
        return;
    if(y < 0 || y > Constant::mapWidth - 1)
        return;
    
    Sprite* sprite = Sprite::create("square_white.png");
    switch(board[x][y])
    {
        case 1:
            sprite->setTexture("square_red.png");
            break;
        case 2:
            sprite->setTexture("square_blue.png");
            break;
        case 3:
            sprite->setTexture("square_green.png");
            break;
        case 4:
            sprite->setTexture("square_pink.png");
            break;
        case 5:
            sprite->setTexture("square_sky.png");
            break;
        case 6:
            sprite->setTexture("square_yellow.png");
            break;
        case 7:
            sprite->setTexture("square_gray.png");
            break;
    }
    
    int originWidth = sprite->getContentSize().width;
    int originHeight = sprite->getContentSize().height;
    sprite->setScale((double)Constant::blockSize / (double)originWidth, (double)Constant::blockSize / (double)originHeight);
    sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setPosition(Vec2(y * Constant::blockSize + Constant::blockGap,
                             (Constant::mapHeight - x) * Constant::blockSize + Constant::blockGap));
    this->addChild(sprite);
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
