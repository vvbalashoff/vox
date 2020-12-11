// ******************************************************************************
//
// Filename:	container.cpp
// Project:     Vox
// Author:		Steven Ball
//
// Purpose:
//   A container to hold multiple components. Inclues
//   functionality for adding and removing components.
//
// Revision History:
//   Initial Revision - 23/09/06
//
// Copyright (c) 2005-2006, Steven Ball
//
// ******************************************************************************

#pragma once

#include "component.h"


typedef std::vector<Component*> ComponentList;


class Container : public Component
{
public:
	/* Public methods */
	Container(Renderer* pRenderer);

	~Container();

	void AddEventListeners() override;
	void RemoveEventListeners() override;

	bool IsContainer() const override;
	bool IsRootContainer() const override;

	bool HasChildren() const;

	void SetEnabled(bool enable) override;

	void SetDebugRender(bool debug) override;

	void Add(Component* component);
	void Remove(Component* component);
	void RemoveAll();

	const ComponentList& GetChildren() const;
	Component* GetChild(int n) const;
	int GetNumberOfChildren() const;

	void DepthSortComponentChildren();

	EComponentType GetComponentType() const override;

  void SetAudio(bool set) override;
  void SetAudioVolume(float volume) override;

	void Update(float deltaTime) override;

protected:
	/* Protected methods */
	void DrawSelf() override;
	void DrawChildren() override;

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */
	ComponentList m_vpComponentList;

private:
	/* Private members */
};
