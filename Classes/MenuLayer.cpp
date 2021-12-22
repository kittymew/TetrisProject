#include "MenuLayer.h"
#include "LevelLayer.h"
#include "ShopLayer.h"

#include "LobbyScene.h"

USING_NS_CC;

Layer* MenuLayer::createLayer()
{
    auto layer = MenuLayer::create();
    
    return layer;
}

bool MenuLayer::init()
{
    if(!Layer::init())
        return false;
    
    auto gameLabel = Label::createWithSystemFont("게임시작", "Arial", 50);
    auto restLabel = Label::createWithSystemFont("이어하기", "Arial", 50);
    auto shopLabel = Label::createWithSystemFont("상점", "Arial", 50);
    auto optnLabel = Label::createWithSystemFont("옵션", "Arial", 50);
    auto quitLabel = Label::createWithSystemFont("종료", "Arial", 50);
    
    auto gameMenu = MenuItemLabel::create(gameLabel, CC_CALLBACK_1(MenuLayer::gameMenuCallback, this)); // this를 넘겨주는 이유가 뭐지? __target__ 인자
    auto restMenu = MenuItemLabel::create(restLabel, CC_CALLBACK_1(MenuLayer::restMenuCallback, this));
    auto shopMenu = MenuItemLabel::create(shopLabel, CC_CALLBACK_1(MenuLayer::shopMenuCallback, this));
    auto optnMenu = MenuItemLabel::create(optnLabel, CC_CALLBACK_1(MenuLayer::optionMenuCallback, this));
    auto quitMenu = MenuItemLabel::create(quitLabel, CC_CALLBACK_1(MenuLayer::quitMenuCallback, this));
    
    auto menu = Menu::create(gameMenu, restMenu, shopMenu, optnMenu, quitMenu, NULL);
    menu->alignItemsVertically();
    this->addChild(menu);
    
    return true;
}

void MenuLayer::gameMenuCallback(Ref* sender)
{
    auto scene = (LobbyScene*)Director::getInstance()->getRunningScene();
    if(nullptr == scene)
        return;
    
    Layer* pLayer = LevelLayer::create();;
    
    scene->changeLayer(pLayer);
}

void MenuLayer::restMenuCallback(Ref* sender)
{
    return;
}

void MenuLayer::shopMenuCallback(Ref* sender)
{
    auto scene = (LobbyScene*)Director::getInstance()->getRunningScene();
    if(nullptr == scene)
        return;
    
    Layer* pLayer = Shop::createLayer();;
    
    scene->changeLayer(pLayer);
}

void MenuLayer::optionMenuCallback(Ref* sender)
{
    return;
}

void MenuLayer::quitMenuCallback(Ref* sender)
{
    Director::getInstance()->end();
}
