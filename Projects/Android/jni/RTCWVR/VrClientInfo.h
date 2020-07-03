#if !defined(vr_client_info_h)
#define vr_client_info_h

typedef struct {
    float  fov;
    qboolean weapon_stabilised;

    vec3_t worldPosition;

    vec3_t hmdPosition;
    vec3_t hmdorientation;
    vec3_t positionDeltaThisFrame;

    vec3_t weaponangles;
    vec3_t weaponoffset;

    vec3_t flashlightangles;
    vec3_t flashlightoffset;
} vr_client_info_t;

#endif //vr_client_info_h