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
	if (tree_ == nullptr)
	{
		position_tree = D3DXVECTOR3(184.0f, 11.0f, -268.0f);

		tree_ = new Tree;
		tree_->Initialize("tree_test.obj", position_tree);
		trees.push_back(tree_);

	}
	if (tree_0 == nullptr)
	{
		position_tree = D3DXVECTOR3(188.0f, 11.0f, -290.0f);

		tree_0 = new Tree;
		tree_0->Initialize("tree/tree1_a.obj", position_tree);
		trees.push_back(tree_0);

	}

	RawFileRead_Position("tree_rage.raw");
	std::vector<D3DXVECTOR3> treePos;
	for (auto iter : vRawPosition)
	{
		treePos.push_back(iter);
	}

	std::string fileName;
	float subLength = 0.f;
	D3DXVECTOR3 prev = D3DXVECTOR3(0, 0, 0);
	for (auto iter : treePos)
	{
		position_tree = iter;

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

	int nnn = 0;

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
	if (GetAsyncKeyState(VK_F3) & 0x0001)
	{
		static int count_WireMode = 0;

		++count_WireMode;
		for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
		{
			if (count_WireMode % 2 == 0)
			{
				count_WireMode = 0;
				(*iter)->SetWireMode(true);
			}
			else
			{
				(*iter)->SetWireMode(false);
			}
		}
	}

	if (GetAsyncKeyState(VK_F4) & 0x0001)
	{
		static int count_ColBoxDraw = 0;

		++count_ColBoxDraw;
		for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
		{
			if (count_ColBoxDraw % 2 == 0)
			{
				count_ColBoxDraw = 0;
				(*iter)->SetIsColliderSphereDraw(true);
			}
			else
			{
				(*iter)->SetIsColliderSphereDraw(false);
			}
		}
	}
}

void PlantTree::Render( )
{
	if (tree_)
	{
		tree_->Render( );
	}
	if (tree_0)
	{
		tree_0->Render( );
	}
	for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
	{
		(*iter)->Render( );
	}
}

void PlantTree::RawFileRead_Position(const char* fileName, int bitNum/*= 8 bit*/)
{
	char fullPath[256];

	strcpy_s(fullPath, RAW_POSITION_DIRECTORY);
	strcat_s(fullPath, fileName);

	FILE* fp = nullptr;
	fopen_s(&fp, fullPath, "rb");
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		int vertexCount = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		//총 픽셀 개수 보정
		int vertexSizeCount = vertexCount / (bitNum / 8);
		int mapSize = (int)(sqrt(vertexSizeCount));

		assert(vertexSizeCount == mapSize * mapSize && "가로세로길이가 동일해야 합니다.");

		std::vector<unsigned char> fileData;
		fileData.resize(vertexSizeCount);

		if (bitNum == 24)
		{
			int d[3];
			for (int i = 0; i < vertexSizeCount; ++i)
			{
				d[0] = fgetc(fp);
				d[1] = fgetc(fp) << 8;
				d[2] = fgetc(fp) << 16;
				int a = d[0] | d[1] | d[2];
				fileData[i] = a;
			}
		}
		else if (bitNum == 16)
		{
			int d[2];
			for (int i = 0; i < vertexSizeCount; ++i)
			{
				d[0] = fgetc(fp);
				d[1] = fgetc(fp) << 8;
				int a = d[0] | d[1];
				fileData[i] = a;
			}
		}
		else
		{
			for (int i = 0; i < vertexSizeCount; ++i)
			{
				fileData[i] = fgetc(fp);
			}
		}
		fclose(fp);

		//	vPosition.resize(vertexSizeCount);
		for (int z = 1; z < mapSize - 1; ++z)
		{
			for (int x = 1; x < mapSize - 1; ++x)
			{
				int index = z * mapSize + x;

				if (fileData[index] < 1 || fileData[index] > 25) continue;
				if (x % 3 == 0 && z % 3 == 0)
					vRawPosition.push_back(D3DXVECTOR3((float)x, fileData[index], (float)-z));
			}
		}
	}
}