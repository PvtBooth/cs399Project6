/******************************************************************************/
/*!
\file   Sound.h
\author Ryan Booth
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes all of the function declarations that handle FMOD
        and editing sound.
*/
/******************************************************************************/

#pragma once

#include "TransformComponent.h"

#ifdef __cplusplus
extern "C" {
    /* Assume C declarations for C++ */
#endif

    /*------------------------------------------------------------------------------
    // Public Consts:
    //----------------------------------------------------------------------------*/
    typedef enum Sound_Effects {
        SOUND_BASIC_SHOOT,
        SOUND_DEEP_SHOOT,
        SOUND_SHOTGUN_SHOOT,
        SOUND_ENEMY_EXPLODE,
        SOUND_PLAYER_EXPLODE,
        SOUND_ABILITY_GRAV,
        SOUND_ABILITY_MISSLE,
        SOUND_ABILITY_BOOST,
        SOUND_ABILITY_BOMB,
        SOUND_MENU_SELECT,
        SOUND_JUMPGATE_ATTACH,

        SOUND_MAX
    }Sound_Effects;
    /*------------------------------------------------------------------------------
    // Public Structures:
    //----------------------------------------------------------------------------*/
    
    typedef struct FMOD_SOUND FMOD_SOUND;

    /*------------------------------------------------------------------------------
    // Public Variables:
    //----------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------
    // Public Functions:
    //----------------------------------------------------------------------------*/

    /*!
    \brief
    */
    void Sound_Init();

    /*!
    \param	dt Change in time (in seconds) since last game loop.
    */
    void Sound_Update(float dt);

    void Sound_Play(Sound_Effects effect);

    void Sound_SoundEffectsVolume(Sound_Effects effect, float volume);
    
    void Sound_SpeakerLocation(Vector2 entity_position);

    void Sound_Magnitude(Sound_Effects effect, float entityx_position);

    void Sound_PlayEffect(Sound_Effects type);

    void Sound_CreateSoundAsset(const char *file, FMOD_SOUND** soundEffect);

    /*!
    \brief
    */
    void Sound_Exit();

    /*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

