// ******************************************************************************
// Filename:    ModsManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/03/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include <vector>
#include <string>
using namespace std;


class Mod
{
public:
	string m_modName;
	bool m_erase;
	bool m_gameplayMod;
	bool m_graphicsMod;
	bool m_soundMod;
	bool m_HUDMod;
	bool m_miscMod;
};

class ModsManager
{
public:
	/* Public methods */
	ModsManager();
	~ModsManager();

	// Clearing
	void ClearMods();

	// Loading
	void LoadMods();

	// Saving
	void SaveMods();

	// Adding and removing active mods
	void AddMod(string modName, bool gameplayMod, bool graphicsMod,	bool soundMod, bool HUDMod, bool miscMod);
	void RemoveMod(string modName, bool gameplayMod, bool graphicsMod, bool soundMod, bool HUDMod, bool miscMod);

	// Accessors
	int GetNumMods();
	Mod* GetMod(int index);

	// Graphics pack
	string GetGraphicsPack();

	// Sound pack
	string GetSoundPack();

	// HUD Theme
	string GetHUDTextureTheme();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	vector<Mod*> m_vpMods;
};
