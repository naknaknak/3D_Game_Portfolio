#include "stdafx.h"
#include "Quest.h"

Quest::Quest( )
{
}

Quest::~Quest( )
{
}

void Quest::Initialize( )
{
	if (imageBack == nullptr)
	{
		imageBack = new image;
		imageBack->init("./UI/panel-info.png", D3DXVECTOR3(RESOLUTION_X * 0.8f, RESOLUTION_Y * 0.4f, 0));
	}

	text_title.init( );
	text_title.SetPosition(D3DXVECTOR3(70, 100, 0));
	text_title.SetParent(imageBack->GetWorld( ));
	text_title.SetDrawSize(D3DXVECTOR2(imageBack->GetSize( ).x, 25));

	text_body.init( );
	text_body.SetPosition(D3DXVECTOR3(100, 150, 0));
	text_body.SetParent(imageBack->GetWorld( ));
	text_body.SetDrawSize(imageBack->GetSize( ));

	// button
	buttonOk.Initialize(
		"./UI/btn-med-up.png", "./UI/btn-med-over.png", "./UI/btn-med-down.png",
		D3DXVECTOR3(imageBack->GetSize( ).x * 0.5f, imageBack->GetSize( ).y - 100, 0));
	buttonOk.SetParent(imageBack->GetWorld( ));

	//퀘스트 좌측에 표시
	//남은 염소.카운트
	text_list.init( );
	text_list.SetPosition(D3DXVECTOR3(RESOLUTION_X - 250, 145, 0));
	text_list.SetDrawSize(imageBack->GetSize( ));
	text_list.SetTextColor(D3DCOLOR_XRGB(22, 0xff, 0xff));

	text_listLabel.init( );
	text_listLabel.SetPosition(D3DXVECTOR3(RESOLUTION_X - 250, 170, 0));
	text_listLabel.SetDrawSize(imageBack->GetSize( ));
	text_listLabel.SetTextColor(D3DCOLOR_XRGB(0xff, 0xff, 0xff));

	text_listNum.init( );
	text_listNum.SetPosition(D3DXVECTOR3(RESOLUTION_X - 110, 170, 0));
	text_listNum.SetDrawSize(imageBack->GetSize( ));
	text_listNum.SetTextColor(D3DCOLOR_XRGB(0xff, 0xff, 0xff));

	//퀘스트 완료 버튼
	buttonCompleteOk.Initialize(
		"./UI/btn-med-up.png", "./UI/btn-med-over.png", "./UI/btn-med-down.png",
		D3DXVECTOR3(RESOLUTION_X * 0.5f, RESOLUTION_Y * 0.5f, 0));
	buttonCompleteOk.SetText("- 퀘 스 트 완 료 -");

	bShowCompleteOK = false;
}

void Quest::Destroy( )
{
	if (imageBack)
	{
		delete imageBack;
		imageBack = nullptr;
	}

}

void Quest::Update( )
{
	text_title.Update( );
	text_body.Update( );
	text_list.Update( ); 
	text_listLabel.Update( );
	text_listNum.Update( );
	buttonOk.Update( );
	buttonCompleteOk.Update( );
	//text_buttonOK.Update( );

}

void Quest::Render( )
{
	if (m_bShow)
	{
		imageBack->Render( );
		text_title.Render( );
		text_body.Render( );
		buttonOk.Render( );

		buttonOk.ClickAction(notLook_QuestBody);
		if (notLook_QuestBody) m_bShow = false;
	}
	if (notLook_QuestBody)
	{
		text_list.Render( );
		text_listLabel.Render( );
		text_listNum.Render( );
	}
	if (bShowCompleteOK)
	{
		buttonCompleteOk.Render( );

		buttonCompleteOk.ClickAction(notLook_CompleteOK);
		if (notLook_CompleteOK) bShowCompleteOK = false;

	}
}

void Quest::SetQuest(int questType)
{
	//	시작 퀘스트 : 내 목장에서 염소들이 탈출 했어, 나쁜 염소들을 혼내줘~!!(보상은 마을(중간) NPC에게)
	//	NPC : 164 / 7 / -204(염소 주인)
	//
	//	중간 보스 퀘스트 : (염소들이 탈출한 원흉이 나타난다.) 퀘스트 갱신!!원흉을 혼내줘라!!
	//	NPC : 중간보스 출현(145 / 7 / -72) (잡몹 잡고 퀘스트 갱신)
	//
	//	보스 퀘스트 : (새로운 사업) 암흑 염소의 출현!!"암흑 염소를 잡으면 돈이 될 것 같아. 잡아줘!! 
	//	NPC : 173 / 7 / -91 (마을 입구, 마을 이장)
	//
	char* strTitle = nullptr;
	char* strQuest = nullptr;
	char* strlist_Body = nullptr;

	type = (QuestType)questType;

	switch (type)
	{
		case QUEST_START:
		{
			strQuest = "내 목장에서 염소들이 탈출 했어, \n\n나쁜 염소들을 혼내줘~!!";
			strTitle = "퀘스트 : 염소 탈출";
			strlist_Body = "남은 염소 : ";
			break;
		}
		case QUEST_MIDDLE:
		{
			strQuest = "퀘스트 갱신! \n\n염소들이 탈출한 원흉이 나타났다! \n";
			strTitle = "퀘스트 : 염소 탈출의 원흉";
			strlist_Body = "Brick 처치 : ";
			break;
		}
		case QUEST_END:
		{
			strQuest = "암흑 염소의 출현!! \n\n암흑 염소를 잡으면 돈이 될 것 같다. \n\n암흑 염소를 잡고 백년대계를 세우자~!!";
			strTitle = "퀘스트 : 새로운 사업";
			strlist_Body = "Brick 처치 : ";
			break;
		}
		default:
		{
			assert(0); // 퀘스트 아님?
		}
	}

	text_title.SetText(strTitle);
	text_body.SetText(strQuest);
	text_listLabel.SetText(strlist_Body);

	notLook_QuestBody = false;
	notLook_CompleteOK = false;
}
void Quest::SetCount_QuestListShow(int& count_element)
{
	std::string strListTitle = text_title.GetText( );
	text_list.SetText(strListTitle);

	std::string strListBody = text_listLabel.GetText( );
	text_listNum.SetText(strListBody);

	char strCount[32];
	sprintf_s(strCount, "%d", count_element);
	text_listNum.SetText(strCount);

}