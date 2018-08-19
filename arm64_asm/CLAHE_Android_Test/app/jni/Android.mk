LOCAL_PATH := $(call my-dir)

############################################################################################################
# Include 3rd party libraries
############################################################################################################


############################################################################################################
# Build Zayhu Core Library
############################################################################################################

include $(CLEAR_VARS)

LOCAL_MODULE := clahe
TOOLS_DIR := $(LOCAL_PATH)/tools


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/ \
    $(LOCAL_PATH)/tools/ 

LOCAL_SRC_FILES := \
	ndkutils.c \
	com_zayhu_video_codec_ads.cpp \
	test_clahe.cpp \
	realtime_clahe.cpp \
	Histogram.cpp \
	log1.cpp \
	$(TOOLS_DIR)/tictoc.cpp \
	QImage.cpp

LOCAL_CFLAGS := -DANDROID_NDK -DANDROID

LOCAL_LDLIBS := -llog -latomic -ljnigraphics -landroid
LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true

include $(BUILD_SHARED_LIBRARY)

############################################################################################################
# Done
############################################################################################################
