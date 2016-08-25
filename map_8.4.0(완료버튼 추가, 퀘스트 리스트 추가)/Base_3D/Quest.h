#pragma once

#include "image.h"
#include "TextDraw.h"
#include "Button.h"

enum QuestType
{
	QUEST_START = 0,
	QUEST_MIDDLE,
	QUEST_END,
	QUEST_NUM
};

class Quest
{
public:

private:
	QuestType type = QUEST_START;

	image* imageBack = nullptr;
	TextDraw text_title;
	TextDraw text_body;
	TextDraw text_list;
	TextDraw text_list1;

	Button buttonOk; 
	Button buttonCompleteOk;
	//TextDraw text_buttonOK;


	bool m_bShow = false;
	bool notLook_QuestBody = false;
	bool bShowCompleteOK = false;
	bool notLook_CompleteOK = false;

public:
	Quest( );
	~Quest( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );

	void SetCount_QuestListShow(int count_element);
	void AcceptQuest(bool& ok) { buttonOk.ClickAction(ok); }
	void ShowQuest(bool show) { m_bShow = show; }

	void SetQuest(int questType);
	void ShowCompleteOK(bool completeQuest) { bShowCompleteOK = completeQuest; };

};

