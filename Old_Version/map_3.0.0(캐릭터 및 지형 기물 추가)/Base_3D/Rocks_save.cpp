#include "stdafx.h"
#include "Rocks_save.h"
#include "Rock_location.h"


Rocks_save::Rocks_save( )
{
}


Rocks_save::~Rocks_save( )
{
}

void Rocks_save::Initialize( )
{
	char* name;
	char str1[256] = "rock/standing_stone_1.ASE";
	char str2[256] = "rock/standing_stone_2.ASE";
	char str3[256] = "rock/standing_stone_3.ASE";
	int rage = 40;
	int rock_num = 20;
	int rockPosDelta = rock_num - 1;
	float rotY = D3DX_PI;
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);

	for (int i = 0; i < rock_num; ++i)
	{
		Rock_location* rock = new Rock_location;
		position = D3DXVECTOR3(293.5f + cosf(D3DXToRadian(360.f * i / rockPosDelta)) * rage, 13.0f + RandomUtil::GetInteger(4, 0), -157.5f + sinf(D3DXToRadian(360.f * i / rockPosDelta)) * rage);

		rotY = D3DXToRadian(RandomUtil::GetInteger(360, 0));
		int randomInt = RandomUtil::GetInteger(2, 1);
		switch (randomInt)
		{
			case 1:
			{
				name = str1;
				break;
			}
			case 2:
			{
				name = str2;
				break;
			}
			case 3:
			{
				name = str3;
				break;
			}
		}
		rock->Initialize(name, scale, position, rotY);

		AddRock(rock);
	}
	//Rock_location* rock_1 = new Rock_location;
	//float rotX = 0.7f;
	////rock1->Initialize("rock/standing_stone_middle.ASE", scale, D3DXVECTOR3(290.5f, 11.0f, -157.5f), rotY, rotX);
	//rock_1->Initialize("rock/standing_stone_3.ASE", scale, D3DXVECTOR3(290.5f, 11.0f, -157.5f), rotY, rotX);
	//AddRock(rock_1);

	/* ¸ð¾ÆÀÌ */
	D3DXVECTOR3 moai_position = D3DXVECTOR3(218.5f, 20.0f, -325.5f );
	D3DXVECTOR3 moai_scale = D3DXVECTOR3(0.08f, 0.08f, 0.08f);
	float correct_x = 15.f;
	float correct_z = 5.f;
	float moai_Broken_addHeight = 5.f;
	float moai2_addHeight = 9.f;
	Rock_location* moai_1 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	moai_1->Initialize("moai/moai_1_1a.ASE", moai_scale, D3DXVECTOR3(moai_position.x, moai_position.y, moai_position.z), rotY);
	AddRock(moai_1);

	Rock_location* moai_broken_body = new Rock_location;
	float rotX = D3DX_PI * 1.8f;
	//rotY = D3DX_PI * 1.2f;
	moai_broken_body->Initialize("moai/broken_body.ASE", moai_scale, 
		D3DXVECTOR3(moai_position.x - correct_x, moai_position.y - moai_Broken_addHeight, moai_position.z + correct_z), rotY, rotX);
	AddRock(moai_broken_body);

	Rock_location* moai_broken_head = new Rock_location;
	rotY = D3DX_PI;
	rotX = D3DX_PI;
	moai_broken_head->Initialize("moai/broken_head.ASE", moai_scale, 
		D3DXVECTOR3(moai_position.x - correct_x, moai_position.y - moai_Broken_addHeight, moai_position.z + correct_z - 8), rotY, rotX);
	AddRock(moai_broken_head);

	Rock_location* moai_2 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	moai_2->Initialize("moai/moai_2_1a.ASE", moai_scale, D3DXVECTOR3(moai_position.x - correct_x * 2, moai_position.y + moai2_addHeight, moai_position.z + correct_z * 2), rotY);
	AddRock(moai_2);

	Rock_location* moai_3 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	moai_3->Initialize("moai/moai_2_1a.ASE", moai_scale, D3DXVECTOR3(moai_position.x - correct_x * 3, moai_position.y + moai2_addHeight, moai_position.z + correct_z * 3), rotY);
	AddRock(moai_3);

	Rock_location* moai_4 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	moai_4->Initialize("moai/moai_2_1a.ASE", moai_scale, D3DXVECTOR3(moai_position.x - correct_x * 4, moai_position.y + moai2_addHeight, moai_position.z + correct_z * 4), rotY);
	AddRock(moai_4);

	Rock_location* moai_hat_1 = new Rock_location;
	rotX = D3DX_PI * 0.55f;
	rotY = D3DX_PI * 1.8f;
	moai_hat_1->Initialize("moai/hat2_1.ASE", moai_scale, D3DXVECTOR3(moai_position.x + 14, moai_position.y, moai_position.z - 10), rotY, rotX);
	AddRock(moai_hat_1);
}

void Rocks_save::Destroy( )
{
	for (auto iter = vRocks.begin( ); iter != vRocks.end( ); )
	{
 		SAFE_DELETE((*iter));
		iter = vRocks.erase(iter);
	}
}

void Rocks_save::Update( )
{
	if (GetAsyncKeyState(VK_F3) & 0x0001)
	{
		static int count = 0;
		++count;
		if (count % 2 == 0)
		{
			count = 0;
			wireMode = true;
		}
		else
		{
			wireMode = false;
		}
	}

	for (auto iter = vRocks.cbegin( ); iter != vRocks.cend( ); ++iter)
	{
		(*iter)->Update( );
	}
}
void Rocks_save::AddRock(Rock_location* rl)
{
	vRocks.push_back(rl);
}
void Rocks_save::Render( )
{
	GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, wireMode ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	for (auto iter = vRocks.crbegin( ); iter != vRocks.crend( ); ++iter)
	{
		(*iter)->Render( );
	}
	GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}