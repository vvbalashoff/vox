// ******************************************************************************
// Filename:    3dGeometry.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   3d geometry functionality.
//
// Revision History:
//   Initial Revision - 03/08/08
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include <glm/vec3.hpp>
using namespace glm;


class Plane3D
{
public:
	// Constructors
	Plane3D();
	Plane3D(const vec3& lNormal, const vec3& lPoint);
	Plane3D(const vec3& lv1, const vec3& lv2, const vec3& lv3);
	Plane3D(float a, float b, float c, float d);

	// Operations
	float GetPointDistance(const vec3& lPoint) const;

public:
	vec3 mPoint;
	vec3 mNormal;
	float d;
};


class Line3D
{
public:
	// Constructors
	Line3D();
	Line3D(const vec3& lStart, const vec3& lEnd);
	Line3D(float x1, float y1, float z1, float x2, float y2, float z2);

	// Properties
	vec3 GetMidPoint() const;
	vec3 GetVector() const;
	float GetLength() const;
	float GetLengthSquared() const;

	// Operations
	vec3 GetInterpolatedPoint(float t) const;

public:
	vec3 mStartPoint;
	vec3 mEndPoint;
};


class Bezier3
{
public:
	// Constructors
	Bezier3();
	Bezier3(const vec3& lStart, const vec3& lEnd, const vec3& lControl);
	Bezier3(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float xControl, float yControl, float zControl);

	// Operations
	vec3 GetInterpolatedPoint(float t) const;

public:
	vec3 mStartPoint;
	vec3 mEndPoint;
	vec3 mControlPoint;
};


class Bezier4
{
public:
	// Constructors
	Bezier4();
	Bezier4(const vec3& lStart, const vec3& lEnd, const vec3& lControl1, const vec3& lControl2);
	Bezier4(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float xControl1, float yControl1, float zControl1, float xControl2, float yControl2, float zControl2);

	// Operations
	vec3 GetInterpolatedPoint(float t) const;

public:
	vec3 mStartPoint;
	vec3 mEndPoint;
	vec3 mControlPoint1;
	vec3 mControlPoint2;
};
