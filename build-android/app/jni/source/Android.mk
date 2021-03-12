########################
#
# Main Application Entry
#
########################
LOCAL_PATH := $(call my-dir)
PROJ_PATH  := $(LOCAL_PATH)/../../../..
CUGL_PATH  := $(PROJ_PATH)/cugl
include $(CLEAR_VARS)

LOCAL_MODULE := main
LOCAL_C_INCLUDES := $(CUGL_PATH)/include
LOCAL_C_INCLUDES += $(PROJ_PATH)/source

define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

ALL_FILES_IN_SOURCE = $(call walk, $(PROJ_PATH)/source)
CPP_SOURCE_FILES := $(filter %.cpp, $(ALL_FILES_IN_SOURCE))

LOCAL_SRC_FILES := $(CPP_SOURCE_FILES:$(LOCAL_PATH)/%=%)

# Line the libraries
LOCAL_SHARED_LIBRARIES := hidapi
LOCAL_SHARED_LIBRARIES += SDL2
LOCAL_SHARED_LIBRARIES += SDL2_image
LOCAL_SHARED_LIBRARIES += SDL2_ttf
LOCAL_SHARED_LIBRARIES += SDL2_codec
LOCAL_STATIC_LIBRARIES := CUGL

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lGLESv3 -lOpenSLES -llog

include $(BUILD_SHARED_LIBRARY)
