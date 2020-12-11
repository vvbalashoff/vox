#include "AnimatedText.h"


AnimatedText::AnimatedText()
{
	mScale = 1.0f;

	m_bStarted = false;
	m_bPaused = false;

	mAutoDelete = false;
}

AnimatedText::~AnimatedText()
{
}

void AnimatedText::SetText(const std::string &lText)
{
	mText = lText;

	// TODO : URGENT - Also need to work out the text width again!!
	// But need the renderer... ahhhh!
}

const std::string& AnimatedText::GetText() const
{
	return mText;
}

void AnimatedText::SetTextWidth(int lWidth)
{
	mTextWidth = lWidth;
}

int AnimatedText::GetTextWidth() const
{
	return mTextWidth;
}

// Playing effects
void AnimatedText::StartEffect()
{
	m_bStarted = true;
}

void AnimatedText::StopEffect()
{
	m_bStarted = false;
}

void AnimatedText::PauseEffect()
{
	m_bPaused = true;
}

void AnimatedText::UnPauseEffect()
{
	m_bPaused = false;
}

// Deletion
bool AnimatedText::GetErased() const
{
	return m_bErased;
}

void AnimatedText::SetErased(bool lErase)
{
	m_bErased = lErase;
}

void AnimatedText::SetAutoDelete(bool lAutoDelete)
{
	mAutoDelete = lAutoDelete;
}
