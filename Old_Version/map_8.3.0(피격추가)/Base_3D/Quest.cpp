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
	text_body.SetDrawSize(D3DXVECTOR2(imageBack->GetSize( ).x, 25));

	text_body.init( );
	text_body.SetPosition(D3DXVECTOR3(100, 150, 0));
	text_body.SetParent(imageBack->GetWorld( ));
	text_body.SetDrawSize(imageBack->GetSize( ));

	// button
	buttonOk.Initialize("./UI/btn-med-up.png", "./UI/btn-med-over.png", "./UI/btn-med-down.png", D3DXVECTOR3(imageBack->GetSize( ).x * 0.5f, imageBack->GetSize( ).y - 100, 0));
	buttonOk.SetParent(imageBack->GetWorld( ));

	//text_buttonOK.init( );
	////text_buttonOK.SetPosition(D3DXVECTOR3(position.x, position.y, 0));
	//text_buttonOK.SetParent(buttonOk.GetWorld());
	//text_buttonOK.SetPosition(D3DXVECTOR3(0, 0, 0));
	//text_buttonOK.SetDrawSize(buttonOk.GetSize());
	//text_buttonOK.SetText("- 수 락 -");

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
	
	buttonOk.Update( );
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

		bool notLook = false;
		buttonOk.ClickAction(notLook);
		if (notLook) m_bShow = false;
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

	type = (QuestType)questType;

	switch (type)
	{
		case QUEST_START:
		{
			strQuest = "내 목장에서 염소들이 탈출 했어, \n\n나쁜 염소들을 혼내줘~!!";
			strTitle = "퀘스트 : 염소 탈출";
			break;
		}
		case QUEST_MIDDLE:
		{
			strQuest = "퀘스트 갱신! \n\n염소들이 탈출한 원흉을 나타났다! \n\n덩치 큰 흰 염소를 혼내주자!!!";
			strTitle = "퀘스트 : 염소 탈출의 원흉";
			break;
		}
		case QUEST_END:
		{
			strQuest = "암흑 염소의 출현!! \n\n암흑 염소를 잡으면 돈이 될 것 같다. \n\n암흑 염소를 잡고 백년대계를 세우자~!!";
			strTitle = "퀘스트 : 새로운 사업";
			break;
		}
		default:
		{
			assert(0); // 퀘스트 아님?
		}
	}

	text_title.SetText(strTitle);
	text_body.SetText(strQuest);
	
}
