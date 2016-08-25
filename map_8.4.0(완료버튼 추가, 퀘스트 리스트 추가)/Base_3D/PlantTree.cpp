#include "stdafx.h"
#include "PlantTree.h"

PlantTree::PlantTree( )
{
}

PlantTree::~PlantTree( )
{
}

void PlantTree::Initialize( )
{
	D3DXVECTOR3 position_tree;

	hm->RawFileRead_Position("tree_rage.raw", vRawPosition);

	std::string fileName;
	float subLength = 0.f;
	D3DXVECTOR3 prev = D3DXVECTOR3(0, 0, 0);
	
	for (int z = 1; z < 512 - 1; ++z)
	{
		for (int x = 1; x < 512 - 1; ++x)
		{
			int index = z * 512 + x;

			if (vRawPosition[index].y < 1 || vRawPosition[index].y > 25) continue;
			if (x % 3 == 0 && z % 3 == 0)
			{
				position_tree = vRawPosition[index];
				if (hm)
				{
					hm->GetHeight(position_tree.y, position_tree.x, position_tree.z);
				}

				fileName = "tree/tree";
				char num[32] = { (char)RandomUtil::GetInteger(52, 49) };
				fileName += num;
				char ab[32] = { '_', (char)RandomUtil::GetInteger(98, 97) };
				fileName += ab;
				char dotObj[32] = ".obj";
				fileName += dotObj;

				Tree* plantTree = new Tree;
				plantTree->Initialize(fileName.c_str( ), position_tree);

				trees.push_back(plantTree);
			}
		}
	}

}

void PlantTree::Destroy( )
{
	for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	trees.clear( );
	trees.shrink_to_fit( );
}

void PlantTree::Update( )
{
	static int count_Mode = 0;

	++count_Mode;
	for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
	{
		count_Mode = 0;
		(*iter)->SetWireMode(wireMode);
		(*iter)->SetIsColliderBoxDraw(IsColliderBoxDraw);
		(*iter)->SetIsColliderSphereDraw(isColliderSphereDraw);
	}

}

void PlantTree::Render( )
{
	for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
	{
		(*iter)->Render( );
	}
}
