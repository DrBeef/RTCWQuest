#if !defined(vrcommon_h)
#define vrcommon_h

#include <VrApi_Input.h>

#include <android/log.h>

#include "mathlib.h"

#define LOG_TAG "RTCWVR"

#ifndef NDEBUG
#define DEBUG 1
#endif

#define ALOGE(...) __android_log_print( ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__ )

#if DEBUG
#define ALOGV(...) __android_log_print( ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__ )
#else
#define ALOGV(...)
#endif

qboolean rtcw_initialised;

long long global_time;

float playerHeight;
float playerYaw;

qboolean showingScreenLayer;
float vrFOV;

ovrTracking2 tracking;

vec3_t worldPosition;

vec3_t hmdPosition;
vec3_t hmdorientation;
vec3_t positionDeltaThisFrame;

vec3_t weaponangles;
vec3_t weaponoffset;

vec3_t flashlightangles;
vec3_t flashlightoffset;

#define DUCK_NOTDUCKED 0
#define DUCK_BUTTON 1
#define DUCK_CROUCHED 2
int ducked;

qboolean player_moving;


float radians(float deg);
float degrees(float rad);
qboolean isMultiplayer();
double GetTimeInMilliSeconds();
float length(float x, float y);
float nonLinearFilter(float in);
qboolean between(float min, float val, float max);
void rotateAboutOrigin(float v1, float v2, float rotation, vec2_t out);
void QuatToYawPitchRoll(ovrQuatf q, vec3_t rotation, vec3_t out);
void handleTrackedControllerButton(ovrInputStateTrackedRemote * trackedRemoteState, ovrInputStateTrackedRemote * prevTrackedRemoteState, uint32_t button, int key);
void interactWithTouchScreen(ovrTracking *tracking, ovrInputStateTrackedRemote *newState, ovrInputStateTrackedRemote *oldState);


//Called from engine code
qboolean RTCWVR_useScreenLayer();
void RTCWVR_GetScreenRes(uint32_t *width, uint32_t *height);
void RTCWVR_Vibrate(float duration, int channel, float intensity );
qboolean RTCWVR_processMessageQueue();
void RTCWVR_FrameSetup();
void RTCWVR_setUseScreenLayer(qboolean use);
void RTCWVR_processHaptics();
void RTCWVR_getHMDOrientation();
void RTCWVR_getTrackedRemotesOrientation();

void RTCWVR_incrementFrameIndex();

void RTCWVR_prepareEyeBuffer(int eye );
void RTCWVR_finishEyeBuffer(int eye );
void RTCWVR_submitFrame();

#endif //vrcommon_h