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

	Button buttonOk;
	TextDraw text_buttonOK;

	bool m_bShow = false;


public:
	Quest( );
	~Quest( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );

	void SetQuest(int questType);
	void SelectOK(bool& ok) { buttonOk.ClickAction(ok); }

	void ShowQuest(bool show) { m_bShow = show; }
};

