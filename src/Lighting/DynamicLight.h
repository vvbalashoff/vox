// ******************************************************************************
// Filename:    DynamicLight.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 20/01/13
//
// Copyright (c) 2005-2011, Steven Ball
// ******************************************************************************

#pragma once

#include "../Renderer/colour.h"
#include <glm/vec3.hpp>
using namespace glm;


class DynamicLight
{
public:
	/* Public methods */
	DynamicLight();
	~DynamicLight();

	void SetErase(bool erase);
	bool NeedsErasing();

	void Update(float dt);

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */
	unsigned int m_lightId;

	vec3 m_position;

	float m_radius;

	float m_diffuseScale;

	Colour m_colour;

	float m_lifeTime;
	float m_maxLifeTime;
	bool m_dyingLight;

protected:
	/* Protected members */

private:
	/* Private members */
	bool m_erase;
};
