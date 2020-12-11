// ******************************************************************************
// Filename:    SceneryManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 04/10/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../blocks/ChunkManager.h"
#include "../Renderer/Renderer.h"


class SceneryObject
{
public:
	string m_name;
	string m_objectFilename;
	vec3 m_positionOffset;
	vec3 m_worldFileOffset;
	QubicleImportDirection m_importDirection;
	QubicleImportDirection m_parentImportDirection;

	QubicleBinary* m_pQubicleBinaryFile;
	float m_length;
	float m_height;
	float m_width;

	float m_scale;
	float m_rotation;

	float m_radius;

	bool m_canSelect;

	bool m_outlineRender;
	bool m_hoverRender;

	vec2 m_screenPosition;
};

typedef std::vector<SceneryObject*> SceneryObjectList;


class SceneryManager
{
public:
	/* Public methods */
	SceneryManager(Renderer* pRenderer, ChunkManager* pChunkManager);
	~SceneryManager();

	void ClearSceneryObjects();

	int GetNumSceneryObjects();
	void ResetNumRenderSceneryCounter();
	int GetNumRenderSceneryObjects();
	SceneryObject* GetSceneryObject(int index);
	SceneryObject* GetSceneryObject(string name);
	SceneryObject* AddSceneryObject(string name, string filename, const vec3& pos, const vec3& worldFileOffset, QubicleImportDirection importDirection, QubicleImportDirection parentImportDirection, float scale, float rotation);
	SceneryObject* AddSceneryObject(string name, string filename, const vec3& pos, const vec3& worldFileOffset, QubicleImportDirection importDirection, QubicleImportDirection parentImportDirection, QubicleBinary* pQubicleBinaryFile, float length, float height, float width, float scale, float rotation);
	void DeleteSceneryObject(string name);

	void DeleteLayout(string name);
	void UpdateLayoutPosition(string name, vec3 newPosition);
	void UpdateLayoutDirection(string name, QubicleImportDirection direction);

	// Render modes
	void SetRenderOutlines(bool outlines);
	void SetRenderLabels(bool labels);

	// Updating
	void Update(float dt);

	// Rendering
	void Render(bool reflection, bool silhouette, bool shadow, bool renderOnlyOutline, bool renderOnlyNormal);
	void RenderDebug();
	void RenderSceneryObject(SceneryObject* pSceneryObject, bool outline, bool reflection, bool silhouette, bool boundingBox, bool shadow);
	void RenderOutlineScenery();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;
	ChunkManager* m_pChunkManager;

	SceneryObjectList m_vpSceneryObjectList;

	int m_numRenderScenery;

	bool m_renderOutlines;
	bool m_renderLabels;
};
