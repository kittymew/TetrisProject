#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LevelLayer : public cocos2d::Layer
{
public:
    void levelClickCallback(Ref* sender, cocos2d::ui::ListView::EventType event);
    void menuCallback();
    
public:
    static Layer* create();
    virtual bool init();
};
