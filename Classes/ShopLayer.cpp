#include "ShopLayer.h"
#include "MenuLayer.h"

USING_NS_CC;

Layer* Shop::createLayer()
{
    auto layer = Shop::create();
    
    return layer;
}

bool Shop::init()
{
    if(!Layer::init())
        return false;
    
    auto label = Label::createWithSystemFont("Shop", "Arial", 80);
    label->setAnchorPoint(Vec2(0, 0));
    this->addChild(label);
    
    auto backItem = MenuItemFont::create("Back", CC_CALLBACK_0(Shop::menuCallback, this));
    auto menu = Menu::create(backItem, NULL);
    
    this->addChild(menu);
    
    return true;
}

void Shop::menuCallback()
{
    auto layer = MenuLayer::createLayer();
    auto scene = Director::getInstance()->getRunningScene();
    
    if(scene != nullptr)
    {
        scene->removeAllChildren();
        scene->addChild(layer);
    }
}


