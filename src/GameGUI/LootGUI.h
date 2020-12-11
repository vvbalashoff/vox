// ******************************************************************************
// Filename:    LootGUI.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 18/11/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../blocks/ChunkManager.h"
#include "../Player/Player.h"
#include "../Inventory/InventoryManager.h"
#include "../gui/draggablerenderrectangle.h"
#include "../gui/formattedlabel.h"
#include "../gui/guiwindow.h"
#include "../gui/openglgui.h"

class FrontendManager;
class InventoryGUI;
class CharacterGUI;
class LootGUI;
class ActionBar;
class Item;


class LootSlotItem
{
public:
	LootGUI* m_pLootGUI;
	InventoryItem* m_pInventoryItem;
	DraggableRenderRectangle* m_pInventoryIcon;	

	int m_slotX;
	int m_slotY;

	bool m_lootingAnimation;

	bool m_erase;
};

class LootGUI
{
public:
	/* Public methods */
	LootGUI(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, ChunkManager* pChunkManager, Player* pPlayer, InventoryManager* pInventoryManager, int windowWidth, int windowHeight);
	~LootGUI();

	void SetInventoryGUI(InventoryGUI* pInventoryGUI);
	void SetCharacterGUI(CharacterGUI* pCharacterGUI);
	void SetActionBar(ActionBar* pActionBar);

	void SetWindowDimensions(int windowWidth, int windowHeight);

	// Skinning the GUI
	void SkinGUI();
	void UnSkinGUI();

	// Loading
	void Load(bool loadDelay = false, float loadDelayTime = 0.0f);
	void Unload();
	bool IsLoadDelayed();
	bool IsLoaded();

	void GetLootDimensions(int indexX, int indexY, int* x, int* y, int* width, int *height);

	GUIWindow* GetLootWindow();

	LootSlotItem* GetLootSlotItem(int x, int y);

	void AddLootItemFromInventory(InventoryItem* pInventoryItem, int x, int y);
	void LoadItems(Item* pItem);
	void AddLootItem(InventoryItem* pInventoryItem, int x, int y);
	void AddLootItem(const char* filename, const char* iconFilename, InventoryType itemType, eItem item, EquipSlot equipSlot, ItemQuality itemQuality, const char* title, const char* description, bool left, bool right, float r, float g, float b, int quantity, int x, int y);
	void RemoveLootItem(LootSlotItem* plootItemSlot);

	void CreateLootItems();
	void DeleteLootItems();

	// Tooltips
	void UpdateToolTipAppear(float dt);
	void ShowTooltip(LootSlotItem* pInventoryItem);
	void HideTooltip();

	void Update(float dt);

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */
	static const int MAX_NUM_SLOTS_HORIZONTAL = 6;
	static const int MAX_NUM_SLOTS_VERTICAL = 3;

protected:
	/* Protected members */
	static void _LootItemPressed(void *apData);
	void LootItemPressed(LootSlotItem* pInventoryItem);

	static void _LootItemReleased(void *apData);
	void LootItemReleased(LootSlotItem* pInventoryItem);

	static void _LootItemEntered(void *apData);
	void LootItemEntered(LootSlotItem* pInventoryItem);

	static void _LootItemExited(void *apData);
	void LootItemExited(LootSlotItem* pInventoryItem);

	static void _CloseExitPressed(void *pData);
	void CloseExitPressed();

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
	ActionBar* m_pActionBar;

	int m_windowWidth;
	int m_windowHeight;

	bool m_loaded;

	int m_lootWindowWidth;
	int m_lootWindowHeight;
	int m_titlebarWidth;
	int m_titlebarHeight;

	Item* m_pStorageItem;

	// Load delay
	bool m_loadDelay;
	float m_loadDelayTime;

	// GUI components
	GUIWindow* m_pLootWindow;
	Icon* m_pTitleBarIcon;
	Icon* m_pTitleBarBackgroundIcon;
	Icon* m_pLootWindowBackgroundIcon;
	Button* m_pCloseExitButton;

	Icon* m_pLootButtonBackgroundSlotBorder_Common;
	Icon* m_pLootButtonBackgroundSlotBorder_Uncommon;
	Icon* m_pLootButtonBackgroundSlotBorder_Magical;
	Icon* m_pLootButtonBackgroundSlotBorder_Rare;
	Icon* m_pLootButtonBackgroundSlotBorder_Epic;

	// Pressed icons
	std::vector<LootSlotItem*> m_vpLootSlotItems;

	int m_pressedX;
	int m_pressedY;
	LootSlotItem* m_pPressedLootSlotItem;

	// Tooltip
	Icon* m_pTooltipBackground_Common;
	Icon* m_pTooltipBackground_Uncommon;
	Icon* m_pTooltipBackground_Magical;
	Icon* m_pTooltipBackground_Rare;
	Icon* m_pTooltipBackground_Epic;
	Label* m_pTooltipNameLabel;
	FormattedLabel* m_pTooltipDescriptionLabel;
	Label* m_pTooltipSlotLabel;
	Label* m_pTooltipQualityLabel;
	bool m_toolTipVisible;
	int m_tooltipWidth;
	int m_tooltipHeight;
	int m_tooltipDescBorder;
	ItemQuality m_tooltipQuality;
	float m_tooltipAppearDelayTimer;
	bool m_toolTipComponentsAdded;
};
