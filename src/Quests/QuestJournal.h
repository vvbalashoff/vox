// ******************************************************************************
// Filename:    QuestJournal.h
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

class QuestManager;
class Player;


enum QuestEntryStatus
{
	QuestEntryStatus_Uncompleted = 0,
	QuestEntryStatus_Completed,
};

class QuestJournalEntry
{
public:
	Quest* m_pQuest;

	QuestEntryStatus m_status;
};


typedef std::vector<QuestJournalEntry*> QuestJournalEntryList;


class QuestJournal
{
public:
	/* Public methods */
	QuestJournal(QuestManager* pQuestManager);
	~QuestJournal();

	void ClearJournal();

	void SetPlayer(Player* pPlayer);

	void AddQuestJournalEntry(Quest* pQuest);
	
	void UpdateQuestJournalEntry(Quest* pQuest);

	void CompleteAllCurrentQuests();

	int GetNumCurrentQuests();
	int GetNumCompletedQuests();
	Quest* GetCurrentQuest(int index);
	Quest* GetCompletedQuest(int index);

	void ExportQuestJournal(int playerNum);
	void ImportQuestJournal(int playerNum);

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
	QuestManager* m_pQuestManager;
	Player* m_pPlayer;

	QuestJournalEntryList m_vpQuestJournalList;
};
