Android
-------
* Add Teak.cpp to Android.mk
* And/Or... Add Teak.cpp and Teak.h to CMakeLists (if you use Cmake)

* Note: Look for `Teak.Integration` log errors
* Add teak.aar to `proj.android/app/build.gradle` (http://sdks.teakcdn.com/android/teak.aar)
* Add `teak.xml` with configuration variable to `proj.android/app/res/values/`
* Add `<meta-data android:name="io.teak.sdk.initialize" android:value="true"/>` to your main `<activity>`
* Add Teak dependencies to `proj.android/app/build.gradle`

    // This should be included already, but if it's not this is the dependency
    // implementation 'androidx.localbroadcastmanager:localbroadcastmanager:1.0.0'

    implementation 'com.android.installreferrer:installreferrer:1.0'

    implementation 'com.google.android.gms:play-services-ads:19+'
    implementation 'com.google.android.gms:play-services-base:17+'
    implementation 'com.google.android.gms:play-services-basement:17+'

    implementation 'com.google.firebase:firebase-messaging:19.0.1'

* Add your push credentials to the Teak Dashboard

iOS
---
* Add Teak.cpp and Teak.h to the Xcode Project
* And/Or... Add Teak.cpp and Teak.h to CMakeLists (if you use Cmake)

* Add Teak.framework to the `Link Binary With Libraries` in `Build Phases` (http://sdks.teakcdn.com/ios/Teak.framework.zip)
* Add TeakResources.bundle to the `Copy Bundle Resources` in `Build Phases` (http://sdks.teakcdn.com/ios/TeakResources.bundle.zip)
* Add Teak dependencies

    UserNotifications.framework
    StoreKit.framework
    AdSupport.framework

* `TeakAppId` and `TeakApiKey` to the `Info.plist`
* If your AppController is named something besides the default `AppController` change this line to reflect that
    Teak_Plant(NSClassFromString(@"AppController"), appId, apiKey);
* Add `remote-notification` to the list of your supported `Background Modes` (Signing & Capabilities -> Background Modes)
* Make sure Push Notifications capability is added to Signing & Capibilities
* Request push permissions
* Add your push credentials to the Teak Dashboard
