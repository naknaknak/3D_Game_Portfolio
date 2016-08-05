#pragma once
#include "AllocateHierarchy.h"

class SkinnedMesh
{
	//friend class SkinnedMeshManager;	// �̷��� �ϸ� SkinnedMeshManager�� SkinnedMesh�� private�� ������� ��� �� �� �ְ� �ȴ�.

public:
	SkinnedMesh();
	~SkinnedMesh();

	void Initialize(char* path, char* fileName);	// ���ϴ� ������ Ŭ�δ� �ϴ� �Լ�
	void Destroy();
	void UpdateAndRender();

	void SetAnimationIndex(int index);	// ��� �� �ִϸ��̼� ����
	void SetAnimationName(const char *animationName);
	void SetAnimationName(const char * animationName, double *time);
	void SetRandomTrackPosition();		// �׽�Ʈ��(�ִϸ��̼� ��� ��ġ�� �����ϰ� ����)


	void Load(char* path, char* fileName);	//���� X���� �θ��� �Լ�. SkinnedMeshManager�ȿ����� ��� �� ��

	inline void SetPosition(D3DXVECTOR3 v){	position = v; boundingSphere.center = v; }	
	inline BoundingSphere* GetBoundingSphere() { return &boundingSphere;	}
	inline D3DXVECTOR3 GetPositon() { return position; }
protected:
	// �ϳ��� ����(������ ����)
	Bone* rootFrame = nullptr;
	DWORD workingPaletteSize = 0;
	D3DXMATRIX* workingPalette = nullptr;
	LPD3DXEFFECT effect = nullptr;
	BoundingSphere boundingSphere = BoundingSphere();
	BoundingBox boundingBox = BoundingBox();

	// ��ü���� ������ �־�� �� ��
	LPD3DXANIMATIONCONTROLLER animController = nullptr;
	D3DXVECTOR3 position = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 scaleFactor = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	int currentAnimationIndex = 0;
	double selectedAnimationLength = 0.0f;
	double currentAnimationTime = 0.0f;

	// Ŭ�� �Ȱ��� �������� ǥ�ÿ�
	bool isClone = false;
		
	// ��Ÿ ���������� �ʿ��� �Լ���
	void Update(Bone* current, D3DXMATRIXA16* parentMatrix);
	void Render(Bone* bone);
	void SetupBoneMatrixPointers(Bone* bone);	//�� �ʱ�ȭ

	LPD3DXEFFECT LoadEffect(char* fileName);	//����Ʈ �θ��� �Լ�. �ڼ��Ѱ� ���̴���

	float alpha = 1.0f;
};

