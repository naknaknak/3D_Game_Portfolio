#include "stdafx.h"
#include "SkyBox.h"

SkyBox::SkyBox( )
{
}


SkyBox::~SkyBox( )
{
}

//void SkyBox::Initialize(char* textureFileName, float size)
void SkyBox::Initialize(char* textureFileName, float fRad/* = 128.f*/, UINT slices, UINT stacks )
{
	//const int VERTEX_COUNT = 8;
	//D3DXVECTOR3 vecVertex[VERTEX_COUNT];
	//vecVertex[0] = D3DXVECTOR3(-1.0f * size, -1.0f * size, -1.0f * size);
	//vecVertex[1] = D3DXVECTOR3(-1.0f * size, 1.0f * size, -1.0f * size);
	//vecVertex[2] = D3DXVECTOR3(1.0f * size, 1.0f * size, -1.0f * size);
	//vecVertex[3] = D3DXVECTOR3(1.0f * size, -1.0f * size, -1.0f * size);
	//vecVertex[4] = D3DXVECTOR3(-1.0f * size, -1.0f * size, 1.0f * size);
	//vecVertex[5] = D3DXVECTOR3(-1.0f * size, 1.0f * size, 1.0f * size);
	//vecVertex[6] = D3DXVECTOR3(1.0f * size, 1.0f * size, 1.0f * size);
	//vecVertex[7] = D3DXVECTOR3(1.0f * size, -1.0f * size, 1.0f * size);
	//
	////front
	//triangles[0] = TextureTriangle(vecVertex[0], vecVertex[1], vecVertex[2], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[1] = TextureTriangle(vecVertex[0], vecVertex[2], vecVertex[3], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	////left
	//triangles[2] = TextureTriangle(vecVertex[4], vecVertex[5], vecVertex[1], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[3] = TextureTriangle(vecVertex[4], vecVertex[1], vecVertex[0], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	////back
	//triangles[4] = TextureTriangle(vecVertex[7], vecVertex[6], vecVertex[5], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[5] = TextureTriangle(vecVertex[7], vecVertex[5], vecVertex[4], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	////right
	//triangles[6] = TextureTriangle(vecVertex[3], vecVertex[2], vecVertex[6], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[7] = TextureTriangle(vecVertex[3], vecVertex[6], vecVertex[7], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	////up
	////triangles[8] = TextureTriangle(vecVertex[1], vecVertex[5], vecVertex[6], D3DXVECTOR2(0, 1.f * 0.125), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	////triangles[9] = TextureTriangle(vecVertex[1], vecVertex[6], vecVertex[2], D3DXVECTOR2(0, 1.f * 0.125), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1.f * 0.125));
	//triangles[8] = TextureTriangle(vecVertex[1], vecVertex[5], vecVertex[6], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[9] = TextureTriangle(vecVertex[1], vecVertex[6], vecVertex[2], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	////down
	//triangles[10] = TextureTriangle(vecVertex[4], vecVertex[0], vecVertex[3], D3DXVECTOR2(0, 1), D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0));
	//triangles[11] = TextureTriangle(vecVertex[4], vecVertex[3], vecVertex[7], D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(1, 1));
	//
	//texture = TextureManager::GetTexture(textureFileName);
	//
	//D3DXMatrixIdentity(&world);


	// sphere
//	texture = TextureManager::GetTexture(textureFileName);

	if (textureFileName != nullptr)
	{
		pTexDiffuse = TextureManager::GetTexture(textureFileName);
		pTexNormal = TextureManager::GetNormalMap(textureFileName);
	}
	sphereMesh = CreateMappedSphere(fRad, slices, stacks);
	
	D3DXMatrixIdentity(&world);
	
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

}
LPD3DXMESH SkyBox::CreateMappedSphere(float fRad, UINT slices, UINT stacks)
{
	// create the sphere
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(GameManager::GetDevice( ), fRad, slices, stacks, &mesh, NULL)))
		return NULL;

	// create a copy of the mesh with texture coordinates,
	// since the D3DX function doesn't include them
	LPD3DXMESH texMesh;
	if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice( ), &texMesh)))
		return mesh;	// failed, return un-textured mesh
	
	mesh->Release( );		// finished with the original mesh, release it


	// lock the vertex buffer
	FVF_PositionNormalTexture* pVerts;
	//if (SUCCEEDED(texMesh->LockVertexBuffer(0, (BYTE **)&pVerts)))
	if (SUCCEEDED(texMesh->LockVertexBuffer(0, (LPVOID*)&pVerts)))
	{
		int numVerts = texMesh->GetNumVertices( );		// get vertex count

		// loop through the vertices
		for (int i = 0; i < numVerts; i++)
		{
			// calculate texture coordinates
			pVerts->tex.x = asinf(pVerts->normal.x) / D3DX_PI + 0.5f;
			pVerts->tex.y = asinf(pVerts->normal.y) / D3DX_PI + 0.5f;

			//pVerts->tex.x = 0.5f - (atan2f(pVerts->normal.z, pVerts->normal.x) / (2 * D3DX_PI));
			//pVerts->tex.y = 0.5f - asinf(pVerts->normal.y) / (D3DX_PI);
			
			//pVerts->tex.y = pVerts->normal.y * 0.5 + 0.5;

			//if (pVerts->tex.x <(FLOAT)0.9) pVerts->tex.x = (FLOAT)0.0;


			//pVerts->tex.x = pVerts->pos.x / sqrtf((pVerts->pos.x * pVerts->pos.x) + (pVerts->pos.y * pVerts->pos.x) + (pVerts->pos.z * pVerts->pos.z));
			//pVerts->tex.y = pVerts->pos.y / sqrtf((pVerts->pos.x * pVerts->pos.x) + (pVerts->pos.y * pVerts->pos.x) + (pVerts->pos.z * pVerts->pos.z));

			//float theta = asinf(pVerts->normal.z);
			//float phi = atan2(pVerts->normal.y, pVerts->normal.x);
			//
			//pVerts->tex = D3DXVECTOR2(phi / 2 / 3.14159265, theta /  3.14159265);

			// go to next vertex
			pVerts++; 

		}
	
		texMesh->UnlockVertexBuffer( );		// unlock the vertex buffer
	}

	// return pointer to caller
	return texMesh;
}

void SkyBox::Destroy( )
{
	SAFE_RELEASE(sphereMesh);


}

void SkyBox::Update( )
{
	D3DXMATRIXA16 roty;
	D3DXMatrixRotationY(&roty, (float)GameManager::GetTick( ) * 0.03f);
	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	world *= roty * trans;


	if (GetAsyncKeyState( VK_RETURN) & 0x8001)
	{
		wireMode = true;
	}
	else
	{
		wireMode = false;
	}

}

void SkyBox::Render( )
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	GameManager::GetDevice( )->SetTexture(0, pTexNormal);
	GameManager::GetDevice( )->SetTexture(1, pTexDiffuse);
	GameManager::GetDevice( )->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GameManager::GetDevice( )->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

	//GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BUMPENVMAPLUMINANCE);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	// box
	//GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	//GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);
	//GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	//GameManager::GetDevice()->SetTexture(0, texture);
	//GameManager::GetDevice()->DrawPrimitiveUP(
	//	D3DPT_TRIANGLELIST,
	//	TRIANGLE_COUNT,
	//	&triangles[0],
	//	sizeof(FVF_PositionTexture));

	// sphere
	GameManager::GetDevice( )->SetRenderState(D3DRS_LIGHTING, false);
	GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	//GameManager::GetDevice( )->SetTexture(0, texture);
	GameManager::GetDevice( )->SetMaterial(&material);
	GameManager::GetDevice( )->SetTransform(D3DTS_WORLD, &world);
	sphereMesh->DrawSubset(0);

	if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	GameManager::GetDevice( )->SetTexture(0, nullptr);
	GameManager::GetDevice( )->SetTexture(1, nullptr);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}
