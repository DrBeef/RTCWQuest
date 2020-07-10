#if !defined(vr_client_info_h)
#define vr_client_info_h

typedef struct {
    float  fov;
    qboolean weapon_stabilised;
    qboolean right_handed;

    vec3_t hmdposition;
    vec3_t hmdposition_last; // Don't use this, it is just for calculating delta!
    vec3_t hmdposition_delta;

    vec3_t hmdorientation;
    vec3_t hmdorientation_last; // Don't use this, it is just for calculating delta!
    vec3_t hmdorientation_delta;

    vec3_t weaponangles_unadjusted;
    vec3_t weaponangles;
    vec3_t weaponangles_last; // Don't use this, it is just for calculating delta!
    vec3_t weaponangles_delta;

    vec3_t weaponoffset;

    qboolean scopeengaged; // Scope has been engaged on a scoped weapon
    qboolean scopeready;   // Scope can be engaged
    qboolean scopedweapon; // Weapon supports scope

    vec3_t offhandangles;
    vec3_t offhandoffset;
} vr_client_info_t;

#endif //vr_client_info_h