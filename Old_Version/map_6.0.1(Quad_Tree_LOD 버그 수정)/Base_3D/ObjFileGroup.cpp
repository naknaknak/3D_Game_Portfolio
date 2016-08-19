#include "stdafx.h"
#include "ObjFileGroup.h"


ObjFileGroup::ObjFileGroup(MaterialTexture* _materialTexture, std::vector<FVF_PositionNormalTexture>& _vertex)
	:materialTexture(_materialTexture)
{
	if (materialTexture)
	{
		materialTexture->AddReference();
	}
	vertex = std::move(_vertex);
}


ObjFileGroup::~ObjFileGroup()
{
	SAFE_RELEASE(materialTexture);
}

void ObjFileGroup::Render()
{
	if ( materialTexture )
	{
		GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		GameManager::GetDevice( )->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		GameManager::GetDevice( )->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		GameManager::GetDevice()->SetMaterial(&( materialTexture->material ));
		GameManager::GetDevice()->SetTexture(0, materialTexture->texture);
		GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	}
	else
	{
		D3DMATERIAL9 mat;
		ZeroMemory(&mat, sizeof(D3DMATERIAL9));
		mat.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		mat.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		GameManager::GetDevice()->SetMaterial(&mat);
		GameManager::GetDevice()->SetTexture(0, nullptr);
	}
	GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		vertex.size() / 3,
		&vertex[0],
		sizeof(FVF_PositionNormalTexture));
	GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}