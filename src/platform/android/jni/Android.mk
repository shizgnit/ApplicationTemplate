# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)
#
# libpng
#
include $(CLEAR_VARS)

TOP_PATH := $(LOCAL_PATH)/../../../
COMMON_PATH := $(TOP_PATH)/common
THIRD_PARTY_PATH := $(TOP_PATH)/third_party

LOCAL_CFLAGS :=
LOCAL_C_INCLUDES += $(THIRD_PARTY_PATH)/png

LOCAL_MODULE := libpng
LOCAL_SRC_FILES :=\
	$(THIRD_PARTY_PATH)/png/png.c \
	$(THIRD_PARTY_PATH)/png/pngerror.c \
	$(THIRD_PARTY_PATH)/png/pngget.c \
	$(THIRD_PARTY_PATH)/png/pngmem.c \
	$(THIRD_PARTY_PATH)/png/pngpread.c \
	$(THIRD_PARTY_PATH)/png/pngread.c \
	$(THIRD_PARTY_PATH)/png/pngrio.c \
	$(THIRD_PARTY_PATH)/png/pngrtran.c \
	$(THIRD_PARTY_PATH)/png/pngrutil.c \
	$(THIRD_PARTY_PATH)/png/pngset.c \
	$(THIRD_PARTY_PATH)/png/pngtrans.c \
	$(THIRD_PARTY_PATH)/png/pngwio.c \
	$(THIRD_PARTY_PATH)/png/pngwrite.c \
	$(THIRD_PARTY_PATH)/png/pngwtran.c \
	$(THIRD_PARTY_PATH)/png/pngwutil.c

include $(BUILD_STATIC_LIBRARY)

#
# mylo
#
include $(CLEAR_VARS)

LOCAL_CFLAGS := -fpermissive -D_DEBUG
LOCAL_C_INCLUDES += $(COMMON_PATH)/mylo
LOCAL_C_INCLUDES += $(THIRD_PARTY_PATH)/png
LOCAL_C_INCLUDES += $(THIRD_PARTY_PATH)/linmath
LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_MODULE    := libmylo
LOCAL_SRC_FILES := \
  $(COMMON_PATH)/mylo/buffer.cpp \
  $(COMMON_PATH)/mylo/compression.cpp \
  $(COMMON_PATH)/mylo/configuration.cpp \
  $(COMMON_PATH)/mylo/filesystem.cpp \
  $(COMMON_PATH)/mylo/asset.cpp \
  $(COMMON_PATH)/mylo/identity.cpp \
  $(COMMON_PATH)/mylo/regex.cpp \
  $(COMMON_PATH)/mylo/stream.cpp \
  $(COMMON_PATH)/mylo/trace.cpp \
  $(COMMON_PATH)/mylo/timer.cpp \
  $(COMMON_PATH)/mylo/utilities.cpp \
  $(COMMON_PATH)/mylo/xml.cpp \
  $(COMMON_PATH)/mylo/xpath.cpp

include $(BUILD_STATIC_LIBRARY)

#
# prototype
#
include $(CLEAR_VARS)

LOCAL_CFLAGS := -fpermissive
LOCAL_C_INCLUDES += $(TOP_PATH)
LOCAL_C_INCLUDES += $(COMMON_PATH)/mylo
LOCAL_C_INCLUDES += $(THIRD_PARTY_PATH)/png
LOCAL_C_INCLUDES += $(THIRD_PARTY_PATH)/linmath
LOCAL_STATIC_LIBRARIES := libmylo libpng libz

#LOCAL_LDLIBS := -lz

LOCAL_MODULE    := ApplicationTemplate
#LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := android.cpp \
                   $(TOP_PATH)/ApplicationTemplate.cpp
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lz

include $(BUILD_SHARED_LIBRARY)
