#if !defined(vrorientation_h)
#define vrorientation_h

#include "mathlib.h"

typedef struct {
    vec3_t worldPosition;

    vec3_t hmdPosition;
    vec3_t hmdorientation;
    vec3_t positionDeltaThisFrame;

    vec3_t weaponangles;
    vec3_t weaponoffset;

    vec3_t flashlightangles;
    vec3_t flashlightoffset;
} vr_orientation_t;

#endif //vrorientation_h