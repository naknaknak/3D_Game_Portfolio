#include "stdafx.h"
#include "HeightMap.h"

#include "Raw_NormalTexture.h"


HeightMap::HeightMap( )
{
}


HeightMap::~HeightMap( )
{
}

void HeightMap::Initialize(char* mapFileName, char* textureFileName, int bitNum/*= 8*/, HEIGHT_FLAG hf/*= HM_DEFAULT*/)
{
	//HM_DEFAULT = 0,
	//	HM_ALPHA,
	//	HM_NORMAL,
	//	HM_NUMBER

	if (hf == HEIGHT_FLAG::HM_NORMAL)
	{
		rfsection = new Raw_NormalTexture;
	}

	rfsection->Initialize(vertex, mapSize, mapFileName, textureFileName, bitNum);
	assert(vertex.size( ) != 0 && "vertex에 값이 안 들어감.");
	assert(mapSize != -1 && "mapSize에 값이 안 들어감.");
}

void HeightMap::Destroy( )
{
	rfsection->Destroy( );
}

void HeightMap::Render( )
{
	rfsection->Render( );
}

bool HeightMap::GetHeight(float & outHeight, float x, float z)
{
	if (x < 0 || z>0 || x > mapSize || z < -mapSize)
	{
		return false;
	}

	int intX = (int)x;
	int intZ = (int)-z;

	//	0--1
	//  | /|
	//  |/ |
	//  2--3
	int _0 = (intX + 0) + (intZ + 0) * mapSize;
	int _1 = (intX + 1) + (intZ + 0) * mapSize;
	int _2 = (intX + 0) + (intZ + 1) * mapSize;
	int _3 = (intX + 1) + (intZ + 1) * mapSize;

	float fDeltaX = x - intX;
	float fDeltaZ = -z - intZ;

	if (fDeltaX + fDeltaZ < 1)
	{
		D3DXVECTOR3 v0to1 = vertex[_1] - vertex[_0];
		D3DXVECTOR3 v0to2 = vertex[_2] - vertex[_0];
		v0to1 *= fDeltaX;
		v0to2 *= fDeltaZ;
		outHeight = (v0to1 + v0to2).y + vertex[_0].y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		D3DXVECTOR3 v3to1 = vertex[_1] - vertex[_3];
		D3DXVECTOR3 v3to2 = vertex[_2] - vertex[_3];
		v3to1 *= fDeltaX;
		v3to2 *= fDeltaZ;
		outHeight = (v3to1 + v3to2).y + vertex[_3].y;
	}
	return true;
}

bool HeightMap::GetHeight(D3DXVECTOR3 & pos, float x, float z)
{
	if (-mapSize + 2 > z || z > 0) return false;
	if (0 > x || x > mapSize - 2) return false;

	int ox = (int)x;
	int oz = (int)z * -1;

	D3DXVECTOR3 p0, p1, p2, p3;
	float cmpx = fabsf(x - (float)ox);
	float cmpz = fabsf(-1 * z - (float)oz);

	p0 = vertex.at((ox + 0) + (oz + 0) * mapSize);
	p1 = vertex.at((ox + 1) + (oz + 0) * mapSize);
	p2 = vertex.at((ox + 0) + (oz + 1) * mapSize);
	p3 = vertex.at((ox + 1) + (oz + 1) * mapSize);


	if (cmpx + cmpz < 1)
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

	if (find == true)
	{
		pos.y = 1000.0f - distance;

		//pos = p0 + (p1 - p0) * u + (p2 - p0) * v;
	}

	return find;
}