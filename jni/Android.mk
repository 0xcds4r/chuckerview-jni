LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := chuckerpatch
LOCAL_LDLIBS := -lm -llog -lc -lz -ljnigraphics -landroid -lEGL -lGLESv2 -lOpenSLES

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_CPPFLAGS := -w -s -fvisibility=default -pthread -Wall -fpack-struct=1 -O2 -std=c++14 -fexceptions -ferror-limit=5
include $(BUILD_SHARED_LIBRARY)