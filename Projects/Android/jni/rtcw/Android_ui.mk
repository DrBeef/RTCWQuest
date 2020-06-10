LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS)


LOCAL_MODULE    := uiarm

LOCAL_CFLAGS = $(RTCW_BASE_CFLAGS)


LOCAL_CFLAGS += -fPIC 

LOCAL_LDFLAGS += $(RTCW_BASE_LDFLAGS)

LOCAL_C_INCLUDES = $(LOCAL_PATH)


UI_FILES= \
  src/game/bg_misc.c \
  src/game/q_math.c\
  src/game/q_shared.c\
  src/ui/ui_atoms.c\
  src/ui/ui_gameinfo.c\
  src/ui/ui_main.c\
  src/ui/ui_players.c\
  src/ui/ui_shared.c\
  src/ui/ui_syscalls.c\
  src/ui/ui_util.c\
  
  
LOCAL_SRC_FILES = $(UI_FILES)


include $(BUILD_SHARED_LIBRARY)






