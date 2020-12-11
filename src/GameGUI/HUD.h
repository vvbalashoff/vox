// ******************************************************************************
// Filename:    HUD.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   The HUD is the on-screen display of components and interface objects that
//   display certain information for the player. For example the character's
//   health and mana, or the current enemy name and health points.
//   Also functionality for finite life HUD elements, such as level up text, or
//   'death' text and respawn button.
//
// Revision History:
//   Initial Revision - 03/03/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../blocks/ChunkManager.h"
#include "../Player/Player.h"
#include "../Inventory/InventoryManager.h"
#include "../gui/openglgui.h"

class FrontendManager;
class InventoryGUI;
class CharacterGUI;
class QuestGUI;
class CraftingGUI;

class HUD
{
public:
	/* Public methods */
	HUD(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, ChunkManager* pChunkManager, Player* pPlayer, InventoryManager* pInventoryManager, int windowWidth, int windowHeight);
	~HUD();

	void SetInventoryGUI(InventoryGUI* pInventoryGUI);
	void SetCharacterGUI(CharacterGUI* pCharacterGUI);
	void SetQuestGUI(QuestGUI* pQuestGUI);
	void SetCraftingGUI(CraftingGUI* pCraftingGUI);

	void SetWindowDimensions(int windowWidth, int windowHeight);

	// Skinning the GUI
	void SkinGUI();
	void UnSkinGUI();

	// Loading
	void Load();
	void Unload();
	bool IsLoaded();

	// Player death
	void PlayerDied();
	void PlayerRespawn();

	// Level up
	void LevelUp();

	// Enemy health
	void SetEnemyHealth(float health, float healthMax, float damagedone);
	void SetEnemyName(const char* name);

	// Updating
	void Update(float dt);
	void UpdatePlayerData();

	// Rendering
	void Render();
	void RenderPlayerHealth();
	void RenderPlayerMagic();
	void RenderPlayerExperience();
	void RenderEnemyHealth();

protected:
	/* Protected methods */
	static void _DeathTextFinished(void *apData);
	void DeathTextFinished();

	static void _RespawnPressed(void *apData);
	void RespawnPressed();

	static void _LevelUpTextFinished(void *apData);
	void LevelUpTextFinished();

	// TODO : Checkboxes for GUI panels on the HUD
	//static void _InventoryCheckboxPressed(void *apData);
	//void InventoryCheckboxPressed();

	//static void _CharacterCheckboxPressed(void *apData);
	//void CharacterCheckboxPressed();

	//static void _QuestCheckboxPressed(void *apData);
	//void QuestCheckboxPressed();

	//static void _CraftingCheckboxPressed(void *apData);
	//void CraftingCheckboxPressed();

	//static void _SkillsCheckboxPressed(void *apData);
	//void SkillsCheckboxPressed();

	//static void _PetCheckboxPressed(void *apData);
	//void PetCheckboxPressed();

private:
	/* Private methods */

public:
	/* Public members */
	static const int MAX_ENEMY_NAME_SIZE = 128;

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;
	OpenGLGUI* m_pGUI;
	FrontendManager* m_pFrontendManager;
	ChunkManager* m_pChunkManager;
	Player* m_pPlayer;
	InventoryManager* m_pInventoryManager;
	InventoryGUI* m_pInventoryGUI;
	CharacterGUI* m_pCharacterGUI;
	QuestGUI* m_pQuestGUI;
	CraftingGUI* m_pCraftingGUI;

	int m_windowWidth;
	int m_windowHeight;

	bool m_loaded;

	// Player death
	bool m_playerDied;
	float m_deathHeaderAlpha;
	Label* m_pDeathHeaderLabel;
	int m_respawnButtonWidth;
	int m_respawnButtonHeight;
	Button* m_pRespawnButton;

	// Level up
	bool m_levelUp;
	float m_levelUpAlpha;
	Label* m_pLevelUpLabel;

	// Player portrait
	Icon* m_pPortraitBackgroundIcon;

	// Player health
	float m_playerHealthX;
	float m_playerHealthY;
	float m_playerHealthWidth;
	float m_playerHealthHeight;
	float m_playerHealthTitleOffset;
	Label* m_pHealthLabel;
	bool m_updatePlayerHealthTextLocation;
	Icon* m_pHealthContainerIcon;
	Icon* m_pHealthFillerIcon;
	Icon* m_pHealthFillerBackgroundIcon;

	// Player magic
	float m_playerMagicX;
	float m_playerMagicY;
	float m_playerMagicWidth;
	float m_playerMagicHeight;
	float m_playerMagicTitleOffset;
	Label* m_pMagicLabel;
	bool m_updatePlayerMagicTextLocation;
	Icon* m_pManaFillerIcon;
	Icon* m_pManaFillerBackgroundIcon;

	// Player experience
	float m_playerExperienceX;
	float m_playerExperienceY;
	float m_playerExperienceWidth;
	float m_playerExperienceHeight;
	float m_playerExperienceTitleOffset;
	Label* m_pExperienceLabel;
	bool m_updatePlayerExperienceTextLocation;
	Icon* m_pExperienceContainerIcon;
	Icon* m_pExperienceFillerIcon;
	Icon* m_pExperienceFillerBackgroundIcon;

	// Enemy health
	float m_enemyHealthX;
	float m_enemyHealthY;
	float m_enemyHealthWidth;
	float m_enemyHealthHeight;
	float m_enemyHealthWidthDecreaser;
	Label* m_pEnemyHealthLabel;
	Label* m_pEnemyNameLabel;
	float m_enemyHealth;
	float m_enemyHealthMax;
	float m_enemyInfoDisappearTime;
	float m_enemyInfoDisappearTimer;
	bool m_removedEnemyInfo;
	bool m_updateEnemyTextLocation;
	Icon* m_pEnemyHealthContainerIcon;
	Icon* m_pEnemyHealthFillerIcon;
	Icon* m_pEnemyHealthFillerBackgroundIcon;

	// Portrait
	Icon* m_pDynamicPortraitIcon;

	// Checkbox components for opening/closing GUI panels
	// TODO : Checkboxes for GUI panels on the HUD
	//CheckBox* m_pInventoryCheckBox;
	//CheckBox* m_pCharacterCheckBox;
	//CheckBox* m_pQuestCheckBox;
	//CheckBox* m_pCraftingCheckBox;
	//CheckBox* m_pSkillsCheckBox;
	//CheckBox* m_pPetCheckBox;

	// Checkbox icons
	//Icon* m_pInventoryCheckboxIcon;
	//Icon* m_pInventoryCheckboxIcon_Hover;
	//Icon* m_pInventoryCheckboxIcon_Pressed;
	//Icon* m_pInventoryCheckboxIcon_Toggled;
	//Icon* m_pInventoryCheckboxIcon_Toggled_Hover;
	//Icon* m_pInventoryCheckboxIcon_Toggled_Pressed;
	//Icon* m_pCharacterCheckboxIcon;
	//Icon* m_pCharacterCheckboxIcon_Hover;
	//Icon* m_pCharacterCheckboxIcon_Pressed;
	//Icon* m_pCharacterCheckboxIcon_Toggled;
	//Icon* m_pCharacterCheckboxIcon_Toggled_Hover;
	//Icon* m_pCharacterCheckboxIcon_Toggled_Pressed;
	//Icon* m_pQuestCheckboxIcon;
	//Icon* m_pQuestCheckboxIcon_Hover;
	//Icon* m_pQuestCheckboxIcon_Pressed;
	//Icon* m_pQuestCheckboxIcon_Toggled;
	//Icon* m_pQuestCheckboxIcon_Toggled_Hover;
	//Icon* m_pQuestCheckboxIcon_Toggled_Pressed;
	//Icon* m_pCraftingCheckboxIcon;
	//Icon* m_pCraftingCheckboxIcon_Hover;
	//Icon* m_pCraftingCheckboxIcon_Pressed;
	//Icon* m_pCraftingCheckboxIcon_Toggled;
	//Icon* m_pCraftingCheckboxIcon_Toggled_Hover;
	//Icon* m_pCraftingCheckboxIcon_Toggled_Pressed;
};
