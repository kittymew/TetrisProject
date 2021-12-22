/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "TitleScene.h"

#pragma once

USING_NS_CC; // using namespace cocos2d; 매크로

AppDelegate::AppDelegate()
{
    
}

AppDelegate::~AppDelegate()
{
    
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance(); // Director는 싱글톤
    auto glView = director->getOpenGLView(); // 현재 활성화된 창을 가져옴
    
    if(!glView) // 현재 창이 없다면
    {
        glView = GLViewImpl::create("Hello World"); // glView를 하나 생성
//        glView->setFrameSize(400, 1000); // 사이즈 지정
        glView->setDesignResolutionSize(600, 1000, ResolutionPolicy::SHOW_ALL);
        director->setOpenGLView(glView); // 현재 활성화된 뷰로 지정
    }
    
    // 창을 생성, 창으로 볼 게임 세상이 아직 없음
    // 게임 세상을 만드는 건 HelloWorld에서
    auto scene = TitleScene::create();
    director->runWithScene(scene);
    
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    
}

void AppDelegate::applicationWillEnterForeground()
{
    
}
