#pragma once

#include "FrustumCulling.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Initialize(D3DXVECTOR3* target = nullptr);
	void Destroy();
	void Update();
	void Render();

	LRESULT CameraInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Frustum Culling
	inline bool IsInCameraFrustum(BoundingSphere* target) { return frustum.IsInFrustum(target); }
	inline bool IsInCameraFrustum(D3DXVECTOR3* target, float radius) { return frustum.IsInFrustum(target, radius); }
	inline bool IsInCameraFrustum(D3DXVECTOR3* target) { return frustum.IsInFrustum(target); }
	// Fast Frustum
	//inline bool IsInCameraFrustum(D3DXVECTOR3* target, float radius){ return frustum.IsInSphere(target, radius); }
	//inline bool IsInCameraFrustum(D3DXVECTOR3* target) { return frustum.IsInPosition(target); }

	// Get(), Set()
	inline D3DXVECTOR3 GetCameraDirectionXZ(){	return lookingDirection;}
	inline float GetRotateY() { return camRotY; }
	inline D3DXVECTOR3* GetEyePosition( ) { return &eyePosition; }

	inline void SetLookTarget(D3DXVECTOR3* target){ lookTarget = target; }
	inline void SetDistance(float distance){ camDistance = distance; }
	inline void SetRotateX(float rotation){ camRotX = rotation; }
	inline void SetHeightMap(HeightMap* inputHeightMap) { hm = inputHeightMap; }
private:

	// Frustum
	float fFov = 0.0f;
	float fAspect = 0.0f;
	float fNear = 0.0f;
	float fFar = 0.0f;

	// Camera local
	D3DXVECTOR3 eyePosition = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 upVector = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 lookingDirection;
	D3DXVECTOR3 vecForward;
	D3DXVECTOR3 vecRight;

	// Camera Move / Drag
	float camRotX = 0.53f;		//radian
	float camRotY = 0.0f;
	float camDistance = 4.5f;
	float deltaDistance = 0.0f;

	bool isDrag = false;
	POINT prevMousePos;
	bool dontGoFar = false;

	D3DXVECTOR3* lookTarget = nullptr;
	
	FrustumCulling frustum;
	HeightMap* hm = nullptr;
};

