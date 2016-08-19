#include "stdafx.h"
#include "QuadTree.h"

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)


QuadTree * QuadTree::_AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	QuadTree* pChild;

	pChild = new QuadTree(this);
	pChild->_SetCorners(nCornerTL, nCornerTR, nCornerBL, nCornerBR);

	return pChild;
}

bool QuadTree::_SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;

	m_nCenter = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.25f);

	return true;
}

bool QuadTree::_SubDivide( )
{
	int nTopEdgeCenter;
	int nBottomEdgeCenter;
	int nLeftEdgeCenter;
	int nRightEdgeCenter;
	int nCentralPoint;

	// ��ܺ� ���
	nTopEdgeCenter = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) * 0.5f);
	// �ϴܺ� ���
	nBottomEdgeCenter = (int)((m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.5f);
	// ������ ���
	nLeftEdgeCenter = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) * 0.5f);
	// ������ ���
	nRightEdgeCenter = (int)((m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) * 0.5f);
	// �� ���
	nCentralPoint = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.25f);

	// �� �̻� ������ �Ұ����Ѱ�? �׷��ٸ� SubDivide() ����
	if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
	{
		return false;
	}

	// 4���� �ڽĳ�� �߰�
	m_pChild[CORNER_TL] = _AddChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
	m_pChild[CORNER_TR] = _AddChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
	m_pChild[CORNER_BL] = _AddChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
	m_pChild[CORNER_BR] = _AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

	return true;
}

int QuadTree::_GenTriIndex(int nTriangles, LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio)
{
	// �ø��� ����� �׳� ����
	//if ((m_bCulled) && _IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio))
	//{
	//	if (m_pParent)
	//	{
	//		if (m_pParent->IsCulled( ))
	//		{
	//			m_bCulled = FALSE;
	//			return nTriangles;
	//		}
	//	}
	//}

	if (m_bCulled)
	{
		m_bCulled = FALSE;
		return nTriangles;
	}

	LPDWORD p = ((LPDWORD)pIB) + nTriangles * 3;
	if (_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition(), fLODRatio))	/// ���� ��尡 ��� ������ ����ΰ�?
	{
		if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
		{		// ���� ��� �ﰢ��
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			++nTriangles;

			// ���� �ϴ� �ﰢ��
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			++nTriangles;
			return nTriangles;
		}

		bool b[4] = { 0, };

		// ��� �̿����(neightbor node)�� ��°����Ѱ�?   
		if (m_pNeighbor[EDGE_UP]) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);
		// �ϴ� �̿����(neightbor node)�� ��°����Ѱ�?   
		if (m_pNeighbor[EDGE_DN]) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?   
		if (m_pNeighbor[EDGE_LT]) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?   
		if (m_pNeighbor[EDGE_RT]) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);

		// �̿������� ��δ� ��°����ϴٸ� ������� �̿������� ���� LOD�̹Ƿ� �κк����� �ʿ����.   
		if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTriangles++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTriangles++;
			return nTriangles;
		}


		int n;
		if (!b[EDGE_UP]) // ��� �κк����� �ʿ��Ѱ�?   
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TR]; nTriangles++;
		}
		else    // ��� �κк����� �ʿ���� ���   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = m_nCorner[CORNER_TR]; nTriangles++;
		}

		if (!b[EDGE_DN]) // �ϴ� �κк����� �ʿ��Ѱ�?   
		{
			n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BL]; nTriangles++;
		}
		else    // �ϴ� �κк����� �ʿ���� ���   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = m_nCorner[CORNER_BL]; nTriangles++;
		}

		if (!b[EDGE_LT]) // ���� �κк����� �ʿ��Ѱ�?   
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TL]; nTriangles++;
		}
		else    // ���� �κк����� �ʿ���� ���   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = m_nCorner[CORNER_TL]; nTriangles++;
		}

		if (!b[EDGE_RT]) // ���� �κк����� �ʿ��Ѱ�?   
		{
			n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BR]; nTriangles++;
		}
		else    // ���� �κк����� �ʿ���� ���   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = m_nCorner[CORNER_BR]; nTriangles++;
		}

		return nTriangles;   // �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!   
	}
	
	// �ڽ� ���� �˻�
	if (m_pChild[CORNER_TL]) nTriangles = m_pChild[CORNER_TL]->_GenTriIndex(nTriangles, pIB, pHeightMapVertex, fLODRatio);
	if (m_pChild[CORNER_TR]) nTriangles = m_pChild[CORNER_TR]->_GenTriIndex(nTriangles, pIB, pHeightMapVertex, fLODRatio);
	if (m_pChild[CORNER_BL]) nTriangles = m_pChild[CORNER_BL]->_GenTriIndex(nTriangles, pIB, pHeightMapVertex, fLODRatio);
	if (m_pChild[CORNER_BR]) nTriangles = m_pChild[CORNER_BR]->_GenTriIndex(nTriangles, pIB, pHeightMapVertex, fLODRatio);

	return nTriangles;
}

void QuadTree::Destroy( )
{
	for (int i = 0; i < CORNER_NUM; ++i)
	{
		SAFE_DELETE(m_pChild[i]);
	}
}

int QuadTree::_IsInFrustum(D3DXVECTOR3 * pHeightMapVertex)
{
	bool b[4];
	bool bInSphere;

	// ��豸 �ȿ� �ִ°�?
	bInSphere = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCenter), m_fRadius);
	if (!bInSphere) return FRUSTUM_OUT;		// ��豸 �ȿ� ������ �������� ����ü �˻� ����

	//����Ʈ���� 4���� ��� ����ü �׽�Ʈ, �� ������ ����ü �˻�
	b[0] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[0]));
	b[1] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[1]));
	b[2] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[2]));
	b[3] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[3]));

	// 4�� ��� ����ü �ȿ� ����
	if ((b[0] + b[1] + b[2] + b[3]) == CORNER_NUM) return FRUSTUM_COMPLETELY_IN;

	// �Ϻκ��� ����ü�� �ִ� ���
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::_FrustumCull(D3DXVECTOR3 * pHeightMapVertex)
{
	int ret;
	ret = _IsInFrustum(pHeightMapVertex);
	switch (ret)
	{
		case FRUSTUM_COMPLETELY_IN:		// ����ü�� ������ ���Ե�, ������� �˻� �ʿ� ����.
			_AllInFrustum( );
			return;
		case FRUSTUM_PARTIALLY_IN:		// ����ü�� �Ϻ� ���Ե�, ������� �˻� �ʿ� ��.
			m_bCulled = FALSE;
			break;
		case FRUSTUM_OUT:				// ����ü���� ������ ���, ������� �˻� �ʿ� ����.
			m_bCulled = TRUE;
			return;
	}
	if (m_pChild[CORNER_TL]) m_pChild[CORNER_TL]->_FrustumCull(pHeightMapVertex);
	if (m_pChild[CORNER_TR]) m_pChild[CORNER_TR]->_FrustumCull(pHeightMapVertex);
	if (m_pChild[CORNER_BL]) m_pChild[CORNER_BL]->_FrustumCull(pHeightMapVertex);
	if (m_pChild[CORNER_BR]) m_pChild[CORNER_BR]->_FrustumCull(pHeightMapVertex);

}

void QuadTree::_AllInFrustum( )
{
	m_bCulled = FALSE;
	if (!m_pChild[0]) return;
	m_pChild[CORNER_TL]->_AllInFrustum( );
	m_pChild[CORNER_TR]->_AllInFrustum( );
	m_pChild[CORNER_BL]->_AllInFrustum( );
	m_pChild[CORNER_BR]->_AllInFrustum( );
}

void QuadTree::_BuildNeighborNode(QuadTree * pRoot, D3DXVECTOR3 * pHeightMapVertex, int cx)
{
	int	n;
	int	_0, _1, _2, _3;

	for (int i = 0; i < EDGE_NUM; i++)
	{
		_0 = m_nCorner[CORNER_TL];
		_1 = m_nCorner[CORNER_TR];
		_2 = m_nCorner[CORNER_BL];
		_3 = m_nCorner[CORNER_BR];
		// �̿������ 4�� �ڳʰ��� ��´�.
		n = _GetNodeIndex(i, cx, _0, _1, _2, _3);
		// �ڳʰ����� �̿������ �����͸� ���´�.
		if (n >= 0) m_pNeighbor[i] = pRoot->_FindNode(pHeightMapVertex, _0, _1, _2, _3);
	}

	// �ڽĳ��� ���ȣ��
	if (m_pChild[0])
	{
		m_pChild[CORNER_TL]->_BuildNeighborNode(pRoot, pHeightMapVertex, cx);
		m_pChild[CORNER_TR]->_BuildNeighborNode(pRoot, pHeightMapVertex, cx);
		m_pChild[CORNER_BL]->_BuildNeighborNode(pRoot, pHeightMapVertex, cx);
		m_pChild[CORNER_BR]->_BuildNeighborNode(pRoot, pHeightMapVertex, cx);
	}
}

bool QuadTree::_BuildQuadTree(D3DXVECTOR3 * pHeightMapVertex)
{
	if (_SubDivide( ))
	{
		// ���� ��ܰ� ���� �ϴ��� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[CORNER_TL])) - *((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[CORNER_BR]));

		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 2�� ������ �������� ���Ѵ�.
		m_fRadius = D3DXVec3Length(&v) * 0.5f;

		m_pChild[CORNER_TL]->_BuildQuadTree(pHeightMapVertex);
		m_pChild[CORNER_TR]->_BuildQuadTree(pHeightMapVertex);
		m_pChild[CORNER_BL]->_BuildQuadTree(pHeightMapVertex);
		m_pChild[CORNER_BR]->_BuildQuadTree(pHeightMapVertex);
	}

	return true;
}

QuadTree * QuadTree::_FindNode(D3DXVECTOR3 * pHeightMapVertex, int _0, int _1, int _2, int _3)
{
	QuadTree* p = NULL;

	// ��ġ�ϴ� ����� ��尪�� ����
	if (((m_nCorner[CORNER_TL] == _0) && (m_nCorner[CORNER_TR] == _1)) && ((m_nCorner[CORNER_BL] == _2) && (m_nCorner[CORNER_BR] == _3)))
		return this;

	// �ڽ� ��尡 �ְ�?
	if (m_pChild[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) * 0.25;

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pHeightMapVertex[n].x;
		pt.y = (int)pHeightMapVertex[n].z;

		// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
		SetRect(&rc, 
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_BR]].z);
		// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
		if (IsInRect(&rc, pt))
			return m_pChild[0]->_FindNode(pHeightMapVertex, _0, _1, _2, _3);

		SetRect(&rc, 
			(int)pHeightMapVertex[m_pChild[CORNER_TR]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_TR]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMapVertex[m_pChild[CORNER_TR]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_TR]->m_nCorner[CORNER_BR]].z);
		if (IsInRect(&rc, pt))
			return m_pChild[1]->_FindNode(pHeightMapVertex, _0, _1, _2, _3);

		SetRect(&rc,
			(int)pHeightMapVertex[m_pChild[CORNER_BL]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_BL]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMapVertex[m_pChild[CORNER_BL]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_BL]->m_nCorner[CORNER_BR]].z);
		if (IsInRect(&rc, pt))
			return m_pChild[2]->_FindNode(pHeightMapVertex, _0, _1, _2, _3);

		SetRect(&rc,
			(int)pHeightMapVertex[m_pChild[CORNER_BR]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_BR]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMapVertex[m_pChild[CORNER_BR]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_BR]->m_nCorner[CORNER_BR]].z);
		if (IsInRect(&rc, pt))
			return m_pChild[3]->_FindNode(pHeightMapVertex, _0, _1, _2, _3);
	}

	return nullptr;
}

int QuadTree::_GetNodeIndex(int ed, int cx, int & _0, int & _1, int & _2, int & _3)
{
	int	n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _b - _a;	// ���� ����� �¿�����

	switch (ed)
	{
		case EDGE_UP:	// ���� ���� �̿������ �ε���
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// �Ʒ� ���� �̿������ �ε���
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// ���� ���� �̿������ �ε���
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// ���� ���� �̿������ �ε���
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if ((_0 / cx) != (_a / cx)) return -1;
			if ((_1 / cx) != (_b / cx)) return -1;
			break;
	}

	n = (_0 + _1 + _2 + _3) * 0.25;	// ��� �ε���
	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

	return 0;
}

QuadTree::QuadTree( )
{
}

QuadTree::QuadTree(int cx, int cy)
{
	int i;
	m_nCenter = 0;
	for (i = 0; i < CORNER_NUM; i++);
	{
		m_pChild[i] = nullptr;
	}
	m_nCorner[CORNER_TL] = 0;
	m_nCorner[CORNER_TR] = cx - 1;
	m_nCorner[CORNER_BL] = cx * (cy - 1);
	m_nCorner[CORNER_BR] = cx * cy - 1;
	m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.25f;

}

QuadTree::QuadTree(QuadTree * pParent)
{
	int i;
	m_nCenter = 0;
	for (i = 0; i < CORNER_NUM; ++i)
	{
		m_pChild[i] = nullptr;
		m_pNeighbor[i] = nullptr;
		m_nCorner[i] = 0;
	}
}

bool QuadTree::Build(D3DXVECTOR3* pHeightMapVertex)
{
	// ����Ʈ�� ����   
	_BuildQuadTree(pHeightMapVertex);
	// �̿���� ����   
	_BuildNeighborNode(this, pHeightMapVertex, m_nCorner[CORNER_TR] + 1);
	return TRUE;
}

int QuadTree::GenerateIndex(LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio)
{
	_FrustumCull(pHeightMapVertex);
	return _GenTriIndex(0, pIB, pHeightMapVertex, fLODRatio);
}

QuadTree::~QuadTree( )
{
}
// pt�� rc�ȿ� ���ԵǴ��� �˻��Ѵ�.(PtInRect()��� API�Լ��� ����(-)�� ó���� ���ߴ�.)
BOOL QuadTree::IsInRect(RECT* rc, POINT pt)
{
	if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
		(rc->bottom <= pt.y) && (pt.y <= rc->top))
		return TRUE;

	return FALSE;
}