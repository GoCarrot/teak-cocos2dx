/* Teak -- Copyright (C) 2016-2019 Teak.io, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

// From Teak.cpp
extern void Teak_Cocos2dx_SendMessage(const char* eventStr, const char* jsonStr);

// From TeakHooks.m
extern void Teak_Plant(Class appDelegateClass, NSString* appId, NSString* appSecret);

// From Teak.m
extern NSString* const TeakNotificationAppLaunch;
extern NSString* const TeakOnReward;
extern NSString* const TeakForegroundNotification;
extern NSString* const TeakAdditionalData;

extern NSDictionary* TeakWrapperSDK;
extern NSDictionary* TeakXcodeVersion;

typedef void (^TeakLogListener)(NSString* _Nonnull event,
                                NSString* _Nonnull level,
                                NSDictionary* _Nullable eventData);

extern void TeakSetLogListener(TeakLogListener listener);

void Teak_Cocos2dx_NSSendMessage(NSDictionary* userInfo, const char* eventName, bool sendEmptyOnError) {
   NSError* error = nil;

   NSData* jsonData = [NSJSONSerialization dataWithJSONObject:userInfo
                                                      options:0
                                                        error:&error];

   if (error != nil) {
      NSLog(@"[Teak:Cocos2dx] Error converting to JSON: %@", error);
      if (sendEmptyOnError) {
        Teak_Cocos2dx_SendMessage(eventName, "{}");
      }
   } else {
      NSString* jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
      Teak_Cocos2dx_SendMessage(eventName, [jsonString UTF8String]);
   }
}

__attribute__((constructor))
static void teak_init()
{
   TeakWrapperSDK = @{@"cocos" : @"alpha"};
   TeakXcodeVersion = @{@"product" : [NSNumber numberWithInt:__apple_build_version__]};

   NSString* appId = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"TeakAppId"];
   NSString* apiKey = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"TeakApiKey"];
   Teak_Plant(NSClassFromString(@"AppController"), appId, apiKey);

   TeakSetLogListener(^(NSString* _Nonnull event,
                        NSString* _Nonnull level,
                        NSDictionary* _Nullable eventData) {
      NSError* error = nil;
      NSData* jsonData = [NSJSONSerialization dataWithJSONObject:eventData
                                                         options:0
                                                           error:&error];
      if (error != nil) {
         NSLog(@"[Teak:Cocos2dx] Error converting to JSON: %@", error);
      } else {
         NSString* jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
         Teak_Cocos2dx_SendMessage("TeakLogEvent", [jsonString UTF8String]);
      }
   });

   [[NSNotificationCenter defaultCenter] addObserverForName:TeakNotificationAppLaunch
                                                     object:nil
                                                      queue:nil
                                                 usingBlock:^(NSNotification* notification) {
                                                    Teak_Cocos2dx_NSSendMessage(notification.userInfo, "TeakNotificationLaunch", true);
                                                 }];

   [[NSNotificationCenter defaultCenter] addObserverForName:TeakOnReward
                                                     object:nil
                                                      queue:nil
                                                 usingBlock:^(NSNotification* notification) {
                                                    Teak_Cocos2dx_NSSendMessage(notification.userInfo, "TeakRewardClaimAttempt", false);
                                                 }];

   [[NSNotificationCenter defaultCenter] addObserverForName:TeakForegroundNotification
                                                     object:nil
                                                      queue:nil
                                                 usingBlock:^(NSNotification* notification) {
                                                    Teak_Cocos2dx_NSSendMessage(notification.userInfo, "TeakForegroundNotification", true);
                                                 }];

   [[NSNotificationCenter defaultCenter] addObserverForName:TeakAdditionalData
                                                     object:nil
                                                      queue:nil
                                                 usingBlock:^(NSNotification* notification) {
                                                    Teak_Cocos2dx_NSSendMessage(notification.userInfo, "TeakAdditionalData", false);
                                                 }];
}
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
