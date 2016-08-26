#include "stdafx.h"
#include "HeightMap.h"


HeightMap::HeightMap( )
{
}


HeightMap::~HeightMap( )
{
}

void HeightMap::Initialize(char* mapFileName, char* textureFileName)
{
	D3DXMatrixIdentity(&world);
	if (textureFileName != nullptr)
	{
		pTexDiffuse = TextureManager::GetTexture(textureFileName);
		if (SupportsBumpMapping( )) // 현재의 장치에서의 범프 맵핑의 지원을 검출한다.
		{
			pTexNormal = TextureManager::GetNormalMap(textureFileName);
		}
	}
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);


	RawFileRead_Position("move_way.raw", vertex_MoveCheck);

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

		mapSize = (int)(sqrt(vertexCount));

		assert(vertexCount == mapSize*mapSize && "가로세로길이가 동일해야 합니다.");

		std::vector<unsigned char> fileData;
		fileData.resize(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			fileData[i] = fgetc(fp);
		}
		fclose(fp);

		tileCount = mapSize - 1;

		std::vector<FVF_PositionNormalTexture> fvfVertex;
		fvfVertex.resize(vertexCount);
		vertex.resize(vertexCount);

		for (int z = 0; z < mapSize; ++z)
		{
			for (int x = 0; x < mapSize; ++x)
			{
				int index = z * mapSize + x;
				FVF_PositionNormalTexture v;
				v.pos = D3DXVECTOR3((float)x, fileData[index] * 0.2f, (float)-z);
				v.normal = D3DXVECTOR3(0, 1, 0);
				v.tex = D3DXVECTOR2(x / (float)tileCount, z / (float)tileCount);

				fvfVertex[index] = v;
				vertex[index] = v.pos;
			}
		}
		vertex_CameraCollision = vertex;

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

		int bufferSize = fvfVertex.size( ) * sizeof(FVF_PositionNormalTexture);
		GameManager::GetDevice( )->CreateVertexBuffer(
			bufferSize,
			0,
			FVF_PositionNormalTexture::FVF,
			D3DPOOL_MANAGED,
			&vertexBuffer,
			nullptr);
		LPVOID pV;
		vertexBuffer->Lock(0, 0, &pV, 0);
		memcpy_s(pV, bufferSize, &fvfVertex[0], bufferSize);
		vertexBuffer->Unlock( );

		bufferSize = indexData.size( ) * sizeof(DWORD);
		GameManager::GetDevice( )->CreateIndexBuffer(
			bufferSize,
			0,
			D3DFMT_INDEX32,
			D3DPOOL_MANAGED,
			&indexBuffer,
			nullptr);
		LPVOID pI;
		indexBuffer->Lock(0, 0, &pI, 0);
		memcpy_s(pI, bufferSize, &indexData[0], bufferSize);
		indexBuffer->Unlock( );
	}
	if (m_pQuadTree == nullptr)
	{
		m_fLODRatio = 0.03f;
		m_pQuadTree = new QuadTree(mapSize, mapSize); 	// 텍스쳐 파일의 크기에 맞춰서 쿼드트리를 생성한다.
		m_pQuadTree->Build(&vertex[0]);
	}
}
void HeightMap::RawFileRead_Position(const char* fileName, std::vector<D3DXVECTOR3>& pos)
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
		int vertexSizeCount = vertexCount;
		int mapSize = (int)(sqrt(vertexSizeCount));

		assert(vertexSizeCount == mapSize * mapSize && "가로세로길이가 동일해야 합니다.");

		std::vector<unsigned char> fileData;
		fileData.resize(vertexSizeCount);


		for (int i = 0; i < vertexSizeCount; ++i)
		{
			fileData[i] = fgetc(fp);
		}

		fclose(fp);

		pos.resize(vertexSizeCount);
		ZeroMemory(&pos[0], sizeof(D3DXVECTOR3) * vertexSizeCount);

		for (int z = 1; z < mapSize - 1; ++z)
		{
			for (int x = 1; x < mapSize - 1; ++x)
			{
				int index = z * mapSize + x;
				if (fileData[index] < 1) continue;
				pos.at(index) = D3DXVECTOR3((float)x, fileData[index], (float)-z);
			}
		}
	}
}
void HeightMap::Destroy( )
{
	SAFE_DELETE(m_pQuadTree);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	pTexDiffuse = nullptr;
}

void HeightMap::DebuggingKey( )
{
	if (GetAsyncKeyState(VK_F2) & 0x0001)
	{
		static int count_WireMode = 0;

		++count_WireMode;
		if (count_WireMode % 2 == 0)
		{
			count_WireMode = 0;
			wireMode = TRUE;
			bQuadTree = TRUE;
		}
		else
		{
			wireMode = FALSE;
			bQuadTree = FALSE;
		}
	}
}

BOOL HeightMap::SupportsBumpMapping( )
{
	D3DCAPS9 d3dCaps;

	GameManager::GetDevice( )->GetDeviceCaps(&d3dCaps);

	// Does this device support the two bump mapping blend operations?
	if (0 == d3dCaps.TextureOpCaps & (D3DTEXOPCAPS_BUMPENVMAP |
		D3DTEXOPCAPS_BUMPENVMAPLUMINANCE))
		return FALSE;

	// Does this device support up to three blending stages?
	if (d3dCaps.MaxTextureBlendStages < 3)
		return FALSE;

	return TRUE;
}
void HeightMap::Render( )
{
	if(bQuadTree) QuadTreeDraw( );

	if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	GameManager::GetDevice( )->SetTexture(0, pTexNormal);
	GameManager::GetDevice( )->SetTexture(1, pTexDiffuse);
	GameManager::GetDevice( )->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	GameManager::GetDevice( )->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

	// Set luminance controls.This is only needed when using
	// a bump map that contains luminance, and when the 
	// D3DTOP_BUMPENVMAPLUMINANCE texture blending operation is
	// being used.
	//
	// Note These calls rely on the following inline shortcut function:
	// inline DWORD F2DW( FLOAT f ) { return *((DWORD*) &f); }
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_BUMPENVLSCALE, F2DW(0.5f));
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f));
	//
	//GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BUMPENVMAPLUMINANCE);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
	//GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	//GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);


	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);


	GameManager::GetDevice( )->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice( )->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice( )->SetMaterial(&material);
	//
	GameManager::GetDevice( )->SetStreamSource(0, vertexBuffer, 0, sizeof(FVF_PositionNormalTexture));
	GameManager::GetDevice( )->SetFVF(FVF_PositionNormalTexture::FVF);
	GameManager::GetDevice( )->SetIndices(indexBuffer);
	GameManager::GetDevice( )->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		vertexCount,
		0,
		triangleCount);

	GameManager::GetDevice( )->SetTexture(0, nullptr);
	GameManager::GetDevice( )->SetTexture(1, nullptr);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	Debuging( );
}
void HeightMap::Debuging( )
{
	//char str[256];
	//RECT rc = { 15, 250, 16, 51 };
	//sprintf_s(str, "vecLight.x: %.3f, vecLight.y: %.3f vecLight.z: %.3f", vecLight.x, vecLight.y, vecLight.z);
	//UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

	//RECT rc1 = { 15, 270, 10, 10 };
	//sprintf_s(str, "VectorToRGBA: %d", dwFactor);
	//UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc1, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

	//RECT rc2 = { 15, 90, 10, 10 };
	//sprintf_s(str, "AnimNumAnimationSets: %d", animController->GetNumAnimationSets( ));
	//UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc2, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}
HRESULT HeightMap::QuadTreeDraw( )
{
	LPVOID pI;

	triangleCount = m_pQuadTree->GenerateIndex((LPVOID)&indexData[0], &vertex[0], m_fLODRatio);		// 수행시간 0.1 초
	int bufferSize = triangleCount * 3 * sizeof(DWORD);

	if (FAILED(indexBuffer->Lock(0, bufferSize, (void**)&pI, 0))) return E_FAIL;
	memcpy_s(pI, bufferSize, &indexData[0], bufferSize);
	indexBuffer->Unlock( );

	return S_OK;
}
bool HeightMap::GetHeight(float& outHeight, float x, float z)
{
	if (x < 1 || z>-1 || x > tileCount || z < -tileCount)
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

