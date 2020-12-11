#pragma once

#include "../Maths/3dmaths.h"
#include "../Renderer/colour.h"

#include <string>
using namespace std;


enum TextEffect
{
	TextEffect_NoMovement = 0,
	TextEffect_VerticalBounce,
	TextEffect_RandomBounce,
	TextEffect_FadeUp,

	TextEffect_NumEffects,
};


enum TextDrawMode
{
	TextDrawMode_2D_Screen = 0,
	TextDrawMode_3D_Screen,
	TextDrawMode_3D_World,

	TextDrawEffect_NumDrawModes,
};

enum TextDrawStyle
{
	TextDrawStyle_Normal = 0,
	TextDrawStyle_Outline,

	TextDrawStyle_NumDrawStyles,
};

class AnimatedText
{
public:
	AnimatedText();
	~AnimatedText();

	void SetText(const string &lText);
	const string& GetText() const;

	void SetTextWidth(int lWidth);
	int GetTextWidth() const;

	// Playing effects
	void StartEffect();
	void StopEffect();
	void PauseEffect();
	void UnPauseEffect();

	// Deletion
	bool GetErased() const;
	void SetErased(bool lErase);

	void SetAutoDelete(bool lAutoDelete);

public:
	vec3 mPosition;
	vec3 mVelocity;

	Colour mColour;
	Colour mOutlineColour;

	unsigned int mFontID;
	unsigned int mOutlineFontID;
	unsigned int mViewportID;

	TextEffect mEffect;

	TextDrawMode mDrawMode;

	TextDrawStyle mDrawStyle;

	float mLifeTimeMax;
	float mLifeTime;

	float mScale;

	bool m_bStarted;
	bool m_bPaused;

	bool m_bErased;

	bool mAutoDelete;

private:
	string mText;

	int mTextWidth;
};
