// ******************************************************************************
//
// Filename:	guiwindowminimizebutton.cpp
// Project:     Vox
// Author:		Steven Ball
//
// Purpose:
//   A button that is included in the title bar of a gui window
//   clicking on this button will minimize the window.
//
// Revision History:
//   Initial Revision - 26/09/07
//
// Copyright (c) 2005-2006, Steven Ball
//
// ******************************************************************************

#include "guiwindowminimizebutton.h"
#include "guiwindow.h"


GUIWindowMinimizeButton::GUIWindowMinimizeButton(Renderer* pRenderer, unsigned int GUIFont, const std::string &label)
  : Button(pRenderer, GUIFont, label)
{
	// Get the height of the title bar (TEMP : This shouldnt be the correct way to make the button dimensions!)
	int distanceFromBorder = 2;
	int buttonSize = m_pRenderer->GetFreeTypeTextHeight(GUIFont, "a") - distanceFromBorder;

	//int buttonX = parent->GetDimensions().m_width - ((buttonSize + distanceFromBorder) * 2);
	//SetDimensions(buttonX, distanceFromBorder/2, buttonSize, buttonSize);

	SetDisplayLabel(false);
}

GUIWindowMinimizeButton::~GUIWindowMinimizeButton()
{
}

void GUIWindowMinimizeButton::SetGUIWindowParent(GUIWindow *pParent)
{
	m_pParentGUIWindow = pParent;
}

void GUIWindowMinimizeButton::OnMouseEnter()
{
}

void GUIWindowMinimizeButton::OnMouseExit()
{
}

void GUIWindowMinimizeButton::OnMousePressed()
{
}

void GUIWindowMinimizeButton::OnMouseReleased()
{
}

void GUIWindowMinimizeButton::OnMouseClicked()
{
	if(IsDisabled())
	{
		return;
	}

	// Dont allow minizing the window if we have disabled this
	if(m_pParentGUIWindow->m_bAllowMinimizing == false)
	{
		return;
	}

	m_pParentGUIWindow->SetMinimized(!m_pParentGUIWindow->GetMinimized());
}