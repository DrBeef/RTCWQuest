RTCWQuest
==========

Welcome to the one and only fully wireless 6DoF implementation of Return to Castle Wolfenstein in VR.

This is built solely for the Oculus Quest VR HMD and will *not* run on any other device.

The easiest way to install this on your Quest is using SideQuest, a Desktop app designed to simplify sideloading apps and games ( even beat saber songs on quest ) on Standalone Android Headsets like Oculus Quest and Oculus Go. It supports drag and drop for installing APK files!

Download SideQuest here:
https://github.com/the-expanse/SideQuest/releases



IMPORTANT NOTE
--------------

This is just an engine port; though the apk does contain the DEMO version of RTCW, not the full game. If you wish to play the full game you must purchase it yourself, steam is most straightforward:  [STEAM LINK]


Copying the Full Game PAK files to your Oculus Quest
----------------------------------------------------

Copy the PAK files from the installed RTCW game folder on your PC to the /RTCWQuest/Main folder on your Oculus Quest when it is connected to the PC. You have to have run RTCWQuest at least once for the folder to be created and if you don't see it when you connect your Quest to the PC you might have to restart the Quest.



Caveats
-------

WARNING:  There is a good chance that unless you have your VR-legs this will probably make you feel a bit sick. The moment you start to feel under the weather YOU MUST STOP PLAYING for a good period of time before you try again. I will not be held responsible for anyone making themselves ill.


Controls
========

The controls are reversed for left handed players

* Open the in-game menu with the left-controller menu button (same irrespective of right/left handed control)
* A Button - Crouch
* B Button - Jump
* Y Button - Bring up the notebook
* X Button - Show / Hide the HUD
* Dominant-Hand Controller - Weapon orientation
* Dominant-Hand Thumbstick - left/right Snap or Smooth turn, up/down weapon change
* Dominant-Hand Thumbstick click - "activate" button (open doors, pull levers, pickup chairs etc)
* Dominant-Hand Trigger - Fire
* Dominant Grip Button - short click = reload, long press and trigger to switch weapon to alt-fire (more on this shortly)
* Off-Hand Controller - Direction of movement (or HMD direction if configured)
* Off-Hand Thumbstick - locomotion
* Off-Hand Trigger - Run / Teleport
* Off-Hand Grip Button - Two handed Weapon Stabilisation - will increase accuracy of weapons
* Off-Hand Thumbstick click - Kick

Alt Fire
--------

Press and hold the grip button then pull the trigger to select the alternative weapon mode.


*Weapon Specific Controls*

Sniper Scopes
-------------

The FG42 and the Mauser can have their scopes toggled on and off by using the weapon alt mode (note the scoped Mauser is only available once the scope has been picked up in the game). When the scope is attached, to trigger the scoped view, ensure you are holding the weapon in two hands (use off-hand grip button) and then look through the scope on the weapon, this will switch you to scope mode. Move the controllers like you would when looking through the scope to aim.

Knife
-----

There is no fire button, stab or swing to use it like a knife


Grenades / Pineapples / Dynamite
--------------------------------

These are all throwable. Hold the trigger and then throw the item, releasing the trigger at the apex of the throw.


Backpack
--------

There is a handy backpack feature which acts as a quick access to certain weapons/pickups. To access the backpack move your dominant controller out of sight over your right shoulder. The press and hold the following buttons to access the following items:

*Dominant Hand Controller*

* A (or X for left handed) - Knife (useful to quickly smash up stuff or stab)
* B (or Y for left handed) - Binoculars (once you have picked them up in game), raise them to your face to look through them, pull trigger to zoom in/out
* Grip button - Grenades / Dynamite - You can then throw them in the usual manner using the trigger

Release the button to return to the previously active weapon

*Off Hand Controller*

* X (or A for left handed) - Quick Save Game
* Y (or B for left handed) - Load Last Quick Save


Things to note / FAQs:
----------------------

* Mods and Texture packs do work

Credits:
--------

The game includes Hi Res textures, skins and items previously collected by the user Realistic. We could not find the original author of these weapons, nor does the pack include any licensing at all. If you are the author of these textures please feel free to contact us and we would be happy to credit you or remove.

I would like to thank the following teams and individual for making this possible:

* Emile Belanger - For once again providing the android port upon which this is based. See his other Android ports [here](http://www.beloko.com/)
* Baggyg - My long-time VR friend whose roles in this have been varied and all helpful, also the creator of excellent websites/artwork/assets for this mod as well as heavy  model alterations to be more VR friendly (removing hands, tweaking iron sights, alignment etc)
* VR_Bummser - PR person extraordinaire, play tester and all round helpful and good guy. Hoping I can buy him a beer in person one day!
* The [SideQuest](https://sidequestvr.com/#/news) team - For making it easy for people to install this
* GLE4ES for the OpenGL -> OpenGLES2 translation: without which this project wouldn't have worked at all: https://github.com/ptitSeb/gl4es


Building:
---------

You need the following:

* Android Developer Studio
* Latest Android Native Development Kit
* Oculus Mobile SDK 1.30.0
* The RTCWQuest folder cloned from GitHub should be below VrSamples in the extracted SDK
* Create a local.properties file in the root of the extracted Oculus Mobile SDK that contains the ndk.dir and sdk.dir properties for where your SDK/NDK are located (see Gradle documentation regarding this)
* To build debug you will need a _android.debug.keystore_ file placed in the following folder:
_oculus_sdk_dir_/VrSamples/RTCWQuest/Projects/Android
