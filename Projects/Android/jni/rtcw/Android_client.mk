LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS)


LOCAL_MODULE    := rtcw_client


LOCAL_CFLAGS = $(RTCW_BASE_CFLAGS)  -DBOTLIB

LOCAL_LDFLAGS = $(RTCW_BASE_LDFLAGS)

LOCAL_C_INCLUDES = $(LOCAL_PATH) \
                $(GL4ES_PATH) \
				$(GL4ES_PATH)/include

 
ANDROID_SRC = \
	code/android/ifaddrs.c \
	# code/android/android-jni.cpp \
	# code/android/in_android.c \
	
SPLINES_FILES = \
  src/splines/math_angles.cpp \
  src/splines/math_matrix.cpp \
  src/splines/math_quaternion.cpp \
  src/splines/math_vector.cpp \
  src/splines/q_parse.cpp \
  src/splines/splines.cpp \
  src/splines/util_str.cpp   \
  #src/splines/q_shared.cpp \
  
  
BOTLIB_FILES = \
  src/botlib/be_aas_bspq3.c \
  src/botlib/be_aas_cluster.c \
  src/botlib/be_aas_debug.c \
  src/botlib/be_aas_entity.c \
  src/botlib/be_aas_file.c \
  src/botlib/be_aas_main.c \
  src/botlib/be_aas_move.c \
  src/botlib/be_aas_optimize.c \
  src/botlib/be_aas_reach.c \
  src/botlib/be_aas_route.c \
  src/botlib/be_aas_routealt.c \
  src/botlib/be_aas_routetable.c \
  src/botlib/be_aas_sample.c \
  src/botlib/be_ai_char.c \
  src/botlib/be_ai_chat.c \
  src/botlib/be_ai_gen.c \
  src/botlib/be_ai_goal.c \
  src/botlib/be_ai_move.c \
  src/botlib/be_ai_weap.c \
  src/botlib/be_ai_weight.c \
  src/botlib/be_ea.c \
  src/botlib/be_interface.c \
  src/botlib/l_crc.c \
  src/botlib/l_libvar.c \
  src/botlib/l_log.c \
  src/botlib/l_memory.c \
  src/botlib/l_precomp.c \
  src/botlib/l_script.c \
  src/botlib/l_struct.c \

JPEGLIB_FILES = \
  src/jpeg-6/jcapimin.c  \
  src/jpeg-6/jchuff.c  \
  src/jpeg-6/jcinit.c  \
  src/jpeg-6/jccoefct.c  \
  src/jpeg-6/jccolor.c  \
  src/jpeg-6/jfdctflt.c  \
  src/jpeg-6/jcdctmgr.c  \
  src/jpeg-6/jcphuff.c  \
  src/jpeg-6/jcmainct.c  \
  src/jpeg-6/jcmarker.c  \
  src/jpeg-6/jcmaster.c  \
  src/jpeg-6/jcomapi.c \
  src/jpeg-6/jcparam.c \
  src/jpeg-6/jcprepct.c \
  src/jpeg-6/jcsample.c  \
  src/jpeg-6/jdapimin.c  \
  src/jpeg-6/jdapistd.c  \
  src/jpeg-6/jdatasrc.c  \
  src/jpeg-6/jdcoefct.c  \
  src/jpeg-6/jdcolor.c  \
  src/jpeg-6/jddctmgr.c  \
  src/jpeg-6/jdhuff.c  \
  src/jpeg-6/jdinput.c  \
  src/jpeg-6/jdmainct.c  \
  src/jpeg-6/jdmarker.c  \
  src/jpeg-6/jdmaster.c  \
  src/jpeg-6/jdpostct.c  \
  src/jpeg-6/jdsample.c  \
  src/jpeg-6/jdtrans.c  \
  src/jpeg-6/jerror.c   \
  src/jpeg-6/jidctflt.c  \
  src/jpeg-6/jmemmgr.c  \
  src/jpeg-6/jmemnobs.c    \
  src/jpeg-6/jutils.c  \
  
  
RENDERER_FILES = \
  src/renderer/tr_animation.c    \
  src/renderer/tr_backend.c    \
  src/renderer/tr_bsp.c    \
  src/renderer/tr_cmds.c    \
  src/renderer/tr_cmesh.c \
  src/renderer/tr_curve.c    \
  src/renderer/tr_flares.c    \
  src/renderer/tr_font.c     \
  src/renderer/tr_image.c     \
  src/renderer/tr_init.c      \
  src/renderer/tr_light.c    \
  src/renderer/tr_main.c     \
  src/renderer/tr_marks.c     \
  src/renderer/tr_mesh.c     \
  src/renderer/tr_model.c     \
  src/renderer/tr_noise.c     \
  src/renderer/tr_scene.c     \
  src/renderer/tr_shade.c     \
  src/renderer/tr_shade_calc.c    \
  src/renderer/tr_shader.c     \
  src/renderer/tr_shadows.c     \
  src/renderer/tr_sky.c     \
  src/renderer/tr_surface.c    \
  src/renderer/tr_world.c     \



CLIENT_FILES = \
  src/client/cl_cgame.c   \
  src/client/cl_cin.c       \
  src/client/cl_console.c  \
  src/client/cl_input.c   \
  src/client/cl_keys.c     \
  src/client/cl_main.c     \
  src/client/cl_net_chan.c  \
  src/client/cl_parse.c   \
  src/client/cl_scrn.c    \
  src/client/cl_ui.c         \
  
COMMON_FILES = \
  src/qcommon/cm_load.c \
  src/qcommon/cm_patch.c \
  src/qcommon/cm_polylib.c \
  src/qcommon/cm_test.c \
  src/qcommon/cm_trace.c \
  src/qcommon/cmd.c \
  src/qcommon/common.c \
  src/qcommon/cvar.c \
  src/qcommon/files.c \
  src/qcommon/md4.c \
  src/qcommon/msg.c \
  src/qcommon/net_chan.c \
  src/qcommon/huffman.c \
  src/qcommon/unzip.c \
 

SOUND_FILES = \
  src/client/snd_adpcm.c \
  src/client/snd_dma.c \
  src/client/snd_mem.c \
  src/client/snd_mix.c \
  src/client/snd_wavelet.c \
  


SERVER_FILES = \
  src/server/sv_bot.c \
  src/server/sv_ccmds.c \
  src/server/sv_client.c \
  src/server/sv_game.c \
  src/server/sv_init.c \
  src/server/sv_main.c \
  src/server/sv_net_chan.c \
  src/server/sv_snapshot.c \
  src/server/sv_world.c \
 
 VM_FILES = \
  src/qcommon/vm.c \
  src/qcommon/vm_interpreted.c \
  src/qcommon/vm_none.c \
 
 UNIX_FILES = \
  src/unix/unix_net.c \
  src/unix/unix_shared.c \
  src/unix/linux_common.c\
  src/unix/linux_qgl.c   \
  src/android/android_glimp.c   \
  src/android/android_snd.c \
  src/game/q_shared.c \
  src/game/q_math.c \
  src/android/android_main.c \
  src/android/in_android.c
  # src/android/android-jni.cpp \
   

RTCWVR_SRC_FILES :=  ../RTCWVR/RTCWVR_SurfaceView.c \
       ../RTCWVR/VrCompositor.c \
       ../RTCWVR/VrInputCommon.c \
       ../RTCWVR/VrInputDefault.c \
       ../RTCWVR/mathlib.c \
       ../RTCWVR/matrixlib.c \
       ../RTCWVR/argtable3.c
	   
  
LOCAL_SRC_FILES =  $(UNIX_FILES) $(RENDERER_FILES) $(SPLINES_FILES) $(BOTLIB_FILES) $(JPEGLIB_FILES) $(CLIENT_FILES) $(COMMON_FILES) $(SOUND_FILES) $(SERVER_FILES) $(VM_FILES) $(RTCWVR_SRC_FILES)

LOCAL_LDLIBS := -lGLESv3 -landroid -lEGL -ldl -llog -lOpenSLES -lz -lm

LOCAL_LDLIBS += -fuse-ld=bfd
 
# LOCAL_STATIC_LIBRARIES :=  libjpeg libpng
LOCAL_SHARED_LIBRARIES :=  vrapi gl4es

include $(BUILD_SHARED_LIBRARY)


$(call import-module,VrApi/Projects/AndroidPrebuilt/jni)




