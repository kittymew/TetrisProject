#pragma once

#include "cocos2d.h"

class Shop : public cocos2d::Layer
{
public:
    static cocos2d::Layer* createLayer();

    virtual bool init();
    
    void menuCallback();
    
    CREATE_FUNC(Shop);
};
