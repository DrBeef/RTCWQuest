/************************************************************************************

Filename	:	VrInputDefault.c
Content		:	Handles default controller input
Created		:	August 2019
Authors		:	Simon Brown

*************************************************************************************/

#include <VrApi.h>
#include <VrApi_Helpers.h>
#include <VrApi_SystemUtils.h>
#include <VrApi_Input.h>
#include <VrApi_Types.h>
#include <android/keycodes.h>

#include "VrInput.h"
#include "VrCvars.h"

#include "../rtcw/src/client/client.h"

#define WP_AKIMBO           20

void SV_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask, int capsule );

void HandleInput_Default( ovrInputStateTrackedRemote *pDominantTrackedRemoteNew, ovrInputStateTrackedRemote *pDominantTrackedRemoteOld, ovrTracking* pDominantTracking,
                          ovrInputStateTrackedRemote *pOffTrackedRemoteNew, ovrInputStateTrackedRemote *pOffTrackedRemoteOld, ovrTracking* pOffTracking,
                          int domButton1, int domButton2, int offButton1, int offButton2 )

{
	//Ensure handedness is set correctly
	vr.right_handed = vr_control_scheme->value < 10;

	vr.teleportenabled = vr_teleport->integer != 0;

    static qboolean dominantGripPushed = false;
	static float dominantGripPushTime = 0.0f;
    static bool canUseBackpack = false;


    //Need this for the touch screen
    ovrTracking * pWeapon = pDominantTracking;
    ovrTracking * pOff = pOffTracking;
    if (vr.weaponid == WP_AKIMBO &&
            !vr.right_handed &&
            !RTCWVR_useScreenLayer())
    {
        //Revert to same weapon controls as right-handed if using akimbo
        pWeapon = pOffTracking;
        pOff = pDominantTracking;
    }

    {
        //Set gun angles - We need to calculate all those we might need (including adjustments) for the client to then take its pick
        vec3_t rotation = {0};
        rotation[PITCH] = 30;
        QuatToYawPitchRoll(pWeapon->HeadPose.Pose.Orientation, rotation, vr.weaponangles_knife);
        rotation[PITCH] = vr_weapon_pitchadjust->value;
        QuatToYawPitchRoll(pWeapon->HeadPose.Pose.Orientation, rotation, vr.weaponangles);

        VectorSubtract(vr.weaponangles_last, vr.weaponangles, vr.weaponangles_delta);
        VectorCopy(vr.weaponangles, vr.weaponangles_last);

        ALOGV("        weaponangles_last: %f, %f, %f",
              vr.weaponangles_last[0], vr.weaponangles_last[1], vr.weaponangles_last[2]);

    }

    //Menu button
	handleTrackedControllerButton(&leftTrackedRemoteState_new, &leftTrackedRemoteState_old, ovrButton_Enter, K_ESCAPE);

    static qboolean resetCursor = qtrue;
    if ( RTCWVR_useScreenLayer() )
    {
        interactWithTouchScreen(resetCursor, pDominantTrackedRemoteNew, pDominantTrackedRemoteOld);
        resetCursor = qfalse;

        handleTrackedControllerButton(pDominantTrackedRemoteNew, pDominantTrackedRemoteOld, domButton1, K_MOUSE1);
        handleTrackedControllerButton(pDominantTrackedRemoteNew, pDominantTrackedRemoteOld, ovrButton_Trigger, K_MOUSE1);
        handleTrackedControllerButton(pDominantTrackedRemoteNew, pDominantTrackedRemoteOld, domButton2, K_ESCAPE);
    }
    else
    {
        resetCursor = qtrue;

        static bool canUseQuickSave = false;
        if (pOffTracking->Status & (VRAPI_TRACKING_STATUS_POSITION_TRACKED | VRAPI_TRACKING_STATUS_POSITION_VALID)) {
            canUseQuickSave = false;
        }
        else if (!canUseQuickSave) {
            int channel = (vr_control_scheme->integer >= 10) ? 1 : 0;
            RTCWVR_Vibrate(40, channel, 0.5); // vibrate to let user know they can switch
            canUseQuickSave = true;
        }

        if (canUseQuickSave)
        {
            if (((pOffTrackedRemoteNew->Buttons & offButton1) !=
                 (pOffTrackedRemoteOld->Buttons & offButton1)) &&
                (pOffTrackedRemoteNew->Buttons & offButton1)) {
                sendButtonActionSimple("savegame quicksave");
            }

            if (((pOffTrackedRemoteNew->Buttons & offButton2) !=
                 (pOffTrackedRemoteOld->Buttons & offButton2)) &&
                (pOffTrackedRemoteNew->Buttons & offButton2)) {
                sendButtonActionSimple("loadgame quicksave");
            }
        }



        float distance = sqrtf(powf(pOff->HeadPose.Pose.Position.x - pWeapon->HeadPose.Pose.Position.x, 2) +
                               powf(pOff->HeadPose.Pose.Position.y - pWeapon->HeadPose.Pose.Position.y, 2) +
                               powf(pOff->HeadPose.Pose.Position.z - pWeapon->HeadPose.Pose.Position.z, 2));

        float distanceToHMD = sqrtf(powf(vr.hmdposition[0] - pWeapon->HeadPose.Pose.Position.x, 2) +
                                    powf(vr.hmdposition[1] - pWeapon->HeadPose.Pose.Position.y, 2) +
                                    powf(vr.hmdposition[2] - pWeapon->HeadPose.Pose.Position.z, 2));

        //Turn on weapon stabilisation?
        if ((pOffTrackedRemoteNew->Buttons & ovrButton_GripTrigger) !=
            (pOffTrackedRemoteOld->Buttons & ovrButton_GripTrigger)) {

            if (pOffTrackedRemoteNew->Buttons & ovrButton_GripTrigger)
            {
                if (distance < 0.50f)
                {
                    vr.weapon_stabilised = qtrue;
                }
            }
            else
            {
                vr.weapon_stabilised = qfalse;
            }
        }

        //Engage scope if conditions are right
        qboolean scopeready = vr.weapon_stabilised && (distanceToHMD < SCOPE_ENGAGE_DISTANCE);
        static qboolean lastScopeready = qfalse;
        if (scopeready != lastScopeready) {
            if (vr.scopedweapon && !vr.scopedetached) {
                if (!vr.scopeengaged && scopeready) {
                    ALOGV("**WEAPON EVENT**  trigger scope mode");
                    sendButtonActionSimple("weapalt");
                } else if (vr.scopeengaged && !scopeready) {
                    //Set this here so we don't retrigger scope by accident too soon
                    vr.scopeengaged = qfalse;
                    ALOGV("**WEAPON EVENT**  disable scope mode");
                    sendButtonActionSimple("weapalt");
                    RTCWVR_ResyncClientYawWithGameYaw();
                }
                lastScopeready = scopeready;
            }
        }

        static qboolean binocularstate = qfalse;
        qboolean binocularsactive = (vr.hasbinoculars && vr.backpackitemactive == 3 &&
                (distanceToHMD < BINOCULAR_ENGAGE_DISTANCE) &&
                (pDominantTracking->Status & (VRAPI_TRACKING_STATUS_POSITION_TRACKED | VRAPI_TRACKING_STATUS_POSITION_VALID)));
        if (binocularstate != binocularsactive)
        {
            //Engage scope if conditions are right
            binocularstate = binocularsactive;
            sendButtonAction("+zoom", binocularstate);
        }

        //dominant hand stuff first
        {
            //Record recent weapon position for trajectory based stuff
            for (int i = (NUM_WEAPON_SAMPLES-1); i != 0; --i)
            {
                VectorCopy(vr.weaponoffset_history[i-1], vr.weaponoffset_history[i]);
                vr.weaponoffset_history_timestamp[i] = vr.weaponoffset_history_timestamp[i-1];
            }
            VectorCopy(vr.weaponoffset, vr.weaponoffset_history[0]);
            vr.weaponoffset_history_timestamp[0] = vr.weaponoffset_timestamp;

			///Weapon location relative to view
            vr.weaponoffset[0] = pWeapon->HeadPose.Pose.Position.x - vr.hmdposition[0];
            vr.weaponoffset[1] = pWeapon->HeadPose.Pose.Position.y - vr.hmdposition[1];
            vr.weaponoffset[2] = pWeapon->HeadPose.Pose.Position.z - vr.hmdposition[2];
            vr.weaponoffset_timestamp = Sys_Milliseconds( );

            //Does weapon velocity trigger attack (knife) and is it fast enough
            static qboolean velocityTriggeredAttack = false;
            if (vr.velocitytriggered)
            {
                static qboolean fired = qfalse;
                float velocity = sqrtf(powf(pWeapon->HeadPose.LinearVelocity.x, 2) +
                                       powf(pWeapon->HeadPose.LinearVelocity.y, 2) +
                                       powf(pWeapon->HeadPose.LinearVelocity.z, 2));

                velocityTriggeredAttack = (velocity > VELOCITY_TRIGGER);

                if (fired != velocityTriggeredAttack) {
                    ALOGV("**WEAPON EVENT**  veocity triggered %s", velocityTriggeredAttack ? "+attack" : "-attack");
                    sendButtonAction("+attack", velocityTriggeredAttack);
                    fired = velocityTriggeredAttack;
                }
            }
            else if (velocityTriggeredAttack)
            {
                //send a stop attack as we have an unfinished velocity attack
                velocityTriggeredAttack = qfalse;
                ALOGV("**WEAPON EVENT**  veocity triggered -attack");
                sendButtonAction("+attack", velocityTriggeredAttack);
            }

            if (vr.weapon_stabilised || vr.dualwield)
            {
                float z = pOff->HeadPose.Pose.Position.z - pWeapon->HeadPose.Pose.Position.z;
                float x = pOff->HeadPose.Pose.Position.x - pWeapon->HeadPose.Pose.Position.x;
                float y = pOff->HeadPose.Pose.Position.y - pWeapon->HeadPose.Pose.Position.y;
                float zxDist = length(x, z);

                if (zxDist != 0.0f && z != 0.0f) {
                    if (vr.dualwield) {
                        //SUPER FUDGE
                        VectorSet(vr.weaponangles, vr.weaponangles[PITCH],
                                  -90.0f-degrees(atan2f(x, -z)), degrees(atanf(y / zxDist)));
                    }
                    else
                    {
                        VectorSet(vr.weaponangles, -degrees(atanf(y / zxDist)),
                                  -degrees(atan2f(x, -z)), vr.weaponangles[ROLL]);
                    }
                }
            }

            static bool finishReloadNextFrame = false;
            if (finishReloadNextFrame)
            {
                ALOGV("**WEAPON EVENT**  -reload");
                sendButtonActionSimple("-reload");
                finishReloadNextFrame = false;
            }

            if (pDominantTracking->Status & (VRAPI_TRACKING_STATUS_POSITION_TRACKED | VRAPI_TRACKING_STATUS_POSITION_VALID)) {
                canUseBackpack = false;
            }
            else if (!canUseBackpack && vr.backpackitemactive == 0) {
                int channel = (vr_control_scheme->integer >= 10) ? 0 : 1;
                    RTCWVR_Vibrate(40, channel, 0.5); // vibrate to let user know they can switch

                canUseBackpack = true;
            }

            dominantGripPushed = (pDominantTrackedRemoteNew->Buttons &
                                  ovrButton_GripTrigger) != 0;
            bool dominantButton1Pushed = (pDominantTrackedRemoteNew->Buttons &
                                     domButton1) != 0;
            bool dominantButton2Pushed = (pDominantTrackedRemoteNew->Buttons &
                                          domButton2) != 0;

            if (!canUseBackpack)
            {
                if (dominantGripPushed) {
                    if (dominantGripPushTime == 0) {
                        dominantGripPushTime = GetTimeInMilliSeconds();
                    }
                }
                else
                {
                    if (vr.backpackitemactive == 1) {
                        //Restores last used weapon if possible
                        char buffer[32];
                        sprintf(buffer, "weapon %i", vr.lastweaponid);
                        sendButtonActionSimple(buffer);
                        vr.backpackitemactive = 0;
                    }
                    else if ((GetTimeInMilliSeconds() - dominantGripPushTime) <
                        vr_reloadtimeoutms->integer) {
                        sendButtonActionSimple("+reload");
                        finishReloadNextFrame = true;
                    }
                    dominantGripPushTime = 0;
                }

                if (!dominantButton1Pushed && vr.backpackitemactive == 2)
                {
                    char buffer[32];
                    sprintf(buffer, "weapon %i", vr.lastweaponid);
                    sendButtonActionSimple(buffer);
                    vr.backpackitemactive = 0;
                }

                if (!dominantButton2Pushed && vr.backpackitemactive == 3)
                {
                    vr.backpackitemactive = 0;
                }
            } else {
                if (vr.backpackitemactive == 0) {
                    if (dominantGripPushed) {
                        vr.lastweaponid = vr.weaponid;
                        //Initiate grenade from backpack mode
                        sendButtonActionSimple("weaponbank 6");
                        int channel = (vr_control_scheme->integer >= 10) ? 0 : 1;
                        RTCWVR_Vibrate(80, channel, 0.8); // vibrate to let user know they switched
                        vr.backpackitemactive = 1;
                    }
                    else if (dominantButton1Pushed)
                    {
                        vr.lastweaponid = vr.weaponid;
                        //Initiate knife from backpack mode
                        sendButtonActionSimple("weapon 1");
                        int channel = (vr_control_scheme->integer >= 10) ? 0 : 1;
                        RTCWVR_Vibrate(80, channel, 0.8); // vibrate to let user know they switched
                        vr.backpackitemactive = 2;
                    }
                    else if (dominantButton2Pushed && vr.hasbinoculars)
                    {
                        int channel = (vr_control_scheme->integer >= 10) ? 0 : 1;
                        RTCWVR_Vibrate(80, channel, 0.8); // vibrate to let user know they switched
                        vr.backpackitemactive = 3;
                    }
                }
            }
        }

        float controllerYawHeading = 0.0f;
        //off-hand stuff
        {
            vr.offhandoffset[0] = pOff->HeadPose.Pose.Position.x - vr.hmdposition[0];
            vr.offhandoffset[1] = pOff->HeadPose.Pose.Position.y - vr.hmdposition[1];
            vr.offhandoffset[2] = pOff->HeadPose.Pose.Position.z - vr.hmdposition[2];

            vec3_t rotation = {0};
            QuatToYawPitchRoll(pOff->HeadPose.Pose.Orientation, rotation, vr.offhandangles);

			if (vr_walkdirection->value == 0) {
				controllerYawHeading = vr.offhandangles[YAW] - vr.hmdorientation[YAW];
			}
			else
			{
				controllerYawHeading = 0.0f;
			}
        }

        //Right-hand specific stuff
        {
            //This section corrects for the fact that the controller actually controls direction of movement, but we want to move relative to the direction the
            //player is facing for positional tracking
            vec2_t v;
            rotateAboutOrigin(-vr.hmdposition_delta[0] * vr_positional_factor->value,
                              vr.hmdposition_delta[2] * vr_positional_factor->value, - vr.hmdorientation[YAW], v);
            positional_movementSideways = v[0];
            positional_movementForward = v[1];

            ALOGV("        positional_movementSideways: %f, positional_movementForward: %f",
                  positional_movementSideways,
                  positional_movementForward);

            //Jump (B Button)
            if (vr.backpackitemactive != 2 && !canUseBackpack) {
                handleTrackedControllerButton(pDominantTrackedRemoteNew,
                                              pDominantTrackedRemoteOld, domButton2, K_SPACE);
            }


            //We need to record if we have started firing primary so that releasing trigger will stop firing, if user has pushed grip
            //in meantime, then it wouldn't stop the gun firing and it would get stuck
            static qboolean firing = false;
            if (dominantGripPushed && vr.backpackitemactive == 0)
            {
                if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) !=
                    (pDominantTrackedRemoteOld->Buttons & ovrButton_Trigger))
                {
                    if (!vr.scopedweapon) {
                        if (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) {
                            ALOGV("**WEAPON EVENT**  weapalt");
                            sendButtonActionSimple("weapalt");
                        }
                        else if (firing)
                        {
                            //no longer firing
                            firing = qfalse;
                            ALOGV("**WEAPON EVENT**  Grip Pushed %sattack", (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) ? "+" : "-");
                            sendButtonAction("+attack", firing);
                        }
                    }
                    else if (vr.detachablescope)
                    {
                        if (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) {
                            //See if we are able to detach the scope
                            ALOGV("**WEAPON EVENT**  weapdetachscope");
                            sendButtonActionSimple("weapdetachscope");
                        }
                    }
                    else
                    {
                        //Just ignore grip and fire
                        firing = (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger);
                        ALOGV("**WEAPON EVENT**  Grip Pushed %sattack", (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) ? "+" : "-");
                        sendButtonAction("+attack", firing);
                    }
                }
            }
            else
            {
                //Fire Primary
                if (vr.backpackitemactive != 3 && // Can't fire while holding binoculars
                    !vr.velocitytriggered && // Don't fire velocity triggered weapons
                    (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) !=
                    (pDominantTrackedRemoteOld->Buttons & ovrButton_Trigger)) {

                    ALOGV("**WEAPON EVENT**  Not Grip Pushed %sattack", (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) ? "+" : "-");
                    firing = (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger);
                    sendButtonAction("+attack", firing);
                }
            }

            //Duck with A
            if (vr.backpackitemactive != 2 &&
                !canUseBackpack &&
                (pDominantTrackedRemoteNew->Buttons & domButton1) !=
                (pDominantTrackedRemoteOld->Buttons & domButton1) &&
                ducked != DUCK_CROUCHED) {
                ducked = (pDominantTrackedRemoteNew->Buttons & domButton1) ? DUCK_BUTTON
                                                                           : DUCK_NOTDUCKED;
                sendButtonAction("+movedown", (pDominantTrackedRemoteNew->Buttons & domButton1));
            }

			//Weapon/Inventory Chooser
			static qboolean itemSwitched = false;
			if (between(-0.2f, pDominantTrackedRemoteNew->Joystick.x, 0.2f) &&
				(between(0.8f, pDominantTrackedRemoteNew->Joystick.y, 1.0f) ||
				 between(-1.0f, pDominantTrackedRemoteNew->Joystick.y, -0.8f)))
			{
				if (!itemSwitched) {
					if (between(0.8f, pDominantTrackedRemoteNew->Joystick.y, 1.0f))
					{
                        sendButtonActionSimple("weapprev");
					}
					else
					{
                        sendButtonActionSimple("weapnext");
					}
					itemSwitched = true;
				}
			} else {
				itemSwitched = false;
			}
        }

        //Left-hand specific stuff
        {
            //"Use" (open doors etc)
            if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Joystick) !=
                (pDominantTrackedRemoteOld->Buttons & ovrButton_Joystick)) {

                sendButtonAction("+activate",
                                 (pDominantTrackedRemoteNew->Buttons & ovrButton_Joystick) ? 1 : 0);
            }

            //Apply a filter and quadratic scaler so small movements are easier to make
            float dist = length(pOffTrackedRemoteNew->Joystick.x, pOffTrackedRemoteNew->Joystick.y);
            float nlf = nonLinearFilter(dist);
            float x = nlf * pOffTrackedRemoteNew->Joystick.x;
            float y = nlf * pOffTrackedRemoteNew->Joystick.y;

            vr.player_moving = (fabs(x) + fabs(y)) > 0.05f;

            //Adjust to be off-hand controller oriented
            vec2_t v;
            rotateAboutOrigin(x, y, controllerYawHeading, v);

            //Move a lot slower if scope is engaged or teleport is standard locomotion mode
            remote_movementSideways = v[0] * (vr.scopeengaged || vr.teleportenabled ? 0.3f : 1.0f);
            remote_movementForward = v[1] * (vr.scopeengaged || vr.teleportenabled ? 0.3f : 1.0f);
            ALOGV("        remote_movementSideways: %f, remote_movementForward: %f",
                  remote_movementSideways,
                  remote_movementForward);


            if (!canUseQuickSave) {
                if (((pOffTrackedRemoteNew->Buttons & offButton1) !=
                     (pOffTrackedRemoteOld->Buttons & offButton1)) &&
                    (pOffTrackedRemoteNew->Buttons & offButton1)) {

                    if (dominantGripPushed) {
                        //If cheats enabled, give all weapons/pickups to player
                        Cbuf_AddText("give all\n");
                    } else {
                        vr.visible_hud = !vr.visible_hud;
                    }
                }
            }

            //notebook
            if (!canUseQuickSave) {
                if (((pOffTrackedRemoteNew->Buttons & offButton2) !=
                     (pOffTrackedRemoteOld->Buttons & offButton2)) &&
                    (pOffTrackedRemoteNew->Buttons & offButton2)) {
                    sendButtonActionSimple("notebook");
                }
            }


            //Kick!
            if ((pOffTrackedRemoteNew->Buttons & ovrButton_Joystick) !=
                (pOffTrackedRemoteOld->Buttons & ovrButton_Joystick)) {
                sendButtonAction("+kick", (pOffTrackedRemoteNew->Buttons & ovrButton_Joystick));
            }

            //We need to record if we have started firing primary so that releasing trigger will stop definitely firing, if user has pushed grip
            //in meantime, then it wouldn't stop the gun firing and it would get stuck
            if (!vr.teleportenabled)
            {
                //Run
                handleTrackedControllerButton(pOffTrackedRemoteNew,
                                              pOffTrackedRemoteOld,
                                              ovrButton_Trigger, K_SHIFT);
            } else {
                if (pOffTrackedRemoteNew->Buttons & ovrButton_Trigger)
                {
                    vr.teleportseek = qtrue;
                }
                else if (vr.teleportseek)
                {
                    vr.teleportseek = qfalse;
                    vr.teleportexecute = vr.teleportready;
                    vr.teleportready = qfalse;
                }
            }


            //Resync Yaw on mounted gun transition
            static int usingMountedGun = false;
            if (vr.mountedgun != usingMountedGun)
            {
                resyncClientYawWithGameYaw = 10; // HACK
                usingMountedGun = vr.mountedgun;
            }

            //No snap turn when using mounted gun
            static int increaseSnap = true;
            if (!vr.mountedgun) {
                if (pDominantTrackedRemoteNew->Joystick.x > 0.6f) {
                    if (increaseSnap) {
                        snapTurn -= vr_snapturn_angle->value;
                        if (vr_snapturn_angle->value > 10.0f) {
                            increaseSnap = false;
                        }

                        if (snapTurn < -180.0f) {
                            snapTurn += 360.f;
                        }

                        RTCWVR_ResyncClientYawWithGameYaw();
                    }
                } else if (pDominantTrackedRemoteNew->Joystick.x < 0.4f) {
                    increaseSnap = true;
                }

                static int decreaseSnap = true;
                if (pDominantTrackedRemoteNew->Joystick.x < -0.6f) {
                    if (decreaseSnap) {
                        snapTurn += vr_snapturn_angle->value;

                        //If snap turn configured for less than 10 degrees
                        if (vr_snapturn_angle->value > 10.0f) {
                            decreaseSnap = false;
                        }

                        if (snapTurn > 180.0f) {
                            snapTurn -= 360.f;
                        }

                        RTCWVR_ResyncClientYawWithGameYaw();
                    }
                } else if (pDominantTrackedRemoteNew->Joystick.x > -0.4f) {
                    decreaseSnap = true;
                }
            }
            else {
                if (fabs(pDominantTrackedRemoteNew->Joystick.x) > 0.5f) {
                    if (increaseSnap)
                    {
                        RTCWVR_ResyncClientYawWithGameYaw();
                    }
                    increaseSnap = false;
                }
                else
                {
                    increaseSnap = true;
                }
            }
        }

        updateScopeAngles();
    }

    //Save state
    rightTrackedRemoteState_old = rightTrackedRemoteState_new;
    leftTrackedRemoteState_old = leftTrackedRemoteState_new;
}