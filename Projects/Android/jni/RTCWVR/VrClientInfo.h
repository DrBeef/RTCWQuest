#if !defined(vr_client_info_h)
#define vr_client_info_h

typedef struct {
    float  fov;
    qboolean weapon_stabilised;

    vec3_t hmdposition;
    vec3_t hmdposition_last; // Don't use this, it is just for calculating delta!
    vec3_t hmdposition_delta;

    vec3_t hmdorientation;
    vec3_t hmdorientation_last; // Don't use this, it is just for calculating delta!
    vec3_t hmdorientation_delta;

    vec3_t weaponangles;
    vec3_t weaponangles_last; // Don't use this, it is just for calculating delta!
    vec3_t weaponangles_delta;

    vec3_t weaponoffset;

    vec3_t flashlightangles;
    vec3_t flashlightoffset;
} vr_client_info_t;

#endif //vr_client_info_h