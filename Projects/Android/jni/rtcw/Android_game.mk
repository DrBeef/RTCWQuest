LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS)


LOCAL_MODULE    := qagamearm

LOCAL_CFLAGS = $(RTCW_BASE_CFLAGS)


LOCAL_CFLAGS += -fPIC -DGAMEDLL
LOCAL_LDFLAGS += $(RTCW_BASE_LDFLAGS) #-shared -ldl -lm  -rdynamic

LOCAL_C_INCLUDES = $(LOCAL_PATH)


 
GAME_FILES = \
  src/game/ai_cast.c \
  src/game/ai_cast_characters.c \
  src/game/ai_cast_debug.c \
  src/game/ai_cast_events.c \
  src/game/ai_cast_fight.c \
  src/game/ai_cast_func_attack.c \
  src/game/ai_cast_func_boss1.c \
  src/game/ai_cast_funcs.c \
  src/game/ai_cast_script.c \
  src/game/ai_cast_script_actions.c \
  src/game/ai_cast_script_ents.c \
  src/game/ai_cast_sight.c \
  src/game/ai_cast_think.c \
  src/botai/ai_chat.c \
  src/botai/ai_cmd.c \
  src/botai/ai_dmnet.c \
  src/botai/ai_dmq3.c \
  src/botai/ai_main.c \
  src/botai/ai_team.c  \
  src/game/bg_animation.c \
  src/game/bg_misc.c \
  src/game/bg_pmove.c \
  src/game/bg_slidemove.c \
  src/game/g_active.c \
  src/game/g_alarm.c \
  src/game/g_bot.c \
  src/game/g_client.c \
  src/game/g_cmds.c \
  src/game/g_combat.c \
  src/game/g_items.c \
  src/game/g_main.c \
  src/game/g_mem.c \
  src/game/g_misc.c \
  src/game/g_missile.c \
  src/game/g_mover.c \
  src/game/g_props.c \
  src/game/g_save.c \
  src/game/g_script.c \
  src/game/g_script_actions.c \
  src/game/g_session.c \
  src/game/g_spawn.c \
  src/game/g_svcmds.c \
  src/game/g_syscalls.c \
  src/game/g_target.c \
  src/game/g_team.c \
  src/game/g_tramcar.c \
  src/game/g_trigger.c \
  src/game/g_utils.c \
  src/game/g_weapon.c \
  src/game/q_math.c \
  src/game/q_shared.c \
  
  
  
LOCAL_SRC_FILES = $(GAME_FILES)

include $(BUILD_SHARED_LIBRARY)






