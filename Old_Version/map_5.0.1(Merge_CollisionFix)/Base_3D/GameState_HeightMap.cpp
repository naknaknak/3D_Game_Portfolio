#include "stdafx.h"
#include "GameState_HeightMap.h"
#include "Tree.h"

GameState_HeightMap::GameState_HeightMap()
{
}


GameState_HeightMap::~GameState_HeightMap()
{
}

void GameState_HeightMap::Initialize()
{
	if ( sky == nullptr )
	{
		sky = new SkyBox;
		sky->Initialize("cloud_1.png", 4096);
	}
	if (grid == nullptr)
	{
		grid = new Grid;
		grid->Initialize();
		grid->SetShowArrow(true);
		grid->SetShowGround(true);
	}
	if ( heightMap == nullptr )
	{
		heightMap = new HeightMap;
		heightMap->Initialize("field_.raw", "field_1.png", 8, HEIGHT_FLAG::HM_NORMAL);
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
	//if ( girl == nullptr )
	//{
	//	girl = new Character_HeightMapMove();
	//	girl->Initialize();
	//	girl->SetHeightMap(heightMap);
	//	//girl->SetPosition(D3DXVECTOR3(5.f, 10.0f, -5.f));
	//	girl->SetPosition(D3DXVECTOR3(35.5f, 20.0f, -65.5f));
	//	GameManager::GetCamera()->SetLookTarget(girl->GetPositionAdress());
	//}
	if (goat == nullptr)
	{
		goat = new Player();
		goat->Initialize("Goat/", "Goat.X");
		
		goat->SetPosition(D3DXVECTOR3(193.0f, 12.0f, -306.0f));
		goat->SetTrees(trees->GetTrees());

		goat->SetHeightMap(heightMap); 
		goat->ChangeCharacterState(CharacterState::CHARACTER_IDLE);
 		GameManager::GetCamera()->SetLookTarget(goat->GetPositionAddress());
	}

	/*if (zombie == nullptr)
	{
		zombie = new Player( );
		zombie->Initialize("Zombie/", "zombie_attack02_bite.X");
		zombie->InitializeAnimation( );
		zombie->SetPosition(D3DXVECTOR3(195.0f, 13.0f, -306.0f));
		 
		zombie->SetHeightMap(heightMap);
		zombie->SetAnimationIndex(0);
		GameManager::GetCamera( )->SetLookTarget(goat->GetPositionAddress( ));
	}*/
	
}

void GameState_HeightMap::Destroy()
{
	SAFE_DELETE(zombie);
	SAFE_DELETE(goat);
	/*SAFE_DELETE(girl);*/
	SAFE_DELETE(trees);
	SAFE_DELETE(rock);
	SAFE_DELETE(heightMap);
	SAFE_DELETE(grid);
	SAFE_DELETE(sky);
}

void GameState_HeightMap::Reset()
{
}

void GameState_HeightMap::Update()
{
	if ( sky )
	{
		sky->Update();
	}
	if (rock)
	{
		rock->Update( );
	}
	
	if ( girl )
	{
		//girl->Update();
		//girl->GroundCheck();
	} 
	if (goat)
	{
		goat->Update();
	}
}

void GameState_HeightMap::Render()
{
	if ( sky )
	{
		sky->Render();
	}
	if ( grid )
	{
		grid->Render();
	}
	if (rock)
	{
		rock->Render( );
	}
	if ( heightMap )
	{
		heightMap->Render();
	}
	if ( girl )
	{
		girl->Render();
	}
	if (goat)
	{
		goat->Render();
		/*BoundingSphere goat_boundingSphere = *goat->GetBoundingSphere( );
		std::vector<Tree*>& vTrees = trees->GetTrees( );
		for (auto iter = vTrees.begin( ); iter != vTrees.end( ); ++iter)
		{
			bool aa = Collision::IsSphereToSphere(*goat->GetBoundingSphere( ), (*iter)->GetBoundingSphere( ));
			if (Collision::IsSphereToSphere(*goat->GetBoundingSphere( ), (*iter)->GetBoundingSphere( )))
			{
				int n = 0;

				(*iter)->SetBoundingSphereMaterialColor(D3DXCOLOR(0.f, 0.f, 0.8f, 1.0f));
			}
			else
			{
				(*iter)->SetBoundingSphereMaterialColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}*/
	}
	/*if (zombie)
	{
		zombie->UpdateAndRender( );
	}*/
	if (trees)
	{
		trees->Render( );
	}

	RECT rc = { 10, RESOLUTION_Y - 55, 11, 11 };
	UI_Manager::GetFont()->DrawTextA(NULL, "GameState_HeightMap", strlen("GameState_HeightMap"), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

void GameState_HeightMap::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(10.0f);	
	Initialize();
}

void GameState_HeightMap::OnLeaveState()
{
	Destroy();
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
