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

	// 상단변 가운데
	nTopEdgeCenter = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) * 0.5f);
	// 하단변 가운데
	nBottomEdgeCenter = (int)((m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.5f);
	// 좌측변 가운데
	nLeftEdgeCenter = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) * 0.5f);
	// 우측변 가운데
	nRightEdgeCenter = (int)((m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) * 0.5f);
	// 한 가운데
	nCentralPoint = (int)((m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.25f);

	// 더 이상 분할이 불가능한가? 그렇다면 SubDivide() 종료
	if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
	{
		return false;
	}

	// 4개의 자식노드 추가
	m_pChild[CORNER_TL] = _AddChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
	m_pChild[CORNER_TR] = _AddChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
	m_pChild[CORNER_BL] = _AddChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
	m_pChild[CORNER_BR] = _AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

	return true;
}

int QuadTree::_GenTriIndex(int nTriangles, LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio)
{
	// 컬링된 노드라면 그냥 리턴
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
	if (_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition(), fLODRatio))	/// 현재 노드가 출력 가능한 노드인가?
	{
		if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
		{		// 좌측 상단 삼각형
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			++nTriangles;

			// 우측 하단 삼각형
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			++nTriangles;
			return nTriangles;
		}

		bool b[4] = { 0, };

		// 상단 이웃노드(neightbor node)가 출력가능한가?   
		if (m_pNeighbor[EDGE_UP]) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);
		// 하단 이웃노드(neightbor node)가 출력가능한가?   
		if (m_pNeighbor[EDGE_DN]) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);
		// 좌측 이웃노드(neightbor node)가 출력가능한가?   
		if (m_pNeighbor[EDGE_LT]) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);
		// 우측 이웃노드(neightbor node)가 출력가능한가?   
		if (m_pNeighbor[EDGE_RT]) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->_IsVisible(pHeightMapVertex, GameManager::GetCamera( )->GetEyePosition( ), fLODRatio);

		// 이웃노드들이 모두다 출력가능하다면 현재노드와 이웃노드들이 같은 LOD이므로 부분분할이 필요없다.   
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
		if (!b[EDGE_UP]) // 상단 부분분할이 필요한가?   
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TR]; nTriangles++;
		}
		else    // 상단 부분분할이 필요없을 경우   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = m_nCorner[CORNER_TR]; nTriangles++;
		}

		if (!b[EDGE_DN]) // 하단 부분분할이 필요한가?   
		{
			n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BL]; nTriangles++;
		}
		else    // 하단 부분분할이 필요없을 경우   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = m_nCorner[CORNER_BL]; nTriangles++;
		}

		if (!b[EDGE_LT]) // 좌측 부분분할이 필요한가?   
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TL]; nTriangles++;
		}
		else    // 좌측 부분분할이 필요없을 경우   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = m_nCorner[CORNER_TL]; nTriangles++;
		}

		if (!b[EDGE_RT]) // 우측 부분분할이 필요한가?   
		{
			n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) * 0.5f;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = n; nTriangles++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BR]; nTriangles++;
		}
		else    // 우측 부분분할이 필요없을 경우   
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = m_nCorner[CORNER_BR]; nTriangles++;
		}

		return nTriangles;   // 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!   
	}
	
	// 자식 노드들 검색
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

	// 경계구 안에 있는가?
	bInSphere = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCenter), m_fRadius);
	if (!bInSphere) return FRUSTUM_OUT;		// 경계구 안에 없으면 점단위의 절두체 검사 생략

	//쿼드트리의 4군데 경계 절두체 테스트, 점 단위의 절두체 검사
	b[0] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[0]));
	b[1] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[1]));
	b[2] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[2]));
	b[3] = GameManager::GetCamera( )->IsInCameraFrustum((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[3]));

	// 4개 모두 절두체 안에 있음
	if ((b[0] + b[1] + b[2] + b[3]) == CORNER_NUM) return FRUSTUM_COMPLETELY_IN;

	// 일부분이 절두체에 있는 경우
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::_FrustumCull(D3DXVECTOR3 * pHeightMapVertex)
{
	int ret;
	ret = _IsInFrustum(pHeightMapVertex);
	switch (ret)
	{
		case FRUSTUM_COMPLETELY_IN:		// 절두체에 완전히 포함됨, 하위노드 검색 필요 없음.
			_AllInFrustum( );
			return;
		case FRUSTUM_PARTIALLY_IN:		// 절두체에 일부 포함됨, 하위노드 검색 필요 함.
			m_bCulled = FALSE;
			break;
		case FRUSTUM_OUT:				// 절두체에서 완전히 벗어남, 하위노드 검색 필요 없음.
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
		// 이웃노드의 4개 코너값을 얻는다.
		n = _GetNodeIndex(i, cx, _0, _1, _2, _3);
		// 코너값으로 이웃노드의 포인터를 얻어온다.
		if (n >= 0) m_pNeighbor[i] = pRoot->_FindNode(pHeightMapVertex, _0, _1, _2, _3);
	}

	// 자식노드로 재귀호출
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
		// 좌측 상단과 우측 하단의 거리를 구한다.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[CORNER_TL])) - *((D3DXVECTOR3*)(pHeightMapVertex + m_nCorner[CORNER_BR]));

		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 2로 나누어 반지름을 구한다.
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

	// 일치하는 노드라면 노드값을 리턴
	if (((m_nCorner[CORNER_TL] == _0) && (m_nCorner[CORNER_TR] == _1)) && ((m_nCorner[CORNER_BL] == _2) && (m_nCorner[CORNER_BR] == _3)))
		return this;

	// 자식 노드가 있가?
	if (m_pChild[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) * 0.25;

		// 현재 맵상에서의 위치
		pt.x = (int)pHeightMapVertex[n].x;
		pt.y = (int)pHeightMapVertex[n].z;

		// 4개의 코너값을 기준으로 자식노드의 맵 점유범위를 얻는다.
		SetRect(&rc, 
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMapVertex[m_pChild[CORNER_TL]->m_nCorner[CORNER_BR]].z);
		// pt값이 점유범위안에 있다면 자식노드로 들어간다.
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
	gap = _b - _a;	// 현재 노드의 좌우폭값

	switch (ed)
	{
		case EDGE_UP:	// 위쪽 방향 이웃노드의 인덱스
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// 아래 방향 이웃노드의 인덱스
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// 좌측 방향 이웃노드의 인덱스
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// 우측 방향 이웃노드의 인덱스
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if ((_0 / cx) != (_a / cx)) return -1;
			if ((_1 / cx) != (_b / cx)) return -1;
			break;
	}

	n = (_0 + _1 + _2 + _3) * 0.25;	// 가운데 인덱스
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
	// 쿼드트리 구축   
	_BuildQuadTree(pHeightMapVertex);
	// 이웃노드 구축   
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
// pt가 rc안에 포함되는지 검사한다.(PtInRect()라는 API함수는 음수(-)값 처리를 못했다.)
BOOL QuadTree::IsInRect(RECT* rc, POINT pt)
{
	if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
		(rc->bottom <= pt.y) && (pt.y <= rc->top))
		return TRUE;

	return FALSE;
}