#include "GameoverLayer.h"
#include "LobbyScene.h"

using namespace cocos2d;

Layer* GameoverLayer::create()
{
    Layer *pRet = new(std::nothrow) GameoverLayer();
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

bool GameoverLayer::init()
{
    if(!Layer::init())
        return false;
    
    Label* label = Label::createWithSystemFont("GAME OVER", "Arial", 56);
    Size winsize = Director::getInstance()->getWinSize();
    label->setPosition(Vec2(winsize.width / 2, winsize.height * 2 / 3));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label);
    
    Label* backLabel = Label::createWithSystemFont("retry", "Arial", 36);
    MenuItemLabel* backMenuItem = MenuItemLabel::create(backLabel, CC_CALLBACK_0(GameoverLayer::menuCallback, this));
    Menu* menu = Menu::create(backMenuItem, NULL);
    this->addChild(menu);
    
    return true;
}

void GameoverLayer::menuCallback()
{
    Director::getInstance()->replaceScene(LobbyScene::create());
}
