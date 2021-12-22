#pragma once

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
    struct Info
    {
        int lv;
        
    public:
        static std::shared_ptr<Info> create(int lv);
        
    private:
        Info() = default;
        
    }; using InfoPtr = std::shared_ptr<Info>;
    
public:
    static GameScene* create(const InfoPtr& pInfo);
    virtual bool init(const InfoPtr& pInfo);
};
