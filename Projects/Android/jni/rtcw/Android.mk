

RTCW_BASE_CFLAGS = \
 -pipe -DPANDORA -DHAVE_GLES -DARM  -DC_ONLY

#-DNEON  -mfpu=neon 

RTCW_BASE_LDFLAGS = \
-Wl, -llog

include $(TOP_DIR)/rtcw/Android_game.mk
include $(TOP_DIR)/rtcw/Android_ui.mk
include $(TOP_DIR)/rtcw/Android_cgame.mk
include $(TOP_DIR)/rtcw/Android_client.mk

