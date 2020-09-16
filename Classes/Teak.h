#ifndef __TEAK_H__
#define __TEAK_H__

#include <string>
#include "base/CCEventDispatcher.h"

class Teak {
public:
    static Teak* getInstance();

    void identifyUser(const std::string& userId);
    cocos2d::EventDispatcher* getEventDispatcher() const { return _eventDispatcher; };

private:
    void init();
    bool _initialized;
    cocos2d::EventDispatcher* _eventDispatcher;
};

#endif // __TEAK_H__
