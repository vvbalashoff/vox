// ******************************************************************************
// Filename:    CraftingGUI.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 15/03/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../blocks/ChunkManager.h"
#include "../Player/Player.h"
#include "../Inventory/InventoryManager.h"
#include "../gui/draggablerenderrectangle.h"
#include "../gui/formattedlabel.h"
#include "../gui/textbox.h"
#include "../gui/progressbar.h"
#include "../gui/scrollbar.h"
#include "../gui/guiwindow.h"
#include "../gui/openglgui.h"

class FrontendManager;
class CraftingGUI;

class CraftingRecipe
{
public:
	~CraftingRecipe()
	{
		/* DONT DELETE the items, just clear the array, they will be deleted by the inventory manager
		for(unsigned int i = 0; i < m_vpCraftingItems.size(); i++)
		{
			delete m_vpCraftingItems[i];
			m_vpCraftingItems[i] = 0;
		}
		*/
		m_vpCraftingItems.clear();
	}

	std::vector<InventoryItem*> m_vpCraftingItems;
	InventoryItem* m_pResultItem;
};

class RecipeSlotItem
{
public:
	CraftingGUI* m_pCraftingGUI;
	InventoryItem* m_pInventoryItem;
	Button* m_pResultsIcon;
	string m_recipeName;
	CraftingRecipe* m_pCraftingReceipe;
	int m_slotIndex;

	bool m_erase;
};

class IngredientsSlotItem
{
public:
	CraftingGUI* m_pCraftingGUI;
	InventoryItem* m_pInventoryItem;
	Button* m_pIngredientIcon;
	int m_slotXIndex;
	int m_slotYIndex;

	bool m_erase;
};


class CraftingGUI
{
public:
	/* Public methods */
	CraftingGUI(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, ChunkManager* pChunkManager, Player* pPlayer, InventoryManager* pInventoryManager, int windowWidth, int windowHeight);
	~CraftingGUI();

	void SetWindowDimensions(int windowWidth, int windowHeight);

	// Skinning the GUI
	void SkinGUI();
	void UnSkinGUI();

	// Loading
	void Load(bool loadDelay = false, float loadDelayTime = 0.0f);
	void Unload();
	bool IsLoadDelayed();
	bool IsLoaded();

	void CraftingComplete();

	void SetInteractionitem(Item* pInteractionItem);

	// Recipes
	void SetCraftingRecipesForItem(eItem craftingItem);
	void AddCraftingRecipe(CraftingRecipe* pRecipe);
	void DeleteCraftingRecipes();
	void CreateRecipeButtons();
	void DeleteRecipeButtons();
	void CreateIngredientsButtons();
	void DeleteIngredientsButtons();
	bool CanCraftRecipe();

	// Tooltips
	void UpdateToolTipAppear(float dt);
	void ShowTooltip(RecipeSlotItem* pRecipeItem);
	void ShowTooltip(IngredientsSlotItem* pIngredientItem);
	void HideTooltip();

	// Updating
	void Update(float dt);
	void UpdateCraftButton();
	void UpdateFilteredRecipes();

	// Rendering
	void Render();

protected:
	/* Protected methods */
	static void _CloseExitPressed(void *pData);
	void CloseExitPressed();

	static void _RecipeItemEntered(void *apData);
	void RecipeItemEntered(RecipeSlotItem* pRecipeButtonData);

	static void _RecipeItemExited(void *apData);
	void RecipeItemExited(RecipeSlotItem* pRecipeButtonData);

	static void _IngredientItemEntered(void *apData);
	void IngredientItemEntered(IngredientsSlotItem* pIngredientButtonData);

	static void _IngredientItemExited(void *apData);
	void IngredientItemExited(IngredientsSlotItem* pIngredientButtonData);

	static void _CraftPressed(void *apData);
	void CraftPressed(bool fakeCraft = false);

	static void _ResultsItemPressed(void *apData);
	void ResultsItemPressed(RecipeSlotItem* pRecipeButtonData);

private:
	/* Private methods */

public:
	/* Public members */

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

	int m_windowWidth;
	int m_windowHeight;

	bool m_loaded;

	int m_craftingWindowWidth;
	int m_craftingWindowHeight;
	int m_titlebarWidth;
	int m_titlebarHeight;
	int m_craftingResultsScrollAreaX;
	int m_craftingResultsScrollAreaY;
	int m_craftingResultsScrollAreaWidth;
	int m_craftingResultsScrollAreaHeight;
	int m_craftingProgressX;
	int m_craftingProgressY;
	int m_craftingProgressWidth;
	int m_craftingProgressHeight;

	// Load delay
	bool m_loadDelay;
	float m_loadDelayTime;

	// GUI components
	GUIWindow* m_pCraftingWindow;
	Icon* m_pTitleBarIcon;
	Icon* m_pTitleBarBackgroundIcon;
	Icon* m_pCraftingWindowBackgroundIcon;
	Button* m_pCloseExitButton;
	ScrollBar* m_pResultsScrollbar;
	Icon* m_pCraftButton_Icon;
	Icon* m_pCraftButton_Icon_Hover;
	Icon* m_pCraftButton_Icon_Pressed;
	Icon* m_pCraftButton_Icon_Disabled;
	Button* m_pCraftButton;
	ProgressBar* m_pCraftingProgressBar;
	Icon* m_pCraftingProgressBarFiller;
	Icon* m_pCraftingProgressBarBackgroundFiller;
	TextBox* m_pSearchBox;
	Icon* m_pSearchTextboxIcon;
	Icon* m_pRecipeButton_Icon;
	Icon* m_pRecipeButton_Hover_Icon;
	Icon* m_pRecipeButton_Pressed_Icon;
	Icon* m_pRecipeButton_Selected_Icon;

	Icon* m_pInventoryBackgroundSlotBorder_Common;
	Icon* m_pInventoryBackgroundSlotBorder_Uncommon;
	Icon* m_pInventoryBackgroundSlotBorder_Magical;
	Icon* m_pInventoryBackgroundSlotBorder_Rare;
	Icon* m_pInventoryBackgroundSlotBorder_Epic;

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

	// Crafting params
	bool m_crafting;
	float m_craftingTime;
	float m_craftingTimer;

	string m_cachedSearchText;

	std::vector<RecipeSlotItem*> m_vpRecipeSlotItem;
	std::vector<RecipeSlotItem*> m_vpRecipeSlotItem_Filtered;
	std::vector<CraftingRecipe*> m_vpCraftingRecipes;
	std::vector<IngredientsSlotItem*> m_vpIngredientsSlotItem;

	// Selected recipe
	RecipeSlotItem* m_pRecipeSlotItemSelected;

	// Interaction item
	Item* m_pInteractionItem;
};
