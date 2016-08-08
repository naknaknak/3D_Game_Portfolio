#include "stdafx.h"
#include "Trees.h"

Trees::Trees( )
{
}

Trees::~Trees( )
{
}

void Trees::Initialize( )
{

	if (tree_ == nullptr)
	{
		tree_ = new Obj;
		tree_->Initialize("tree_test.obj");
		tree_->SetPosition(D3DXVECTOR3(184.0f, 11.0f, -268.0f));
	}
	if (tree_0 == nullptr)
	{
		tree_0 = new Obj;
		tree_0->Initialize("tree/tree1_a.obj");
		tree_0->SetPosition(D3DXVECTOR3(188.0f, 11.0f, -290.0f));
	}
	
	RawFileRead_Position("trees_512.raw");

	std::vector<D3DXVECTOR3> treePos;

	//rawPos.swap();
	float a = vRawPosition.back().y;
	for (auto iter : vRawPosition)
	{
		if (iter.y < 1 || iter.y > 23) continue;
		treePos.push_back(iter);
	}

	std::string fileName;
	float subLength = 0.f;
	D3DXVECTOR3 prev = D3DXVECTOR3(0, 0, 0);
	for (auto iter : treePos)
	{
		D3DXVECTOR3 vecLength = iter - prev;
		subLength = D3DXVec3LengthSq(&vecLength);
		if (subLength < 20)
		{
			continue;
		}

		fileName = "tree/tree";
		char num[32] = { (char)RandomUtil::GetInteger(52, 49) };
		fileName += num;
		char ab[32] = {'_', (char)RandomUtil::GetInteger(98, 97) };
		fileName += ab;
		char dotObj[32] = ".obj";
		fileName += dotObj;

		Obj* plantTree = new Obj;
		plantTree->Initialize(fileName.c_str());

		hm->GetHeight(iter.y, iter.x, iter.z);
		
		plantTree->SetPosition(iter);

		trees.push_back(plantTree);
		prev = iter;
	}


}

void Trees::Destroy( )
{
	SAFE_DELETE(tree_0);
	SAFE_DELETE(tree_);
	for (auto iter = trees.begin( ); iter != trees.end( ); ++iter)
	{
		(*iter)->Destroy( );
		delete *iter;
	}
	trees.clear( );
	trees.shrink_to_fit();
}

void Trees::Update( )
{
}

void Trees::Render( )
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

void Trees::RawFileRead_Position(const char* fileName, int bitNum/*= 8 bit*/)
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
		for (int z = 1; z < mapSize-1; ++z)
		{
			for (int x = 1; x < mapSize-1; ++x)
			{
				int index = z * mapSize + x;

				if (fileData[index] < 1) continue;
				if (x % 3 == 0 || z % 2 == 0) continue;
				vRawPosition.push_back(D3DXVECTOR3((float)x, fileData[index], (float)-z));
			}
		}
	}
}