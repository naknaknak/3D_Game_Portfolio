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
	//text_buttonOK.SetText("- �� �� -");

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
	//	���� ����Ʈ : �� ���忡�� ���ҵ��� Ż�� �߾�, ���� ���ҵ��� ȥ����~!!(������ ����(�߰�) NPC����)
	//	NPC : 164 / 7 / -204(���� ����)
	//
	//	�߰� ���� ����Ʈ : (���ҵ��� Ż���� ������ ��Ÿ����.) ����Ʈ ����!!������ ȥ�����!!
	//	NPC : �߰����� ����(145 / 7 / -72) (��� ��� ����Ʈ ����)
	//
	//	���� ����Ʈ : (���ο� ���) ���� ������ ����!!"���� ���Ҹ� ������ ���� �� �� ����. �����!! 
	//	NPC : 173 / 7 / -91 (���� �Ա�, ���� ����)
	//
	char* strTitle = nullptr;
	char* strQuest = nullptr;

	type = (QuestType)questType;

	switch (type)
	{
		case QUEST_START:
		{
			strQuest = "�� ���忡�� ���ҵ��� Ż�� �߾�, \n\n���� ���ҵ��� ȥ����~!!";
			strTitle = "����Ʈ : ���� Ż��";
			break;
		}
		case QUEST_MIDDLE:
		{
			strQuest = "����Ʈ ����! \n\n���ҵ��� Ż���� ������ ��Ÿ����! \n\n��ġ ū �� ���Ҹ� ȥ������!!!";
			strTitle = "����Ʈ : ���� Ż���� ����";
			break;
		}
		case QUEST_END:
		{
			strQuest = "���� ������ ����!! \n\n���� ���Ҹ� ������ ���� �� �� ����. \n\n���� ���Ҹ� ��� ����踦 ������~!!";
			strTitle = "����Ʈ : ���ο� ���";
			break;
		}
		default:
		{
			assert(0); // ����Ʈ �ƴ�?
		}
	}

	text_title.SetText(strTitle);
	text_body.SetText(strQuest);
	
}
