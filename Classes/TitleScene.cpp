#include "TitleScene.h"
#include "LobbyScene.h"

USING_NS_CC;

TitleScene* TitleScene::create()
{
    TitleScene *pRet = new(std::nothrow) TitleScene();
    if (pRet && pRet->init())
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

bool TitleScene::init()
{
    if(!Scene::init()) // 부모 클래스의 layer class의 init()이 먼저 실행
        return false;
    
    auto titleLabel = Label::createWithSystemFont("Tetris", "Arial", 96);
    auto winSize = Director::getInstance()->getWinSize();
    titleLabel->setPosition(Vec2(winSize.width/2, winSize.height * 2/3));
    this->addChild(titleLabel); // Hello World 레이어의 자식으로 label 추가
    
    auto nextItem = MenuItemFont::create("Press and Start", CC_CALLBACK_0(TitleScene::goLobbyScene, this));
    Vector<MenuItem*> menuItems;
    menuItems.pushBack(nextItem);
    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2(winSize.width/2, winSize.height * 1/3));
    this->addChild(menu);
  
    return true;
}

void TitleScene::goLobbyScene()
{
    Director::getInstance()->replaceScene(LobbyScene::create());
}
