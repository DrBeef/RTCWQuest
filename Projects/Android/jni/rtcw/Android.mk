

RTCW_BASE_CFLAGS = \
-fsigned-char -O0 \
 -Ofast -ffast-math -fno-strict-aliasing -fstrength-reduce -ftree-vectorize -fsingle-precision-constant \
 -pipe -DPANDORA -DHAVE_GLES -DARM  -DC_ONLY

#-DNEON  -mfpu=neon 

RTCW_BASE_LDFLAGS = \
-Wl,--fix-cortex-a8 -llog

include $(TOP_DIR)/rtcw/Android_game.mk
include $(TOP_DIR)/rtcw/Android_ui.mk
include $(TOP_DIR)/rtcw/Android_cgame.mk
include $(TOP_DIR)/rtcw/Android_client.mk

