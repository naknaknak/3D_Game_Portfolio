#include "stdafx.h"
#include "SkinnedMesh.h"
#include "AllocateHierarchy.h"


SkinnedMesh::SkinnedMesh( )
{
}

SkinnedMesh::~SkinnedMesh( )
{
}

void SkinnedMesh::Initialize(char* path, char* fileName)
{
	// ���� ������ ã�Ƽ� ������ ����
	SkinnedMesh* original = SkinnedMeshManager::Get( ).GetOriginal(path, fileName);

	rootFrame = original->rootFrame;
	workingPaletteSize = original->workingPaletteSize;
	workingPalette = original->workingPalette;
	effect = original->effect;
	boundingSphere = original->boundingSphere;

	// ���ϸ��̼��� ���� ���ƾ� �ϴ� Ŭ��
	original->animController->CloneAnimationController(
		original->animController->GetMaxNumAnimationOutputs( ),
		original->animController->GetMaxNumAnimationSets( ),
		original->animController->GetMaxNumTracks( ),
		original->animController->GetMaxNumEvents( ),
		&animController);

	isClone = true;

}

void SkinnedMesh::Destroy( )
{
	if (isClone == false)
	{
		// �̰��� �����ϴ� �κ��̹Ƿ� ���� �����ø� ���
		AllocateHierarchy ah;
		D3DXFrameDestroy((LPD3DXFRAME)rootFrame, &ah);
		SAFE_DELETE_ARRAY_ONLY(workingPalette);
		SAFE_RELEASE(effect);
	}

	// �̰� Ŭ�к��� �� ���� �����Ƿ� ������ ����
	SAFE_RELEASE(animController);
}

void SkinnedMesh::UpdateAndRender( )
{
	//rootFrame�� �����ϱ� ������ �� ������Ʈ�� �Ͼ �� ��� �׸��� �� �ۿ� ����
	if (animController)
	{
		animController->AdvanceTime(GameManager::GetTick( ), nullptr);
	}

	if (rootFrame)
	{
		D3DXMATRIXA16 local;
		D3DXMatrixTranslation(&local, position.x, position.y, position.z);
		D3DXMATRIXA16 scale;
		D3DXMatrixScaling(&scale, scaleFactor.x, scaleFactor.y, scaleFactor.z);
		local = scale*local;
		Update(rootFrame, &local);
		Render(rootFrame);
	}
}

void SkinnedMesh::Update(Bone* current, D3DXMATRIXA16* parentMatrix)
{
	if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0)
	{
		alpha += (float)GameManager::GetTick( ) * 0.01f;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0)
	{
		alpha -= (float)GameManager::GetTick( ) * 0.01f;
	}
	if (alpha > 1.0f)
	{
		alpha = 1.0f;
	}
	else if (alpha < 0.0f)
	{
		alpha = 0.0f;
	}
	/*D3DXVECTOR3 pos = GetPosition();
	if (hm->GetHeight(pos, pos.x, pos.z) == false)
	{
		float tick = (float)GameManager::GetTick();
		if ((GetAsyncKeyState('W') & 0x8000) != 0)
		{
			pos -= (-direction * moveSpeed * tick);
		}
		else if ((GetAsyncKeyState('S') & 0x8000) != 0)
		{
			pos += (-direction * moveSpeed * tick);
		}
	}
	SetPosition(pos);*/
	//��������� ��� ������(��)�� ���ؼ� ����.
	current->CombinedTransformationMatrix = current->TransformationMatrix;
	if (parentMatrix)
	{
		current->CombinedTransformationMatrix = current->CombinedTransformationMatrix * (*parentMatrix);
	}

	if (current->pFrameSibling)	//����
	{
		Update((Bone*)current->pFrameSibling, parentMatrix);
	}

	if (current->pFrameFirstChild)	//ù��° �ڽ�
	{
		Update((Bone*)current->pFrameFirstChild, &(current->CombinedTransformationMatrix));
	}
}

void SkinnedMesh::Render(Bone* current)
{
	assert(current);

	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ boneMatrixPointers�� �����Ѵ�.
	if (current->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)current->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(boneMesh->bufferBoneCombos->GetBufferPointer( ));

		D3DXMATRIXA16 matViewProj, matView, matProj;
		GameManager::GetDevice( )->GetTransform(D3DTS_VIEW, &matView);
		GameManager::GetDevice( )->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		GameManager::GetDevice( )->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		effect->SetFloat("alpha", alpha);
		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < boneMesh->numAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < boneMesh->numPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					workingPalette[dwPalEntry] =
						boneMesh->boneOffsetMatrices[dwMatrixIndex] *
						(*boneMesh->boneMatrixPointers[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			effect->SetMatrixArray("amPalette",
				workingPalette,
				boneMesh->numPaletteEntries);

			effect->SetMatrix("g_mViewProj", &matViewProj);
			effect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			effect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			effect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			effect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			effect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));



			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			effect->SetTexture("g_txScene", boneMesh->textures[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			effect->SetInt("CurNumBones", boneMesh->maxNumFaceInfls - 1);

			// set the technique we use to draw
			effect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			effect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				effect->BeginPass(uiPass);
				boneMesh->workingMesh->DrawSubset(dwAttrib);
				effect->EndPass( );
			}
			effect->End( );
		}
	}

	//��������� ��� ������(��)�� ���ؼ� ����.
	if (current->pFrameSibling)
	{
		Render((Bone*)current->pFrameSibling);
	}

	if (current->pFrameFirstChild)
	{
		Render((Bone*)current->pFrameFirstChild);
	}

	//LPD3DXANIMATIONSET animset = nullptr;
	//animController->GetAnimationSetByName("Idle", &animset);
	//LPCSTR animName;
	//animName = animset->GetName( );
	//double numanim = animset->GetPeriod( );
	//std::string numstring;
	//numstring = std::to_string(numanim);
	//RECT rc = { 15,50,16,51 };
	//UI_Manager::GetFont( )->DrawTextA(nullptr, numstring.c_str( ), numstring.length( ), &rc, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	//SAFE_RELEASE(animset);


	Debuging( );
}

void SkinnedMesh::Debuging( )
{
	char str[256];
	RECT rc = { 15, 50, 16, 51 };
	sprintf_s(str, "AnimName: %s, AnimPeriod: %.3f", animName, animPeriod);
	UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

	//RECT rc1 = { 15, 70, 10, 10 };
	//sprintf_s(str, "AnimMaxNumTracks: %d", animController->GetMaxNumTracks());
	//UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc1, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

	//RECT rc2 = { 15, 90, 10, 10 };
	//sprintf_s(str, "AnimNumAnimationSets: %d", animController->GetNumAnimationSets( ));
	//UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc2, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}
void SkinnedMesh::SetAnimationIndex(int index)
{
	//�ִϸ��̼� ����
	if (!animController)
		return;
	currentAnimationIndex = index;

	LPD3DXANIMATIONSET animSet = nullptr;
	animController->GetAnimationSet(index, &animSet);
	animController->SetTrackAnimationSet(0, animSet);
	SAFE_RELEASE(animSet);
}
void SkinnedMesh::SetAnimationName(const char *animationName)
{
	//�ִϸ��̼� ����
	if (!animController)
		return;


	LPD3DXANIMATIONSET animSet = nullptr;
	animController->GetAnimationSetByName(animationName, &animSet);
	animController->SetTrackAnimationSet(0, animSet);

	SAFE_RELEASE(animSet);
}
void SkinnedMesh::SetAnimationName(const char *animationName, double * animationTime)
{
	//�ִϸ��̼� ����
	if (!animController)
		return;


	LPD3DXANIMATIONSET animSet = nullptr;
	animController->GetAnimationSetByName(animationName, &animSet);
	animController->SetTrackAnimationSet(0, animSet);

	*animationTime = animSet->GetPeriod( );
	animPeriod = (float)*animationTime;
	animName = (char*)animSet->GetName( );
	SAFE_RELEASE(animSet);
}
void SkinnedMesh::SetRandomTrackPosition( )
{
	//��������  �ִϸ��̼� ������ ����
	animController->SetTrackPosition(0, (rand( ) % 100) / 10.0f);
}

void SkinnedMesh::Load(char* path, char* fileName)
{
	effect = LoadEffect("./Data/Shader/MultiAnimation.hpp");

	int paletteSize = 0;
	effect->GetInt("MATRIX_PALETTE_SIZE", &paletteSize);

	AllocateHierarchy ah;
	ah.SetDirectory(path);
	ah.SetDefaultPaletteSize(paletteSize);

	boundingSphere.center = (ah.GetMin( ) + ah.GetMax( )) / 2.0f;
	boundingSphere.radius = D3DXVec3Length(&(ah.GetMin( ) - ah.GetMax( )));

	char fullPath[512];
	strcpy_s(fullPath, path);
	strcat_s(fullPath, fileName);
	HRESULT result = D3DXLoadMeshHierarchyFromXA(
		fullPath,
		D3DXMESH_MANAGED,
		GameManager::GetDevice( ),
		&ah,
		NULL,
		(LPD3DXFRAME*)&rootFrame,
		&animController);

	if (workingPalette)
		delete[] workingPalette;

	workingPaletteSize = ah.GetMaxPaletteSize( );
	workingPalette = new D3DXMATRIX[workingPaletteSize];
	if (workingPalette == NULL)
	{
		workingPaletteSize = 0;
	}

	if (rootFrame)
		SetupBoneMatrixPointers(rootFrame);
}

LPD3DXEFFECT SkinnedMesh::LoadEffect(char* fileName)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL, NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	GameManager::GetDevice( )->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFileA(GameManager::GetDevice( ),
		fileName,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer( ));
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void SkinnedMesh::SetupBoneMatrixPointers(Bone* bone)
{
	assert(bone);

	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ boneMatrixPointers�� �����Ѵ�.
	if (bone->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		if (boneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = boneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
			// pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
			// D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
			// ã�Ƽ� �����Ʈ������ �ɾ����.
			DWORD dwNumBones = pSkinInfo->GetNumBones( );
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR boneName = pSkinInfo->GetBoneName(i);
				if (boneName == nullptr || strlen(boneName) == 0)
					continue;
				Bone* influence = (Bone*)D3DXFrameFind(rootFrame, boneName);
				boneMesh->boneMatrixPointers[i] = &(influence->CombinedTransformationMatrix);
			}
		}
	}

	//��������� ��� �����ӿ� ���ؼ� ����.
	if (bone->pFrameSibling)
	{
		SetupBoneMatrixPointers((Bone*)bone->pFrameSibling);
	}

	if (bone->pFrameFirstChild)
	{
		SetupBoneMatrixPointers((Bone*)bone->pFrameFirstChild);
	}
}

