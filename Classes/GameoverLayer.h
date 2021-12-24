#pragma once

#include "cocos2d.h"

class GameoverLayer : public cocos2d::Layer
{
public:
    void menuCallback();
    
public:
    static Layer* create();
    virtual bool init();
};
