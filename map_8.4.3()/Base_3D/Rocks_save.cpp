#include "stdafx.h"
#include "Rocks_save.h"


Rocks_save::Rocks_save( )
{
}


Rocks_save::~Rocks_save( )
{
}

void Rocks_save::Initialize( )
{
	int rage = 40;
	int rock_num = 5;
	float rotY = D3DX_PI;
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
	std::string fileName;

	for (int i = 1; i <= rock_num; ++i)
	{
		Rock_location* rock = new Rock_location;
		position = D3DXVECTOR3(
			293.5f + cosf(D3DXToRadian(360.f * i / rock_num)) * rage,
			13.0f + RandomUtil::GetInteger(4, 0),
			-157.5f + sinf(D3DXToRadian(360.f * i / rock_num)) * rage);

		//rotY = D3DXToRadian(RandomUtil::GetInteger(360, 0));
		rotY = 0;

		fileName = "rock/standing_stone_";
		char num[32] = { (char)RandomUtil::GetInteger(51, 49) };
		fileName += num;
		char dotObj[32] = ".ASE";
		fileName += dotObj;
		 
		rock->Initialize(fileName.c_str(), scale, position, rotY);

		AddRock(rock);
	}

	/* ¸ð¾ÆÀÌ */
	D3DXVECTOR3 correct_position;
	D3DXVECTOR3 moai_position = D3DXVECTOR3(218.5f, 20.0f, -325.5f ); 
	D3DXVECTOR3 moai_scale = D3DXVECTOR3(0.08f, 0.08f, 0.08f);
	float correct_y = 0.f;
	float correct_x = -15.f;
	float correct_z = 5.f;
	float moai_Broken_addHeight = -5.50f;
	float moai2_addHeight = 7.0f;
	Rock_location* moai_1 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	correct_position = D3DXVECTOR3(moai_position.x, moai_position.y, moai_position.z);
	moai_1->Initialize("moai/moai_1_1a.ASE", moai_scale, correct_position, rotY);
	AddRock(moai_1);

	Rock_location* moai_broken_body = new Rock_location;
	float rotX = D3DX_PI * 1.8f;
	//rotY = D3DX_PI * 1.2f;
	correct_position = D3DXVECTOR3(moai_position.x + correct_x, moai_position.y + moai_Broken_addHeight, moai_position.z + correct_z);
	moai_broken_body->Initialize("moai/broken_body.ASE", moai_scale, correct_position, rotY, rotX);
	AddRock(moai_broken_body);

	Rock_location* moai_broken_head = new Rock_location;
	rotY = D3DX_PI;
	rotX = D3DX_PI;
	correct_position = D3DXVECTOR3(moai_position.x + correct_x, moai_position.y + moai_Broken_addHeight, moai_position.z + correct_z - 8);
	moai_broken_head->Initialize("moai/broken_head.ASE", moai_scale, correct_position, rotY, rotX);
	AddRock(moai_broken_head);

	Rock_location* moai_2 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	correct_position = D3DXVECTOR3(moai_position.x + correct_x * 2, moai_position.y + moai2_addHeight, moai_position.z + correct_z * 2);
	moai_2->Initialize("moai/moai_2_1a.ASE", moai_scale, correct_position, rotY);
	AddRock(moai_2);

	Rock_location* moai_3 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	correct_position = D3DXVECTOR3(moai_position.x + correct_x * 3, moai_position.y + moai2_addHeight, moai_position.z + correct_z * 3);
	moai_3->Initialize("moai/moai_2_1a.ASE", moai_scale,correct_position, rotY);
	AddRock(moai_3);

	Rock_location* moai_4 = new Rock_location;
	rotY = D3DX_PI * 1.2f;
	correct_position = D3DXVECTOR3(moai_position.x + correct_x * 4, moai_position.y + moai2_addHeight, moai_position.z + correct_z * 4);
	moai_4->Initialize("moai/moai_2_1a.ASE", moai_scale, correct_position, rotY);
	AddRock(moai_4);

	Rock_location* moai_hat_1 = new Rock_location;
	rotX = D3DX_PI * 0.55f;
	rotY = D3DX_PI * 1.8f;
	correct_position = D3DXVECTOR3(moai_position.x + 14, moai_position.y, moai_position.z - 10);
	moai_hat_1->Initialize("moai/hat2_1.ASE", moai_scale, correct_position, rotY, rotX);
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
	static int count_Mode = 0;

	++count_Mode;
	for (auto iter = vRocks.cbegin( ); iter != vRocks.cend( ); ++iter)
	{
		count_Mode = 0;
		(*iter)->SetWireMode(wireMode);
		(*iter)->SetIsColliderBoxDraw(IsColliderBoxDraw);
		(*iter)->SetIsColliderSphereDraw(isColliderSphereDraw);
	}
}
void Rocks_save::AddRock(Rock_location* rl)
{
	vRocks.push_back(rl);
}
void Rocks_save::Render( )
{
	for (auto iter = vRocks.crbegin( ); iter != vRocks.crend( ); ++iter)
	{
		(*iter)->Update( );
		(*iter)->Render( );
	}

}