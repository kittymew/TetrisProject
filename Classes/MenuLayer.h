#pragma once

#include "cocos2d.h"

class MenuLayer : public cocos2d::Layer
{
public:
    static cocos2d::Layer* createLayer();
    
    virtual bool init();
    
    void gameMenuCallback(Ref* sender);
    void restMenuCallback(Ref* sender);
    void shopMenuCallback(Ref* sender);
    void optionMenuCallback(Ref* sender);
    void quitMenuCallback(Ref* sender);
    
    CREATE_FUNC(MenuLayer);
};
