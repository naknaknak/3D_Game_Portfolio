#include "stdafx.h"
#include "FrustumCulling.h"


FrustumCulling::FrustumCulling( )
{
}


FrustumCulling::~FrustumCulling( )
{
}

void FrustumCulling::Initialize( )
{
	projectionSpacePointArray.clear( );
	projectionSpacePointArray.resize(8);
	projectionSpacePointArray[0] = D3DXVECTOR3(-1, -1, 0);
	projectionSpacePointArray[1] = D3DXVECTOR3(-1, -1, 1);
	projectionSpacePointArray[2] = D3DXVECTOR3(1, -1, 1);
	projectionSpacePointArray[3] = D3DXVECTOR3(1, -1, 0);
	projectionSpacePointArray[4] = D3DXVECTOR3(-1, 1, 0);
	projectionSpacePointArray[5] = D3DXVECTOR3(-1, 1, 1);
	projectionSpacePointArray[6] = D3DXVECTOR3(1, 1, 1);
	projectionSpacePointArray[7] = D3DXVECTOR3(1, 1, 0);

	frustumPlaneArray.clear( );
	frustumPlaneArray.resize(6);
}

void FrustumCulling::Update( )
{
	D3DXMATRIXA16 projection, view;
	GameManager::GetDevice( )->GetTransform(D3DTS_PROJECTION, &projection);
	GameManager::GetDevice( )->GetTransform(D3DTS_VIEW, &view);

	std::vector<D3DXVECTOR3> frustum(8);
	for (size_t i = 0; i < projectionSpacePointArray.size( ); ++i)
	{
		D3DXVec3Unproject(
			&frustum[i],
			&projectionSpacePointArray[i],
			nullptr,
			&projection,
			&view,
			nullptr);
	}
	D3DXPlaneFromPoints(&frustumPlaneArray[0],
		&frustum[0], &frustum[3], &frustum[2]);//아래쪽 면
	D3DXPlaneFromPoints(&frustumPlaneArray[1],
		&frustum[7], &frustum[6], &frustum[2]);//오른쪽 면
	D3DXPlaneFromPoints(&frustumPlaneArray[2],
		&frustum[6], &frustum[5], &frustum[1]);	//far면
	D3DXPlaneFromPoints(&frustumPlaneArray[3],
		&frustum[5], &frustum[4], &frustum[0]);	//왼쪽 면
	D3DXPlaneFromPoints(&frustumPlaneArray[4],
		&frustum[4], &frustum[7], &frustum[3]);	//near면
	D3DXPlaneFromPoints(&frustumPlaneArray[5],
		&frustum[5], &frustum[6], &frustum[7]);	//위쪽 면

}

bool FrustumCulling::IsInFrustum(BoundingSphere* target)
{
	for (size_t i = 0; i < frustumPlaneArray.size( ); ++i)
	{
		if (D3DXPlaneDotCoord(&frustumPlaneArray[i], &target->center) > target->radius)
		{
			return false;
		}
	}

	return true;
}

bool FrustumCulling::IsInFrustum(D3DXVECTOR3 * target)
{
	for (size_t i = 0; i < frustumPlaneArray.size( ); ++i)
	{
		float fDist = D3DXPlaneDotCoord(&frustumPlaneArray[i], target);
		if (fDist > 0.001f)
		{
			return false;
		}
	}
	return true;
}

bool FrustumCulling::IsInFrustum(D3DXVECTOR3 * target, float radius)
{
	for (size_t i = 0; i < frustumPlaneArray.size( ); ++i)
	{
		float fDist = D3DXPlaneDotCoord(&frustumPlaneArray[i], target);
		if (fDist - radius > 0.001f)
		{
			return false;
		}
	}
	return true;
}
void FrustumCulling::SetPerspective(const float fFOV, const float fViewAspect, const float fNearZ, const float fFarZ)
{
	m_fRightFactor = tanf(fFOV);
	m_fUpFactor = m_fRightFactor * fViewAspect;
	m_fNear = fNearZ;
	m_fFar = fFarZ;
}
void FrustumCulling::Build(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vForward, const D3DXVECTOR3& vRight, const D3DXVECTOR3& vUp)
{
	m_vEyePosition = vEye;
	m_vForward = vForward;
	m_vRight = vRight;
	m_vUp = vUp;
}
bool FrustumCulling::IsInPosition(D3DXVECTOR3* target)
{
	D3DXVECTOR3	vecDirTarget = *target - m_vEyePosition;
	
	float fForwardDot = D3DXVec3Dot(&vecDirTarget, &m_vForward);		// m_vForward와 vecDirTarget을 내적하여 상대적 위치를 구한다.
	if (fForwardDot < m_fNear || m_fFar < fForwardDot)				//투영된 위치가 near와 far 사이에 있느가?
		return false;

	float fRightDot = D3DXVec3Dot(&vecDirTarget, &m_vRight);	//오른쪽 벡터와 점을 내적하여 투영된 위치를 구한다.
	float rLimit = m_fRightFactor * fForwardDot;
	if (fRightDot < -rLimit || rLimit < fRightDot)						//점의 위치가 절두체 범위에 벗어나는지 검사한다.
		return false;
	
	float fUpDot = D3DXVec3Dot(&vecDirTarget, &m_vUp);		// 위쪽 벡터와 점을 내적하여 투영된 위치를 구한다.
	float uLimit = m_fUpFactor * fForwardDot;
	if (fUpDot < -uLimit || uLimit < fUpDot)						// 점의 위치가 절두체 범위에 벗어나는지 검사한다.
		return false;

	return true;
}

bool FrustumCulling::IsInSphere(D3DXVECTOR3 * target, float radius)
{
	D3DXVECTOR3	vecDirTarget = *target - m_vEyePosition;

	float fForwardDot = D3DXVec3Dot(&vecDirTarget, &m_vForward);				// m_vForward와 vecDirTarget을 내적하여 상대적 위치를 구한다.
	if (fForwardDot + radius < m_fNear || m_fFar < fForwardDot - radius);		//투영된 위치가 near와 far 사이에 있느가?
		return false;

	float fRightDot = D3DXVec3Dot(&vecDirTarget, &m_vRight);					//오른쪽 벡터와 점을 내적하여 투영된 위치를 구한다.
	float rLimit = m_fRightFactor * fForwardDot;
	if (fRightDot + radius < -rLimit || rLimit < fRightDot - radius)			//점의 위치가 절두체 범위에 벗어나는지 검사한다.
		return false;

	float fUpDot = D3DXVec3Dot(&vecDirTarget, &m_vUp);							// 위쪽 벡터와 점을 내적하여 투영된 위치를 구한다.
	float uLimit = m_fUpFactor * fForwardDot;
	if (fUpDot + radius < -uLimit || uLimit < fUpDot - radius)					// 점의 위치가 절두체 범위에 벗어나는지 검사한다.
		return false;

	return true;

}