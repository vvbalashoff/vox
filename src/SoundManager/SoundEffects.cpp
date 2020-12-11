// ******************************************************************************
// Filename:    SoundEffectsEnum.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 13/06/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

#include "SoundManager/SoundEffectsEnum.h"

string g_soundEffectFilenames[eSoundEffect_NUM] =
{
	"",							// eSoundEffect_None

	"footstep_01.wav",			// eSoundEffect_FootStep01
	"footstep_02.wav",			// eSoundEffect_FootStep02
	"footstep_03.wav",			// eSoundEffect_FootStep03
	"footstep_04.wav",			// eSoundEffect_FootStep04
	"land.wav",					// eSoundEffect_JumpLand

	"equip_cloth.wav",			// eSoundEffect_EquipCloth
	"equip_sword.wav",			// eSoundEffect_EquipSword
	"equip_move.wav",			// eSoundEffect_EquipMove

	"open_chest.wav",			// eSoundEffect_ChestOpen
};
