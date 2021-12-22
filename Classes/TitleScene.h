#pragma once

#include "cocos2d.h"

// Scene : 하나의 우주, 빈 영역
// Layer : 우주에 속한 하나의 차원, 실제 상호작용이 가능한 영역 (Scene에 포함되는 영역)

class TitleScene : public cocos2d::Scene
{
public:
    void goLobbyScene();
    
public:
    static TitleScene* create(); // CREATE_FUNC(HelloWorld); 매크로
    virtual bool init(); // 초기화할 때 자동으로 호출되는 함수
};
