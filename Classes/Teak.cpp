#include "Teak.h"

#include <mutex>
#include "editor-support/spine/Json.h"

#include "base/CCScheduler.h"
#include "base/CCDirector.h"
#include "base/CCEventCustom.h"
#include "base/CCEventListenerCustom.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <android/log.h>
#   define LOG_TAG     "Teak.cpp"
#   define LOG_I(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG,__VA_ARGS__)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#   include <os/log.h>
#   define LOG_I(...)  os_log_info(OS_LOG_DEFAULT, __VA_ARGS__)
extern "C" {
extern void TeakIdentifyUser(const char* userId, const char* optOutJsonArray, const char* email);
} // extern "C"
#endif

// Calling something on main thread:

    // auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    // scheduler->schedule([](float dt){
    //     LOG_I("CALLED THE CALLBACK THING");
    // }, this, 1.0f, 0 /* do it once */, 0.0f, false, "myCallbackKey");

std::mutex g_message_queue_mutex;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"
#endif

static Teak* _TeakInstance = new Teak();

cocos2d::EventListener* _listener;

Teak* Teak::getInstance() {
    if (!_TeakInstance->_initialized) {
        _TeakInstance->init();
    }
    return _TeakInstance;
}

void Teak::init() {
    if (this->_initialized) return;
    this->_initialized = true;

    this->_eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();

    // Hax
    _listener = cocos2d::EventListenerCustom::create("TeakLogEvent", [=](cocos2d::EventCustom* event){
       char* buf = static_cast<char*>(event->getUserData());
       LOG_I("CALLED SEND MESSAGE: %s", buf);
    });

    this->_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
}

void Teak::identifyUser(const std::string& userId) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniHelper::callStaticVoidMethod("io.teak.sdk.Teak", "identifyUser", userId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    TeakIdentifyUser(userId.c_str(), NULL, NULL);
#else
#endif
}

extern "C" {

void Teak_Cocos2dx_SendMessage(const char* eventStr, const char* jsonStr) {
    std::lock_guard<std::mutex> guard(g_message_queue_mutex);

    cocos2d::EventCustom eventToDispatch(eventStr);
    if (jsonStr != NULL) {
        eventToDispatch.setUserData((void*)jsonStr);
    }
    Teak::getInstance()->getEventDispatcher()->dispatchEvent(&eventToDispatch);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JNIEXPORT void JNICALL Java_io_teak_sdk_wrapper_cocos2dx_TeakCocos2dx_nativeSendMessage(JNIEnv* env, jobject thiz, jstring event, jstring json) {
        std::string eventStr = cocos2d::JniHelper::jstring2string(event);
        std::string jsonStr = cocos2d::JniHelper::jstring2string(json);

        if (eventStr.empty()) return;

        Teak_Cocos2dx_SendMessage(eventStr.c_str(), jsonStr.c_str());
    }

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
} // extern "C"
