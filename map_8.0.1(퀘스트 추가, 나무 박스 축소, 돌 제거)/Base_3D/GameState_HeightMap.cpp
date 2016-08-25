#include "stdafx.h"
#include "GameState_HeightMap.h"
#include "Tree.h"


/* Debugging Key */
// F2 : 지면 와이어프레임.(LOD 확인용)
// F3 : 나무, 돌 와이어프레임
// F4 : 나무, 돌 경계상자 와이어프레임
// F5 : 나무, 돌 경계구 와이어프레임
// F6 : 스킬 경계구, 상자;


GameState_HeightMap::GameState_HeightMap( )
{
}


GameState_HeightMap::~GameState_HeightMap( )
{
}	

void GameState_HeightMap::Initialize( )
{
	if (sky == nullptr)
	{
		sky = new SkyBox;
		sky->Initialize("cloud_1.png", 2048, 512, 128);
		//sky->Initialize("cloud_1.png", 4096);
	}
	if (heightMap == nullptr)
	{
		heightMap = new HeightMap;
		heightMap->Initialize("field_.raw", "field_1.png");
		GameManager::GetCamera()->SetHeightMap(heightMap);
		//heightMap->Initialize("field_.raw", "field_1.png", 8, HEIGHT_FLAG::HM_NORMAL);
	}
	if (rock == nullptr)
	{
		rock = new Rocks_save( );
		rock->Initialize( );
	}
	if (trees == nullptr)
	{
		trees = new PlantTree( );
		trees->SetHeightMap(heightMap);
		trees->Initialize( );
	}

	if (lilith == nullptr)
	{ 
		lilith = new Player();
		lilith->Initialize("lilith/", "lilith.X");
		
		lilith->SetPosition(D3DXVECTOR3(193.0f, 12.0f, -306.0f));
		lilith->SetTrees(trees->GetTrees());
		
		lilith->SetHeightMap(heightMap); 
		lilith->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
 		GameManager::GetCamera()->SetLookTarget(lilith->GetPositionAddress());
		GameManager::GetCamera()->SetDistance(20.0f);
	}
	if (statusBar == nullptr)
	{
		statusBar = new ChracterStatusBar;
		statusBar->Initialize( );
	}
	if (quest == nullptr)
	{
		quest = new Quest;
		quest->Initialize();
	}

	Monster* monster = nullptr;
	monster = new Minion( );
	monster->SetInitialPosition(D3DXVECTOR3(401, 22, -292));
	monster->Initialize("Goat/", "Goat.x");
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monster->SetPlayer(lilith);
	monsters.push_back(monster);


	monster = nullptr;
	monster = new Minion( );
	monster->SetInitialPosition(D3DXVECTOR3(401, 22, -299));
	monster->Initialize("Goat/", "Goat.x");
	monster->SetMonsters(monsters);
	monster->SetHeightMap(heightMap);
	monster->SetPlayer(lilith);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monsters.push_back(monster);

	monster = nullptr;
	monster = new Minion( );
	monster->SetInitialPosition(D3DXVECTOR3(408, 22, -292));
	monster->Initialize("Goat/", "Goat.x");
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monster->SetPlayer(lilith);
	monster->SetMonsters(monsters);
	monsters.push_back(monster);

	monster = nullptr;
	monster = new Minion( );
	monster->SetInitialPosition(D3DXVECTOR3(399, 22, -290));
	monster->Initialize("Goat/", "Goat.x");
	monster->SetMonsters(monsters);
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monster->SetPlayer(lilith);
	monsters.push_back(monster);

}

void GameState_HeightMap::Destroy( )
{
	for (auto iter = monsters.begin( ); iter != monsters.end( ); iter++)
	{
		SAFE_DELETE(*iter);
	}
	monsters.clear( );
//	SAFE_DELETE(goat);
	SAFE_DELETE(trees);
	SAFE_DELETE(rock);
	SAFE_DELETE(heightMap);
	SAFE_DELETE(sky);
	SAFE_DELETE(statusBar);
}

void GameState_HeightMap::Reset( )
{
}
void GameState_HeightMap::Update( )
{
	if (sky) sky->Update( );
/*	if (goat)
	{
		goat->Update( );
		float damage = 0.f;
		BoundingSphere goat_boundingSphere = *goat->GetBoundingSphere( );
		std::vector<Tree*>& vTrees = trees->GetTrees( );
		for (auto iter = vTrees.begin( ); iter != vTrees.end( ); ++iter)
		{
			bool aa = Collision::IsBoxToSphere((*iter)->GetBoundingBox( ), *goat->GetBoundingSphere( ) );
			if (Collision::IsBoxToSphere((*iter)->GetBoundingBox( ), *goat->GetBoundingSphere( )))
			{
				(*iter)->SetBoundingBoxMaterialColor(D3DXCOLOR(0.f, 0.f, 0.8f, 1.0f));

				goat->DealDamage(goat, 2);

				quest->SetQuest(0);
				quest->ShowQuest(true);
				quest->SelectOK(selectOK[QUEST_START]);
			}
			else
			{
				(*iter)->SetBoundingBoxMaterialColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}

		statusBar->GetPlayerHP(*goat->GetPlayerHP( ), 256.f);
	}*/
	if (lilith)
	{
		lilith->Update();
		statusBar->GetPlayerHP(*lilith->GetPlayerHP(), 100.0f);
	}

	for (auto iter = monsters.begin( ); iter != monsters.end( ); iter++)
	{
		(*iter)->Update( );
	}

	if (statusBar)statusBar->Update( );
	if (quest)	quest->Update( );

	DebuggingKey( );
}

void GameState_HeightMap::Render( )
{
	if (sky) sky->Render( );
	if (rock) rock->Render( );
	if (heightMap) heightMap->Render( );

	if (lilith)
	{
		lilith->Render( );
	}
	for (auto iter = monsters.begin( ); iter != monsters.end( ); iter++)
	{
		(*iter)->Render( );
	}
	if (trees) trees->Render( );
	if (statusBar)statusBar->Render( );
	if (quest)	quest->Render( );

}
void GameState_HeightMap::Debugging( )
{
	char str[256];
	RECT rc = { RESOLUTION_X * 0.5, 90, 10, 10 };
	sprintf_s(str, "Quest: 0: %d / 1: %d / 2: %d", selectOK[QUEST_START], selectOK[QUEST_MIDDLE], selectOK[QUEST_END]);
	UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(50, 50, 255));

}
void GameState_HeightMap::DebuggingKey( )
{
	if (quest)
	{
		if (GetAsyncKeyState('1') & 0x0001)
		{
			quest->SetQuest(0);
			quest->ShowQuest(true);
			quest->SelectOK(selectOK[QUEST_START]);
		}
		if (GetAsyncKeyState('2') & 0x0001)
		{
			quest->SetQuest(1);
			quest->ShowQuest(true);
			quest->SelectOK(selectOK[QUEST_MIDDLE]);
		}
		if (GetAsyncKeyState('3') & 0x0001)
		{
			quest->SetQuest(2);
			quest->ShowQuest(true);
			quest->SelectOK(selectOK[QUEST_END]);
		}
	}

	if (heightMap)
	{
		heightMap->DebuggingKey( );
	}
	if (GetAsyncKeyState(VK_F3) & 0x0001)
	{
		static int count_WireMode = 0;
		++count_WireMode;
		if (count_WireMode % 2 == 0)
		{
			count_WireMode = 0;
			if (trees) trees->SetWireMode(true);
			if (rock) rock->SetWireMode(true);
		}
		else
		{
			if (trees)trees->SetWireMode(false);
			if (rock) rock->SetWireMode(false);
		}
		if (trees) trees->Update( );
		if (rock)  rock->Update( );
	}

	if (GetAsyncKeyState(VK_F4) & 0x0001)
	{
		static int count_ColBoxDraw = 0;

		++count_ColBoxDraw;
		if (count_ColBoxDraw % 2 == 0)
		{
			count_ColBoxDraw = 0;
			if (trees)trees->SetIsColliderBoxDraw(true);
			if (rock) rock->SetIsColliderBoxDraw(true);
		}
		else
		{
			if (trees)trees->SetIsColliderBoxDraw(false);
			if (rock) rock->SetIsColliderBoxDraw(false);
		}
		if (trees) trees->Update( );
		if (rock)  rock->Update( );
	}

	if (GetAsyncKeyState(VK_F5) & 0x0001)
	{
		static int count_ColSphereDraw = 0;

		++count_ColSphereDraw;
		if (count_ColSphereDraw % 2 == 0)
		{
			count_ColSphereDraw = 0;
			if (trees)trees->SetIsColliderSphereDraw(true);
			if (rock) rock->SetIsColliderSphereDraw(true);
		}
		else
		{
			if (trees)trees->SetIsColliderSphereDraw(false);
			if (rock) rock->SetIsColliderSphereDraw(false);
		}
		if (trees) trees->Update( );
		if (rock)  rock->Update( );
	}
}

void GameState_HeightMap::OnEnterState( )
{
	GameManager::GetCamera( )->SetDistance(10.0f);
	Initialize( );
}

void GameState_HeightMap::OnLeaveState( )
{
	Destroy( );
}

LRESULT GameState_HeightMap::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	//if ( result == 0 && girl )
	//{
	//	girl->InputProc(hWnd, message, wParam, lParam);
	//}
	return result;
}
