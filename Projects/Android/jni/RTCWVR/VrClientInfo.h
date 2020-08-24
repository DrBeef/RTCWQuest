#if !defined(vr_client_info_h)
#define vr_client_info_h

#define NUM_WEAPON_SAMPLES      10

typedef struct {
    qboolean screen;
    float  fov;
    qboolean weapon_stabilised;
    qboolean right_handed;
    qboolean player_moving;
    qboolean visible_hud;
    qboolean dualwield;
    int weaponid;
    int lastweaponid;
    int backpackitemactive; //0 - nothing, 1 - grenades, 2 - knife, 3 - Binoculars
    qboolean mountedgun;

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

    //Lots of scope weapon stuff
    qboolean scopeengaged;          // Scope has been engaged on a scoped weapon
    qboolean scopedweapon;          // Weapon scope is available
    qboolean scopedetached;         // Scope has been detached from weapon
    qboolean detachablescope;       // Scope can be detached from weapon

    //Virtual Stock stuff
    qboolean vstock_weapon;          // Weapon has a stock that can be used "vitually"
    qboolean vstock_engaged;         // Virtual Stock is engaged
    vec3_t   vstock_weapon_offset;      // The offset of the fixed primary hand when Virtual Stock is engaged (set by cgame Weapon code)

    qboolean hasbinoculars;

    qboolean velocitytriggered; // Weapon attack triggered by velocity (knife)

    vec3_t offhandangles;
    vec3_t offhandoffset;

    //
    // Teleport Stuff
    //
    qboolean teleportenabled;
    qboolean teleportseek; // player looking to teleport
    qboolean teleportready; // player pointing to a valid teleport location
    vec3_t   teleportdest; // teleport destination
    qboolean teleportexecute; // execute the teleport



    //////////////////////////////////////
    //    Test stuff for weapon alignment
    //////////////////////////////////////

    char    test_name[256];
    float   test_scale;
    vec3_t  test_angles;
    vec3_t  test_offset;

} vr_client_info_t;

#endif //vr_client_info_h