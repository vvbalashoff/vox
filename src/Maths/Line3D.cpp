// ******************************************************************************
// Filename:    Line3D.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   3D Line implementation.
//
// Revision History:
//   Initial Revision - 03/08/08
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "3dGeometry.h"
//#include <glm/detail/func_geometric.inl>
#include <glm/common.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


// Constructors
Line3D::Line3D()
{
	/* Nothing */
}

Line3D::Line3D(const vec3& lStart, const vec3& lEnd)
{
	mStartPoint = lStart;
	mEndPoint = lEnd;
}

Line3D::Line3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	mStartPoint = vec3(x1, y1, z1);
	mEndPoint = vec3(x2, y2, z2);
}

// Properties
vec3 Line3D::GetMidPoint() const
{
	return GetInterpolatedPoint(0.5f);
}

vec3 Line3D::GetVector() const
{
	return (mEndPoint - mStartPoint);
}

float Line3D::GetLength() const
{
	return length(mEndPoint - mStartPoint);
}

float Line3D::GetLengthSquared() const
{
	return length2(mEndPoint - mStartPoint);
}

// Operations
vec3 Line3D::GetInterpolatedPoint(float t) const
{
	vec3 lScale = (mEndPoint - mStartPoint) * t;

	return (mStartPoint + lScale);
}
