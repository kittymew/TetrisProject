#include <string>

#include "LevelLayer.h"
#include "MenuLayer.h"
#include "GameScene.h"

using namespace cocos2d;

Layer* LevelLayer::create()
{
    Layer *pRet = new(std::nothrow) LevelLayer();
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

bool LevelLayer::init()
{
    if(!Layer::init())
        return false;
    
    auto winsize = Director::getInstance()->getWinSize();
    
    auto levelListView = cocos2d::ui::ListView::create();
    levelListView->setPosition(Vec2(winsize.width * 0.5, winsize.height * 0.5));
    levelListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    levelListView->setAnchorPoint(Vec2(0.5, 0.5));
    levelListView->setContentSize(Size(winsize.width * 0.6, winsize.height * 0.9));
    levelListView->addEventListener(CC_CALLBACK_2(LevelLayer::levelClickCallback, this));
    
    for(int levelIdx = 1; levelIdx <= 20; ++levelIdx)
    {
        auto label = Label::createWithSystemFont("Level " + std::to_string(levelIdx), "Arial", 36);
        label->setAnchorPoint(Vec2(0, 0));
        
        ui::Layout* list_layout = ui::Layout::create();
        list_layout->addChild(label);
        list_layout->setContentSize(Size(winsize.width * 0.6, 60));
        list_layout->setAnchorPoint(Vec2(0.5, 0.5));
        list_layout->setTouchEnabled(true);
        levelListView->pushBackCustomItem(list_layout);
    }
    
    this->addChild(levelListView);
    
    // back button
    auto backItem = MenuItemFont::create("Back", CC_CALLBACK_0(LevelLayer::menuCallback, this));
    backItem->setAnchorPoint(Vec2(0, 1));
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2(0, winsize.height));
    this->addChild(menu);
    
    return true;
}

void LevelLayer::levelClickCallback(Ref* sender, ui::ListView::EventType event)
{
    ui::ListView *listview = static_cast<ui::ListView*>(sender);
    
    if(event == ui::ListView::EventType::ON_SELECTED_ITEM_START)
    {
        
    }
    else if (event == ui::ListView::EventType::ON_SELECTED_ITEM_END)
    {
        ssize_t level = listview->getCurSelectedIndex();
        // FIXME
        
        auto levelInfo = GameScene::Info::create((int) level);
        auto scene = GameScene::create(levelInfo);
        Director::getInstance()->replaceScene(scene);
    }
}

void LevelLayer::menuCallback()
{
    auto layer = MenuLayer::createLayer();
    auto scene = Director::getInstance()->getRunningScene();
    
    if(scene != nullptr)
    {
        scene->removeAllChildren();
        scene->addChild(layer);
    }
}
