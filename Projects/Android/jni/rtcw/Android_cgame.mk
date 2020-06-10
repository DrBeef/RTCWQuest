LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS)


LOCAL_MODULE    := cgamearm

LOCAL_CFLAGS = $(RTCW_BASE_CFLAGS)


LOCAL_CFLAGS += -fPIC -DCGAMEDLL

LOCAL_LDFLAGS += $(RTCW_BASE_LDFLAGS)

LOCAL_C_INCLUDES = $(LOCAL_PATH)


 
GAME_FILES = \
  src/game/bg_animation.c\
  src/game/bg_misc.c\
  src/game/bg_pmove.c\
  src/game/bg_slidemove.c\
  src/cgame/cg_consolecmds.c\
  src/cgame/cg_draw.c\
  src/cgame/cg_drawtools.c\
  src/cgame/cg_effects.c\
  src/cgame/cg_ents.c\
  src/cgame/cg_event.c\
  src/cgame/cg_flamethrower.c\
  src/cgame/cg_info.c\
  src/cgame/cg_localents.c\
  src/cgame/cg_main.c\
  src/cgame/cg_marks.c\
  src/cgame/cg_newDraw.c\
  src/cgame/cg_particles.c\
  src/cgame/cg_players.c\
  src/cgame/cg_playerstate.c\
  src/cgame/cg_predict.c\
  src/cgame/cg_scoreboard.c\
  src/cgame/cg_servercmds.c\
  src/cgame/cg_snapshot.c\
  src/cgame/cg_sound.c\
  src/cgame/cg_syscalls.c\
  src/cgame/cg_trails.c\
  src/cgame/cg_view.c\
  src/cgame/cg_weapons.c\
  src/game/q_math.c\
  src/game/q_shared.c\
  src/ui/ui_shared.c\
  
  
  
LOCAL_SRC_FILES = $(GAME_FILES)

include $(BUILD_SHARED_LIBRARY)






