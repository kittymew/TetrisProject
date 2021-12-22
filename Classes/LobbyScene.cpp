#include "LobbyScene.h"

#include "MenuLayer.h"

USING_NS_CC;

LobbyScene* LobbyScene::create()
{
    LobbyScene *pRet = new(std::nothrow) LobbyScene();
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

bool LobbyScene::init()
{
    if(!Scene::init()) // 부모 클래스의 layer class의 init()이 먼저 실행
        return false;
    
    auto menulayer = MenuLayer::create();
    this->addChild(menulayer, 0);
  
    return true;
}

void LobbyScene::changeLayer(cocos2d::Layer* pLayer)
{
    this->removeAllChildren();
    this->addChild(pLayer);
}
