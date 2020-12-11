#include "TextEffectsManager.h"

#include "../utils/Random.h"
#include "../utils/Interpolator.h"


TextEffectsManager::TextEffectsManager(Renderer* pRenderer)
{
	mpRenderer = pRenderer;
	mpCamera = nullptr;
}

TextEffectsManager::~TextEffectsManager()
{
	AnimatedTextList::iterator iterator;

	// Erase all text effects
	m_animatedTextMutexLock.lock();
	for(iterator = m_vpAnimatedTextList.begin(); iterator != m_vpAnimatedTextList.end();)
	{
		if((*iterator)->mAutoDelete)
		{
			delete (*iterator);
		}

		iterator = m_vpAnimatedTextList.erase(iterator);
	}
	m_animatedTextMutexLock.unlock();
}

void TextEffectsManager::SetCamera(Camera* pCamera)
{
	mpCamera = pCamera;
}

AnimatedText* TextEffectsManager::CreateTextEffect(unsigned int lFontID, unsigned int lOutlineFontID, unsigned int lViewportID, TextDrawMode lDrawMode, TextEffect lEffect, TextDrawStyle lDrawStyle, const vec3& lPosition, const Colour& lColour, const Colour& lOutlineColour, const string &lText, float lifeTime)
{
	AnimatedText* lpEffect = new AnimatedText();

	// Actually create the text effect
	lpEffect->mPosition = lPosition;
	lpEffect->mColour = lColour;
	lpEffect->mOutlineColour = lOutlineColour;

	lpEffect->mEffect = lEffect;
	lpEffect->mDrawMode = lDrawMode;
	lpEffect->mDrawStyle = lDrawStyle;

	lpEffect->mFontID = lFontID;
	lpEffect->mOutlineFontID = lOutlineFontID;
	lpEffect->mViewportID = lViewportID;

	lpEffect->SetText(lText);

	lpEffect->m_bErased = false;

	lpEffect->SetTextWidth(mpRenderer->GetFreeTypeTextWidth(lpEffect->mFontID, "%s", lpEffect->GetText().c_str()));

	switch(lEffect)
	{
		case TextEffect_NoMovement:
		{
			lpEffect->mLifeTimeMax = lifeTime;
			lpEffect->mLifeTime = lifeTime;
			lpEffect->mScale = 1.0f;

			lpEffect->mVelocity = vec3(0.0f, 0.0f, 0.0f);
		}
		break;

		case TextEffect_VerticalBounce:
		{
			lpEffect->mLifeTimeMax = lifeTime;
			lpEffect->mLifeTime = lifeTime;
			lpEffect->mScale = 1.0f;

			lpEffect->mVelocity = vec3(0.0f, 3.0f, 0.0f);
		}
		break;

		case TextEffect_RandomBounce:
		{
			lpEffect->mLifeTimeMax = lifeTime;
			lpEffect->mLifeTime = lifeTime;
			lpEffect->mScale = 1.0f;

			float randomX = GetRandomNumber(-3, 3, 1) * 0.5f;
			float randomZ = GetRandomNumber(-3, 3, 1) * 0.5f;
			lpEffect->mVelocity = vec3(randomX, 3.0f, randomZ);
		}
		break;

		case TextEffect_FadeUp:
		{
			lpEffect->mLifeTimeMax = lifeTime;
			lpEffect->mLifeTime = lifeTime;
			
			lpEffect->mScale = 0.2f;
			Interpolator::GetInstance()->AddFloatInterpolation(&lpEffect->mScale, 0.2f, 1.0f, 0.1f, -100.0f);

			lpEffect->mVelocity = vec3(0.0f, 1.0f, 0.0f);
		}
		break;
	}

	// Push onto the vector list
	m_animatedTextMutexLock.lock();
	m_vpAnimatedTextList.push_back(lpEffect);
	m_animatedTextMutexLock.unlock();

	// Return a pointer to this particle effect
	return lpEffect;
}

void TextEffectsManager::Update(float lDeltaTime)
{
	AnimatedTextList::iterator iterator;

	// Update all effects
	m_animatedTextMutexLock.lock();
	for(iterator = m_vpAnimatedTextList.begin(); iterator != m_vpAnimatedTextList.end(); ++iterator)
	{
		AnimatedText* lpAnimatedText = (*iterator);

		if(!lpAnimatedText->m_bStarted)
		{
			// Dont update effects that have not started yet
			continue;
		}

		if(!lpAnimatedText->m_bPaused)
		{
			// If we are not paused then tick down the life timer
			lpAnimatedText->mLifeTime -= lDeltaTime;
		}

		if(lpAnimatedText->mLifeTime <= 0.0f)
		{
			// If we have elapsed our timer, then erase the effect and continue
			lpAnimatedText->SetErased(true);

			continue;
		}

		float lInterpolateRatio = lpAnimatedText->mLifeTime / lpAnimatedText->mLifeTimeMax;

		switch(lpAnimatedText->mEffect)
		{
			case TextEffect_NoMovement:
			{
				lpAnimatedText->mColour.SetAlpha(lInterpolateRatio);
				lpAnimatedText->mOutlineColour.SetAlpha(lInterpolateRatio);
			}
			break;

			case TextEffect_VerticalBounce:
			case TextEffect_RandomBounce:
			{
				// Simulate gravity
				lpAnimatedText->mVelocity += vec3(0.0f, -9.81f, 0.0f) * lDeltaTime;
				lpAnimatedText->mPosition += lpAnimatedText->mVelocity * lDeltaTime;

				lpAnimatedText->mColour.SetAlpha(lInterpolateRatio);
				lpAnimatedText->mOutlineColour.SetAlpha(lInterpolateRatio);
			}
			break;

			case TextEffect_FadeUp:
			{
				lpAnimatedText->mPosition += lpAnimatedText->mVelocity * lDeltaTime;

				lpAnimatedText->mColour.SetAlpha(lInterpolateRatio);
				lpAnimatedText->mOutlineColour.SetAlpha(lInterpolateRatio);
			}
			break;
		}
	}

	// Go through and erase any effects that are waiting to be erased
	for(iterator = m_vpAnimatedTextList.begin(); iterator != m_vpAnimatedTextList.end();)
	{
		if((*iterator)->GetErased())
		{
			if((*iterator)->mAutoDelete)
			{
				delete (*iterator);
			}

			iterator = m_vpAnimatedTextList.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
	m_animatedTextMutexLock.unlock();
}

void TextEffectsManager::Render()
{
	AnimatedTextList::const_iterator iterator;

	mpRenderer->PushMatrix();

		// Render all effects
		m_animatedTextMutexLock.lock();
		for(iterator = m_vpAnimatedTextList.begin(); iterator != m_vpAnimatedTextList.end(); ++iterator)
		{
			AnimatedText* lpAnimatedText = (*iterator);

			if(!lpAnimatedText->m_bStarted)
			{
				// Dont render effects that have not started yet
				continue;
			}

			vec3 lpTextPosition;
			if(lpAnimatedText->mDrawMode == TextDrawMode_2D_Screen)
			{
				lpTextPosition = lpAnimatedText->mPosition;
			}
			else if(lpAnimatedText->mDrawMode == TextDrawMode_3D_Screen)
			{
				if(mpRenderer->PointInFrustum(lpAnimatedText->mViewportID, lpAnimatedText->mPosition) == false)
				{
					continue;
				}

				mpRenderer->PushMatrix();		
					mpRenderer->SetProjectionMode(PM_PERSPECTIVE, lpAnimatedText->mViewportID);
					mpCamera->Look();

					int winx, winy;
					mpRenderer->GetScreenCoordinatesFromWorldPosition(lpAnimatedText->mPosition, &winx, &winy);

				mpRenderer->PopMatrix();

				lpTextPosition = vec3((float)winx - lpAnimatedText->GetTextWidth()/2, (float)winy, 0.0f);
			}
			else if(lpAnimatedText->mDrawMode == TextDrawMode_3D_World)
			{
				mpRenderer->SetProjectionMode(PM_PERSPECTIVE, lpAnimatedText->mViewportID);
				mpCamera->Look();

				lpTextPosition = lpAnimatedText->mPosition;
			}

			mpRenderer->PushMatrix();
				mpRenderer->SetRenderMode(RM_SOLID);
				mpRenderer->SetProjectionMode(PM_2D, lpAnimatedText->mViewportID);
				mpRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

				// Draw styles
				mpRenderer->RenderFreeTypeText(lpAnimatedText->mFontID, lpTextPosition.x, lpTextPosition.y, 0.0f, lpAnimatedText->mColour, lpAnimatedText->mScale, "%s", lpAnimatedText->GetText().c_str());

				if(lpAnimatedText->mDrawStyle == TextDrawStyle_Outline)
				{
					mpRenderer->RenderFreeTypeText(lpAnimatedText->mOutlineFontID, lpTextPosition.x, lpTextPosition.y, 0.0f, lpAnimatedText->mOutlineColour, lpAnimatedText->mScale, "%s", lpAnimatedText->GetText().c_str());
				}

			mpRenderer->PopMatrix();
		}
		m_animatedTextMutexLock.unlock();

	mpRenderer->PopMatrix();
}
