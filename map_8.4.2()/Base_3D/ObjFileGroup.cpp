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
		GameManager::GetDevice()->SetMaterial(&( materialTexture->material ));
		//GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		//GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		
		
		GameManager::GetDevice( )->SetTexture(0, materialTexture->pTexNormal);
		GameManager::GetDevice( )->SetTexture(1, materialTexture->pTexDiffuse);
		GameManager::GetDevice( )->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		GameManager::GetDevice( )->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

		//GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BUMPENVMAPLUMINANCE);
		GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

		GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	
		GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		GameManager::GetDevice( )->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		GameManager::GetDevice( )->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
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
	//GameManager::GetDevice( )->SetRenderState(D3DRS_LIGHTING, false);
	GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		vertex.size() / 3,
		&vertex[0],
		sizeof(FVF_PositionNormalTexture));


	GameManager::GetDevice( )->SetTexture(0, nullptr);
	GameManager::GetDevice( )->SetTexture(1, nullptr);
	GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)
	GameManager::GetDevice( )->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GameManager::GetDevice( )->SetRenderState(D3DRS_LIGHTING, true);

}