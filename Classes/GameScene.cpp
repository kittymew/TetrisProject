#include "GameScene.h"
#include "TetrisLayer.h"

using namespace cocos2d;

std::shared_ptr<GameScene::Info> GameScene::Info::create(int lv)
{
    InfoPtr pInfo(new Info);
    pInfo->lv = lv;
    
    return pInfo;
}

GameScene* GameScene::create(const InfoPtr& pInfo)
{
    GameScene *pRet = new(std::nothrow) GameScene();
    if (pRet && pRet->init(pInfo))
    {
        pRet->autorelease(); // 메모리 자동 관리
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameScene::init(const InfoPtr& pInfo)
{
    if(!Scene::init())
        return false;
    
    auto layer = TetrisLayer::create();
    this->addChild(layer);
    
    return true;
}
