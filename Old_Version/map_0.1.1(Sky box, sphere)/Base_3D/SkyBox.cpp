#include "stdafx.h"
#include "SkyBox.h"

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::Initialize(char* textureFileName, float size)
//void SkyBox::Initialize(char* textureFileName, float rad/* = 128.f*/, int vertexNumSqrt/* = 128*/)
{
	const int VERTEX_COUNT = 8;
	D3DXVECTOR3 vecVertex[VERTEX_COUNT];
	vecVertex[0] = D3DXVECTOR3(-1.0f * size, -1.0f * size, -1.0f * size);
	vecVertex[1] = D3DXVECTOR3(-1.0f * size, 1.0f * size, -1.0f * size);
	vecVertex[2] = D3DXVECTOR3(1.0f * size, 1.0f * size, -1.0f * size);
	vecVertex[3] = D3DXVECTOR3(1.0f * size, -1.0f * size, -1.0f * size);
	vecVertex[4] = D3DXVECTOR3(-1.0f * size, -1.0f * size, 1.0f * size);
	vecVertex[5] = D3DXVECTOR3(-1.0f * size, 1.0f * size, 1.0f * size);
	vecVertex[6] = D3DXVECTOR3(1.0f * size, 1.0f * size, 1.0f * size);
	vecVertex[7] = D3DXVECTOR3(1.0f * size, -1.0f * size, 1.0f * size);

	//front
	triangles[0] = TextureTriangle(vecVertex[0], vecVertex[1], vecVertex[2], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	triangles[1] = TextureTriangle(vecVertex[0], vecVertex[2], vecVertex[3], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	//left
	triangles[2] = TextureTriangle(vecVertex[4], vecVertex[5], vecVertex[1], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	triangles[3] = TextureTriangle(vecVertex[4], vecVertex[1], vecVertex[0], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	//back
	triangles[4] = TextureTriangle(vecVertex[7], vecVertex[6], vecVertex[5], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	triangles[5] = TextureTriangle(vecVertex[7], vecVertex[5], vecVertex[4], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	//right
	triangles[6] = TextureTriangle(vecVertex[3], vecVertex[2], vecVertex[6], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	triangles[7] = TextureTriangle(vecVertex[3], vecVertex[6], vecVertex[7], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	//up
	//triangles[8] = TextureTriangle(vecVertex[1], vecVertex[5], vecVertex[6], D3DXVECTOR2(0, 1.f * 0.125), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[9] = TextureTriangle(vecVertex[1], vecVertex[6], vecVertex[2], D3DXVECTOR2(0, 1.f * 0.125), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1.f * 0.125));
	triangles[8] = TextureTriangle(vecVertex[1], vecVertex[5], vecVertex[6], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	triangles[9] = TextureTriangle(vecVertex[1], vecVertex[6], vecVertex[2], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	//down
	triangles[10] = TextureTriangle(vecVertex[4], vecVertex[0], vecVertex[3], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	triangles[11] = TextureTriangle(vecVertex[4], vecVertex[3], vecVertex[7], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));

	D3DXMatrixIdentity(&world);

	texture = TextureManager::GetTexture(textureFileName);

	//D3DXCreateTextureFromFile(
	//	GameManager::GetDevice(),
	//	L"./Data/Texture/Box.jpg",
	//	&texture
	//);
	/// /////////////////////////////
	//radius = rad;
	//vertexNum_sqrt = vertexNumSqrt;
	//vertexSize = vertexNum_sqrt * vertexNum_sqrt;
	//vertexIndexNum = (vertexNum_sqrt - 1) * (vertexNum_sqrt - 1);
	//
	//// 구 만들기
	//GameManager::GetDevice()->CreateVertexBuffer(
	//	vertexSize * sizeof(FVF_PositionNormalTexture),
	//	D3DUSAGE_WRITEONLY, FVF_PositionNormalTexture::FVF,
	//	D3DPOOL_MANAGED,
	//	&vertexBuffer, nullptr);
	//
	//FVF_PositionNormalTexture *pv;
	//vertexBuffer->Lock(0, 0, (void**)&pv, 0);
	//
	//for ( int v = 0; v<vertexNum_sqrt; ++v )
	//{
	//	for ( int u = 0; u<vertexNum_sqrt; ++u )
	//	{
	//		float al = 2.f * D3DX_PI * ((float)u / (vertexNum_sqrt - 1.0f));
	//		float th = 2.f * D3DX_PI * ((float)v / (vertexNum_sqrt - 1.0f));
	//		// 노말 좌표
	//		pv[v * vertexNum_sqrt + u].normal.x = sin(th) * sin(al);
	//		pv[v * vertexNum_sqrt + u].normal.y = cos(th);
	//		pv[v * vertexNum_sqrt + u].normal.z = sin(th) * cos(al);
	//		// 버텍스 좌표
	//		pv[v * vertexNum_sqrt + u].pos.x = pv[v * vertexNum_sqrt + u].normal.x * radius;
	//		pv[v * vertexNum_sqrt + u].pos.y = pv[v * vertexNum_sqrt + u].normal.y * radius;
	//		pv[v * vertexNum_sqrt + u].pos.z = pv[v * vertexNum_sqrt + u].normal.z * radius;
	//		// 텍스쳐 좌표
	//		pv[v * vertexNum_sqrt + u].tex = D3DXVECTOR2((1.0f - (al / D3DX_PI * 0.5f)),(1.0f - (th / D3DX_PI)));
	//	}
	//}
	//
	//vertexBuffer->Unlock();
	//
	//// 인덱스 버퍼
	//GameManager::GetDevice()->CreateIndexBuffer(
	//	sizeof(DWORD) * 6 * vertexIndexNum,
	//	D3DUSAGE_WRITEONLY,
	//	D3DFMT_INDEX32, D3DPOOL_MANAGED,
	//	&indexBuffer,
	//	nullptr);
	//
	//DWORD *pi;
	//indexBuffer->Lock(0, 0, (void**)&pi, 0);
	//	
	//int i = 0;
	//for ( int v = 0; v<vertexNum_sqrt - 1; ++v ) {
	//	for ( int u = 0; u<vertexNum_sqrt - 1; ++u ) {
	//		// 1
	//		pi[i++] = v * vertexNum_sqrt + u;
	//		pi[i++] = v * vertexNum_sqrt + u + 1;
	//		pi[i++] = (v + 1) * vertexNum_sqrt + u;
	//		// 2
	//		pi[i++] = (v + 1) * vertexNum_sqrt + u;
	//		pi[i++] = v * vertexNum_sqrt + u + 1;
	//		pi[i++] = (v + 1) * vertexNum_sqrt + u + 1;
	//	}
	//}
	//indexBuffer->Unlock();
}

void SkyBox::Destroy()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	//
	SAFE_RELEASE(texture);
	texture = nullptr;

}

void SkyBox::Update()
{
	D3DXMATRIXA16 roty;
	D3DXMatrixRotationY(&roty, GameManager::GetTick() * 0.03f);
	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	world *= roty * trans;
}

void SkyBox::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	//GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		TRIANGLE_COUNT,
		&triangles[0],
		sizeof(FVF_PositionTexture));


	///
	//GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	//GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	//GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	//GameManager::GetDevice()->SetStreamSource(0, vertexBuffer, 0, sizeof(FVF_PositionNormalTexture));
	//GameManager::GetDevice()->SetIndices(indexBuffer);
	//GameManager::GetDevice()->SetTexture(0, texture);
	//
	////GameManager::GetDevice()->SetMaterial(&material);
	//GameManager::GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLESTRIP,
	//	0, 0,
	//	vertexSize-2,
	//	0, 
	//	vertexIndexNum * 2);

	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}
