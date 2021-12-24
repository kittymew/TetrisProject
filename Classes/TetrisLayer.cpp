#include <set>

#include "TetrisLayer.h"
#include "ConstValue.h"
#include "GameoverLayer.h"

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
    
    for(int x = 0; x < Constant::mapHeight; ++x)
    {
        for(int y = 0; y < Constant::mapWidth; ++y)
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
    _time = 1000; // test value
    schedule(CC_SCHEDULE_SELECTOR(TetrisLayer::update), Constant::speedDown);
}

void TetrisLayer::update(float dt)
{
    _time -= dt;
    
    int backupX = 0;
    int backupY = 0;

    if(isCurBlock == false)
    {
        TetrisLayer::addBlock();
        if(TetrisLayer::enableAddBlock() == false)
        {
            TetrisLayer::gameEnd();
        }
        TetrisLayer::drawBlock();
    }
    else
    {
        if(TetrisLayer::isGround())
        {
            if(TetrisLayer::enableGround() == false)
            {
                TetrisLayer::gameEnd();
            }
            TetrisLayer::checkFullRow();
        }
        else
        {
            bool moveDown = TetrisLayer::moveBlock(Constant::down);
            TetrisLayer::drawBlock();
        }
    }

    if(_time <= 0)
    {
        TetrisLayer::gameEnd();
    }
}

void TetrisLayer::gameEnd()
{
    unschedule(CC_SCHEDULE_SELECTOR(TetrisLayer::update)); // 스케줄러 중지
    _eventDispatcher->removeAllEventListeners();
    isCurBlock = false;
    
    Layer* layer = GameoverLayer::create();
    Scene* scene = Director::getInstance()->getRunningScene();
    if(scene != nullptr)
    {
        scene->removeAllChildren();
        scene->addChild(layer);
    }
}

void TetrisLayer::addBlock()
{
    int shape = RandomHelper::random_int(0, 6);
    curBlock = Block(shape);
    arrPt block = curBlock.getBlock();
    int rotation = curBlock.getRotationCount();
    for(int y = 0; y < 4; ++y)
    {
        if(block[rotation][0][y] >= 1)
            break;
        if(y == 3)
        {
            curBlock.setPositionX(-1);
        }
    }
    isCurBlock = true;
}

bool TetrisLayer::enableAddBlock()
{
    if(isCurBlock == false)
    {
        // 에러
    }
    
    int x = curBlock.getPositionX();
    int y = curBlock.getPositionY();
    int rotation = curBlock.getRotationCount();
    arrPt block = curBlock.getBlock();

    for(int xidx = x; xidx < x+ 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[rotation][xidx - x][yidx - y] < 1)
                continue;
            if(board[xidx][yidx] >= 1)
                return false;
        }
    }
    
    return true;
}

bool TetrisLayer::enableGround()
{
    if(isCurBlock == false)
    {
        // 에러
    }
    
    int x = curBlock.getPositionX();
    int y = curBlock.getPositionY();
    int rotation = curBlock.getRotationCount();
    arrPt block = curBlock.getBlock();

    for(int xidx = x; xidx < x+ 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[rotation][xidx - x][yidx - y] < 1)
                continue;
            if(xidx - x < 0)
                return false;
            else
                return true;
        }
    }
    
    return true;
}

bool TetrisLayer::moveBlock(int key)
{
    int backupX = curBlock.getPositionX();
    int backupY = curBlock.getPositionY();
    
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
            drawBlock();
            return true;
        case Constant::collision_ground:
        case Constant::collision_lwall:
        case Constant::collision_rwall:
        case Constant::collision_block:
        case Constant::collision_ceiling:
            curBlock.setPositionX(backupX);
            curBlock.setPositionY(backupY);
            return false;
        case Constant::collision_error: // FIXME error 처리
        default:
            return false; // FIXME, 여기에 걸리는 상황이 오면 안 됨, error 처리?
    }
}

void TetrisLayer::rotateBlock()
{
    clearBlock(curBlock.getPositionX(), curBlock.getPositionY());
    int backupRotation = curBlock.getRotationCount();
    int backupX = curBlock.getPositionX();
    int backupY = curBlock.getPositionY();
    
    curBlock.rotate();
    
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
            case Constant::collision_ceiling:
                curBlock.goDown();
                break;
            case Constant::collision_block:
            {
                int direction = getDirectionMaxCollision();
                if(direction == Constant::left)
                {
                    curBlock.goLeft();
                }
                else if(direction == Constant::right)
                {
                    curBlock.goRight();
                }
                else if(direction == Constant::up)
                {
                    curBlock.goUp();
                }
                break;
            }
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

int TetrisLayer::getDirectionMaxCollision()
{
    // 블록배열 (4, 4)에서 가운데 지점을 기준으로 4 구역으로 나눠 각 구역의 충돌 개수 계산
    // 좌표계로 생각하면 영점 기준으로 1, 2 사분면을 위 구역
    // 3, 4사분면은 아래 / 1, 4 사분면은 왼쪽 / 2, 3 사분면은 오른쪽 구역으로 계산해 이동 방향 리턴(가장 충돌을 피할 수 있는 방향)
    // -> 회전하면서 아래로 이동하는 것은 어색하여 아래 구역이 많을 때 위를 반환하는 것으로 변경
    if(isCurBlock == false)
        return Constant::collision_error;
    
    arrPt block = curBlock.getBlock();
    int collisionCountWithArea[4] = {0, 0, 0, 0}; // 1, 2, 3, 4 사분면
    
    for(int xidx = 0; xidx < 4; ++xidx)
    {
        for(int yidx = 0; yidx < 4; ++yidx)
        {
            if(block[curBlock.getRotationCount()][xidx][yidx] <= 0) // 빈 공간이면 다음 칸 체크
                continue;
            if(board[xidx + curBlock.getPositionX()][yidx + curBlock.getPositionY()] >= 1) // 다른 블록과 충돌
            {
                if(xidx <= 1)
                {
                    ++collisionCountWithArea[0];
                    ++collisionCountWithArea[1];
                }
                else
                {
                    ++collisionCountWithArea[2];
                    ++collisionCountWithArea[3];
                }
                
                if(yidx <= 1)
                {
                    ++collisionCountWithArea[1];
                    ++collisionCountWithArea[2];
                }
                else
                {
                    ++collisionCountWithArea[0];
                    ++collisionCountWithArea[3];
                }
            }
        }
    }
    
    int areaUpCount = collisionCountWithArea[0] + collisionCountWithArea[1];
    int areaLeftCount = collisionCountWithArea[1] + collisionCountWithArea[2];
    int areaRightCount = collisionCountWithArea[0] + collisionCountWithArea[3];
    int areaDownCount = collisionCountWithArea[2] + collisionCountWithArea[3];
    
    if(areaUpCount > areaLeftCount && areaUpCount > areaRightCount)
        return Constant::up;
    if(areaDownCount > areaLeftCount && areaDownCount > areaRightCount)
        return Constant::up;
    if(areaLeftCount > areaRightCount)
        return Constant::right;
    return Constant::left;
}

int TetrisLayer::checkCollision()
{
    if(isCurBlock == false)
        return Constant::collision_error;
    
    int x = curBlock.getPositionX();
    int y = curBlock.getPositionY();
    int rotation = curBlock.getRotationCount();
    arrPt block = curBlock.getBlock();
    
    for(int xidx = 0; xidx < 4; ++xidx)
    {
        for(int yidx = 0; yidx < 4; ++yidx)
        {
            if(block[rotation][xidx][yidx] <= 0) // 빈 공간이면 다음 칸 체크
                continue;
            if(xidx + x > Constant::mapHeight - 1) // 바닥과 충돌
                return Constant::collision_ground;
            if(xidx + x < 0) // 천장과 충돌
                return Constant::collision_ceiling;
            if(yidx + y > Constant::mapWidth - 1) // 벽과 충돌
                return Constant::collision_rwall;
            if(yidx + y < 0)
                return Constant::collision_lwall;
            if(board[xidx + x][yidx + y] >= 1) // 다른 블록과 겹친 상태
                return Constant::collision_block;
        }
    }
    return Constant::collision_no;
}

void TetrisLayer::clearBlock(int x, int y)
{
    int rotation = curBlock.getRotationCount();
    arrPt block = curBlock.getBlock();
    
    for(int xidx = x; xidx < x + 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[rotation][xidx - x][yidx - y] >= 1)
            {
                board[xidx][yidx] = 0;
                changeSprite(xidx, yidx);
            }
        }
    }
}

void TetrisLayer::drawBlock()
{
    if(isCurBlock == false)
        return;
    
    int x = curBlock.getPositionX();
    int y = curBlock.getPositionY();
    int rotation = curBlock.getRotationCount();
    arrPt block = curBlock.getBlock();
    
    for(int xidx = x; xidx < x + 4; ++xidx)
    {
        for(int yidx = y; yidx < y + 4; ++yidx)
        {
            if(block[rotation][xidx - x][yidx - y] >= 1)
            {
                board[xidx][yidx] = block[rotation][xidx - x][yidx - y];
                changeSprite(xidx, yidx);
            }
        }
    }
}

void TetrisLayer::drawBoard()
{
    for(int x = 0; x < Constant::mapHeight; ++x)
    {
        for(int y = 0; y < Constant::mapWidth; ++y)
        {
            createSprite(x, y);
        }
    }
}

void TetrisLayer::createSprite(int x, int y)
{
    if(x < 0 || x > Constant::mapHeight - 1)
        return;
    if(y < 0 || y > Constant::mapWidth - 1)
        return;
    
    Sprite* sprite = Sprite::create("square_white.png");
    
    int originWidth = sprite->getContentSize().width;
    int originHeight = sprite->getContentSize().height;
    sprite->setScale((double)Constant::blockSize / (double)originWidth, (double)Constant::blockSize / (double)originHeight);
    sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setPosition(Vec2(y * Constant::blockSize + Constant::blockGap,
                             (Constant::mapHeight - x) * Constant::blockSize + Constant::blockGap));
    this->addChild(sprite);
    
    boardSprite[x][y] = sprite;
}

void TetrisLayer::changeSprite(int x, int y)
{
    Sprite* sprite = boardSprite[x][y];
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
        default:
            sprite->setTexture("square_white.png");
            break;
    }
}

bool TetrisLayer::isGround()
{
    if(isCurBlock == false)
        return false; // FIXME 이곳에 걸리는 일이 없어야 정상(update 함수에서 확인함), 만약 걸린다면 에러 표시
    
    int x = curBlock.getPositionX();
    int y = curBlock.getPositionY();
    int rotation = curBlock.getRotationCount();
    arrPt block = curBlock.getBlock();
    std::set<int> checkColumns;
    
    for(int xidx = 3; xidx >= 0; --xidx) // 아래서부터 탐색
    {
        for(int yidx = 0; yidx < 4; ++yidx)
        {
            if(checkColumns.find(yidx) != checkColumns.end()) // 이미 검사한 열이라면 다음 칸 체크
                continue;
            if(block[rotation][xidx][yidx] <= 0) // 빈 공간이면 다음 칸 체크
                continue;
            
            int nextX = xidx + x + 1;
            if((nextX > Constant::mapHeight - 1) || (board[nextX][yidx + y] >= 1))
            {
                return true;
            }
            checkColumns.insert(yidx); // 해당 열은 검사했으니 같은 열의 윗칸들은 검사하지 않도록 함
        }
    }
    
    return false;
}

void TetrisLayer::checkFullRow()
{
    if(isCurBlock == false)
        return;
    
    int x = curBlock.getPositionX(); // 검사할 줄: x <= idx < x + 4
    isCurBlock = false; // 비활성화, 키 입력 등으로 움직이지 않도록
    int rowStatus[Constant::mapHeight];
    for(int idx = 0; idx < Constant::mapHeight; ++idx)
    {
        rowStatus[idx] = 0;
    }
    
    for(int xidx = x + 3; xidx >= x; --xidx)
    {
        if(xidx >= Constant::mapHeight)
            continue;
        if(xidx < 0)
            break;
        
        for(int yidx = 0; yidx < Constant::mapWidth; ++yidx)
        {
            if(board[xidx][yidx] < 1)
                break;
            if(yidx == Constant::mapWidth - 1) // break에 걸리지 않고 끝까지 체크 되었으면 full
            {
                rowStatus[xidx] = 1;
            }
        }
    }
    
    int downAmount = 0;
    // x + 3부터 한 줄 씩 완성되었는지 탐색 후 옮긴다.
    for(int xidx = x + 3; x >= 0; --xidx)
    {
        if(xidx >= Constant::mapHeight)
            continue;
        if(xidx < 0)
            break;
        
        if(rowStatus[xidx] > 0)
        {
            ++downAmount;
        }
        else // 내리기
        {
            if(downAmount == 0)
                continue;
            
            bool isBlankRow = true;
            int blankRowCount = 0;
            for(int yidx = 0; yidx < Constant::mapWidth; ++yidx)
            {
                board[xidx + downAmount][yidx] = board[xidx][yidx];
                changeSprite(xidx + downAmount, yidx);
                if(board[xidx][yidx] >= 1)
                {
                    isBlankRow = false;
                }
            }
            if(isBlankRow == true)
            {
                ++blankRowCount;
                if(blankRowCount >= downAmount)
                    break;
            }
        }
    }
}

void TetrisLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(isCurBlock == false)
        return;
    
    if(keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
        TetrisLayer::rotateBlock();
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
    {
        bool moveLeft = TetrisLayer::moveBlock(Constant::left);
        drawBlock();
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        bool moveRight = TetrisLayer::moveBlock(Constant::right);
        drawBlock();
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        // move down or not doing
    }
}
