#pragma once

#include "cocos2d.h"

class LobbyScene : public cocos2d::Scene
{
public:
    static LobbyScene* create();
    virtual bool init(); // 초기화할 때 자동으로 호출되는 함수
    
public:
    void changeLayer(cocos2d::Layer* pLayer);
    
};
