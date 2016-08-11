#include "stdafx.h"
#include "Raw_NormalTexture.h"


Raw_NormalTexture::Raw_NormalTexture( )
{
}


Raw_NormalTexture::~Raw_NormalTexture( )
{
}

void Raw_NormalTexture::Initialize(OUT std::vector<D3DXVECTOR3>& vertex, OUT int & mapSize, char * mapFileName, char * textureFileName, int bitNum)
{
	D3DXMatrixIdentity(&world);

	if (textureFileName != nullptr)
	{
		pTexDiffuse = TextureManager::GetTexture(textureFileName);
	}

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	//
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

		tileCount = mapSize - 1;

		std::vector<FVF_PositionNormalColorTexture> fvfVertex;
		fvfVertex.resize(vertexSizeCount);
		vertex.resize(vertexSizeCount);
		for (int z = 0; z < mapSize; ++z)
		{
			for (int x = 0; x < mapSize; ++x)
			{
				int index = z * mapSize + x;
				FVF_PositionNormalColorTexture v;
				v.pos = D3DXVECTOR3((float)x, fileData[index] * 0.2f + 0.1f, (float)-z);
				v.normal = D3DXVECTOR3(0, 1, 0);
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

				DWORD a = (DWORD)(255.0f * fileData[index]);

				//fvfVertex[index].color = D3DCOLOR_ARGB(a, r, g, b);
				fvfVertex[index].color = D3DCOLOR_ARGB(a, 230, 230, 230,);
			}
		}

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

		//버벡스 버퍼, 인덱스 버퍼 만들기
		int bufferSize = fvfVertex.size( ) * sizeof(FVF_PositionNormalColorTexture);
		GameManager::GetDevice( )->CreateVertexBuffer(
			bufferSize,
			0,
			FVF_PositionNormalColorTexture::FVF,
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
}
DWORD Raw_NormalTexture::VectortoRGBA(D3DXVECTOR3* v, float fHeight)
{
	DWORD r = (DWORD)(127.0f * v->x + 128.0f);
	DWORD g = (DWORD)(127.0f * v->y + 128.0f);
	DWORD b = (DWORD)(127.0f * v->z + 128.0f);
	DWORD a = (DWORD)(255.0f * fHeight);
	
	return ((a << 24L) + (r << 16L) + (g << 8) + (b << 0L));
}
void Raw_NormalTexture::Destroy( )
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	//pTexDiffuse = nullptr;
}

void Raw_NormalTexture::Render( )
{
	Animate( );

	GameManager::GetDevice( )->SetTexture(0, pTexDiffuse);
	GameManager::GetDevice( )->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	DWORD dwFactor = VectortoRGBA(&vecLight, 0.0f);

	GameManager::GetDevice( )->SetRenderState(D3DRS_TEXTUREFACTOR, dwFactor);

	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);

	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	GameManager::GetDevice( )->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice( )->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice( )->SetMaterial(&material);
	GameManager::GetDevice( )->SetFVF(FVF_PositionNormalColorTexture::FVF);
	GameManager::GetDevice( )->SetStreamSource(0, vertexBuffer, 0, sizeof(FVF_PositionNormalColorTexture));
	GameManager::GetDevice( )->SetIndices(indexBuffer);
	GameManager::GetDevice( )->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		vertexSizeCount,
		0,
		triangleCount);

	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
}

void Raw_NormalTexture::Update( )
{

}

void Raw_NormalTexture::Animate( )
{
	D3DXMatrixIdentity(&world);

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(GameManager::Get( ).GetWindowHandle( ), &pt);
	vecLight.x = -(((2.0f * pt.x) / RESOLUTION_X) - 1);
	vecLight.y = -(((2.0f * pt.y) / RESOLUTION_Y) - 1);
	vecLight.z = 0.0f;

	if (D3DXVec3Length(&vecLight) > 1.0f)
	{
		D3DXVec3Normalize(&vecLight, &vecLight);
	}
	else
	{
		vecLight.z = sqrtf(1.0f - vecLight.x * vecLight.x - vecLight.y * vecLight.y);
	}
}
