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

#include "VrInput.h"
#include "VrCvars.h"

#include "../rtcw/src/client/client.h"

cvar_t	*sv_cheats;



void HandleInput_Default( ovrInputStateTrackedRemote *pDominantTrackedRemoteNew, ovrInputStateTrackedRemote *pDominantTrackedRemoteOld, ovrTracking* pDominantTracking,
                          ovrInputStateTrackedRemote *pOffTrackedRemoteNew, ovrInputStateTrackedRemote *pOffTrackedRemoteOld, ovrTracking* pOffTracking,
                          int domButton1, int domButton2, int offButton1, int offButton2 )

{
	//Ensure handedness is set correctly
	Cvar_Set("hand", vr_control_scheme->value < 10 ? "0" : "1");

	//Get the cvar
    sv_cheats = Cvar_Get("cheats", "1", CVAR_ARCHIVE);

    static qboolean dominantGripPushed = false;
	static float dominantGripPushTime = 0.0f;
    static int grabMeleeWeapon = 0;
    static bool canUseBackpack = false;


    //switch to screen layer override
    if ((pOffTrackedRemoteNew->Buttons & ovrButton_Joystick) !=
        (pOffTrackedRemoteOld->Buttons & ovrButton_Joystick)) {
        showingScreenLayer = !showingScreenLayer;
    }


    //Menu button
	handleTrackedControllerButton(&leftTrackedRemoteState_new, &leftTrackedRemoteState_old, ovrButton_Enter, K_ESCAPE);

    if ( RTCWVR_useScreenLayer() )
    {
        interactWithTouchScreen(pDominantTracking, pDominantTrackedRemoteNew, pDominantTrackedRemoteOld);

        handleTrackedControllerButton(pDominantTrackedRemoteNew, pDominantTrackedRemoteOld, domButton1, K_MOUSE1);
        handleTrackedControllerButton(pDominantTrackedRemoteNew, pDominantTrackedRemoteOld, ovrButton_Trigger, K_MOUSE1);
        handleTrackedControllerButton(pDominantTrackedRemoteNew, pDominantTrackedRemoteOld, domButton2, K_ESCAPE);
    }
    else
    {

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



        float distance = sqrtf(powf(pOffTracking->HeadPose.Pose.Position.x - pDominantTracking->HeadPose.Pose.Position.x, 2) +
                               powf(pOffTracking->HeadPose.Pose.Position.y - pDominantTracking->HeadPose.Pose.Position.y, 2) +
                               powf(pOffTracking->HeadPose.Pose.Position.z - pDominantTracking->HeadPose.Pose.Position.z, 2));

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

        //dominant hand stuff first
        {
			///Weapon location relative to view
            vr.weaponoffset[0] = pDominantTracking->HeadPose.Pose.Position.x - vr.hmdPosition[0];
            vr.weaponoffset[1] = pDominantTracking->HeadPose.Pose.Position.y - vr.hmdPosition[1];
            vr.weaponoffset[2] = pDominantTracking->HeadPose.Pose.Position.z - vr.hmdPosition[2];

			{
				vec2_t v;
				rotateAboutOrigin(-vr.weaponoffset[0], vr.weaponoffset[2], (cl.viewangles[YAW] - vr.hmdorientation[YAW]), v);
				vr.weaponoffset[0] = v[0];
				vr.weaponoffset[2] = v[1];
			}

            //Set gun angles - We need to calculate all those we might need (including adjustments) for the client to then take its pick
            vec3_t rotation = {0};
            rotation[PITCH] = vr_weapon_pitchadjust->value;
            QuatToYawPitchRoll(pDominantTracking->HeadPose.Pose.Orientation, rotation, vr.weaponangles);
            vr.weaponangles[YAW] += (cl.viewangles[YAW] - vr.hmdorientation[YAW]);
            vr.weaponangles[ROLL] *= -1.0f;


            if (vr.weapon_stabilised)
            {
                float z = pOffTracking->HeadPose.Pose.Position.z - pDominantTracking->HeadPose.Pose.Position.z;
                float x = pOffTracking->HeadPose.Pose.Position.x - pDominantTracking->HeadPose.Pose.Position.x;
                float y = pOffTracking->HeadPose.Pose.Position.y - pDominantTracking->HeadPose.Pose.Position.y;
                float zxDist = length(x, z);

                if (zxDist != 0.0f && z != 0.0f) {
                    VectorSet(vr.weaponangles, -degrees(atanf(y / zxDist)), (cl.viewangles[YAW] - vr.hmdorientation[YAW]) - degrees(atan2f(x, -z)), vr.weaponangles[ROLL]);
                }
            }

            static bool finishReloadNextFrame = false;
            if (finishReloadNextFrame)
            {
                sendButtonActionSimple("-reload");
                finishReloadNextFrame = false;
            }

            if (pDominantTracking->Status & (VRAPI_TRACKING_STATUS_POSITION_TRACKED | VRAPI_TRACKING_STATUS_POSITION_VALID)) {
                canUseBackpack = false;
            }
            else if (!canUseBackpack && grabMeleeWeapon == 0) {
                int channel = (vr_control_scheme->integer >= 10) ? 0 : 1;
                    RTCWVR_Vibrate(40, channel,
                                    0.5); // vibrate to let user know they can switch

                canUseBackpack = true;
            }

            if ((pDominantTrackedRemoteNew->Buttons & ovrButton_GripTrigger) !=
                (pDominantTrackedRemoteOld->Buttons & ovrButton_GripTrigger)) {

                dominantGripPushed = (pDominantTrackedRemoteNew->Buttons &
                                      ovrButton_GripTrigger) != 0;

                if (grabMeleeWeapon == 0)
                {
                    if (pDominantTracking->Status & (VRAPI_TRACKING_STATUS_POSITION_TRACKED | VRAPI_TRACKING_STATUS_POSITION_VALID)) {

                        if (dominantGripPushed) {
                            dominantGripPushTime = GetTimeInMilliSeconds();
                        } else {
                            if ((GetTimeInMilliSeconds() - dominantGripPushTime) <
                                vr_reloadtimeoutms->integer) {
                                sendButtonActionSimple("+reload");
                                finishReloadNextFrame = true;
                            }
                        }
                    } else{
                        if (dominantGripPushed) {
                            //Initiate crowbar from backpack mode
                            sendButtonActionSimple("weapon0");
                            int channel = (vr_control_scheme->integer >= 10) ? 0 : 1;
                            RTCWVR_Vibrate(80, channel, 0.8); // vibrate to let user know they switched
                            grabMeleeWeapon = 1;
                        }
                    }
                } else if (grabMeleeWeapon == 1 && !dominantGripPushed) {
                    //Restores last used weapon
                    sendButtonActionSimple("lastinv");
                    grabMeleeWeapon = 0;
                }
            }
        }

        float controllerYawHeading = 0.0f;
        //off-hand stuff
        {
            vr.flashlightoffset[0] = pOffTracking->HeadPose.Pose.Position.x - vr.hmdPosition[0];
            vr.flashlightoffset[1] = pOffTracking->HeadPose.Pose.Position.y - vr.hmdPosition[1];
            vr.flashlightoffset[2] = pOffTracking->HeadPose.Pose.Position.z - vr.hmdPosition[2];

			vec2_t v;
			rotateAboutOrigin(-vr.flashlightoffset[0], vr.flashlightoffset[2], (cl.viewangles[YAW] - vr.hmdorientation[YAW]), v);
			vr.flashlightoffset[0] = v[0];
			vr.flashlightoffset[2] = v[1];

            vec3_t rotation = {0};
            QuatToYawPitchRoll(pOffTracking->HeadPose.Pose.Orientation, rotation, vr.flashlightangles);

            vr.flashlightangles[YAW] += (cl.viewangles[YAW] - vr.hmdorientation[YAW]);

			if (vr_walkdirection->value == 0) {
				controllerYawHeading = -cl.viewangles[YAW] + vr.flashlightangles[YAW];
			}
			else
			{
				controllerYawHeading = 0.0f;//-cl.viewangles[YAW];
			}
        }

        //Right-hand specific stuff
        {
            ALOGV("        Right-Controller-Position: %f, %f, %f",
                  pDominantTracking->HeadPose.Pose.Position.x,
				  pDominantTracking->HeadPose.Pose.Position.y,
				  pDominantTracking->HeadPose.Pose.Position.z);

            //This section corrects for the fact that the controller actually controls direction of movement, but we want to move relative to the direction the
            //player is facing for positional tracking
            vec2_t v;
            rotateAboutOrigin(-vr.positionDeltaThisFrame[0] * vr_positional_factor->value,
                              vr.positionDeltaThisFrame[2] * vr_positional_factor->value, - vr.hmdorientation[YAW], v);
            positional_movementSideways = v[0];
            positional_movementForward = v[1];

            ALOGV("        positional_movementSideways: %f, positional_movementForward: %f",
                  positional_movementSideways,
                  positional_movementForward);

            //Jump (B Button)
            handleTrackedControllerButton(pDominantTrackedRemoteNew,
                                          pDominantTrackedRemoteOld, domButton2, K_SPACE);


            //We need to record if we have started firing primary so that releasing trigger will stop firing, if user has pushed grip
            //in meantime, then it wouldn't stop the gun firing and it would get stuck
            static bool firingPrimary = false;

            if (!firingPrimary && dominantGripPushed && (GetTimeInMilliSeconds() - dominantGripPushTime) > vr_reloadtimeoutms->integer)
            {
                //Fire Secondary
                if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) !=
                    (pDominantTrackedRemoteOld->Buttons & ovrButton_Trigger)) {

                    sendButtonAction("+attack2", (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger));
                }
            }
            else
            {
                //Fire Primary
                if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) !=
                    (pDominantTrackedRemoteOld->Buttons & ovrButton_Trigger)) {

                    firingPrimary = (pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger);
                    sendButtonAction("+attack", firingPrimary);
                }
                // we need to release secondary fire if dominantGripPushed has been released before releasing trigger -> should fix the gun jamming and non stop firing secondary attack bug
                if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Trigger) !=
                    (pDominantTrackedRemoteOld->Buttons & ovrButton_Trigger) &&
                    (pDominantTrackedRemoteNew->Buttons& ovrButton_Trigger) == false)
                {
                    sendButtonAction("+attack2", false);
                }
            }

            //Duck with A
            if ((pDominantTrackedRemoteNew->Buttons & domButton1) !=
                (pDominantTrackedRemoteOld->Buttons & domButton1) &&
                ducked != DUCK_CROUCHED) {
                ducked = (pDominantTrackedRemoteNew->Buttons & domButton1) ? DUCK_BUTTON : DUCK_NOTDUCKED;
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
            ALOGV("        Left-Controller-Position: %f, %f, %f",
                  pOffTracking->HeadPose.Pose.Position.x,
				  pOffTracking->HeadPose.Pose.Position.y,
				  pOffTracking->HeadPose.Pose.Position.z);

            //"Use" (open doors etc)
            if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Joystick) !=
                (pDominantTrackedRemoteOld->Buttons & ovrButton_Joystick)) {

                sendButtonAction("+activate", (pDominantTrackedRemoteNew->Buttons & ovrButton_Joystick) ? 1 : 0);
            }

			//Laser-sight - not implemented
/*
			if ((pDominantTrackedRemoteNew->Buttons & ovrButton_Joystick) !=
				(pDominantTrackedRemoteOld->Buttons & ovrButton_Joystick)
				&& (pDominantTrackedRemoteNew->Buttons & ovrButton_Joystick)) {

			    if (vr_lasersight->value != 0.0)
                {
//                    Cvar_ForceSet("vr_lasersight", "0.0");
                } else {
//                    Cvar_ForceSet("vr_lasersight", "1.0");
			    }
			}
*/

			//Apply a filter and quadratic scaler so small movements are easier to make
			float dist = length(pOffTrackedRemoteNew->Joystick.x, pOffTrackedRemoteNew->Joystick.y);
			float nlf = nonLinearFilter(dist);
            float x = nlf * pOffTrackedRemoteNew->Joystick.x;
            float y = nlf * pOffTrackedRemoteNew->Joystick.y;

            player_moving = (fabs(x) + fabs(y)) > 0.05f;

			//Adjust to be off-hand controller oriented
            vec2_t v;
            rotateAboutOrigin(x, y, controllerYawHeading, v);

            remote_movementSideways = v[0];
            remote_movementForward = v[1];
            ALOGV("        remote_movementSideways: %f, remote_movementForward: %f",
                  remote_movementSideways,
                  remote_movementForward);

            //Kick!
            if (!canUseQuickSave) {
                if ((pOffTrackedRemoteNew->Buttons & offButton1) !=
                    (pOffTrackedRemoteOld->Buttons & offButton1)) {
                    sendButtonAction("+kick", (pOffTrackedRemoteNew->Buttons & offButton1));
                }
            }

            //notebook
            if (!canUseQuickSave) {
                if ((pOffTrackedRemoteNew->Buttons & offButton2) !=
                    (pOffTrackedRemoteOld->Buttons & offButton2)) {
                    sendButtonActionSimple("notebook");
                }
            }


            if (((pOffTrackedRemoteNew->Buttons & ovrButton_Joystick) !=
                (pOffTrackedRemoteOld->Buttons & ovrButton_Joystick))
                && (pOffTrackedRemoteNew->Buttons & ovrButton_Joystick)) {

                //If cheats enabled, give all weapons/pickups to player
                if (sv_cheats->value == 1.0f) {
                    Cbuf_AddText("give all\n");
                }
            }

            //We need to record if we have started firing primary so that releasing trigger will stop definitely firing, if user has pushed grip
            //in meantime, then it wouldn't stop the gun firing and it would get stuck
            static bool firingPrimary = false;

			//Run
			handleTrackedControllerButton(pOffTrackedRemoteNew,
										  pOffTrackedRemoteOld,
										  ovrButton_Trigger, K_SHIFT);

            static int increaseSnap = true;
			if (pDominantTrackedRemoteNew->Joystick.x > 0.6f)
			{
				if (increaseSnap)
				{
					snapTurn -= vr_snapturn_angle->value;
                    if (vr_snapturn_angle->value > 10.0f) {
                        increaseSnap = false;
                    }

                    if (snapTurn < -180.0f)
                    {
                        snapTurn += 360.f;
                    }

                    RTCWVR_ResyncClientYawWithGameYaw();
                }
			} else if (pDominantTrackedRemoteNew->Joystick.x < 0.4f) {
				increaseSnap = true;
			}

			static int decreaseSnap = true;
			if (pDominantTrackedRemoteNew->Joystick.x < -0.6f)
			{
				if (decreaseSnap)
				{
					snapTurn += vr_snapturn_angle->value;

					//If snap turn configured for less than 10 degrees
					if (vr_snapturn_angle->value > 10.0f) {
                        decreaseSnap = false;
                    }

                    if (snapTurn > 180.0f)
                    {
                        snapTurn -= 360.f;
                    }

                    RTCWVR_ResyncClientYawWithGameYaw();
				}
			} else if (pDominantTrackedRemoteNew->Joystick.x > -0.4f)
			{
				decreaseSnap = true;
			}
        }
    }

    //Save state
    rightTrackedRemoteState_old = rightTrackedRemoteState_new;
    leftTrackedRemoteState_old = leftTrackedRemoteState_new;
}