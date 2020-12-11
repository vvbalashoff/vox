// ******************************************************************************
// Filename:    CreateCharacter.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 12/10/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif //_WIN32

#include <iostream>
#include <iomanip>

#include "CreateCharacter.h"
#include "../FrontendManager.h"
#include "../utils/FileUtils.h"
#include "../VoxGame.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;


CreateCharacter::CreateCharacter(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, int windowWidth, int windowHeight)
  : FrontendPage(pRenderer, pGUI, pFrontendManager, FrontendScreen_CreateCharacter, windowWidth, windowHeight)
{
	m_pSelectedNPC = NULL;

	m_toolTipVisible = false;

	m_pTooltipBackground = new Icon(m_pRenderer, "", 200, 220);
	m_pTooltipBackground->SetDepth(5.5f);

	char nameText[] = "[NAME]";
	m_pTooltipNameLabel = new Label(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), nameText, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTooltipNameLabel->SetOutline(true);
	m_pTooltipNameLabel->SetOutlineColour(Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pTooltipNameLabel->SetOutlineFont(m_pFrontendManager->GetFrontendFont_30_Outline());
	m_pTooltipNameLabel->SetDepth(5.5f);

	char descText[] = "[DESCRIPTION]";
	m_pTooltipDescriptionLabel = new FormattedLabel(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25(), descText);
	m_pTooltipDescriptionLabel->SetOutline(true);
	m_pTooltipDescriptionLabel->SetColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTooltipDescriptionLabel->SetOutlineColour(Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pTooltipDescriptionLabel->SetOutlineFont(m_pFrontendManager->GetFrontendFont_25_Outline());
	m_pTooltipDescriptionLabel->SetDepth(5.5f);
	m_pTooltipDescriptionLabel->SetWordWrap(true);

	m_pBackButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "Back", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pBackButton->SetLabelOffset(0, 3);
	m_pBackButton->SetPressedOffset(0, -4);
	m_pBackButton->SetCallBackFunction(_BackPressed);
	m_pBackButton->SetCallBackData(this);

	m_pNextButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_50(), m_pFrontendManager->GetFrontendFont_50_Outline(), "Next", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pNextButton->SetLabelOffset(0, 5);
	m_pNextButton->SetPressedOffset(0, -4);
	m_pNextButton->SetCallBackFunction(_SelectPressed);
	m_pNextButton->SetCallBackData(this);

	m_pCreateButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_50(), m_pFrontendManager->GetFrontendFont_50_Outline(), "Create", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCreateButton->SetLabelOffset(0, 5);
	m_pCreateButton->SetPressedOffset(0, -4);
	m_pCreateButton->SetCallBackFunction(_CreatePressed);
	m_pCreateButton->SetCallBackData(this);

	// Presets window
	m_pPresetsWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Preset");
	m_pPresetsWindow->AllowMoving(true);
	m_pPresetsWindow->AllowClosing(false);
	m_pPresetsWindow->AllowMinimizing(false);
	m_pPresetsWindow->AllowScrolling(false);
	m_pPresetsWindow->SetRenderTitleBar(true);
	m_pPresetsWindow->SetRenderWindowBackground(true);
	m_pPresetsWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pPresetsWindow->Hide();

	m_pPresetScrollbar = new ScrollBar(m_pRenderer);
	m_pPresetScrollbar->SetScrollDirection(EScrollBarDirection_Vertical);
	m_pPresetScrollbar->SetScrollSize(0.25f);
	m_pPresetScrollbar->SetScrollPosition(1.0f);
	m_pPresetScrollbar->SetDepth(4.0f);
	m_pPresetScrollbar->SetScissorEnabled(true);

	m_pPresetTitleBarIcon = new Icon(m_pRenderer, "", 44, 44);
	m_pPresetTitleBarIcon->SetDepth(4.0f);

	m_pPresetTitleBarBackgroundIcon = new Icon(m_pRenderer, "", 133, 35);
	m_pPresetTitleBarBackgroundIcon->SetDepth(1.0f);

	m_pPresetWindowBackgroundIcon= new Icon(m_pRenderer, "", 350, 150);
	m_pPresetWindowBackgroundIcon->SetDepth(1.0f);

	m_pPresetsWindow->SetBackgroundIcon(m_pPresetWindowBackgroundIcon);
	m_pPresetsWindow->SetTitlebarBackgroundIcon(m_pPresetTitleBarBackgroundIcon);

	m_pPresetsWindow->AddComponent(m_pPresetTitleBarIcon);
	m_pPresetsWindow->AddComponent(m_pPresetScrollbar);

	m_pPresetButtonBorderDefault = new Icon(m_pRenderer, "", 64, 64);
	m_pPresetButtonBorderDefault->SetDepth(2.0f);
	m_pPresetButtonBorderHover = new Icon(m_pRenderer, "", 64, 64);
	m_pPresetButtonBorderHover->SetDepth(2.0f);
	m_pPresetButtonBorderPressed = new Icon(m_pRenderer, "", 64, 64);
	m_pPresetButtonBorderPressed->SetDepth(2.0f);
	m_pPresetButtonBorderDisabled = new Icon(m_pRenderer, "", 64, 64);
	m_pPresetButtonBorderDisabled->SetDepth(2.0f);

	m_pCustomButtonBorderDefault = new Icon(m_pRenderer, "", 64, 64);
	m_pCustomButtonBorderDefault->SetDepth(2.0f);
	m_pCustomButtonBorderHover = new Icon(m_pRenderer, "", 64, 64);
	m_pCustomButtonBorderHover->SetDepth(2.0f);
	m_pCustomButtonBorderPressed = new Icon(m_pRenderer, "", 64, 64);
	m_pCustomButtonBorderPressed->SetDepth(2.0f);
	m_pCustomButtonBorderDisabled = new Icon(m_pRenderer, "", 64, 64);
	m_pCustomButtonBorderDisabled->SetDepth(2.0f);

	m_pDeleteCustomButtonBorderDefault = new Icon(m_pRenderer, "", 24, 24);
	m_pDeleteCustomButtonBorderDefault->SetDepth(6.0f);
	m_pDeleteCustomButtonBorderHover = new Icon(m_pRenderer, "", 24, 24);
	m_pDeleteCustomButtonBorderHover->SetDepth(6.0f);
	m_pDeleteCustomButtonBorderPressed = new Icon(m_pRenderer, "", 24, 24);
	m_pDeleteCustomButtonBorderPressed->SetDepth(6.0f);
	m_pDeleteCustomButtonBorderDisabled = new Icon(m_pRenderer, "", 24, 24);
	m_pDeleteCustomButtonBorderDisabled->SetDepth(6.0f);

	m_pAddNewButtonBorderDefault = new Icon(m_pRenderer, "", 24, 24);
	m_pAddNewButtonBorderDefault->SetDepth(6.0f);
	m_pAddNewButtonBorderHover = new Icon(m_pRenderer, "", 24, 24);
	m_pAddNewButtonBorderHover->SetDepth(6.0f);
	m_pAddNewButtonBorderPressed = new Icon(m_pRenderer, "", 24, 24);
	m_pAddNewButtonBorderPressed->SetDepth(6.0f);
	m_pAddNewButtonBorderDisabled = new Icon(m_pRenderer, "", 24, 24);
	m_pAddNewButtonBorderDisabled->SetDepth(6.0f);

	m_pCreateCustomPresetButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCreateCustomPresetButton->SetLabelOffset(0, 3);
	m_pCreateCustomPresetButton->SetPressedOffset(0, -4);
	m_pCreateCustomPresetButton->SetCallBackFunction(_CreateCustomPressed);
	m_pCreateCustomPresetButton->SetCallBackData(this);
	m_pCreateCustomPresetButton->SetDepth(3.0f);

	// Customize window
	m_pCustomizeWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Customize");
	m_pCustomizeWindow->AllowMoving(true);
	m_pCustomizeWindow->AllowClosing(false);
	m_pCustomizeWindow->AllowMinimizing(false);
	m_pCustomizeWindow->AllowScrolling(false);
	m_pCustomizeWindow->SetRenderTitleBar(true);
	m_pCustomizeWindow->SetRenderWindowBackground(true);
	m_pCustomizeWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pCustomizeWindow->Hide();

	m_pCustomizeTitleBarIcon = new Icon(m_pRenderer, "", 44, 44);
	m_pCustomizeTitleBarIcon->SetDepth(4.0f);

	m_pCustomizeTitleBarBackgroundIcon = new Icon(m_pRenderer, "", 133, 35);
	m_pCustomizeTitleBarBackgroundIcon->SetDepth(1.0f);
	
	m_pCustomizeWindowBackgroundIcon = new Icon(m_pRenderer, "", 350, 225);
	m_pCustomizeWindowBackgroundIcon->SetDepth(1.0f);

	m_pNameTextBox = new TextBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "", "Name");
	m_pNameTextBox->SetPipHeight(20);
	m_pNameTextBox->SetMaxTextLength(10);
	m_pNameTextBox->SetDepth(2.0f);

	m_pCharacterSizeSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Size", 0.75f, 1.25f, 1.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.01f);
	m_pCharacterSizeSlider->SetIncrementButtonsOffset(2, 0);
	m_pCharacterSizeSlider->SetValuePositionOffset(0, 1);
	m_pCharacterSizeSlider->SetRenderBackgroundBar(false);
	m_pCharacterSizeSlider->SetValuePrecision(2);
	m_pCharacterSizeSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pCharacterSizeSlider->SetDepth(2.0f);

	m_pEyesTexturesPulldown = new PulldownMenu(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Eyes", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pEyesTexturesPulldown->SetDepth(4.0f);
	m_pEyesTexturesPulldown->SetMaxNumItemsDisplayed(5);
	m_pEyesTexturesPulldown->SetRenderHeader(true);
	m_pEyesTexturesPulldown->SetMenuItemChangedCallBackFunction(_EyesTexturesPulldownChanged);
	m_pEyesTexturesPulldown->SetMenuItemChangedCallBackData(this);

	m_pPresetsPulldown = new PulldownMenu(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Presets", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pPresetsPulldown->SetDepth(3.0f);
	m_pPresetsPulldown->SetMaxNumItemsDisplayed(5);
	m_pPresetsPulldown->SetRenderHeader(true);
	m_pPresetsPulldown->SetMenuItemChangedCallBackFunction(_PresetsPulldownChanged);
	m_pPresetsPulldown->SetMenuItemChangedCallBackData(this);

	m_pCustomizeWindow->SetBackgroundIcon(m_pCustomizeWindowBackgroundIcon);
	m_pCustomizeWindow->SetTitlebarBackgroundIcon(m_pCustomizeTitleBarBackgroundIcon);

	m_pCustomizeWindow->AddComponent(m_pCustomizeTitleBarIcon);
	m_pCustomizeWindow->AddComponent(m_pNameTextBox);
	m_pCustomizeWindow->AddComponent(m_pCharacterSizeSlider);
	m_pCustomizeWindow->AddComponent(m_pEyesTexturesPulldown);
	m_pCustomizeWindow->AddComponent(m_pPresetsPulldown);

	// Scale window
	m_pScaleWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Scale");
	m_pScaleWindow->AllowMoving(true);
	m_pScaleWindow->AllowClosing(false);
	m_pScaleWindow->AllowMinimizing(false);
	m_pScaleWindow->AllowScrolling(false);
	m_pScaleWindow->SetRenderTitleBar(true);
	m_pScaleWindow->SetRenderWindowBackground(true);
	m_pScaleWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pScaleWindow->Hide();

	m_pScaleTitleBarIcon = new Icon(m_pRenderer, "", 44, 44);
	m_pScaleTitleBarIcon->SetDepth(4.0f);

	m_pScaleTitleBarBackgroundIcon = new Icon(m_pRenderer, "", 133, 35);
	m_pScaleTitleBarBackgroundIcon->SetDepth(1.0f);

	m_pScaleWindowBackgroundIcon = new Icon(m_pRenderer, "", 350, 125);
	m_pScaleWindowBackgroundIcon->SetDepth(1.0f);

	m_pPresetScaleSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Scale", 0.75f, 1.25f, 1.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.01f);
	m_pPresetScaleSlider->SetIncrementButtonsOffset(2, 0);
	m_pPresetScaleSlider->SetValuePositionOffset(0, 1);
	m_pPresetScaleSlider->SetRenderBackgroundBar(false);
	m_pPresetScaleSlider->SetValuePrecision(2);
	m_pPresetScaleSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pPresetScaleSlider->SetDepth(2.0f);

	m_pEyesSizeSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Eyes", 0.75f, 1.25f, 1.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.01f);
	m_pEyesSizeSlider->SetIncrementButtonsOffset(2, 0);
	m_pEyesSizeSlider->SetValuePositionOffset(0, 1);
	m_pEyesSizeSlider->SetRenderBackgroundBar(false);
	m_pEyesSizeSlider->SetValuePrecision(2);
	m_pEyesSizeSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pEyesSizeSlider->SetDepth(2.0f);

	m_pMouthSizeSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Mouth", 0.75f, 1.25f, 1.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.01f);
	m_pMouthSizeSlider->SetIncrementButtonsOffset(2, 0);
	m_pMouthSizeSlider->SetValuePositionOffset(0, 1);
	m_pMouthSizeSlider->SetRenderBackgroundBar(false);
	m_pMouthSizeSlider->SetValuePrecision(2);
	m_pMouthSizeSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pMouthSizeSlider->SetDepth(2.0f);

	m_pScaleWindow->SetBackgroundIcon(m_pScaleWindowBackgroundIcon);
	m_pScaleWindow->SetTitlebarBackgroundIcon(m_pScaleTitleBarBackgroundIcon);

	m_pScaleWindow->AddComponent(m_pScaleTitleBarIcon);
	m_pScaleWindow->AddComponent(m_pPresetScaleSlider);
	m_pScaleWindow->AddComponent(m_pEyesSizeSlider);
	m_pScaleWindow->AddComponent(m_pMouthSizeSlider);

	// Defaults window
	m_pDefaultsWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Defaults");
	m_pDefaultsWindow->AllowMoving(true);
	m_pDefaultsWindow->AllowClosing(false);
	m_pDefaultsWindow->AllowMinimizing(false);
	m_pDefaultsWindow->AllowScrolling(false);
	m_pDefaultsWindow->SetRenderTitleBar(true);
	m_pDefaultsWindow->SetRenderWindowBackground(true);
	m_pDefaultsWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pDefaultsWindow->Hide();

	m_pDefaultsTitleBarIcon = new Icon(m_pRenderer, "", 44, 44);
	m_pDefaultsTitleBarIcon->SetDepth(4.0f);

	m_pDefaultsTitleBarBackgroundIcon = new Icon(m_pRenderer, "", 133, 35);
	m_pDefaultsTitleBarBackgroundIcon->SetDepth(1.0f);

	m_pDefaultsWindowBackgroundIcon = new Icon(m_pRenderer, "", 350, 125);
	m_pDefaultsWindowBackgroundIcon->SetDepth(1.0f);

	m_pDefaultsWindow->SetBackgroundIcon(m_pDefaultsWindowBackgroundIcon);
	m_pDefaultsWindow->SetTitlebarBackgroundIcon(m_pDefaultsTitleBarBackgroundIcon);

	m_pXOffsetSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "X", -10.0f, 10.0f, 0.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.5f);
	m_pXOffsetSlider->SetIncrementButtonsOffset(2, 0);
	m_pXOffsetSlider->SetValuePositionOffset(0, 1);
	m_pXOffsetSlider->SetRenderBackgroundBar(false);
	m_pXOffsetSlider->SetValuePrecision(2);
	m_pXOffsetSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pXOffsetSlider->SetDepth(2.0f);

	m_pYOffsetSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Y", -10.0f, 10.0f, 0.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.5f);
	m_pYOffsetSlider->SetIncrementButtonsOffset(2, 0);
	m_pYOffsetSlider->SetValuePositionOffset(0, 1);
	m_pYOffsetSlider->SetRenderBackgroundBar(false);
	m_pYOffsetSlider->SetValuePrecision(2);
	m_pYOffsetSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pYOffsetSlider->SetDepth(2.0f);

	m_pZOffsetSlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Z", -10.0f, 10.0f, 0.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), true, 0.5f);
	m_pZOffsetSlider->SetIncrementButtonsOffset(2, 0);
	m_pZOffsetSlider->SetValuePositionOffset(0, 1);
	m_pZOffsetSlider->SetRenderBackgroundBar(false);
	m_pZOffsetSlider->SetValuePrecision(2);
	m_pZOffsetSlider->SetSliderParams(25, 10, ESliderDirection_Horizontal);
	m_pZOffsetSlider->SetDepth(2.0f);

	m_pSaveDefaultsButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pSaveDefaultsButton->SetLabelOffset(0, 5);
	m_pSaveDefaultsButton->SetPressedOffset(0, -4);
	m_pSaveDefaultsButton->SetCallBackFunction(_SaveDefaultsPressed);
	m_pSaveDefaultsButton->SetCallBackData(this);

	m_pDefaultsWindow->AddComponent(m_pDefaultsTitleBarIcon);
	m_pDefaultsWindow->AddComponent(m_pXOffsetSlider);
	m_pDefaultsWindow->AddComponent(m_pYOffsetSlider);
	m_pDefaultsWindow->AddComponent(m_pZOffsetSlider);
	m_pDefaultsWindow->AddComponent(m_pSaveDefaultsButton);

	m_pArrowLeftButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pArrowLeftButton->SetLabelOffset(0, 5);
	m_pArrowLeftButton->SetCallBackFunction_Pressed(_ArrowLeftPressed);
	m_pArrowLeftButton->SetCallBackData_Pressed(this);
	m_pArrowLeftButton->SetCallBackFunction_Released(_ArrowLeftReleased);
	m_pArrowLeftButton->SetCallBackData_Released(this);
	m_pArrowLeftButton->SetDepth(3.0f);

	m_pArrowRightButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pArrowRightButton->SetLabelOffset(0, 5);
	m_pArrowRightButton->SetCallBackFunction_Pressed(_ArrowRightPressed);
	m_pArrowRightButton->SetCallBackData_Pressed(this);
	m_pArrowRightButton->SetCallBackFunction_Released(_ArrowRightReleased);
	m_pArrowRightButton->SetCallBackData_Released(this);
	m_pArrowRightButton->SetDepth(3.0f);

	m_creatingCustom = false;
	shouldUpdatePresetWindowAfterDelete = false;

	SetWindowDimensions(m_windowWidth, m_windowHeight);
}

CreateCharacter::~CreateCharacter()
{
	delete m_pNextButton;
	delete m_pCreateButton;
	delete m_pBackButton;

	DeletePresetButtons();

	delete m_pCustomizeWindow;
	delete m_pCustomizeTitleBarIcon;
	delete m_pCustomizeTitleBarBackgroundIcon;
	delete m_pCustomizeWindowBackgroundIcon;
	delete m_pNameTextBox;
	delete m_pEyesTexturesPulldown;
	delete m_pCharacterSizeSlider;
	delete m_pPresetsPulldown;

	delete m_pPresetsWindow;
	delete m_pPresetTitleBarIcon;
	delete m_pPresetTitleBarBackgroundIcon;
	delete m_pPresetWindowBackgroundIcon;
	delete m_pPresetScrollbar;
	delete m_pCreateCustomPresetButton;

	delete m_pPresetButtonBorderDefault;
	delete m_pPresetButtonBorderHover;
	delete m_pPresetButtonBorderPressed;
	delete m_pPresetButtonBorderDisabled;
	delete m_pCustomButtonBorderDefault;
	delete m_pCustomButtonBorderHover;
	delete m_pCustomButtonBorderPressed;
	delete m_pCustomButtonBorderDisabled;
	delete m_pDeleteCustomButtonBorderDefault;
	delete m_pDeleteCustomButtonBorderHover;
	delete m_pDeleteCustomButtonBorderPressed;
	delete m_pDeleteCustomButtonBorderDisabled;
	delete m_pAddNewButtonBorderDefault;
	delete m_pAddNewButtonBorderHover;
	delete m_pAddNewButtonBorderPressed;
	delete m_pAddNewButtonBorderDisabled;	

	// Scale window
	delete m_pScaleWindow;
	delete m_pScaleTitleBarIcon;
	delete m_pScaleTitleBarBackgroundIcon;
	delete m_pScaleWindowBackgroundIcon;
	delete m_pPresetScaleSlider;
	delete m_pEyesSizeSlider;
	delete m_pMouthSizeSlider;	

	// Defaults window
	delete m_pDefaultsWindow;
	delete m_pDefaultsTitleBarIcon;
	delete m_pDefaultsTitleBarBackgroundIcon;
	delete m_pDefaultsWindowBackgroundIcon;
	delete m_pXOffsetSlider;
	delete m_pYOffsetSlider;
	delete m_pZOffsetSlider;
	delete m_pSaveDefaultsButton;

	delete m_pArrowLeftButton;
	delete m_pArrowRightButton;
}

void CreateCharacter::Reset()
{
}

void CreateCharacter::SetWindowDimensions(int windowWidth, int windowHeight)
{
	FrontendPage::SetWindowDimensions(windowWidth, windowHeight);

	m_tooltipWidth = 200;
	m_tooltipHeight = 220;
	m_tooltipDescBorder = 15;

	m_pNextButton->SetDimensions((m_pFrontendManager->GetWindowWidth() - 225 + 56 - 82), 85, 225, 75);
	m_pCreateButton->SetDimensions((int)((m_pFrontendManager->GetWindowWidth()*0.5f)-350) + 24, 20, 225, 75);
	m_pBackButton->SetDimensions((m_pFrontendManager->GetWindowWidth() - 225 + 112 - 82), 20, 110, 47);

	m_presetsWindowWidth = 350;
	m_presetsWindowHeight = 225;
	m_customizeWindowWidth = 350;
	m_customizeWindowHeight = 190;
	m_scaleWindowWidth = 350;
	m_scaleWindowHeight = 125;
	m_defaultsWindowWidth = 350;
	m_defaultsWindowHeight = 125;
	m_titlebarWidth = 158;
	m_titlebarHeight = 35;
	m_presetButtonWidth = 64;
	m_presetButtonHeight = 64;
	m_presetButtonSpacer = 12;

	m_pCustomizeWindow->SetDimensions((int)((m_windowWidth*0.825f) - (m_customizeWindowWidth*0.5f)), (int)((m_windowHeight*0.8f) - (m_customizeWindowHeight*0.5f)), m_customizeWindowWidth, m_customizeWindowHeight);
	m_pCustomizeWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pCustomizeWindow->SetTitleOffset(50, 5);
	m_pCustomizeWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pCustomizeWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pCustomizeTitleBarIcon->SetDimensions(0, m_customizeWindowHeight, 44, 44);

	m_pNameTextBox->SetDimensions(75, 140, 250, 30);
	m_pCharacterSizeSlider->SetDimensions(75, 105, 200, 24);
	m_pEyesTexturesPulldown->SetDimensions(70, 65, 130, 24);
	m_pPresetsPulldown->SetDimensions(70, 25, 180, 24);

	m_pPresetsWindow->SetDimensions((int)((m_windowWidth*0.825f) - (m_presetsWindowWidth*0.5f)), (int)((m_windowHeight*0.5f) - (m_presetsWindowHeight*0.5f)), m_presetsWindowWidth, m_presetsWindowHeight);
	m_pPresetsWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pPresetsWindow->SetTitleOffset(50, 5);
	m_pPresetsWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pPresetsWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pPresetTitleBarIcon->SetDimensions(0, m_presetsWindowHeight, 44, 44);

	m_pCreateCustomPresetButton->SetDimensions(0, 0, 64, 64);

	m_pScaleWindow->SetDimensions((int)((m_windowWidth*0.825f) - (m_scaleWindowWidth*0.5f)), (int)((m_windowHeight*0.25f) - (m_scaleWindowHeight*0.5f)), m_scaleWindowWidth, m_scaleWindowHeight);
	m_pScaleWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pScaleWindow->SetTitleOffset(50, 5);
	m_pScaleWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pScaleWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pScaleTitleBarIcon->SetDimensions(0, m_scaleWindowHeight, 44, 44);

	m_pPresetScaleSlider->SetDimensions(75, 90, 200, 24);
	m_pEyesSizeSlider->SetDimensions(75, 50, 200, 24);
	m_pMouthSizeSlider->SetDimensions(75, 10, 200, 24);

	m_pDefaultsWindow->SetDimensions((int)((m_windowWidth*0.65f) - (m_defaultsWindowWidth*0.5f)), 30, m_defaultsWindowWidth, m_defaultsWindowHeight);
	m_pDefaultsWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pDefaultsWindow->SetTitleOffset(50, 5);
	m_pDefaultsWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pDefaultsWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pDefaultsTitleBarIcon->SetDimensions(0, m_defaultsWindowHeight, 44, 44);

	m_pXOffsetSlider->SetDimensions(20, 90, 200, 24);
	m_pYOffsetSlider->SetDimensions(20, 50, 200, 24);
	m_pZOffsetSlider->SetDimensions(20, 10, 200, 24);

	m_pSaveDefaultsButton->SetDimensions(m_defaultsWindowWidth - 58, 10, 48, 48);

	m_pArrowLeftButton->SetDimensions((int)((m_pFrontendManager->GetWindowWidth()*0.5f)-350), 105, 48, 48);
	m_pArrowRightButton->SetDimensions((int)((m_pFrontendManager->GetWindowWidth()*0.5f)-130), 105, 48, 48);

	// Scrollbar
	int scrollbarHeight = m_presetsWindowHeight-6;
	m_pPresetScrollbar->SetDimensions(m_presetsWindowWidth-28, 4, 24, scrollbarHeight);
	m_pPresetScrollbar->SetScrollArea(-(m_presetsWindowWidth-40), 0, m_presetsWindowWidth-40, scrollbarHeight);
}

// Skinning the GUI
void CreateCharacter::SkinGUI()
{
	string themeName = VoxGame::GetInstance()->GetModsManager()->GetHUDTextureTheme();

	string iconName = "media/textures/gui/" + themeName + "/common/Tooltips/tooltip_background_common.tga";
	m_pTooltipBackground->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/preset_title_icon.tga";
	m_pPresetTitleBarIcon->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/titlebar_background.tga";
	m_pPresetTitleBarBackgroundIcon->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/preset_window_background.tga";
	m_pPresetWindowBackgroundIcon->SetIcon(iconName);

	m_pPresetsWindow->SetBackgroundIcon(m_pPresetWindowBackgroundIcon);
	m_pPresetsWindow->SetTitlebarBackgroundIcon(m_pPresetTitleBarBackgroundIcon);

	Point location = m_pPresetsWindow->GetLocation();
	m_pPresetsWindow->SetDimensions(location.m_x, location.m_y, m_presetsWindowWidth, m_presetsWindowHeight);
	m_pPresetsWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/preset_button_default.tga";
	m_pPresetButtonBorderDefault->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/preset_button_hover.tga";
	m_pPresetButtonBorderHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/preset_button_pressed.tga";
	m_pPresetButtonBorderPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/preset_button_disabled.tga";
	m_pPresetButtonBorderDisabled->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/custom_button_default.tga";
	m_pCustomButtonBorderDefault->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/custom_button_hover.tga";
	m_pCustomButtonBorderHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/custom_button_pressed.tga";
	m_pCustomButtonBorderPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/custom_button_disabled.tga";
	m_pCustomButtonBorderDisabled->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/delete_default.tga";
	m_pDeleteCustomButtonBorderDefault->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/delete_hover.tga";
	m_pDeleteCustomButtonBorderHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/delete_pressed.tga";
	m_pDeleteCustomButtonBorderPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/delete_disabled.tga";
	m_pDeleteCustomButtonBorderDisabled->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/add_new_button_default.tga";
	m_pAddNewButtonBorderDefault->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/add_new_button_hover.tga";
	m_pAddNewButtonBorderHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/add_new_button_pressed.tga";
	m_pAddNewButtonBorderPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/add_new_button_disabled.tga";
	m_pAddNewButtonBorderDisabled->SetIcon(iconName);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/customize_title_icon.tga";
	m_pCustomizeTitleBarIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/titlebar_background.tga";
	m_pCustomizeTitleBarBackgroundIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/customize_window_background.tga";
	m_pCustomizeWindowBackgroundIcon->SetIcon(iconName);

	m_pCustomizeWindow->SetBackgroundIcon(m_pCustomizeWindowBackgroundIcon);
	m_pCustomizeWindow->SetTitlebarBackgroundIcon(m_pCustomizeTitleBarBackgroundIcon);

	location = m_pCustomizeWindow->GetLocation();
	m_pCustomizeWindow->SetDimensions(location.m_x, location.m_y, m_customizeWindowWidth, m_customizeWindowHeight);
	m_pCustomizeWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/scale_title_icon.tga";
	m_pScaleTitleBarIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/titlebar_background.tga";
	m_pScaleTitleBarBackgroundIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/scale_window_background.tga";
	m_pScaleWindowBackgroundIcon->SetIcon(iconName);

	m_pScaleWindow->SetBackgroundIcon(m_pScaleWindowBackgroundIcon);
	m_pScaleWindow->SetTitlebarBackgroundIcon(m_pScaleTitleBarBackgroundIcon);

	location = m_pScaleWindow->GetLocation();
	m_pScaleWindow->SetDimensions(location.m_x, location.m_y, m_scaleWindowWidth, m_scaleWindowHeight);
	m_pScaleWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/scale_title_icon.tga";
	m_pDefaultsTitleBarIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/titlebar_background.tga";
	m_pDefaultsTitleBarBackgroundIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/defaults_window_background.tga";
	m_pDefaultsWindowBackgroundIcon->SetIcon(iconName);

	m_pDefaultsWindow->SetBackgroundIcon(m_pDefaultsWindowBackgroundIcon);
	m_pDefaultsWindow->SetTitlebarBackgroundIcon(m_pDefaultsTitleBarBackgroundIcon);

	location = m_pDefaultsWindow->GetLocation();
	m_pDefaultsWindow->SetDimensions(location.m_x, location.m_y, m_defaultsWindowWidth, m_defaultsWindowHeight);
	m_pDefaultsWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);

	iconName = "media/textures/gui/" + themeName + "/CreateCharacter/save_icon.tga";
	m_pSaveDefaultsButton->AddIcon(m_pRenderer, iconName.c_str(), 48, 48, 34, 34, 7, 7, 3.0f);

	m_pCreateCustomPresetButton->SetDefaultIcon(m_pAddNewButtonBorderDefault);
	m_pCreateCustomPresetButton->SetHoverIcon(m_pAddNewButtonBorderHover);
	m_pCreateCustomPresetButton->SetSelectedIcon(m_pAddNewButtonBorderPressed);
	m_pCreateCustomPresetButton->SetDisabledIcon(m_pAddNewButtonBorderDisabled);

	m_pCreateCustomPresetButton->SetDimensions(0, 0, 64, 64);

	m_pArrowLeftButton->SetDefaultIcon(m_pFrontendManager->GetLeftRotateArrow());
	m_pArrowLeftButton->SetHoverIcon(m_pFrontendManager->GetLeftRotateArrow_Hover());
	m_pArrowLeftButton->SetSelectedIcon(m_pFrontendManager->GetLeftRotateArrow_Pressed());
	m_pArrowLeftButton->SetDisabledIcon(m_pFrontendManager->GetLeftRotateArrow());
	m_pArrowRightButton->SetDefaultIcon(m_pFrontendManager->GetRightRotateArrow());
	m_pArrowRightButton->SetHoverIcon(m_pFrontendManager->GetRightRotateArrow_Hover());
	m_pArrowRightButton->SetSelectedIcon(m_pFrontendManager->GetRightRotateArrow_Pressed());
	m_pArrowRightButton->SetDisabledIcon(m_pFrontendManager->GetRightRotateArrow());

	m_pArrowLeftButton->SetDimensions((int)((m_pFrontendManager->GetWindowWidth()*0.5f) - 350), 105, 48, 48);
	m_pArrowRightButton->SetDimensions((int)((m_pFrontendManager->GetWindowWidth()*0.5f) - 130), 105, 48, 48);

	m_pFrontendManager->SetButtonIcons(m_pNextButton, ButtonSize_225x75);
	m_pFrontendManager->SetButtonIcons(m_pCreateButton, ButtonSize_225x75);
	m_pFrontendManager->SetButtonIcons(m_pBackButton, ButtonSize_110x47);

	m_pFrontendManager->SetScrollbarIcons(m_pPresetScrollbar);
	m_pFrontendManager->SetPulldownMenuIcons(m_pEyesTexturesPulldown);
	m_pFrontendManager->SetPulldownMenuIcons(m_pPresetsPulldown);

	m_pFrontendManager->SetSliderIcons(m_pCharacterSizeSlider);
	m_pFrontendManager->SetSliderIcons(m_pPresetScaleSlider);
	m_pFrontendManager->SetSliderIcons(m_pEyesSizeSlider);
	m_pFrontendManager->SetSliderIcons(m_pMouthSizeSlider);
	m_pFrontendManager->SetSliderIcons(m_pXOffsetSlider);
	m_pFrontendManager->SetSliderIcons(m_pYOffsetSlider);
	m_pFrontendManager->SetSliderIcons(m_pZOffsetSlider);

	m_pNextButton->SetNormalLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pNextButton->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	m_pNextButton->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());
	m_pCreateButton->SetNormalLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateButton->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	m_pCreateButton->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());
	m_pBackButton->SetNormalLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pBackButton->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	m_pBackButton->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());
	m_pCreateCustomPresetButton->SetNormalLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateCustomPresetButton->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	m_pCreateCustomPresetButton->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());
}

void CreateCharacter::UnSkinGUI()
{
}

// Loading
void CreateCharacter::Load()
{
	m_pNextButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pBackButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateCustomPresetButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());

	m_defaultEyeWidth = 9.0f;
	m_defaultMouthWidth = 9.0f;
	m_defaultEyeOffset = vec3(-4.5f, -4.5f, 6.54f);
	m_defaultMouthOffset = vec3(-4.5f, -4.5f, 6.56f);

	m_creatingCustom = false;

	m_rightArrowPressed = false;
	m_leftArrowPressed = false;

	m_selectedPresetSection = PresetSection_None;
	shouldUpdatePresetWindowAfterDelete = false;

	LoadSelectionCharacters();

	Item* pCampFire = VoxGame::GetInstance()->GetItemManager()->CreateItem(vec3(25.0f, 0.5f, 3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 45.0f, 0.0f), "media/gamedata/items/CampFire/CampFire.item", eItem_CampFire, "CreateScreen Camp Fire", true, false, 0.06f);
	if (pCampFire != NULL)
	{
		pCampFire->SetCreateDyingLights(false);
	}

	m_pHoverCreatioNPC = NULL;
	m_pSelectedNPC = NULL;
	m_pCustomCreationNPC = NULL;

	m_classTextFadeInAlpha = 0.0f;
	m_classTextFadeInFadeInTime = 2.5f;
	m_classTextFadeInFadeInTimer = 0.0f;

	m_pGUI->AddComponent(m_pBackButton);

	m_pGUI->AddWindow(m_pPresetsWindow);
	m_pPresetsWindow->Hide();

	m_pGUI->AddWindow(m_pCustomizeWindow);
	m_pCustomizeWindow->Hide();
	
	m_pGUI->AddWindow(m_pScaleWindow);
	m_pScaleWindow->Hide();

	m_pGUI->AddWindow(m_pDefaultsWindow);
	m_pDefaultsWindow->Hide();

	m_pNameTextBox->SetText("");

	SetTitleAndSubtitle("CREATE CHARACTER", "Create a new adventurer to play as.");
}

void CreateCharacter::Unload()
{
	DeletePresetButtons();

	m_pGUI->RemoveWindow(m_pPresetsWindow);
	m_pGUI->RemoveWindow(m_pCustomizeWindow);
	m_pGUI->RemoveWindow(m_pScaleWindow);

	HideTooltip();

	m_pGUI->RemoveComponent(m_pNextButton);
	m_pGUI->RemoveComponent(m_pCreateButton);
	m_pGUI->RemoveComponent(m_pBackButton);

	m_pGUI->RemoveComponent(m_pArrowLeftButton);
	m_pGUI->RemoveComponent(m_pArrowRightButton);

	for(unsigned int i = 0; i < m_vpCharacterLineUp.size(); i++)
	{
		m_vpCharacterLineUp[i]->UnloadWeapon(true);
		m_vpCharacterLineUp[i]->UnloadWeapon(false);
		VoxGame::GetInstance()->GetNPCManager()->DeleteNPC(m_vpCharacterLineUp[i]->GetName());
	}
	m_vpCharacterLineUp.clear();

	if(m_pCustomCreationNPC != NULL)
	{
		VoxGame::GetInstance()->GetNPCManager()->DeleteNPC(m_pCustomCreationNPC->GetName());
		m_pCustomCreationNPC = NULL;
	}

	VoxGame::GetInstance()->GetItemManager()->RemoveItem("CreateScreen Camp Fire");

	RemoveTitlesAndSubTitle();
}

bool CreateCharacter::LoadDefaultSettings(string defaultFile, PresetSection m_section)
{
	ifstream importFile;
	importFile.open(defaultFile.c_str(), ios::in);

	float scale;
	float offsetX;
	float offsetY;
	float offsetZ;
	float eyeOffsetX;
	float eyeOffsetY;
	float eyeOffsetZ;
	float mouthOffsetX;
	float mouthOffsetY;
	float mouthOffsetZ;
	float eyeWidth;
	float eyeHeight;
	float mouthWidth;
	float mouthHeight;

	if(importFile.is_open())
	{
		string tempString;

		importFile >> tempString >> scale;
		importFile >> tempString >> offsetX;
		importFile >> tempString >> offsetY;
		importFile >> tempString >> offsetZ;
		importFile >> tempString >> eyeOffsetX >> eyeOffsetY >> eyeOffsetZ;
		importFile >> tempString >> mouthOffsetX >> mouthOffsetY >> mouthOffsetZ;
		importFile >> tempString >> eyeWidth >> eyeHeight;
		importFile >> tempString >> mouthWidth >> mouthHeight;

		if(m_section == PresetSection_Head)
		{
			m_defaultEyeWidth = eyeWidth;
			m_defaultMouthWidth = mouthWidth;

			m_defaultEyeOffset = vec3(eyeOffsetX, eyeOffsetY, eyeOffsetZ);
			m_defaultMouthOffset = vec3(mouthOffsetX, mouthOffsetY, mouthOffsetZ);
		}

		if(scale > m_pPresetScaleSlider->GetMaxValue())
			scale = m_pPresetScaleSlider->GetMaxValue();
		if(scale < m_pPresetScaleSlider->GetMinValue())
			scale = m_pPresetScaleSlider->GetMinValue();

		m_pPresetScaleSlider->SetCurrentValue(scale);

		m_pXOffsetSlider->SetCurrentValue(offsetX);
		m_pYOffsetSlider->SetCurrentValue(offsetY);
		m_pZOffsetSlider->SetCurrentValue(offsetZ);

		importFile.close();

		return true;
	}

	return false;
}

void CreateCharacter::LoadSelectionCharacters()
{
	vec3 centralLookPoint = vec3(25.0f, 1.0f, 5.0f);
	
	NPC* pCharacter1 = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("Character1", "Human", "Priest", vec3(29.0f, 0.51f, 1.5f), 0.08f, false, true);
	pCharacter1->SetTargetForwardToLookAtPoint(centralLookPoint);
	pCharacter1->SetBodyTurnStopThreshold(0.01f);
	pCharacter1->SetBodyTurnSpeedMultiplier(6.0f);
	pCharacter1->SetPushingCollisionEnabled(false);
	pCharacter1->SetFrontEndNPC(true);
	pCharacter1->SetPlayerClass(PlayerClass_Priest);
	NPC* pCharacter2 = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("Character2", "Human", "Knight", vec3(27.5f, 0.51f, 0.5f), 0.08f, false, true);
	pCharacter2->SetTargetForwardToLookAtPoint(centralLookPoint);
	pCharacter2->SetBodyTurnStopThreshold(0.01f);
	pCharacter2->SetBodyTurnSpeedMultiplier(6.0f);
	pCharacter2->SetPushingCollisionEnabled(false);
	pCharacter2->SetFrontEndNPC(true);
	pCharacter2->SetPlayerClass(PlayerClass_Knight);
	NPC* pCharacter3 = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("Character3", "Human", "Warrior", vec3(26.0f, 0.51f, -0.5f), 0.08f, false, true);
	pCharacter3->SetTargetForwardToLookAtPoint(centralLookPoint);
	pCharacter3->SetBodyTurnStopThreshold(0.01f);
	pCharacter3->SetBodyTurnSpeedMultiplier(6.0f);
	pCharacter3->SetPushingCollisionEnabled(false);
	pCharacter3->SetFrontEndNPC(true);
	pCharacter3->SetPlayerClass(PlayerClass_Warrior);
	NPC* pCharacter4 = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("Character4", "Human", "Ranger", vec3(24.0f, 0.51f, -0.5f), 0.08f, false, true);
	pCharacter4->SetTargetForwardToLookAtPoint(centralLookPoint);
	pCharacter4->SetBodyTurnStopThreshold(0.01f);
	pCharacter4->SetBodyTurnSpeedMultiplier(6.0f);
	pCharacter4->SetPushingCollisionEnabled(false);
	pCharacter4->SetFrontEndNPC(true);
	pCharacter4->SetPlayerClass(PlayerClass_Ranger);
	NPC* pCharacter5 = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("Character5", "Human", "Mage", vec3(22.5f, 0.51f, 0.5f), 0.08f, false, true);
	pCharacter5->SetTargetForwardToLookAtPoint(centralLookPoint);
	pCharacter5->SetBodyTurnStopThreshold(0.01f);
	pCharacter5->SetBodyTurnSpeedMultiplier(6.0f);
	pCharacter5->SetPushingCollisionEnabled(false);
	pCharacter5->SetFrontEndNPC(true);
	pCharacter5->SetPlayerClass(PlayerClass_Mage);
	NPC* pCharacter6 = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("Character6", "Human", "Necromancer", vec3(21.0f, 0.51f, 1.5f), 0.08f, false, true);
	pCharacter6->SetTargetForwardToLookAtPoint(centralLookPoint);
	pCharacter6->SetBodyTurnStopThreshold(0.01f);
	pCharacter6->SetBodyTurnSpeedMultiplier(6.0f);
	pCharacter6->SetPushingCollisionEnabled(false);
	pCharacter6->SetFrontEndNPC(true);
	pCharacter6->SetPlayerClass(PlayerClass_Necromaner);

	pCharacter1->LoadWeapon(false, GetEquipmentFilenameForType(eEquipment_PriestStaff));
	pCharacter1->SetNPCCombatType(eNPCCombatType_Staff, false);

	pCharacter2->LoadWeapon(false, GetEquipmentFilenameForType(eEquipment_IronSword));
	pCharacter2->LoadWeapon(true, GetEquipmentFilenameForType(eEquipment_IronShield));
	pCharacter2->SetNPCCombatType(eNPCCombatType_MeleeSword, false);

	pCharacter4->LoadWeapon(true, GetEquipmentFilenameForType(eEquipment_WoodenBow));
	pCharacter4->SetNPCCombatType(eNPCCombatType_Archer, true);

	pCharacter5->LoadWeapon(false, GetEquipmentFilenameForType(eEquipment_MageStaff));
	pCharacter5->SetNPCCombatType(eNPCCombatType_Staff, false);

	pCharacter6->LoadWeapon(false, "media/gamedata/weapons/FloatingSkull/FloatingSkull.weapon");
	pCharacter6->SetNPCCombatType(eNPCCombatType_Staff, false);


	// Hack to make sure NPCs return to exact spots - radius distance checking is smaller
	pCharacter1->SetAttackRadius(-pCharacter1->GetRadius());
	pCharacter2->SetAttackRadius(-pCharacter2->GetRadius());
	pCharacter3->SetAttackRadius(-pCharacter3->GetRadius());
	pCharacter4->SetAttackRadius(-pCharacter4->GetRadius());
	pCharacter5->SetAttackRadius(-pCharacter5->GetRadius());
	pCharacter6->SetAttackRadius(-pCharacter6->GetRadius());

	m_vpCharacterLineUp.push_back(pCharacter1);
	m_vpCharacterLineUp.push_back(pCharacter2);
	m_vpCharacterLineUp.push_back(pCharacter3);
	m_vpCharacterLineUp.push_back(pCharacter4);
	m_vpCharacterLineUp.push_back(pCharacter5);
	m_vpCharacterLineUp.push_back(pCharacter6);
}

void CreateCharacter::SetHoverNPC(NPC* pHoverNPC)
{
	if(pHoverNPC != m_pHoverCreatioNPC)
	{
		m_classTextFadeInFadeInTimer = 0.0f;
	}

	m_pHoverCreatioNPC = pHoverNPC;
}

void CreateCharacter::SetSelectedNPC(NPC* pSelectedNPC)
{
	m_pNextButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pBackButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateCustomPresetButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());

	if(m_creatingCustom)
	{
	}
	else
	{
		if(m_pSelectedNPC != pSelectedNPC)
		{
			vec3 centralPoint = vec3(25.0f, 1.0f, 3.0f);

			if(m_pSelectedNPC != NULL)
			{
				m_pSelectedNPC->SetTargetPosition(m_selectionNPCOriginPoint);
				m_pSelectedNPC->SetMoveBackToPosition(m_selectionNPCOriginPoint);
				m_pSelectedNPC->SetLookAtPositionWhenReachedTarget(true, centralPoint);
			}

			m_pSelectedNPC = pSelectedNPC;
			if(m_pSelectedNPC != NULL)
			{
				if(pSelectedNPC->HasReachedTargetPosition())
				{
					m_selectionNPCOriginPoint = m_pSelectedNPC->GetCenter();
					vec3 toCentralPoint = centralPoint - m_pSelectedNPC->GetCenter();
					toCentralPoint.y = 0.0f;
					m_pSelectedNPC->SetTargetPosition(m_pSelectedNPC->GetCenter() + toCentralPoint*0.4f);
					m_pSelectedNPC->SetMoveBackToPosition(m_pSelectedNPC->GetCenter() + toCentralPoint*0.4f);
					m_pSelectedNPC->SetLookAtPositionWhenReachedTarget(true, vec3(m_cameraPosition.x, 1.0f, m_cameraPosition.z));

					vec2 screenPos = m_pSelectedNPC->GetScreenPosition();
					screenPos.y -= 250;
					screenPos.x -= m_tooltipWidth*0.5f;

					//ShowTooltip((int)screenPos.x, (int)screenPos.y);

					if(m_pGUI->GetComponent(m_pNextButton) == NULL)
					{
						m_pGUI->AddComponent(m_pNextButton);
					}
				}
			}
			else
			{
				HideTooltip();

				m_pGUI->RemoveComponent(m_pNextButton);
			}
		}
	}
}

void CreateCharacter::CreatePresetButtons(PresetSection presetSection, bool showWindows)
{
	m_selectedPresetSection = presetSection;

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}
	float scale = m_pCustomCreationNPC->GetVoxelCharacter()->GetBoneMatrixRenderScale(presetName.c_str());
	vec3 offset = m_pCustomCreationNPC->GetVoxelCharacter()->GetBoneMatrixRenderOffset(presetName.c_str());
	m_pPresetScaleSlider->SetCurrentValue(scale);
	m_pXOffsetSlider->SetCurrentValue(offset.x);
	m_pYOffsetSlider->SetCurrentValue(offset.y);
	m_pZOffsetSlider->SetCurrentValue(offset.z);

	if(showWindows)
	{
		m_pPresetsWindow->Show();
		m_pScaleWindow->Show();
	}

	int buttonAndSpacerWidth = m_presetButtonWidth + m_presetButtonSpacer;
	int buttonAndSpacerHeight = m_presetButtonHeight + m_presetButtonSpacer;

	int buttonX = -(m_presetsWindowWidth-42);
	int buttonY = m_presetsWindowHeight - m_presetButtonHeight - 17;

	int MAX_X_BUTTONS = 4;
	int buttonCounter = 0;
	int yCounter = 0;
	int xCounter = 0;

	string presetFolderName;
	switch(presetSection) 
	{
	case PresetSection_Head: { presetFolderName = "head"; } break;
	case PresetSection_Body: { presetFolderName = "body"; } break;
	case PresetSection_Legs: { presetFolderName = "legs"; } break;
	case PresetSection_RightShoulder: { presetFolderName = "right_shoulder"; } break;
	case PresetSection_LeftShoulder: { presetFolderName = "left_shoulder"; } break;
	case PresetSection_RightHand: { presetFolderName = "right_hand"; } break;
	case PresetSection_LeftHand: { presetFolderName = "left_hand"; } break;
	case PresetSection_RightFoot: { presetFolderName = "right_foot"; } break;
	case PresetSection_LeftFoot: { presetFolderName = "left_foot"; } break;
	}

	char presetDirectory[128];
	sprintf(presetDirectory, "media/gamedata/models/createcharacter/presets/%s/*.*", presetFolderName.c_str());
	vector<string> listOfPresetFiles;
	listOfPresetFiles = listFilesInDirectory(presetDirectory);

	char customDirectory[128];
	sprintf(customDirectory, "media/gamedata/models/createcharacter/presets/%s/custom/*.*", presetFolderName.c_str());
	vector<string> listOfCustomFiles;
	listOfCustomFiles = listFilesInDirectory(customDirectory);

	for(int creationCounter = 0; creationCounter < 2; creationCounter++)
	{
		vector<string> listToUse;

		if(creationCounter == 0)
		{
			listToUse = listOfPresetFiles;
		}
		else if(creationCounter == 1)
		{
			listToUse = listOfCustomFiles;
		}

		for (unsigned int i = 0; i < listToUse.size(); i++)
		{
			if(strcmp(listToUse[i].c_str(), ".") == 0 || strcmp(listToUse[i].c_str(), "..") == 0)
			{
				continue;
			}

			int lastindex = (int)listToUse[i].find_last_of(".");
			if (lastindex != -1)
			{
				string characterName = listToUse[i].substr(0, lastindex);
				string extension = listToUse[i].substr(lastindex);
				if (strcmp(extension.c_str(), ".qb") == 0)
				{
					int lastindex = (int)listToUse[i].find_last_of(".");
					string fileWithoutExtension = listToUse[i].substr(0, lastindex);

					Button* pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
					pNewButton->SetDimensions(buttonX, buttonY, m_presetButtonWidth, m_presetButtonHeight);
					pNewButton->SetDepth(2.0f);
					pNewButton->SetPressedOffset(0, -4);

					if (creationCounter == 0)
					{
						pNewButton->SetDefaultIcon(m_pPresetButtonBorderDefault);
						pNewButton->SetHoverIcon(m_pPresetButtonBorderHover);
						pNewButton->SetSelectedIcon(m_pPresetButtonBorderPressed);
						pNewButton->SetDisabledIcon(m_pPresetButtonBorderDisabled);
					}
					else if (creationCounter == 1)
					{
						pNewButton->SetDefaultIcon(m_pCustomButtonBorderDefault);
						pNewButton->SetHoverIcon(m_pCustomButtonBorderHover);
						pNewButton->SetSelectedIcon(m_pCustomButtonBorderPressed);
						pNewButton->SetDisabledIcon(m_pCustomButtonBorderDisabled);
					}

					char lPresetTexture[128];
					if (creationCounter == 0)
					{
						sprintf(lPresetTexture, "media/gamedata/models/createcharacter/presets/%s/%s.tga", presetFolderName.c_str(), fileWithoutExtension.c_str());
					}
					else if (creationCounter == 1)
					{
						sprintf(lPresetTexture, "media/gamedata/models/createcharacter/presets/%s/custom/%s.tga", presetFolderName.c_str(), fileWithoutExtension.c_str());
					}
					pNewButton->AddIcon(m_pRenderer, lPresetTexture, 64, 64, m_presetButtonWidth - 16, m_presetButtonHeight - 16, 8, 8, 3.0f);

					PresetButtonData* pButtonData = new PresetButtonData();
					pButtonData->m_pCreateCharacter = this;
					pButtonData->m_presetSection = presetSection;
					pButtonData->m_presetFilename = fileWithoutExtension;
					pButtonData->m_customPreset = (creationCounter == 1);
					m_vpPresetButtonDataItems.push_back(pButtonData);

					pNewButton->SetCallBackFunction(_PresetButtonPressed);
					pNewButton->SetCallBackData(pButtonData);

					m_pPresetScrollbar->AddScrollAreaItem(pNewButton);
					m_vpPresetButtons.push_back(pNewButton);

					// Create a delete button for custom creations
					if (creationCounter == 1)
					{
						Button* pDeleteButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
						pDeleteButton->SetDimensions(buttonX + 48, buttonY + 48, 24, 24);
						pDeleteButton->SetDepth(3.0f);
						pDeleteButton->SetDefaultIcon(m_pDeleteCustomButtonBorderDefault);
						pDeleteButton->SetHoverIcon(m_pDeleteCustomButtonBorderHover);
						pDeleteButton->SetSelectedIcon(m_pDeleteCustomButtonBorderPressed);
						pDeleteButton->SetDisabledIcon(m_pDeleteCustomButtonBorderDisabled);

						m_pPresetScrollbar->AddScrollAreaItem(pDeleteButton);
						m_vpDeleteButtons.push_back(pDeleteButton);

						// Create a DeleteButtonData linkage to connect the delete button to the custom preset button
						DeleteButtonData* pDeleteButtonData = new DeleteButtonData();
						pDeleteButtonData->m_pCreateCharacter = this;
						pDeleteButtonData->m_pDeleteButton = pDeleteButton;
						pDeleteButtonData->m_pPresetButtonConnectedTo = pNewButton;
						pDeleteButtonData->m_presetSection = presetSection;
						pDeleteButtonData->m_presetFilename = fileWithoutExtension;
						m_vpDeleteButtonDataItems.push_back(pDeleteButtonData);

						pDeleteButton->SetCallBackFunction(_DeleteButtonPressed);
						pDeleteButton->SetCallBackData(pDeleteButtonData);
					}

					buttonX += buttonAndSpacerWidth;

					xCounter++;

					if (xCounter == MAX_X_BUTTONS)
					{
						yCounter++;
						xCounter = 0;

						buttonX = -(m_presetsWindowWidth - 42);
						buttonY -= buttonAndSpacerHeight;
					}
				}
			}
		}
	}

	m_pCreateCustomPresetButton->SetLocation(buttonX, buttonY);

	m_pPresetScrollbar->AddScrollAreaItem(m_pCreateCustomPresetButton);

	UpdateCustomCounter(0);

	m_shouldAllowOverwriteButton = false;
}

void CreateCharacter::DeletePresetButtons()
{
	m_pPresetScrollbar->ClearScrollAreaItems();
	for(unsigned int i = 0; i < m_vpPresetButtons.size(); i++)
	{
		delete m_vpPresetButtons[i];
		m_vpPresetButtons[i] = 0;
	}
	m_vpPresetButtons.clear();

	for(unsigned int i = 0; i < m_vpDeleteButtons.size(); i++)
	{
		delete m_vpDeleteButtons[i];
		m_vpDeleteButtons[i] = 0;
	}
	m_vpDeleteButtons.clear();

	for(unsigned int i = 0; i < m_vpPresetButtonDataItems.size(); i++)
	{
		delete m_vpPresetButtonDataItems[i];
		m_vpPresetButtonDataItems[i] = 0;
	}
	m_vpPresetButtonDataItems.clear();

	for(unsigned int i = 0; i < m_vpDeleteButtonDataItems.size(); i++)
	{
		delete m_vpDeleteButtonDataItems[i];
		m_vpDeleteButtonDataItems[i] = 0;
	}
	m_vpDeleteButtonDataItems.clear();
	
	m_pPresetsWindow->Hide();
	m_pScaleWindow->Hide();
	m_pDefaultsWindow->Hide();
}

void CreateCharacter::ShowTooltip(int x, int y)
{
	if(m_pSelectedNPC != NULL)
	{
		m_pTooltipNameLabel->SetText(m_pSelectedNPC->GetName());

		string characterDescription = "[C=Custom(888888)]Level: [C=White]" + string("8") + "\n[C=Custom(888888)]Race: [C=Green]" + m_pSelectedNPC->GetType() + "\n[C=Custom(888888)]Class: [C=Red]" + m_pSelectedNPC->GetModelName();
		m_pTooltipDescriptionLabel->SetText(characterDescription);
	}

	m_pTooltipBackground->SetDimensions(x, y, m_tooltipWidth, m_tooltipHeight);

	int textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_30(), "%s", m_pTooltipNameLabel->GetText().c_str());
	m_pTooltipNameLabel->SetLocation(x + (int)(m_tooltipWidth*0.5f)-(int)(textWidth*0.5f), y + m_tooltipHeight - 35);

	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_25(), "%s", m_pTooltipDescriptionLabel->GetText().c_str());
	m_pTooltipDescriptionLabel->SetDimensions(x + m_tooltipDescBorder, y + m_tooltipDescBorder, m_tooltipWidth-(m_tooltipDescBorder*2), m_tooltipHeight-(m_tooltipDescBorder*2)-35);

	if(m_toolTipVisible == false)
	{
		m_pGUI->AddComponent(m_pTooltipBackground);
		m_pGUI->AddComponent(m_pTooltipNameLabel);
		m_pGUI->AddComponent(m_pTooltipDescriptionLabel);
	}

	m_toolTipVisible = true;
}

void CreateCharacter::HideTooltip()
{
	m_pGUI->RemoveComponent(m_pTooltipBackground);
	m_pGUI->RemoveComponent(m_pTooltipNameLabel);
	m_pGUI->RemoveComponent(m_pTooltipDescriptionLabel);

	m_toolTipVisible = false;
}

void CreateCharacter::SavedNewPresetModel(string modelToLoadToCharacter)
{
	DeletePresetButtons();
	CreatePresetButtons(m_selectedPresetSection, false);

	// Folder name is lower case
	string presetFolderName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetFolderName = "head"; } break;
	case PresetSection_Body: { presetFolderName = "body"; } break;
	case PresetSection_Legs: { presetFolderName = "legs"; } break;
	case PresetSection_RightShoulder: { presetFolderName = "right_shoulder"; } break;
	case PresetSection_LeftShoulder: { presetFolderName = "left_shoulder"; } break;
	case PresetSection_RightHand: { presetFolderName = "right_hand"; } break;
	case PresetSection_LeftHand: { presetFolderName = "left_hand"; } break;
	case PresetSection_RightFoot: { presetFolderName = "right_foot"; } break;
	case PresetSection_LeftFoot: { presetFolderName = "left_foot"; } break;
	}

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}

	char modelFile[128];
	sprintf(modelFile, "%s.qb", modelToLoadToCharacter.c_str());

	QubicleBinary* pNewFile = new QubicleBinary(m_pRenderer);
	pNewFile->Import(modelFile, true);
	m_pCustomCreationNPC->GetVoxelCharacter()->SwapBodyPart(presetName.c_str(), pNewFile->GetQubicleMatrix(0), true);
}

void CreateCharacter::UpdateCustomCounter(int incrementValue)
{
	string presetFolderName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetFolderName = "head"; } break;
	case PresetSection_Body: { presetFolderName = "body"; } break;
	case PresetSection_Legs: { presetFolderName = "legs"; } break;
	case PresetSection_RightShoulder: { presetFolderName = "right_shoulder"; } break;
	case PresetSection_LeftShoulder: { presetFolderName = "left_shoulder"; } break;
	case PresetSection_RightHand: { presetFolderName = "right_hand"; } break;
	case PresetSection_LeftHand: { presetFolderName = "left_hand"; } break;
	case PresetSection_RightFoot: { presetFolderName = "right_foot"; } break;
	case PresetSection_LeftFoot: { presetFolderName = "left_foot"; } break;
	}

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}

	// Get the name for the new file
	char counterFile[128];
	sprintf(counterFile, "media/gamedata/models/createcharacter/presets/%s/custom/customcounter.txt", presetFolderName.c_str());
	ifstream file;
	// Open the counter file
	file.open(counterFile, ios::in);
	int counter = 0;
	if(file.is_open())
	{		
		file >> counter;

		file.close();
	}

	ofstream file2;
	file2.open(counterFile, ios::out);
	if(file2.is_open())
	{
		file2 << setfill('0') << setw(3) << counter+incrementValue;

		file2.close();
	}

	char newCustomFile[128];
	sprintf(newCustomFile, "media/gamedata/models/createcharacter/presets/%s/custom/Custom%s%03i", presetFolderName.c_str(), presetName.c_str(), counter+incrementValue+1);
	m_newModelNameForVoxelEditor = newCustomFile;

	// TODO : Voxel editor GUI
	//m_pFrontendManager->GetGameWindow()->GetVoxelGUI()->SetModelName(m_modelNameForVoxelEditor);
	//m_pFrontendManager->GetGameWindow()->GetVoxelGUI()->SetModelNameNew(m_newModelNameForVoxelEditor);
}

void CreateCharacter::ClosedVoxelEditor(bool showDefaultsWindow)
{
	m_pPresetsWindow->Show();
	m_pCustomizeWindow->Show();
	m_pScaleWindow->Show();

	if(showDefaultsWindow)
	{
		m_pDefaultsWindow->Show();
	}
}

void CreateCharacter::UpdateEyeTexturePulldown()
{
	m_pEyesTexturesPulldown->RemoveAllPullDownMenuItems();
	m_pEyesTexturesPulldown->ResetPullDownMenu();
	m_pCustomizeWindow->RemoveComponent(m_pEyesTexturesPulldown);

	char importDirectory[128];
	sprintf(importDirectory, "media/gamedata/models/Human/faces/*.*");

	vector<string> listFiles;
	listFiles = listFilesInDirectory(importDirectory);
	for (unsigned int i = 0; i < listFiles.size(); i++)
	{
		if(strcmp(listFiles[i].c_str(), ".") == 0 || strcmp(listFiles[i].c_str(), "..") == 0)
		{
			continue;
		}

		if(listFiles[i].find(".tga") != string::npos)
		{
			// Only add directories
			continue;
		}

		m_pEyesTexturesPulldown->AddPulldownItem(listFiles[i]);
	}

	m_pCustomizeWindow->AddComponent(m_pEyesTexturesPulldown);
	m_pEyesTexturesPulldown->AddEventListeners();
}

void CreateCharacter::UpdatePresetsPulldown()
{
	m_pPresetsPulldown->RemoveAllPullDownMenuItems();
	m_pPresetsPulldown->ResetPullDownMenu();
	m_pCustomizeWindow->RemoveComponent(m_pPresetsPulldown);

	char importDirectory[128];
	sprintf(importDirectory, "media/gamedata/models/createcharacter/presets/full_presets/*.*");

	vector<string> listFiles;
	listFiles = listFilesInDirectory(importDirectory);
	for (unsigned int i = 0; i < listFiles.size(); i++)
	{
		if (strcmp(listFiles[i].c_str(), ".") == 0 || strcmp(listFiles[i].c_str(), "..") == 0)
		{
			continue;
		}

		if (listFiles[i].find(".qb") != string::npos)
		{
			int lastindex = (int)(listFiles[i].find_last_of("."));
			string fileWithoutExtension = listFiles[i].substr(0, lastindex);

			m_pPresetsPulldown->AddPulldownItem(fileWithoutExtension.c_str());
		}
	}

	m_pCustomizeWindow->AddComponent(m_pPresetsPulldown);
	m_pPresetsPulldown->AddEventListeners();
}

void CreateCharacter::Update(float dt)
{
	FrontendPage::Update(dt);

	if(m_creatingCustom)
	{
		m_cameraPosition = vec3(25.0f, 2.65f, 1.5f);
		m_cameraView = vec3(25.0f, 1.9f, -2.0f);
	}
	else
	{
		m_cameraPosition = vec3(25.0f, 2.75f, 8.0f);
		m_cameraView = vec3(25.0f, 2.0f, 0.0f);
	}

	for(unsigned int i = 0; i < m_vpCharacterLineUp.size(); i++)
	{
		if(m_vpCharacterLineUp[i]->IsLookAtPointMode() == true && m_vpCharacterLineUp[i]->IsLookingAtPoint())
		{
			m_vpCharacterLineUp[i]->SetRandomLookMode();
		}
	}

	// Class fade in text alpha
	if(m_classTextFadeInFadeInTimer < m_classTextFadeInFadeInTime)
	{
		m_classTextFadeInFadeInTimer += dt;
	}
	else
	{
		m_classTextFadeInFadeInTimer = m_classTextFadeInFadeInTime;
	}

	// Presets scrollbar
	int buttonAndSpacer = m_presetButtonHeight + m_presetButtonSpacer;

	int visibleSize = m_pPresetScrollbar->GetScrollArea().m_height;
	int numButtons = (int)m_vpPresetButtons.size() + 1; // +1 because of the create-custom button
	int addition = numButtons % 4 == 0 ? 0 : 1;
	int neededHeight = ((numButtons / 4) + addition) * buttonAndSpacer + 8;
	int heightDiff = neededHeight - visibleSize;

	int MAX_X_BUTTONS = 4;

	if(heightDiff > 0)
	{
		m_pPresetScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
		m_pPresetScrollbar->SetDisabled(false);

		float scrollRatio = m_pPresetScrollbar->GetScrollRatio();
		int offsetButtonY = (int)(heightDiff * scrollRatio);

		int buttonCounter = 0;
		int yCounter = 0;
		while(buttonCounter < numButtons)
		{
			int x = 0;
			for(x = 0; x < MAX_X_BUTTONS && buttonCounter < m_vpPresetButtons.size(); x++)
			{
				int yPos = m_presetsWindowHeight - m_presetButtonHeight - 17 - (buttonAndSpacer * yCounter);
				Point position = m_vpPresetButtons[buttonCounter]->GetLocation();
				m_vpPresetButtons[buttonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

				buttonCounter++;
			}

			// Create-custom button
			if(buttonCounter == m_vpPresetButtons.size())
			{
				if(x == MAX_X_BUTTONS)
				{
					yCounter++;
				}

				int yPos = m_presetsWindowHeight - m_presetButtonHeight - 17 - (buttonAndSpacer * yCounter);
				Point position = m_pCreateCustomPresetButton->GetLocation();

				m_pCreateCustomPresetButton->SetLocation(position.m_x, yPos + offsetButtonY);

				buttonCounter++;
			}

			yCounter++;
		}

		for(int i = 0; i < m_vpDeleteButtonDataItems.size(); i++)
		{
			Point customButtonLocation = m_vpDeleteButtonDataItems[i]->m_pPresetButtonConnectedTo->GetLocation();
			m_vpDeleteButtonDataItems[i]->m_pDeleteButton->SetLocation(customButtonLocation.m_x + 48, customButtonLocation.m_y + 48);
		}
	}
	else
	{
		m_pPresetScrollbar->SetScrollSize(0.0f);
		m_pPresetScrollbar->SetDisabled(true);
	}

	// Character sliders
	if(m_pCustomCreationNPC != NULL)
	{
		m_pCustomCreationNPC->GetVoxelCharacter()->SetCharacterScale(m_pCharacterSizeSlider->GetCurrentValue()*0.08f);
		m_pCustomCreationNPC->UpdateRadius();

		if(m_selectedPresetSection != PresetSection_None)
		{
			string presetName;
			switch(m_selectedPresetSection)
			{
			case PresetSection_Head: { presetName = "Head"; } break;
			case PresetSection_Body: { presetName = "Body"; } break;
			case PresetSection_Legs: { presetName = "Legs"; } break;
			case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
			case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
			case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
			case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
			case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
			case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
			}

			vec3 offset = vec3(m_pXOffsetSlider->GetCurrentValue(), m_pYOffsetSlider->GetCurrentValue(), m_pZOffsetSlider->GetCurrentValue());
			m_pCustomCreationNPC->GetVoxelCharacter()->SetCharacterMatrixRenderParams(presetName.c_str(), m_pPresetScaleSlider->GetCurrentValue(), offset.x, offset.y, offset.z);

			// Set the sizes and offsets for the eye and mouth textures
			float eyeTextureSize = m_defaultEyeWidth * m_pEyesSizeSlider->GetCurrentValue();
			float mouthTextureSize = m_defaultMouthWidth * m_pMouthSizeSlider->GetCurrentValue();
			m_pCustomCreationNPC->GetVoxelCharacter()->SetEyesTextureSize(eyeTextureSize, eyeTextureSize);
			m_pCustomCreationNPC->GetVoxelCharacter()->SetMouthTextureSize(mouthTextureSize, mouthTextureSize);

			// Keep eyes and mouth centered
			vec3 eyesOffset = vec3(m_defaultEyeOffset.x * m_pEyesSizeSlider->GetCurrentValue(), m_defaultEyeOffset.y * m_pEyesSizeSlider->GetCurrentValue(), m_defaultEyeOffset.z);
			m_pCustomCreationNPC->GetVoxelCharacter()->SetEyesOffset(eyesOffset);
			vec3 mouthOffset(m_defaultMouthOffset.x * m_pMouthSizeSlider->GetCurrentValue(), m_defaultMouthOffset.y, m_defaultMouthOffset.z);
			m_pCustomCreationNPC->GetVoxelCharacter()->SetMouthOffset(mouthOffset);
		}
	}

	// Arrow rotations
	if(m_rightArrowPressed)
	{
		if(m_pCustomCreationNPC != NULL)
		{
			float rotation = m_pCustomCreationNPC->GetRotation() + 150.0f * dt;
			m_pCustomCreationNPC->SetRotation(rotation);
			float angle = DegToRad(rotation);
			m_pCustomCreationNPC->SetTargetForwards(vec3(sin(angle), 0.0f, cos(angle)));
		}
	}
	if(m_leftArrowPressed)
	{
		if(m_pCustomCreationNPC != NULL)
		{
			float rotation = m_pCustomCreationNPC->GetRotation() - 150.0f * dt;
			m_pCustomCreationNPC->SetRotation(rotation);
			float angle = DegToRad(rotation);
			m_pCustomCreationNPC->SetTargetForwards(vec3(sin(angle), 0.0f, cos(angle)));
		}
	}

	if(shouldUpdatePresetWindowAfterDelete)
	{
		DeletePresetButtons();
		CreatePresetButtons(m_selectedPresetSection, true);

		shouldUpdatePresetWindowAfterDelete = false;
	}
}

void CreateCharacter::Render()
{
	FrontendPage::Render();
}

void CreateCharacter::Render2D()
{
	FrontendPage::Render2D();

	if(m_pHoverCreatioNPC != NULL && m_pHoverCreatioNPC != m_pSelectedNPC)
	{
		vec2 screenPos = m_pHoverCreatioNPC->GetScreenPosition();
		char className[32];
		sprintf(className, "%s", m_pHoverCreatioNPC->GetModelName().c_str());

		int lTextWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_80(), "%s", className);
		float xText = screenPos.x - (int)(lTextWidth*0.5f);
		float yText = screenPos.y + 250.0f;

		m_classTextFadeInAlpha = m_classTextFadeInFadeInTimer / m_classTextFadeInFadeInTime;

		m_pRenderer->PushMatrix();
			m_pRenderer->SetRenderMode(RM_SOLID);
			m_pRenderer->EnableTransparency(BF_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA);
			m_pRenderer->SetProjectionMode(PM_2D, VoxGame::GetInstance()->GetDefaultViewport());
			m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
			m_pRenderer->RenderFreeTypeText(m_pFrontendManager->GetFrontendFont_80(), xText, yText, 1.0f, Colour(1.0f, 1.0f, 1.0f, m_classTextFadeInAlpha), 1.0f, className);
			m_pRenderer->RenderFreeTypeText(m_pFrontendManager->GetFrontendFont_80_Outline(), xText, yText, 1.0f, Colour(0.0f, 0.0f, 0.0f, m_classTextFadeInAlpha), 1.0f, className);
			m_pRenderer->DisableTransparency();
		m_pRenderer->PopMatrix();
	}

	if(m_pSelectedNPC != NULL)
	{
		vec2 screenPos = m_pSelectedNPC->GetScreenPosition();
		char className[32];
		sprintf(className, "%s", m_pSelectedNPC->GetModelName().c_str());

		int lTextWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_80(), "%s", className);
		float xText = screenPos.x - (int)(lTextWidth*0.5f);
		float yText = screenPos.y + 250.0f;

		m_pRenderer->PushMatrix();
			m_pRenderer->SetRenderMode(RM_SOLID);
			m_pRenderer->EnableTransparency(BF_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA);
			m_pRenderer->SetProjectionMode(PM_2D, VoxGame::GetInstance()->GetDefaultViewport());
			m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
			m_pRenderer->RenderFreeTypeText(m_pFrontendManager->GetFrontendFont_80(), xText, yText, 1.0f, Colour(1.0f, 1.0f, 0.0f, 1.0f), 1.0f, className);
			m_pRenderer->RenderFreeTypeText(m_pFrontendManager->GetFrontendFont_80_Outline(), xText, yText, 1.0f, Colour(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, className);
			m_pRenderer->DisableTransparency();
		m_pRenderer->PopMatrix();
	}
}

void CreateCharacter::_SelectPressed(void *apData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)apData;
	lpCreateCharacter->SelectPressed();
}

void CreateCharacter::SelectPressed()
{
	m_pNextButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pBackButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateCustomPresetButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());

	if(m_creatingCustom)
	{
		// Can't press select from custom creation part
	}
	else
	{
		m_classTextFadeInAlpha = 0.0f;
		m_classTextFadeInFadeInTime = 2.5f;
		m_classTextFadeInFadeInTimer = 0.0f;

		m_pGUI->RemoveComponent(m_pNextButton);
		m_pGUI->AddComponent(m_pCreateButton);

		m_pGUI->AddComponent(m_pArrowLeftButton);
		m_pGUI->AddComponent(m_pArrowRightButton);

		m_pCharacterSizeSlider->SetCurrentValue(m_pCharacterSizeSlider->GetDefaultValue());
		m_pPresetScaleSlider->SetCurrentValue(m_pPresetScaleSlider->GetDefaultValue());
		m_pEyesSizeSlider->SetCurrentValue(m_pEyesSizeSlider->GetDefaultValue());
		m_pMouthSizeSlider->SetCurrentValue(m_pMouthSizeSlider->GetDefaultValue());
		m_pXOffsetSlider->SetCurrentValue(m_pXOffsetSlider->GetDefaultValue());
		m_pYOffsetSlider->SetCurrentValue(m_pYOffsetSlider->GetDefaultValue());
		m_pZOffsetSlider->SetCurrentValue(m_pZOffsetSlider->GetDefaultValue());

		m_creatingCustom = true;
		HideTooltip();

		m_selectedPresetSection = PresetSection_None;
		shouldUpdatePresetWindowAfterDelete = false;

		// Create a new NPC based on the selected template for custom character modifications
		m_pCustomCreationNPC = VoxGame::GetInstance()->GetNPCManager()->CreateNPC("CustomCreationCharacter", m_pSelectedNPC->GetType().c_str(), m_pSelectedNPC->GetModelName().c_str(), vec3(24.0f, 0.5f, -2.0f), 0.08f, false, false);
		m_pCustomCreationNPC->SetFrontEndNPC(true);
		m_pCustomCreationNPC->SetPlayerClass(m_pSelectedNPC->GetPlayerClass());
		//m_pCustomCreationNPC->GetVoxelCharacter()->PlayFacialExpression("Angry");
		//m_pCustomCreationNPC->GetVoxelCharacter()->SetRandomLookDirection(false);

		// Set the custom models body part params, based on the selected NPC
		for(int i = 0; i <= PresetSection_LeftFoot; i++)
		{
			// Body part name is uppercase
			string presetName;
			switch(i)
			{
			case PresetSection_Head: { presetName = "Head"; } break;
			case PresetSection_Body: { presetName = "Body"; } break;
			case PresetSection_Legs: { presetName = "Legs"; } break;
			case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
			case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
			case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
			case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
			case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
			case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
			}

			if(i == PresetSection_Head)
			{
				vec3 eyeOffset = m_pSelectedNPC->GetVoxelCharacter()->GetEyesOffset();
				vec3 mouthOffset = m_pSelectedNPC->GetVoxelCharacter()->GetMouthOffset();
				float eyeWidth = m_pSelectedNPC->GetVoxelCharacter()->GetEyeTextureWidth();
				float eyeHeight = m_pSelectedNPC->GetVoxelCharacter()->GetEyeTextureHeight();
				float mouthWidth = m_pSelectedNPC->GetVoxelCharacter()->GetMouthTextureWidth();
				float mouthHeight = m_pSelectedNPC->GetVoxelCharacter()->GetMouthTextureHeight();

				m_pCustomCreationNPC->GetVoxelCharacter()->SetEyesTextureSize(eyeWidth, eyeHeight);
				m_pCustomCreationNPC->GetVoxelCharacter()->SetMouthTextureSize(mouthWidth, mouthHeight);
				m_pCustomCreationNPC->GetVoxelCharacter()->SetEyesOffset(eyeOffset);
				m_pCustomCreationNPC->GetVoxelCharacter()->SetMouthOffset(mouthOffset);

				m_defaultEyeWidth = eyeWidth;
				m_defaultMouthWidth = mouthWidth;
				m_defaultEyeOffset = eyeOffset;
				m_defaultMouthOffset = mouthOffset;
			}

			float scale = m_pSelectedNPC->GetVoxelCharacter()->GetBoneMatrixRenderScale(presetName.c_str());
			vec3 offset = m_pSelectedNPC->GetVoxelCharacter()->GetBoneMatrixRenderOffset(presetName.c_str());

			m_pCustomCreationNPC->GetVoxelCharacter()->SetCharacterMatrixRenderParams(presetName.c_str(), scale, offset.x, offset.y, offset.z);
		}

		m_pHoverCreatioNPC = NULL;
		m_pSelectedNPC = NULL;

		// Remove the other NPCs lineup
		for(unsigned int i = 0; i < m_vpCharacterLineUp.size(); i++)
		{
			if(m_vpCharacterLineUp[i] != m_pCustomCreationNPC)
			{
				m_vpCharacterLineUp[i]->UnloadWeapon(true);
				m_vpCharacterLineUp[i]->UnloadWeapon(false);
				VoxGame::GetInstance()->GetNPCManager()->DeleteNPC(m_vpCharacterLineUp[i]->GetName());
			}
		}
		m_vpCharacterLineUp.clear();

		if(m_pCustomCreationNPC != NULL)
		{
			m_pCustomCreationNPC->SetSubSelectionRender(true);
		}

		SetTitleAndSubtitle("CREATE CHARACTER", "Customize your adventurer.");

		UpdateEyeTexturePulldown();
		UpdatePresetsPulldown();

		m_pCustomizeWindow->Show();
	}
}

void CreateCharacter::_CreatePressed(void *apData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)apData;
	lpCreateCharacter->CreatePressed();
}

void CreateCharacter::CreatePressed()
{
	const string& modelName = m_pNameTextBox->GetText();
	if(!modelName.empty()) // Don't allow empty name
	{
		const string& type = m_pNameTextBox->GetText();		

		char characterBaseFolder[128];
		sprintf(characterBaseFolder, "saves/characters/%s", type.c_str());

		//if(dirExists(characterBaseFolder) == false) // Check if we already have a character with the same name // TODO : dirExists
		{
#ifdef _WIN32
			int ret = _mkdir(characterBaseFolder);
#else
			int ret = mkdir(characterBaseFolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif //_WIN32
			if (ret != 0)
				return;

			char qbFilename[128];
			char facesFilename[128];
			char characterFilename[128];
			
			sprintf(qbFilename, "saves/characters/%s/%s.qb", type.c_str(), modelName.c_str());
			sprintf(facesFilename, "saves/characters/%s/%s.faces", type.c_str(), modelName.c_str());
			sprintf(characterFilename, "saves/characters/%s/%s.character", type.c_str(), modelName.c_str());

			// Save character
			if(m_pCustomCreationNPC != NULL)
			{
				m_pCustomCreationNPC->GetVoxelCharacter()->SaveVoxelCharacter(qbFilename, facesFilename, characterFilename);

				// Load default inventory and stats and save out files for the character
				VoxGame::GetInstance()->GetPlayer()->SetName(modelName);
				VoxGame::GetInstance()->GetPlayer()->SetClass(m_pCustomCreationNPC->GetPlayerClass());
				VoxGame::GetInstance()->GetPlayer()->LoadCharacterSettings();

				VoxGame::GetInstance()->GetNPCManager()->DeleteNPC(m_pCustomCreationNPC->GetName());

				m_pCustomCreationNPC = NULL;
			}

			m_pFrontendManager->SetFrontendScreen(FrontendScreen_SelectCharacter);
		}
	}
}

void CreateCharacter::_BackPressed(void *apData)
{
	assert(apData != nullptr);
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)apData;
	lpCreateCharacter->BackPressed();
}

void CreateCharacter::BackPressed()
{
	m_pNextButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pBackButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());
	m_pCreateCustomPresetButton->SetLabelColour(m_pFrontendManager->GetNormalFontColour());

	if(m_creatingCustom)
	{
		m_creatingCustom = false;
		
		if(m_pCustomCreationNPC != NULL)
		{
			VoxGame::GetInstance()->GetNPCManager()->DeleteNPC(m_pCustomCreationNPC->GetName());
			m_pCustomCreationNPC = NULL;
		}

		LoadSelectionCharacters();
		
		m_pGUI->RemoveComponent(m_pCreateButton);

		m_pGUI->RemoveComponent(m_pArrowLeftButton);
		m_pGUI->RemoveComponent(m_pArrowRightButton);

		SetTitleAndSubtitle("CREATE CHARACTER", "Create a new adventurer to play as.");

		m_pPresetsWindow->Hide();
		m_pCustomizeWindow->Hide();
		m_pScaleWindow->Hide();
		m_pDefaultsWindow->Hide();

		// TODO : Voxel editor GUI
		//m_pFrontendManager->GetGameWindow()->UnloadVoxelEditor();
	}
	else
	{
		m_pFrontendManager->SetFrontendScreen(FrontendScreen_SelectCharacter);
	}
}

void CreateCharacter::_EyesTexturesPulldownChanged(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->EyesTexturesPulldownChanged();
}

void CreateCharacter::EyesTexturesPulldownChanged()
{
	if(m_pCustomCreationNPC != NULL)
	{
		MenuItem* pEyesTexture = m_pEyesTexturesPulldown->GetSelectedMenuItem();
		if(pEyesTexture != NULL)
		{
			m_pCustomCreationNPC->GetVoxelCharacter()->ModifyEyesTextures("media/gamedata/models", "Human", pEyesTexture->GetLabel().GetText().c_str());
		}
	}
}

void CreateCharacter::_PresetsPulldownChanged(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->PresetsPulldownChanged();
}

void CreateCharacter::PresetsPulldownChanged()
{
	if (m_pCustomCreationNPC != NULL)
	{
		MenuItem* pPreset = m_pPresetsPulldown->GetSelectedMenuItem();
		if (pPreset != NULL)
		{
			char characterBaseFolder[128];
			char presetModelFilename[256];
			char ms3dFilename[128];
			char animListFilename[128];
			char facesFilename[128];
			char characterFilename[128];

			const string& labelText = pPreset->GetLabel().GetText();
			sprintf(characterBaseFolder, "media/gamedata/models");
			sprintf(ms3dFilename, "media/gamedata/models/Human/Human.ms3d");
			sprintf(animListFilename, "media/gamedata/models/Human/Human.animlist");
			sprintf(facesFilename, "media/gamedata/models/Human/%s.faces", labelText.c_str());
			sprintf(characterFilename, "media/gamedata/models/createcharacter/presets/full_presets/%s.character", labelText.c_str());
			sprintf(presetModelFilename, "media/gamedata/models/createcharacter/presets/full_presets/%s.qb", labelText.c_str());

			m_pCustomCreationNPC->GetVoxelCharacter()->UnloadCharacter();
			m_pCustomCreationNPC->GetVoxelCharacter()->Reset();
			m_pCustomCreationNPC->GetVoxelCharacter()->LoadVoxelCharacter("Human", presetModelFilename, ms3dFilename, animListFilename, facesFilename, characterFilename, characterBaseFolder, false);
		}
	}	
}

void CreateCharacter::_ArrowLeftPressed(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->ArrowLeftPressed();
}

void CreateCharacter::ArrowLeftPressed()
{
	m_leftArrowPressed = true;
}

void CreateCharacter::_ArrowLeftReleased(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->ArrowLeftReleased();
}

void CreateCharacter::ArrowLeftReleased()
{
	m_leftArrowPressed = false;
}

void CreateCharacter::_ArrowRightPressed(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->ArrowRightPressed();
}

void CreateCharacter::ArrowRightPressed()
{
	m_rightArrowPressed = true;
}

void CreateCharacter::_ArrowRightReleased(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->ArrowRightReleased();
}

void CreateCharacter::ArrowRightReleased()
{
	m_rightArrowPressed = false;
}

void CreateCharacter::_CreateCustomPressed(void *pData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)pData;
	lpCreateCharacter->CreateCustomPressed();
}

void CreateCharacter::CreateCustomPressed()
{
	// TODO : Custom body parts via the Voxel editor GUI
	return;

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}

	QubicleBinary* pQubicleModel = m_pCustomCreationNPC->GetVoxelCharacter()->GetQubicleModel();
	QubicleMatrix* pMatrix = pQubicleModel->GetQubicleMatrix(presetName.c_str());
	// TODO : Voxel editor GUI
	//m_pFrontendManager->GetGameWindow()->GetChunkManager()->ClearVoxelEditorChunks();
	//m_pFrontendManager->GetGameWindow()->GetChunkManager()->ImportQubicleBinaryMatrix(pMatrix, vec3(0.0f, 0.0f, 0.0f), QubicleImportDirection_Normal, true);
	//m_pFrontendManager->GetGameWindow()->SetVoxelEditor(m_modelNameForVoxelEditor, m_newModelNameForVoxelEditor, m_shouldAllowOverwriteButton);
	m_pPresetsWindow->Hide();
	m_pCustomizeWindow->Hide();
	m_pScaleWindow->Hide();
	m_pDefaultsWindow->Hide();
}

void CreateCharacter::_PresetButtonPressed(void *apData)
{
	PresetButtonData* lpPresetButtonData = (PresetButtonData*)apData;
	lpPresetButtonData->m_pCreateCharacter->PresetButtonPressed(lpPresetButtonData);
}

void CreateCharacter::PresetButtonPressed(PresetButtonData* presetButtonData)
{
	m_shouldAllowOverwriteButton = (presetButtonData->m_customPreset == true);

	// Folder name is lower case
	string presetFolderName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetFolderName = "head"; } break;
	case PresetSection_Body: { presetFolderName = "body"; } break;
	case PresetSection_Legs: { presetFolderName = "legs"; } break;
	case PresetSection_RightShoulder: { presetFolderName = "right_shoulder"; } break;
	case PresetSection_LeftShoulder: { presetFolderName = "left_shoulder"; } break;
	case PresetSection_RightHand: { presetFolderName = "right_hand"; } break;
	case PresetSection_LeftHand: { presetFolderName = "left_hand"; } break;
	case PresetSection_RightFoot: { presetFolderName = "right_foot"; } break;
	case PresetSection_LeftFoot: { presetFolderName = "left_foot"; } break;
	}

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}

	char presetFile[128];
	if(presetButtonData->m_customPreset == false)
	{
		sprintf(presetFile, "media/gamedata/models/createcharacter/presets/%s/%s.qb", presetFolderName.c_str(), presetButtonData->m_presetFilename.c_str());
	}
	else
	{
		sprintf(presetFile, "media/gamedata/models/createcharacter/presets/%s/custom/%s.qb", presetFolderName.c_str(), presetButtonData->m_presetFilename.c_str());
	}
	

	QubicleBinary* pNewFile = new QubicleBinary(m_pRenderer);
	pNewFile->Import(presetFile, true);
	m_pCustomCreationNPC->GetVoxelCharacter()->SwapBodyPart(presetName.c_str(), pNewFile->GetQubicleMatrix(0), true);

	// Load the default settings file
	char defaultFile[128];
	if(presetButtonData->m_customPreset == false)
	{
		sprintf(defaultFile, "media/gamedata/models/createcharacter/presets/%s/%s.default", presetFolderName.c_str(), presetButtonData->m_presetFilename.c_str());
	}
	else
	{
		sprintf(defaultFile, "media/gamedata/models/createcharacter/presets/%s/custom/%s.default", presetFolderName.c_str(), presetButtonData->m_presetFilename.c_str());
	}	

	LoadDefaultSettings(defaultFile, presetButtonData->m_presetSection);

	// Store the model name so that we can load the correct file into the voxel editor
	char modelFile[128];
	if(presetButtonData->m_customPreset == false)
	{
		sprintf(modelFile, "media/gamedata/models/createcharacter/presets/%s/%s", presetFolderName.c_str(), presetButtonData->m_presetFilename.c_str());
	}
	else
	{
		sprintf(modelFile, "media/gamedata/models/createcharacter/presets/%s/custom/%s", presetFolderName.c_str(), presetButtonData->m_presetFilename.c_str());
	}	
	m_modelNameForVoxelEditor = modelFile;

	// Get the name for the new file
	char counterFile[128];
	sprintf(counterFile, "media/gamedata/models/createcharacter/presets/%s/custom/customcounter.txt", presetFolderName.c_str());
	ifstream file;
	// Open the counter file
	file.open(counterFile, ios::in);
	int counter = 0;
	if(file.is_open())
	{		
		file >> counter;

		file.close();
	}

	char newCustomFile[128];
	sprintf(newCustomFile, "media/gamedata/models/createcharacter/presets/%s/custom/Custom%s%03i", presetFolderName.c_str(), presetName.c_str(), counter+1);
	m_newModelNameForVoxelEditor = newCustomFile;

	if(presetButtonData->m_customPreset)
	{
		m_pDefaultsWindow->Show();
	}
	else
	{
		m_pDefaultsWindow->Hide();
	}
}

void CreateCharacter::_DeleteButtonPressed(void *apData)
{
	DeleteButtonData* lpDeleteButtonData = (DeleteButtonData*)apData;
	lpDeleteButtonData->m_pCreateCharacter->DeleteButtonPressed(lpDeleteButtonData);
}

void CreateCharacter::DeleteButtonPressed(DeleteButtonData* deleteButtonData)
{
	// Folder name is lower case
	string presetFolderName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetFolderName = "head"; } break;
	case PresetSection_Body: { presetFolderName = "body"; } break;
	case PresetSection_Legs: { presetFolderName = "legs"; } break;
	case PresetSection_RightShoulder: { presetFolderName = "right_shoulder"; } break;
	case PresetSection_LeftShoulder: { presetFolderName = "left_shoulder"; } break;
	case PresetSection_RightHand: { presetFolderName = "right_hand"; } break;
	case PresetSection_LeftHand: { presetFolderName = "left_hand"; } break;
	case PresetSection_RightFoot: { presetFolderName = "right_foot"; } break;
	case PresetSection_LeftFoot: { presetFolderName = "left_foot"; } break;
	}

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}

	char modelFile[128];
	char thumbnailFile[128];
	char defaultFile[128];
	sprintf(modelFile, "media/gamedata/models/createcharacter/presets/%s/custom/%s.qb", presetFolderName.c_str(), deleteButtonData->m_presetFilename.c_str());
	sprintf(thumbnailFile, "media/gamedata/models/createcharacter/presets/%s/custom/%s.tga", presetFolderName.c_str(), deleteButtonData->m_presetFilename.c_str());
	sprintf(defaultFile, "media/gamedata/models/createcharacter/presets/%s/custom/%s.default", presetFolderName.c_str(), deleteButtonData->m_presetFilename.c_str());

	// Remove the custom qubicle file
	remove(modelFile);

	// Remove the thumbnail
	remove(thumbnailFile);

	// Remove the default file
	remove(defaultFile);

	shouldUpdatePresetWindowAfterDelete = true;
}

void CreateCharacter::_SaveDefaultsPressed(void *apData)
{
	CreateCharacter* lpCreateCharacter = (CreateCharacter*)apData;
	lpCreateCharacter->SaveDefaultsPressed();
}

void CreateCharacter::SaveDefaultsPressed() const
{
	// Folder name is lower case
	string presetFolderName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetFolderName = "head"; } break;
	case PresetSection_Body: { presetFolderName = "body"; } break;
	case PresetSection_Legs: { presetFolderName = "legs"; } break;
	case PresetSection_RightShoulder: { presetFolderName = "right_shoulder"; } break;
	case PresetSection_LeftShoulder: { presetFolderName = "left_shoulder"; } break;
	case PresetSection_RightHand: { presetFolderName = "right_hand"; } break;
	case PresetSection_LeftHand: { presetFolderName = "left_hand"; } break;
	case PresetSection_RightFoot: { presetFolderName = "right_foot"; } break;
	case PresetSection_LeftFoot: { presetFolderName = "left_foot"; } break;
	}

	// Body part name is uppercase
	string presetName;
	switch(m_selectedPresetSection)
	{
	case PresetSection_Head: { presetName = "Head"; } break;
	case PresetSection_Body: { presetName = "Body"; } break;
	case PresetSection_Legs: { presetName = "Legs"; } break;
	case PresetSection_RightShoulder: { presetName = "Right_Shoulder"; } break;
	case PresetSection_LeftShoulder: { presetName = "Left_Shoulder"; } break;
	case PresetSection_RightHand: { presetName = "Right_Hand"; } break;
	case PresetSection_LeftHand: { presetName = "Left_Hand"; } break;
	case PresetSection_RightFoot: { presetName = "Right_Foot"; } break;
	case PresetSection_LeftFoot: { presetName = "Left_Foot"; } break;
	}

	char defaultFile[128];
	sprintf(defaultFile, "%s.default", m_modelNameForVoxelEditor.c_str());

	ofstream exportFile;
	exportFile.open(defaultFile);

	if(exportFile.is_open())
	{
		float scale = m_pCustomCreationNPC->GetVoxelCharacter()->GetBoneMatrixRenderScale(presetName.c_str());
		vec3 offset = m_pCustomCreationNPC->GetVoxelCharacter()->GetBoneMatrixRenderOffset(presetName.c_str());

		vec3 eyeOffset = m_pCustomCreationNPC->GetVoxelCharacter()->GetEyesOffset();
		vec3 mouthOffset = m_pCustomCreationNPC->GetVoxelCharacter()->GetMouthOffset();
		float eyeWidth = m_pCustomCreationNPC->GetVoxelCharacter()->GetEyeTextureWidth();
		float eyeHeight = m_pCustomCreationNPC->GetVoxelCharacter()->GetEyeTextureHeight();
		float mouthWidth = m_pCustomCreationNPC->GetVoxelCharacter()->GetMouthTextureWidth();
		float mouthHeight = m_pCustomCreationNPC->GetVoxelCharacter()->GetMouthTextureHeight();

		exportFile << "defaultScale: " << scale << "\n";
		exportFile << "defaultXOffset: " << offset.x << "\n";
		exportFile << "defaultYOffset: " << offset.y << "\n";
		exportFile << "defaultZOffset: " << offset.z << "\n";
		exportFile << "defaultEyesOffset: " << eyeOffset.x << " " << eyeOffset.y << " " << eyeOffset.z << "\n";
		exportFile << "defaultMouthOffset: " << mouthOffset.x << " " << mouthOffset.y << " " << mouthOffset.z << "\n";
		exportFile << "defaultEyesSize: " << eyeWidth << " " << eyeHeight << "\n";
		exportFile << "defaultMouthSize: " << mouthWidth << " " << mouthHeight << "\n";

		exportFile.close();
	}
}