LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_PHOTON_ROOT := $(LOCAL_PATH)/../../../../Photon-AndroidNDK_v3-0-4-0_SDK
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameStartScene.cpp \
				   ../../Classes/Brick.cpp \
				   ../../Classes/BrickGameScene.cpp \
				   ../../Classes/TimeBar.cpp \
				   ../../Classes/GameOverScene.cpp \
				   ../../Classes/NetworkLogic.cpp \
				   ../../Classes/MultiPlayerBrickGameScene.cpp \
				   ../../Classes/Player.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_STATIC_LIBRARIES := photon-cpp-static-prebuilt
LOCAL_STATIC_LIBRARIES += common-cpp-static-prebuilt 
LOCAL_STATIC_LIBRARIES += photon-c-static-prebuilt 
LOCAL_STATIC_LIBRARIES += loadbalancing-cpp-static-prebuilt

LOCAL_CFLAGS := -DCOCOS2D_DEBUG=1

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)

$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Common-c/android/src)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Common-c)
$(call import-module,common-c-prebuilt)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Common-cpp/android/src)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Common-cpp)
$(call import-module,common-cpp-prebuilt)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Photon-c/android/src)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Photon-c)
$(call import-module,photon-c-prebuilt)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Photon-cpp/android/src)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Photon-cpp)
$(call import-module,photon-cpp-prebuilt)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/LoadBalancing-cpp/android/src)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/LoadBalancing-cpp)
$(call import-module,loadbalancing-cpp-prebuilt)