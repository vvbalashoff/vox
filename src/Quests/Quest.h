// ******************************************************************************
// Filename:    Quest.h
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

#include "../Enemy/Enemy.h"
#include "../Items/ItemsEnum.h"

#include <vector>
#include <string>
using namespace std;


enum QuestType
{
    QuestType_CollectX = 0,
    QuestType_KillX,
    QuestType_TalkToPerson,
    QuestType_FindPerson, // TODO : Implement - QuestType_FindPerson
    QuestType_ExplorePlace, // TODO : Implement - QuestType_ExplorePlace
    QuestType_PlaceXScenery,

    QuestType_NUM,
};

class NPC;
class Quest;
class NPCManager;
class InventoryItem;
class InventoryManager;
class QuestJournal;

class QuestObjective
{
public:
	void Reset();

	void UpdateObjective();

	string m_objectiveText;

	QuestType m_questType;

	int m_numberOfX;
	int m_progressX;

	bool m_completed;

    // The enemy to kill
	eEnemyType m_enemie;

    // The item to collect
    eItem m_item;

	// Person to talk to
	NPC* m_talkNPC;

	// The dialog that this talking NPC should say
	string m_talkingNPCDialog;

	// The scenery placement item
	InventoryItem* m_placementItem;

	Quest* m_pParent;
};

typedef std::vector<QuestObjective*> QuestObjectiveList;


class Quest
{
public:
	/* Public methods */
	Quest(string name, string startText, string completedText, string denyText);
	~Quest();

	void SetNPCManager(NPCManager* pNPCManager);
	void SetInventoryManager(InventoryManager* pInventoryManager);
	void SetQuestJournal(QuestJournal* pQuestJournal);

	void SetNPCGiver(NPC* npcGiver);

	void SetQuestReward(InventoryItem* pReward);

	void ClearObjectives();

	void AddQuestObjective(string objectiveText, QuestType type, int numX, eEnemyType enemie, eItem item, NPC* talkNPC, string talkingNPCDialog, InventoryItem* placementItem);

	void ExportQuest();
	void ImportQuest(const char* filename);

	void CompleteQuest();

	bool CanAcceptQuest(string *acceptError);
	void AcceptQuest();

	void Reset();

	void Update();

    bool GetCompleted();

    string GetName();

    string GetStartText();

	string GetCompletedText();

	string GetDenyText();

	int GetNumObjectives();
    QuestObjective* GetObjective(int index);

	NPC* GetNPC();

	InventoryItem* GetReward();

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
	bool m_erase;

    bool m_completed;

    string m_questName;

    string m_questStartText;
	string m_questCompletedText;
	string m_questDenyText;

	QuestObjectiveList m_vpObjectives;

	InventoryItem* m_pQuestReward;

	NPC* m_pNPC;

	NPCManager* m_pNPCManager;
	InventoryManager* m_pInventoryManager;
	QuestJournal* m_pQuestJournal;
};
