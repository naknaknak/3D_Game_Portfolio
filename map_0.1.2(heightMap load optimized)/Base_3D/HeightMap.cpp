#include "stdafx.h"
#include "HeightMap.h"


HeightMap::HeightMap()
{
}


HeightMap::~HeightMap()
{
}

void HeightMap::Initialize(char* mapFileName, char* textureFileName, int bitNum/*= 8*/)
{
	D3DXMatrixIdentity(&world);
	texture = TextureManager::GetTexture(textureFileName);
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	char fullPath[256];
	strcpy_s(fullPath, HEIGHTMAP_DIRECTORY);
	strcat_s(fullPath, mapFileName);

	FILE* fp = nullptr;
	fopen_s(&fp, fullPath, "rb");

	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		vertexCount = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		//총 픽셀 개수 보정
		vertexSizeCount = vertexCount / (bitNum / 8);

		mapSize = (int)(sqrt(vertexSizeCount));

		assert(vertexSizeCount == mapSize*mapSize && "가로세로길이가 동일해야 합니다.");

		std::vector<unsigned char> fileData;
		fileData.resize(vertexSizeCount);

		if ( bitNum == 24 )
		{
			int d[3];
			for ( int i = 0; i < vertexSizeCount; ++i )
			{
				d[0] = fgetc(fp);
				d[1] = fgetc(fp) << 8;
				d[2] = fgetc(fp) << 16;
				int a = d[0] | d[1] | d[2];
				fileData[i] = a;
			}
		}
		else if ( bitNum == 16 )
		{
			int d[2];
			for ( int i = 0; i < vertexSizeCount; ++i )
			{
				d[0] = fgetc(fp);
				d[1] = fgetc(fp) << 8;
				int a = d[0] | d[1];
				fileData[i] = a;
			}
		}
		else
		{
			for ( int i = 0; i < vertexSizeCount; ++i )
			{
				fileData[i] = fgetc(fp);
			}
		}
		fclose(fp);

		tileCount = mapSize - 1;

		std::vector<FVF_PositionNormalTexture> fvfVertex;
		fvfVertex.resize(vertexSizeCount);
		vertex.resize(vertexSizeCount);

		for (int z = 0; z < mapSize; ++z)
		{
			for (int x = 0; x < mapSize; ++x)
			{
				int index = z * mapSize + x;
				FVF_PositionNormalTexture v;
				v.pos = D3DXVECTOR3((float)x, fileData[index] * 0.2f, (float)-z);
				v.normal = D3DXVECTOR3(0,1,0);
				v.tex = D3DXVECTOR2(x / (float)tileCount, z / (float)tileCount);

				fvfVertex[index] = v;
				vertex[index] = v.pos;
			}
		}

		//노말값들 갱신
		//필요한건 벡터 4개
		//왼쪽, 오른쪽, 위쪽, 아래쪽

		for (int z = 1; z < tileCount; ++z)
		{
			for (int x = 1; x < tileCount; ++x)
			{
				int index = z * mapSize + x;
				D3DXVECTOR3 left = vertex[index - 1];
				D3DXVECTOR3 right = vertex[index + 1];
				D3DXVECTOR3 front = vertex[index - mapSize];
				D3DXVECTOR3 rear = vertex[index + mapSize];
				D3DXVECTOR3 leftToRight = right - left;
				D3DXVECTOR3 frontToRear = rear - front;
				D3DXVECTOR3 normal;
				D3DXVec3Cross(&normal, &leftToRight, &frontToRear);
				D3DXVec3Normalize(&normal, &normal);

				fvfVertex[index].normal = normal;
			}
		}

		//버벡스 버퍼, 인덱스 버퍼 만들기
		std::vector<DWORD> indexData;
		triangleCount = tileCount * tileCount * 2;
		indexData.resize(triangleCount * 3);
		for (int z = 0; z < tileCount; ++z)
		{
			for (int x = 0, k = 0; x < tileCount; ++x, ++k)
			{
				int _0 = (x + 0) + (z + 0) * mapSize;
				int _1 = (x + 1) + (z + 0) * mapSize;
				int _2 = (x + 0) + (z + 1) * mapSize;
				int _3 = (x + 1) + (z + 1) * mapSize;

				indexData[z * (6 * tileCount) + k] = (_0);
				indexData[z * (6 * tileCount) + ++k] = (_1);
				indexData[z * (6 * tileCount) + ++k] = (_2);

				indexData[z * (6 * tileCount) + ++k] = (_3);
				indexData[z * (6 * tileCount) + ++k] = (_2);
				indexData[z * (6 * tileCount) + ++k] = (_1);
			}
		}
		int bufferSize = fvfVertex.size() * sizeof(FVF_PositionNormalTexture);
		GameManager::GetDevice()->CreateVertexBuffer(
			bufferSize,
			0,
			FVF_PositionNormalTexture::FVF,
			D3DPOOL_MANAGED,
			&vertexBuffer,
			nullptr);
		LPVOID pV;
		vertexBuffer->Lock(0, 0, &pV, 0);
		memcpy_s(pV, bufferSize, &fvfVertex[0], bufferSize);
		vertexBuffer->Unlock();

		bufferSize = indexData.size() * sizeof(DWORD);
		GameManager::GetDevice()->CreateIndexBuffer(
			bufferSize,
			0,
			D3DFMT_INDEX32,
			D3DPOOL_MANAGED,
			&indexBuffer,
			nullptr);
		LPVOID pI;
		indexBuffer->Lock(0, 0, &pI, 0);
		memcpy_s(pI, bufferSize, &indexData[0], bufferSize);
		indexBuffer->Unlock();
	}
}

void HeightMap::Destroy()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	texture = nullptr;
}

void HeightMap::Render()
{
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	GameManager::GetDevice()->SetStreamSource(0, vertexBuffer, 0, sizeof(FVF_PositionNormalTexture));
	GameManager::GetDevice()->SetIndices(indexBuffer);
	GameManager::GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		vertexSizeCount,
		0,
		triangleCount);
}

bool HeightMap::GetHeight(float & outHeight, float x, float z)
{
	D3DXVECTOR3 p0, p1, p2, p3;
	D3DXVECTOR3 pos;
	int ox = (int)x;
	int oz = (int)z;

	float cmpx = x - (float)ox;
	float cmpz = z - (float)oz;
	p0 = vertex.at((ox + 0) + (oz + 0) * mapSize);
	p1 = vertex.at((ox + 1) + (oz + 0) * mapSize);
	p2 = vertex.at((ox + 0) + (oz + 1) * mapSize);
	p3 = vertex.at((ox + 1) + (oz + 1) * mapSize);

	if ( cmpx + cmpz < 1 )
	{
		//012
		pos = p0 + (p1 - p0) * cmpx + (p2 - p0) * cmpz;
	}
	else
	{
		//321
		pos = p0 + (p3 - p2) * cmpx + (p3 - p1) * cmpz;
	}

	outHeight = pos.y;

	return false;
}

bool HeightMap::GetHeight(D3DXVECTOR3 & pos, float x, float z)
{
	if ( -mapSize + 2 > z || z > 0) return false;
	if ( 0 > x || x > mapSize - 2 ) return false;
	
	int ox = (int)x;
	int oz = (int)z * -1;

	D3DXVECTOR3 p0, p1, p2, p3;
	float cmpx = fabsf(x - (float)ox);
	float cmpz = fabsf(-1 * z - (float)oz);

	p0 = vertex.at((ox + 0) + (oz + 0) * mapSize);
	p1 = vertex.at((ox + 1) + (oz + 0) * mapSize);
	p2 = vertex.at((ox + 0) + (oz + 1) * mapSize);
	p3 = vertex.at((ox + 1) + (oz + 1) * mapSize);


	if ( cmpx + cmpz < 1 )
	{
		//012
		//pos = p0 + (p1 - p0) * cmpx + (p2 - p0) * cmpz;
		OnTheGround(pos, p0, p1, p2);
	}
	else
	{
		//321
		//pos = p2 + (p1 - p2) * cmpx + (p3 - p2) * cmpz;
		//pos = p3 - (p1 - p3) * cmpx + (p2 - p3) * cmpz;
		OnTheGround(pos, p3, p1, p2);
	}

	return true;
}
bool HeightMap::OnTheGround(D3DXVECTOR3& pos, const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2)
{
	bool find = false;

	D3DXVECTOR3 rayStart(pos.x, 1000.0f, pos.z);
	D3DXVECTOR3 rayDirection(0, -1, 0);

		float u, v, distance;
		find = D3DXIntersectTri(
			&p0, &p1, &p2,
			&rayStart, &rayDirection,
			&u, &v, &distance) != 0;

		if ( find == true )
		{
			pos.y = 1000.0f - distance;

			//pos = p0 + (p1 - p0) * u + (p2 - p0) * v;
		}

	return find;
}