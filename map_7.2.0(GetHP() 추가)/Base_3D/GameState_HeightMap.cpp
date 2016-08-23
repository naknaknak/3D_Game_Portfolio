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
		sky->Initialize("cloud_1.png", 4096);
	}
	//if (grid == nullptr)
	//{
	//	grid = new Grid;
	//	grid->Initialize( );
	//	grid->SetShowArrow(true);
	//	grid->SetShowGround(true);
	//}
	if (heightMap == nullptr)
	{
		heightMap = new HeightMap;
		heightMap->Initialize("field_.raw", "field_1.png");
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

	if (goat == nullptr)
	{
		goat = new Player( );
		goat->Initialize("Goat/", "Goat.X");
	
		goat->SetPosition(D3DXVECTOR3(193.0f, 12.0f, -306.0f));
		goat->SetTrees(trees->GetTrees( ));
	
		goat->SetHeightMap(heightMap);
		goat->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		GameManager::GetCamera( )->SetLookTarget(goat->GetPositionAddress( ));
	}
	if (statusBar == nullptr)
	{
		statusBar = new ChracterStatusBar;
		statusBar->Initialize( );
	}



	Monster* monster = nullptr;
	monster = new Minion( );
	monster->Initialize("Goat/", "Goat_white.x");
	monster->SetPosition(D3DXVECTOR3(401, 22, -292));
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monsters.push_back(monster);


	monster = nullptr;
	monster = new Minion( );
	monster->Initialize("Goat/", "Goat_white.x");
	monster->SetPosition(D3DXVECTOR3(401, 22, -299));
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monsters.push_back(monster);

	monster = nullptr;
	monster = new Minion( );
	monster->Initialize("Goat/", "Goat_white.x");
	monster->SetPosition(D3DXVECTOR3(408, 22, -292));
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monsters.push_back(monster);

	monster = nullptr;
	monster = new Minion( );
	monster->Initialize("Goat/", "Goat_white.x");
	monster->SetPosition(D3DXVECTOR3(399, 22, -290));
	monster->SetHeightMap(heightMap);
	monster->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
	monsters.push_back(monster);

}

void GameState_HeightMap::Destroy( )
{
	for (auto iter = monsters.begin( ); iter != monsters.end( ); iter++)
	{
		SAFE_DELETE(*iter);
	}
	monsters.clear( );
	SAFE_DELETE(zombie);
	SAFE_DELETE(goat);
	/*SAFE_DELETE(girl);*/
	SAFE_DELETE(trees);
	SAFE_DELETE(rock);
	SAFE_DELETE(heightMap);
	SAFE_DELETE(grid);
	SAFE_DELETE(sky);
	SAFE_DELETE(statusBar);
}

void GameState_HeightMap::Reset( )
{
}
void GameState_HeightMap::Update( )
{
	if (sky)
	{
		sky->Update( );
	}

	if (girl)
	{
		//girl->Update();
		//girl->GroundCheck();
	}
	if (goat)
	{
		goat->Update( );
	}
	for (auto iter = monsters.begin( ); iter != monsters.end( ); iter++)
	{
		(*iter)->Update( );
	}

	if (statusBar)
	{
		statusBar->Update( );
	}


	DebuggingKey( );
}

void GameState_HeightMap::Render( )
{
	if (sky)
	{
		sky->Render( );
	}
	if (grid)
	{
		grid->Render( );
	}
	if (rock)
	{
		rock->Render( );
	}
	if (heightMap)
	{
		heightMap->Render( );
	}
	if (girl)
	{
		girl->Render( );
	}
	if (goat)
	{
		float damage = 0.f;
		goat->Render( );
		BoundingSphere goat_boundingSphere = *goat->GetBoundingSphere( );
		std::vector<Tree*>& vTrees = trees->GetTrees( );
		for (auto iter = vTrees.begin( ); iter != vTrees.end( ); ++iter)
		{
			bool aa = Collision::IsSphereToSphere(*goat->GetBoundingSphere( ), (*iter)->GetBoundingSphere( ));
			if (Collision::IsSphereToSphere(*goat->GetBoundingSphere( ), (*iter)->GetBoundingSphere( )))
			{
				int n = 0;

				(*iter)->SetBoundingSphereMaterialColor(D3DXCOLOR(0.f, 0.f, 0.8f, 1.0f));
				goat->DealDamage(goat, 10);
			}
			else
			{
				(*iter)->SetBoundingSphereMaterialColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}

		statusBar->GetPlayerHP(*goat->GetPlayerHP( ), 100.f );

	}
	/*if (zombie)
	{
		zombie->UpdateAndRender( );
	}*/
	if (trees)
	{
		trees->Render( );
	}
	for (auto iter = monsters.begin( ); iter != monsters.end( ); iter++)
	{
		(*iter)->Render( );
	}


	if (statusBar)
	{
		statusBar->Render( );
	}



	RECT rc = { 10, RESOLUTION_Y - 55, 11, 11 };
	UI_Manager::GetFont( )->DrawTextA(NULL, "GameState_HeightMap", strlen("GameState_HeightMap"), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}
void GameState_HeightMap::DebuggingKey( )
{
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
