/******************************************************************************/
/*!
\file   Sound.c
\author Ryan Booth
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes all of the functions that handle FMOD and editing
        sound.
*/
/******************************************************************************/

#define UNREFERENCED_PARAMETER(P) (P)
#define true 1
#define false 0

#include "Sound.h"
#include "AEEngine.h"
#include <stdlib.h>
#include <stdio.h>
#include "fmod.h"
#include "fmod_common.h"
#include "fmod_errors.h"
#include "fmod_output.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static	FMOD_SYSTEM           *FmodSystem;
static	FMOD_SOUND            *sound[SOUND_MAX];
static	FMOD_CHANNEL          *channel = 0;
static	FMOD_CHANNELGROUP     *channel_sfx;
static	FMOD_CHANNELGROUP     *channel_test;
FMOD_RESULT       result;
unsigned int      version;
void             *extradriverdata = 0;
static float      VOLUME_MAX = 0.5f;


/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
void ERRCHECK(FMOD_RESULT result);
float GetCamXPosition(Vector2 position);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
\brief Intializes Sound Engine
*/
void Sound_Init()
{
    //Create the FMOD System
    result = FMOD_System_Create(&FmodSystem);
    ERRCHECK(result);
    //Get the appropriate drivers from FMOD
    result = FMOD_System_GetVersion(FmodSystem, &version);
    ERRCHECK(result);
    //Set the computers speakers to stereo output
    result = FMOD_System_SetSoftwareFormat(FmodSystem, 44100, FMOD_SPEAKERMODE_STEREO, 0);
    ERRCHECK(result);
    //Initialize the FMOD System
    result = FMOD_System_Init(FmodSystem, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);
    //Load in all the sounds for gameplay
    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Laser_Shoot15.wav", FMOD_DEFAULT, 0, &sound[SOUND_BASIC_SHOOT]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Shoot322.wav", FMOD_DEFAULT, 0, &sound[SOUND_DEEP_SHOOT]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/shiptype2/Ability2_OnBoost.wav", FMOD_DEFAULT, 0, &sound[SOUND_ABILITY_BOOST]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/AlphaExplosion.wav", FMOD_DEFAULT, 0, &sound[SOUND_ENEMY_EXPLODE]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Select.wav", FMOD_DEFAULT, 0, &sound[SOUND_MENU_SELECT]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/jumpgatetype1/OnPlayerConnect.wav", FMOD_DEFAULT, 0, &sound[SOUND_JUMPGATE_ATTACH]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Shotgun.wav", FMOD_DEFAULT, 0, &sound[SOUND_SHOTGUN_SHOOT]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Explosion.wav", FMOD_DEFAULT, 0, &sound[SOUND_PLAYER_EXPLODE]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Powerup4.wav", FMOD_DEFAULT, 0, &sound[SOUND_ABILITY_GRAV]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Missle.wav", FMOD_DEFAULT, 0, &sound[SOUND_ABILITY_MISSLE]);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(FmodSystem, "./fmod/assets/Bomb.wav", FMOD_DEFAULT, 0, &sound[SOUND_ABILITY_BOMB]);
    ERRCHECK(result);
    //Set up Channel Groups for volume, pitch, pan, etc
    result = FMOD_System_CreateChannelGroup(FmodSystem, NULL, &channel_sfx);
    ERRCHECK(result);

    result = FMOD_System_CreateChannelGroup(FmodSystem, NULL, &channel_test);
    ERRCHECK(result);
}


/*!
\brief Plays a sound *pew pew*
*/
void Sound_Play(Sound_Effects type)
{
   //Play a sound effect on the sfx channel
   result = FMOD_System_PlaySound(FmodSystem, sound[type], channel_sfx, false, &channel);
   ERRCHECK(result);
}

/*!
\brief Changes the volume for the Sound Effects group
*/
void Sound_SoundEffectsVolume(Sound_Effects effect ,float volume)
{
  UNREFERENCED_PARAMETER(effect);
  //Pause the channel to allow volume change
  FMOD_ChannelGroup_SetPaused(channel_sfx, true);
  //Set the requested volume
  FMOD_ChannelGroup_SetVolume(channel_sfx, volume);
  //Unpause the channel
  FMOD_ChannelGroup_SetPaused(channel_sfx, false);
}

/*!
\brief Changes the direction of the sound
*/
void Sound_SpeakerLocation(Vector2 entity_position)
{
   //Get the camera's current position
   float position = GetCamXPosition(entity_position);
   //Set the speaker pan based on the position
   result = FMOD_Channel_SetPan((FMOD_CHANNEL*)channel_sfx, -position);
   ERRCHECK(result);
}

/*!
\brief Changes the volume for a sound effect based on distance
*/
void Sound_Magnitude(Sound_Effects effect, float x_position)
{
    //Initialize variables
    float volume = 1;
    float range = 0.6f;
    float dampener = 0.4f;
    float position = x_position;
    //Flip the position to always be positive
    if (position < 0)
        position = (position) * (-1.0f);
    //Calculate the volume based on distance
    volume = volume - position + range;
    // Reduces volume to not blow up ears
    volume = volume * dampener; 
    //If volume would be so low, set it to 0 to avoid sound playing
    if (volume < 0)
        volume = 0;
    //Set the new sound
    Sound_SoundEffectsVolume(effect ,volume);
}

/*!
\brief Plays a sound effect with direction and magnitude
*/
void Sound_PlayEffect(Sound_Effects type)
{
    //Depricated code
    //float position = GetCamXPosition(entity_position);

    //result = FMOD_Channel_SetPan((FMOD_CHANNEL*)channel_sfx, -position);
    //ERRCHECK(result);

    //Sound_Magnitude(type,position);

    //Set the volume
    Sound_SoundEffectsVolume(type, VOLUME_MAX);
    //Play the sound
    Sound_Play(type);
}


/*!
 \brief Updates the Sound Engine to keep it happy
 */
void Sound_Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
    //Update the FMOD system to ensure it runs consistently
    FMOD_System_Update(FmodSystem);
}

/*!
 * \brief
 *   Creates a sound asset through the FMOD API.
 * \param file
 *   The name of the file to load.
 * \param soundEffect
 *   [Out] Where the sound effect will be loaded to.
 * \return value
 */
void Sound_CreateSoundAsset(const char *file, FMOD_SOUND** soundEffect)
{
  result = FMOD_System_CreateSound(FmodSystem, file, FMOD_DEFAULT, 0, soundEffect);
  ERRCHECK(result);
}

/*!
\brief Safely frees all memory associated with sound
*/
void Sound_Exit()
{
    int i;
    //Loop through the sound array and free all the data allocated upon intialization
    for (i = 0; i < SOUND_MAX; i++)
    {
        result = FMOD_Sound_Release(sound[i]);
        ERRCHECK(result);
    }
    //Close the FMOD system
    result = FMOD_System_Close(FmodSystem);
    ERRCHECK(result);
    //Releases the system allocation
    result = FMOD_System_Release(FmodSystem);
    ERRCHECK(result);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
/*!
\brief Error checks the return value from FMOD function calls and prints them to the screen
*/
void ERRCHECK(FMOD_RESULT status)
{
    //If a return from FMOD wasn't okay, print it to the console
    if (status != FMOD_OK)
    {
        printf("FMOD error: %s \n", FMOD_ErrorString(status));
    }
}
/*!
\brief Gets the camera's current x-position for panning and distance calculations
*/
float GetCamXPosition(Vector2 position)
{
    Vector2 cameraPosition; //Camera position vector

    //Get the camera's position
    cameraPosition = Camera_GetPosition();
    //Calculate the object's distance from the camera
    float pos = (cameraPosition.x - position.x) / 1000.0f;

    return pos;
}