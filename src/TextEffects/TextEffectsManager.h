#pragma once

#include "AnimatedText.h"
#include "Renderer/Renderer.h"
#include "Renderer/camera.h"

#include <vector>
#include <string>
#include <mutex>
using namespace std;

typedef vector<AnimatedText*> AnimatedTextList;


class TextEffectsManager
{
public:
	TextEffectsManager(Renderer* pRenderer);
	~TextEffectsManager();

	void SetCamera(Camera* pCamera);

	AnimatedText* CreateTextEffect(unsigned int lFontID, unsigned int lOutlineFontID, unsigned int lViewportID, TextDrawMode lDrawMode, TextEffect lEffect, TextDrawStyle lDrawStyle, const vec3& lPosition, const Colour& lColour, const Colour& lOutlineColour, const string &lText, float lifeTime);

	void Update(float lDeltaTime);
	void Render();

private:
	Renderer* mpRenderer;
	Camera* mpCamera;
	
	mutex m_animatedTextMutexLock;
	AnimatedTextList m_vpAnimatedTextList;
};
