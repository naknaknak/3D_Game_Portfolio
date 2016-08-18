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
		&frustum[0], &frustum[3], &frustum[2]);//�Ʒ��� ��
	D3DXPlaneFromPoints(&frustumPlaneArray[1],
		&frustum[7], &frustum[6], &frustum[2]);//������ ��
	D3DXPlaneFromPoints(&frustumPlaneArray[2],
		&frustum[6], &frustum[5], &frustum[1]);	//far��
	D3DXPlaneFromPoints(&frustumPlaneArray[3],
		&frustum[5], &frustum[4], &frustum[0]);	//���� ��
	D3DXPlaneFromPoints(&frustumPlaneArray[4],
		&frustum[4], &frustum[7], &frustum[3]);	//near��
	D3DXPlaneFromPoints(&frustumPlaneArray[5],
		&frustum[5], &frustum[6], &frustum[7]);	//���� ��

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
	
	float fForwardDot = D3DXVec3Dot(&vecDirTarget, &m_vForward);		// m_vForward�� vecDirTarget�� �����Ͽ� ����� ��ġ�� ���Ѵ�.
	if (fForwardDot < m_fNear || m_fFar < fForwardDot)				//������ ��ġ�� near�� far ���̿� �ִ���?
		return false;

	float fRightDot = D3DXVec3Dot(&vecDirTarget, &m_vRight);	//������ ���Ϳ� ���� �����Ͽ� ������ ��ġ�� ���Ѵ�.
	float rLimit = m_fRightFactor * fForwardDot;
	if (fRightDot < -rLimit || rLimit < fRightDot)						//���� ��ġ�� ����ü ������ ������� �˻��Ѵ�.
		return false;
	
	float fUpDot = D3DXVec3Dot(&vecDirTarget, &m_vUp);		// ���� ���Ϳ� ���� �����Ͽ� ������ ��ġ�� ���Ѵ�.
	float uLimit = m_fUpFactor * fForwardDot;
	if (fUpDot < -uLimit || uLimit < fUpDot)						// ���� ��ġ�� ����ü ������ ������� �˻��Ѵ�.
		return false;

	return true;
}

bool FrustumCulling::IsInSphere(D3DXVECTOR3 * target, float radius)
{
	D3DXVECTOR3	vecDirTarget = *target - m_vEyePosition;

	float fForwardDot = D3DXVec3Dot(&vecDirTarget, &m_vForward);				// m_vForward�� vecDirTarget�� �����Ͽ� ����� ��ġ�� ���Ѵ�.
	if (fForwardDot + radius < m_fNear || m_fFar < fForwardDot - radius);		//������ ��ġ�� near�� far ���̿� �ִ���?
		return false;

	float fRightDot = D3DXVec3Dot(&vecDirTarget, &m_vRight);					//������ ���Ϳ� ���� �����Ͽ� ������ ��ġ�� ���Ѵ�.
	float rLimit = m_fRightFactor * fForwardDot;
	if (fRightDot + radius < -rLimit || rLimit < fRightDot - radius)			//���� ��ġ�� ����ü ������ ������� �˻��Ѵ�.
		return false;

	float fUpDot = D3DXVec3Dot(&vecDirTarget, &m_vUp);							// ���� ���Ϳ� ���� �����Ͽ� ������ ��ġ�� ���Ѵ�.
	float uLimit = m_fUpFactor * fForwardDot;
	if (fUpDot + radius < -uLimit || uLimit < fUpDot - radius)					// ���� ��ġ�� ����ü ������ ������� �˻��Ѵ�.
		return false;

	return true;

}