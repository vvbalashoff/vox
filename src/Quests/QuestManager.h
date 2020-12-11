// ******************************************************************************
// Filename:    QuestManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 14/08/12
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "Quest.h"
#include "../Enemy/Enemy.h"

class NPCManager;
class InventoryManager;
class QuestJournal;

typedef std::vector<Quest*> QuestList;


class QuestManager
{
public:
	/* Public methods */
	QuestManager();
	~QuestManager();

	void SetNPCManager(NPCManager* pNPCManager);
	void SetInventoryManager(InventoryManager* pInventoryManager);
	void SetQuestJournal(QuestJournal* pQuestJournal);

    void ClearRandomEnemyList();
    void AddRandomEnemyTypeToList(eEnemyType enemy);

    void ClearQuests();

    Quest* CreateQuest(string name, string startText, string completedText, string denyText);
	Quest* CreateRandomQuest();

    Quest* GetQuest(string name);

    void Update(float dt);

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
    QuestList m_vpQuestList;

    vector<eEnemyType> m_vRandomEnemyList;

	NPCManager* m_pNPCManager;
	InventoryManager* m_pInventoryManager;
	QuestJournal* m_pQuestJournal;
};
