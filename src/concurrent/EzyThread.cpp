//
//  EzyThread.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/8/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifdef PF_APPLE
#import <Foundation/NSString.h>
#import <Foundation/NSThread.h>
#endif

#include <vector>
#include <mutex>

#include "EzyThread.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

static std::vector<std::function<void()>> s_mainHandler;
static std::mutex s_mainHandler_mutex;

void EzyThread::setCurrentThreadName(std::string name) {
#ifdef PF_APPLE
    [[NSThread currentThread] setName: [NSString stringWithUTF8String: name.c_str()]];
#endif
}

void EzyThread::runOnMainThread(const std::function<void()>& handler){
    std::unique_lock<std::mutex> lk(s_mainHandler_mutex);
    s_mainHandler.push_back(handler);
}

void EzyThread::executeMainHandler(){
    std::unique_lock<std::mutex> lk(s_mainHandler_mutex);
    for(int i=0;i<s_mainHandler.size();i++){
        s_mainHandler[i]();
    }
    s_mainHandler.clear();
}

EZY_NAMESPACE_END_WITH
