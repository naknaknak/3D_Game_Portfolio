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

	//����Ʈ ������ ǥ��
	//���� ����.ī��Ʈ
	text_list.init( );
	text_list.SetPosition(D3DXVECTOR3(RESOLUTION_X - 250, 145, 0));
	text_list.SetDrawSize(imageBack->GetSize( ));
	text_list.SetTextColor(D3DCOLOR_XRGB(22, 0xff, 0xff));

	text_list1.init( );
	text_list1.SetPosition(D3DXVECTOR3(RESOLUTION_X - 250, 170, 0));
	text_list1.SetDrawSize(imageBack->GetSize( ));
	text_list1.SetTextColor(D3DCOLOR_XRGB(0xff, 0xff, 0xff));

	//����Ʈ �Ϸ� ��ư
	buttonCompleteOk.Initialize(
		"./UI/btn-med-up.png", "./UI/btn-med-over.png", "./UI/btn-med-down.png",
		D3DXVECTOR3(RESOLUTION_X * 0.5f, RESOLUTION_Y * 0.5f, 0));
	buttonCompleteOk.SetText("- �� �� Ʈ �� �� -");

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
	text_list1.Update( );
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
		text_list1.Render( );
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
	char* strlist_Body = nullptr;

	type = (QuestType)questType;

	switch (type)
	{
		case QUEST_START:
		{
			strQuest = "�� ���忡�� ���ҵ��� Ż�� �߾�, \n\n���� ���ҵ��� ȥ����~!!";
			strTitle = "����Ʈ : ���� Ż��";
			strlist_Body = "���� ���� : ";
			break;
		}
		case QUEST_MIDDLE:
		{
			strQuest = "����Ʈ ����! \n\n���ҵ��� Ż���� ������ ��Ÿ����! \n\n��ġ ū �� ���Ҹ� ȥ������!!!";
			strTitle = "����Ʈ : ���� Ż���� ����";
			strlist_Body = "�� ���� : ";
			break;
		}
		case QUEST_END:
		{
			strQuest = "���� ������ ����!! \n\n���� ���Ҹ� ������ ���� �� �� ����. \n\n���� ���Ҹ� ��� ����踦 ������~!!";
			strTitle = "����Ʈ : ���ο� ���";
			strlist_Body = "Brick óġ : ";
			break;
		}
		default:
		{
			assert(0); // ����Ʈ �ƴ�?
		}
	}

	text_title.SetText(strTitle);
	text_body.SetText(strQuest);
	text_list1.SetText(strlist_Body);

	notLook_QuestBody = false;
	notLook_CompleteOK = false;
}
void Quest::SetCount_QuestListShow(int count_element)
{
	std::string strListTitle = text_title.GetText( );
	text_list.SetText(strListTitle);

	std::string strListBody = text_list1.GetText( );
	char strCount[32];
	sprintf_s(strCount, "%d", count_element);
	strListBody += strCount;
	text_list1.SetText(strListBody);

}