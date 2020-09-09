#include "Teak.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"
#endif

static Teak* _TeakInstance = new Teak();

Teak* Teak::getInstance() {
    return _TeakInstance;
}

void Teak::identifyUser(const char* userId) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
#endif
}
