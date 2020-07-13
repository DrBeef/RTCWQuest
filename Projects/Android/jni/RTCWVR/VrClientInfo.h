#if !defined(vr_client_info_h)
#define vr_client_info_h

#define NUM_WEAPON_SAMPLES      10

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

    vec3_t weaponangles_knife;
    vec3_t weaponangles;
    vec3_t weaponangles_last; // Don't use this, it is just for calculating delta!
    vec3_t weaponangles_delta;

    vec3_t weaponoffset;
    float weaponoffset_timestamp;
    vec3_t weaponoffset_history[NUM_WEAPON_SAMPLES];
    float weaponoffset_history_timestamp[NUM_WEAPON_SAMPLES];

    qboolean scopeengaged; // Scope has been engaged on a scoped weapon
    qboolean scopeready;   // Scope can be engaged
    qboolean scopedweapon; // Weapon supports scope

    qboolean velocitytriggered; // Weapon attack triggered by velocity (knife)

    vec3_t offhandangles;
    vec3_t offhandoffset;


    //////////////////////////////////////
    //    Test stuff for weapon alignment
    //////////////////////////////////////

    char    test_name[256];
    float   test_scale;
    vec3_t  test_angles;
    vec3_t  test_offset;

} vr_client_info_t;

#endif //vr_client_info_h