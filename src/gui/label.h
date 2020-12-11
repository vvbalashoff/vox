// ******************************************************************************
//
// Filename:	label.h
// Project:     Vox
// Author:		Steven Ball
//
// Purpose:
//   A simple GUI component that simple displays a text label
//
// Revision History:
//   Initial Revision - 27/09/06
//
// Copyright (c) 2005-2006, Steven Ball
//
// ******************************************************************************

#pragma once

#include "component.h"

class Label : public Component
{
public:
	/* Public methods */
	Label(Renderer* pRenderer, unsigned int GUIFont, const std::string &label, const Colour& colour);

	~Label();

	void SetText(const std::string &label);
	const std::string& GetText() const;

	void SetFontID(unsigned int lFontID);
	unsigned int GetFontID();

	void SetColour(const Colour& lColour);
	void SetOutlineColour(const Colour& lColour);
	void SetOutlineFont(unsigned int OutlineGUIFont);

	void GetColour(float *r, float *g, float *b, float *a);
	void GetOutlineColour(float *r, float *g, float *b, float *a);

	void SetOutline(bool outline);

	void SetVerticalAlignment(EVerticalAlignment alignment);
	EVerticalAlignment GetVerticalAlignment() const;

	void SetHorizontalAlignment(EHorizontalAlignment alignment);
	EHorizontalAlignment GetHorizontalAlignment() const;

	EComponentType GetComponentType() const override;

	void Insert(int index, char* character);
	void Erase(int start, int num);

protected:
	/* Protected methods */
	void DrawSelf() override;

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	std::string m_text;

	Colour m_colour;
	Colour m_outlineColour;

	bool m_outline;

	EVerticalAlignment m_eVerticalAlignment;
	EHorizontalAlignment m_eHorizontalAlignment;

	unsigned int m_GUIFont;
	unsigned int m_OutlineGUIFont;
};
