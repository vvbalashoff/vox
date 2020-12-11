// ******************************************************************************
// Filename:    FrontendManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   A manager class to contain all the common functionality for front-end pages
//   and GUI graphics.
//
// Revision History:
//   Initial Revision - 01/11/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

#include "FrontendPage.h"
#include "FrontendScreens.h"
#include "../NPC/NPCManager.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/camera.h"
#include "../gui/icon.h"
#include "../gui/checkbox.h"
#include "../gui/optionbox.h"
#include "../gui/scrollbar.h"
#include "../gui/slider.h"
#include "../gui/pulldownmenu.h"
#include "../gui/button.h"


enum ButtonSize
{
	ButtonSize_85x25 = 0,
	ButtonSize_65x25,
	ButtonSize_110x47,
	ButtonSize_225x75,

	ButtonSize_NUM,
};

typedef std::vector<FrontendPage*> FrontendPageList;

class FrontendManager
{
public:
	/* Public methods */
	FrontendManager(Renderer* pRenderer, OpenGLGUI* pGUI);
	~FrontendManager();

	// Windows dimensions
	void SetWindowDimensions(int width, int height);
	int GetWindowWidth();
	int GetWindowHeight();

	// Camera
	void SetCamera(Camera* pCamera);

	// Skinning the GUI
	void SkinGUI();
	void UnSkinGUI();

	// Frontend screen
	eFrontendScreen GetFrontendScreen();
	void SetFrontendScreen(eFrontendScreen screen);

	// Load the icon graphics based on a theme
	void LoadCommonGraphics(string themeName);

	// Setup icons for components
	void SetCheckboxIcons(CheckBox* pCheckBox);
	void SetOptionboxIcons(OptionBox* pOptionBox);
	void SetScrollbarIcons(ScrollBar* pScrollbar);
	void SetSliderIcons(Slider* pSlider);
	void SetPulldownMenuIcons(PulldownMenu* pPulldownMenu);
	void SetButtonIcons(Button* pButton, ButtonSize size);
	void SetTabIcons(OptionBox* pTab);

	// Fonts
	unsigned int GetFrontendFont_Large() const { return m_frontendFont_Large; }
	unsigned int GetFrontendFont_Medium() const { return m_frontendFont_Medium; }
	unsigned int GetFrontendFont_Small() const { return m_frontendFont_Small; }
	unsigned int GetFrontendFont_14() const { return m_frontendFont_14; }
	unsigned int GetFrontendFont_14_Outline() const { return m_frontendFont_14_Outline; }
	unsigned int GetFrontendFont_18() const { return m_frontendFont_18; }
	unsigned int GetFrontendFont_18_Outline() const { return m_frontendFont_18_Outline; }
	unsigned int GetFrontendFont_20() const { return m_frontendFont_20; }
	unsigned int GetFrontendFont_20_Outline() const { return m_frontendFont_20_Outline; }
	unsigned int GetFrontendFont_22() const { return m_frontendFont_22; }
	unsigned int GetFrontendFont_22_Outline() const { return m_frontendFont_22_Outline; }
	unsigned int GetFrontendFont_25() const { return m_frontendFont_25; }
	unsigned int GetFrontendFont_25_Outline() const { return m_frontendFont_25_Outline; }
	unsigned int GetFrontendFont_30() const { return m_frontendFont_30; }
	unsigned int GetFrontendFont_30_Outline() const { return m_frontendFont_30_Outline; }
	unsigned int GetFrontendFont_35() const { return m_frontendFont_35; }
	unsigned int GetFrontendFont_35_Outline() const { return m_frontendFont_35_Outline; }
	unsigned int GetFrontendFont_40() const { return m_frontendFont_40; }
	unsigned int GetFrontendFont_40_Outline() const { return m_frontendFont_40_Outline; }
	unsigned int GetFrontendFont_50() const { return m_frontendFont_50; }
	unsigned int GetFrontendFont_50_Outline() const { return m_frontendFont_50_Outline; }
	unsigned int GetFrontendFont_80() const { return m_frontendFont_80; }
	unsigned int GetFrontendFont_80_Outline() const { return m_frontendFont_80_Outline; }

	// Text effects fonts
	unsigned int GetTextEffectFont() const { return m_textEffectFontNormal; }
	unsigned int GetTextEffectOutlineFont() const { return m_textEffectFontOutline; }

	// Label colours
	const Colour& GetNormalFontColour() const { return m_normalColour; }
	const Colour& GetHoverFontColour() const { return m_hoverColour; }
	const Colour& GetPressedFontColour() const { return m_pressedColour; }
	const Colour& GetDisabledFontColour() const { return m_disabledColour; }

	// Checkbox
	Icon* GetCheckboxIcon() const { return m_pCheckboxIcon; };
	Icon* GetCheckboxIconHover() const { return m_pCheckboxIconHover; };
	Icon* GetCheckboxIconPressed() const { return m_pCheckboxIconPressed; };
	Icon* GetCheckboxIconDisabled() const { return m_pCheckboxIconDisabled; };
	Icon* GetCheckboxIconToggled() const { return m_pCheckboxIconToggled; };
	Icon* GetCheckboxIconToggledHover() const { return m_pCheckboxIconToggledHover; };
	Icon* GetCheckboxIconToggledPressed() const { return m_pCheckboxIconToggledPressed; };
	Icon* GetCheckboxIconToggledDisabled() const { return m_pCheckboxIconToggledDisabled; };

	// Optionbox
	Icon* GetOptionboxIcon() const { return m_pOptionboxIcon; };
	Icon* GetOptionboxIconHover() const { return m_pOptionboxIconHover; };
	Icon* GetOptionboxIconPressed() const { return m_pOptionboxIconPressed; };
	Icon* GetOptionboxIconDisabled() const { return m_pOptionboxIconDisabled; };
	Icon* GetOptionboxIconToggled() const { return m_pOptionboxIconToggled; };
	Icon* GetOptionboxIconToggledHover() const { return m_pOptionboxIconToggledHover; };
	Icon* GetOptionboxIconToggledPressed() const { return m_pOptionboxIconToggledPressed; };
	Icon* GetOptionboxIconToggledDisabled() const { return m_pOptionboxIconToggledDisabled; };

	// Scrollbar
	Icon* GetScrollbarArrowDownIcon() const { return m_pScrollbarArrowDownIcon; };
	Icon* GetScrollbarArrowDownHoverIcon() const { return m_pScrollbarArrowDownIconHover; };
	Icon* GetScrollbarArrowDownPressedIcon() const { return m_pScrollbarArrowDownIconPressed; };
	Icon* GetScrollbarArrowDownDisabledIcon() const { return m_pScrollbarArrowDownIconDisabled; };
	Icon* GetScrollbarArrowUpIcon() const { return m_pScrollbarArrowUpIcon; };
	Icon* GetScrollbarArrowUpHoverIcon() const { return m_pScrollbarArrowUpIconHover; };
	Icon* GetScrollbarArrowUpPressedIcon() const { return m_pScrollbarArrowUpIconPressed; };
	Icon* GetScrollbarArrowUpDisabledIcon() const { return m_pScrollbarArrowUpIconDisabled; };
	Icon* GetScrollbarBackgroundIcon() const { return m_pScrollbarBackgroundIcon; };
	Icon* GetScrollbarBackgroundIconDisabled() const { return m_pScrollbarBackgroundIconDisabled; };
	Icon* GetScrollbarIcon() const { return m_pScrollbarIcon; };
	Icon* GetScrollbarHoverIcon() const { return m_pScrollbarIconHover; };
	Icon* GetScrollbarPressedIcon() const { return m_pScrollbarIconPressed; };
	Icon* GetScrollbarDisabledIcon() const { return m_pScrollbarIconDisabled; };

	// Slider
	Icon* GetSliderIconDefault() const { return m_pSliderIconDefault; };
	Icon* GetSliderIconHover() const { return m_pSliderIconHover; };
	Icon* GetSliderIconDrag() const { return m_pSliderIconDrag; };
	Icon* GetSliderIconDefaultBlank() const { return m_pSliderIconDefaultBlank; };
	Icon* GetSliderIconHoverBlank() const { return m_pSliderIconHoverBlank; };
	Icon* GetSliderIconDragBlank() const { return m_pSliderIconDragBlank; };
	Icon* GetSliderIconIncrementButtonDefault() const { return m_pSliderIncrementIcon; };
	Icon* GetSliderIconIncrementButtonHover() const { return m_pSliderIncrementIconHover; };
	Icon* GetSliderIconIncrementButtonPressed() const { return m_pSliderIncrementIconPressed; };
	Icon* GetSliderIconIncrementButtonDisabled() const { return m_pSliderIncrementIconDisabled; };
	Icon* GetSliderIconDecrementButtonDefault() const { return m_pSliderDecrementIcon; };
	Icon* GetSliderIconDecrementButtonHover() const { return m_pSliderDecrementIconHover; };
	Icon* GetSliderIconDecrementButtonPressed() const { return m_pSliderDecrementIconPressed; };
	Icon* GetSliderIconDecrementButtonDisabled() const { return m_pSliderDecrementIconDisabled; };

	// Button
	Icon* GetButtonIcon(ButtonSize size) const { return m_pButtonIcon[size]; };
	Icon* GetButtonIconHover(ButtonSize size) const { return m_pButtonIconHover[size]; };
	Icon* GetButtonIconPressed(ButtonSize size) const { return m_pButtonIconPressed[size]; };
	Icon* GetButtonIconDisabled(ButtonSize size) const { return m_pButtonIconDisabled[size]; };

	// Close button
	Icon* GetCloseExitButtonIcon() const { return m_pCloseExitButtonIcon; }
	Icon* GetCloseExitButtonIcon_Hover() const { return m_pCloseExitButtonIcon_Hover; }
	Icon* GetCloseExitButtonIcon_Pressed() const { return m_pCloseExitButtonIcon_Pressed; }

	// Rotate buttons
	Icon* GetRightRotateArrow() const { return m_pArrowRight_Icon; }
	Icon* GetRightRotateArrow_Hover() const { return m_pArrowRight_Icon_Hover; }
	Icon* GetRightRotateArrow_Pressed() const { return m_pArrowRight_Icon_Pressed; }
	Icon* GetLeftRotateArrow() const { return m_pArrowLeft_Icon; }
	Icon* GetLeftRotateArrow_Hover() const { return m_pArrowLeft_Icon_Hover; }
	Icon* GetLeftRotateArrow_Pressed() const { return m_pArrowLeft_Icon_Pressed; }

	// Tab Options
	Icon* GetTab75OptionIcon() const { return m_pTab75OptionIcon; }
	Icon* GetTab75OptionIcon_Hover() const { return m_pTab75OptionIcon_Hover; }
	Icon* GetTab75OptionIcon_Pressed() const { return m_pTab75OptionIcon_Pressed; }
	Icon* GetTab75OptionIcon_Toggled() const { return m_pTab75OptionIcon_Toggled; }
	Icon* GetTab75OptionIcon_Toggled_Hover() const { return m_pTab75OptionIcon_Toggled_Hover; }
	Icon* GetTab75OptionIcon_Toggled_Pressed() const { return m_pTab75OptionIcon_Toggled_Pressed; }

	// Common, shared frontend page params
	float GetCameraOrbitTimer() const;
	void SetCameraOrbitTimer(float orbit);

	// Frontend functionality
	void SetOptionsReturnToMainMenu(bool mainMenu);
	void SetModsMenuReturnToMainMenu(bool mainMenu);
	void SetHoverNPC(NPC* pHoverNPC);
	void SetSelectedNPC(NPC* pSelectedNPC);
	void SetCharacterSubSelection(string subSelection);
	void GotoNextCreditScreen() const;

	// Constants
	float GetToolTipAppearDelay() const;

	// Updating
	void Update(float dt);
	void UpdateFrontEndCamera(float dt);

	// Rendering
	void Render();
	void Render2D();
	void RenderDebug();

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
	Renderer* m_pRenderer;
	OpenGLGUI* m_pGUI;

	// Window params
	int m_windowWidth;
	int m_windowHeight;

	// Camera
	Camera* m_pCamera;

	// Current frontend screen
	eFrontendScreen m_currentScreen;
	FrontendPage* m_currentPage;

	// Pages
	FrontendPageList m_vpFrontendPages;

	// Fonts
	unsigned int m_frontendFont_Large;
	unsigned int m_frontendFont_Medium;
	unsigned int m_frontendFont_Small;
	unsigned int m_frontendFont_14;
	unsigned int m_frontendFont_14_Outline;
	unsigned int m_frontendFont_18;
	unsigned int m_frontendFont_18_Outline;
	unsigned int m_frontendFont_20;
	unsigned int m_frontendFont_20_Outline;
	unsigned int m_frontendFont_22;
	unsigned int m_frontendFont_22_Outline;
	unsigned int m_frontendFont_25;
	unsigned int m_frontendFont_25_Outline;
	unsigned int m_frontendFont_30;
	unsigned int m_frontendFont_30_Outline;
	unsigned int m_frontendFont_35;
	unsigned int m_frontendFont_35_Outline;
	unsigned int m_frontendFont_40;
	unsigned int m_frontendFont_40_Outline;
	unsigned int m_frontendFont_50;
	unsigned int m_frontendFont_50_Outline;
	unsigned int m_frontendFont_80;
	unsigned int m_frontendFont_80_Outline;

	// Text effects fonts
	unsigned int m_textEffectFontNormal;
	unsigned int m_textEffectFontOutline;

	// Label colours
	Colour m_normalColour;
	Colour m_hoverColour;
	Colour m_pressedColour;
	Colour m_disabledColour;

	// Checkbox
	Icon* m_pCheckboxIcon;
	Icon* m_pCheckboxIconHover;
	Icon* m_pCheckboxIconPressed;
	Icon* m_pCheckboxIconDisabled;
	Icon* m_pCheckboxIconToggled;
	Icon* m_pCheckboxIconToggledHover;
	Icon* m_pCheckboxIconToggledPressed;
	Icon* m_pCheckboxIconToggledDisabled;

	// Optionbox
	Icon* m_pOptionboxIcon;
	Icon* m_pOptionboxIconHover;
	Icon* m_pOptionboxIconPressed;
	Icon* m_pOptionboxIconDisabled;
	Icon* m_pOptionboxIconToggled;
	Icon* m_pOptionboxIconToggledHover;
	Icon* m_pOptionboxIconToggledPressed;
	Icon* m_pOptionboxIconToggledDisabled;

	// Scrollbar
	Icon* m_pScrollbarArrowDownIcon;
	Icon* m_pScrollbarArrowDownIconHover;
	Icon* m_pScrollbarArrowDownIconPressed;
	Icon* m_pScrollbarArrowDownIconDisabled;
	Icon* m_pScrollbarArrowUpIcon;
	Icon* m_pScrollbarArrowUpIconHover;
	Icon* m_pScrollbarArrowUpIconPressed;
	Icon* m_pScrollbarArrowUpIconDisabled;
	Icon* m_pScrollbarBackgroundIcon;
	Icon* m_pScrollbarBackgroundIconDisabled;
	Icon* m_pScrollbarIcon;
	Icon* m_pScrollbarIconHover;
	Icon* m_pScrollbarIconPressed;
	Icon* m_pScrollbarIconDisabled;

	// Slider
	Icon* m_pSliderIconDefault;
	Icon* m_pSliderIconHover;
	Icon* m_pSliderIconDrag;
	Icon* m_pSliderIconDefaultBlank;
	Icon* m_pSliderIconHoverBlank;
	Icon* m_pSliderIconDragBlank;
	Icon* m_pSliderIncrementIcon;
	Icon* m_pSliderIncrementIconHover;
	Icon* m_pSliderIncrementIconPressed;
	Icon* m_pSliderIncrementIconDisabled;
	Icon* m_pSliderDecrementIcon;
	Icon* m_pSliderDecrementIconHover;
	Icon* m_pSliderDecrementIconPressed;
	Icon* m_pSliderDecrementIconDisabled;

	// Buttons
	Icon** m_pButtonIcon;
	Icon** m_pButtonIconHover;
	Icon** m_pButtonIconPressed;
	Icon** m_pButtonIconDisabled;

	// Close button
	Icon* m_pCloseExitButtonIcon;
	Icon* m_pCloseExitButtonIcon_Hover;
	Icon* m_pCloseExitButtonIcon_Pressed;

	// Rotate buttons
	Icon* m_pArrowLeft_Icon;
	Icon* m_pArrowLeft_Icon_Hover;
	Icon* m_pArrowLeft_Icon_Pressed;
	Icon* m_pArrowRight_Icon;
	Icon* m_pArrowRight_Icon_Hover;
	Icon* m_pArrowRight_Icon_Pressed;

	// Tab Options
	Icon* m_pTab75OptionIcon;
	Icon* m_pTab75OptionIcon_Hover;
	Icon* m_pTab75OptionIcon_Pressed;
	Icon* m_pTab75OptionIcon_Toggled;
	Icon* m_pTab75OptionIcon_Toggled_Hover;
	Icon* m_pTab75OptionIcon_Toggled_Pressed;

	// Common, shared frontend page params
	float m_cameraOrbitTimer;

	// Constants
	float m_tooltipAppearDelay;
};
