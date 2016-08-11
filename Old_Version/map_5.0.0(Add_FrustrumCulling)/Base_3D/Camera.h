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

	inline bool IsInCameraFrustum(BoundingSphere* target) { return frustum.IsInFrustum(target); }

	inline D3DXVECTOR3 GetCameraDirectionXZ(){	return lookingDirection;}
	inline float GetRotateY() { return camRotY; }

	inline void SetLookTarget(D3DXVECTOR3* target){ lookTarget = target; }
	inline void SetDistance(float distance){ camDistance = distance; }
	inline void SetRotateX(float rotation){ camRotX = rotation; }
	
private:
	D3DXVECTOR3 eyePosition = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 upVector = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 lookingDirection;

	float camRotX = 0.53f;		//radian
	float camRotY = 0.0f;
	float camDistance = 4.5f;

	bool isDrag = false;
	POINT prevMousePos;

	D3DXVECTOR3* lookTarget = nullptr;
	
	FrustumCulling frustum;

};

