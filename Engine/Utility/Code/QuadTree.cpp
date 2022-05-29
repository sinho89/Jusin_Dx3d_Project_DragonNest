#include "QuadTree.h"

#include "Export_Function.h"
#include "Engine_Include.h"

IMPLEMENT_SINGLETON(Engine::CQuadTree)

Engine::CQuadTree::CQuadTree(void)
: m_iCenter(0)
, m_fRadius(0.f)
, m_bIsIn(false)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_wCorner, sizeof(WORD) * CORNER_END);
}

Engine::CQuadTree::~CQuadTree(void)
{
	Release();
}

HRESULT Engine::CQuadTree::InitQuadTree(const VTXTEX* pTerrainVtx, 
										const WORD& wCntX, 
										const WORD& wCntZ)
{
	m_wCorner[CORNER_LT] = 0;
	m_wCorner[CORNER_RT] = wCntX - 1;
	m_wCorner[CORNER_LB] = wCntX * (wCntZ - 1);
	m_wCorner[CORNER_RB] = wCntX * wCntZ - 1;

	for(int i = 0; i < CORNER_END; ++i)
		m_iCenter += m_wCorner[i];

	m_iCenter = m_iCenter >> 2;
	
	SetChild(pTerrainVtx);

	return S_OK;
}

HRESULT Engine::CQuadTree::InitNeighbor(const VTXTEX* pTerrainVtx, CQuadTree* pParentQuad , const WORD& wCntX, const WORD& wCntZ)
{
	int			iLT, iRT, iLB, iRB;

	for(int i = 0; i < NEIGHBOR_END; ++i)
	{
		if(MakeNeighbor(NEIGHBOR(i), iLT, iRT, iLB, iRB, wCntX, wCntZ) == true)
		{
			int		iCenter = (iLT + iRT + iLB + iRB) >> 2;
			m_pNeighbor[i] = pParentQuad->FindNeighbor(pTerrainVtx, iLT, iRT, iLB, iRB, iCenter);
		}
		else
			m_pNeighbor[i] = NULL;
	}

	for(int i = 0; i < CHILD_END; ++i)
	{
		if(m_pChild[i] != NULL)
			m_pChild[i]->InitNeighbor(pTerrainVtx, pParentQuad, wCntX, wCntZ);
	}
	return S_OK;
}

void Engine::CQuadTree::CullingToQuadTree(const VTXTEX* pTerrainVtx, 
										  INDEX32* pIndex, 
										  DWORD* pTriCnt)
{
	if(m_bIsIn == true)
	{
		if(LevelofDetail(pTerrainVtx))
		{
			bool bIsIn[4] = { false };

			bIsIn[0] = Engine::Get_Frustum()->VertexInFrustum(&pTerrainVtx[m_wCorner[CORNER_LB]].vPos);
			bIsIn[1] = Engine::Get_Frustum()->VertexInFrustum(&pTerrainVtx[m_wCorner[CORNER_RB]].vPos);
			bIsIn[2] = Engine::Get_Frustum()->VertexInFrustum(&pTerrainVtx[m_wCorner[CORNER_RT]].vPos);
			bIsIn[3] = Engine::Get_Frustum()->VertexInFrustum(&pTerrainVtx[m_wCorner[CORNER_LT]].vPos);

			// 오른쪽 위 폴리곤에 대한 인덱스 재배치
			if(bIsIn[0] || bIsIn[1] || bIsIn[2])
			{
				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RB];
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RT];

				++(*pTriCnt);
			}
			// 왼쪽 아래 폴리곤에 대한 인덱스 재배치
			if(bIsIn[0] || bIsIn[2] || bIsIn[3])
			{
				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RT];
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_LT];

				++(*pTriCnt);
			}
			return ;
		}

		for(int i = 0; i < CHILD_END; ++i)
		{
			if(m_pChild[i] != NULL)
				m_pChild[i]->CullingToQuadTree(pTerrainVtx, pIndex, pTriCnt);
		}
	}
}

void Engine::CQuadTree::IsinFrustum(const VTXTEX* pTerrainVtx)
{
	if(m_pChild[CHILD_LT] == NULL)
	{
		m_bIsIn = true;
		return;
	}

	m_bIsIn = Engine::Get_Frustum()->SphereInFrustum(&pTerrainVtx[m_iCenter].vPos, m_fRadius);

	if(m_bIsIn == true)
	{
		for(int i = 0; i < CHILD_END; ++i)
		{
			if(m_pChild[i] != NULL)
				m_pChild[i]->IsinFrustum(pTerrainVtx);
		}
	}
}

void Engine::CQuadTree::SetChild(const VTXTEX* pTerrainVtx)
{
	m_fRadius = D3DXVec3Length(&(pTerrainVtx[CORNER_LT].vPos - pTerrainVtx[m_iCenter].vPos));

	for(int i = 0; i < CHILD_END; ++i)
	{
		m_pChild[i] = MakeChild(CHILD(i));

		if(m_pChild[i] != NULL)
			m_pChild[i]->SetChild(pTerrainVtx);
	}
}

Engine::CQuadTree* Engine::CQuadTree::MakeChild(CHILD eChild)
{
	if(m_wCorner[CORNER_RT] - m_wCorner[CORNER_LT] == 1)
	{
		m_fRadius = 0.f;
		return NULL;
	}

	int		iLC, iTC, iRC, iBC;

	iLC = (m_wCorner[CORNER_LB] + m_wCorner[CORNER_LT]) >> 1;
	iTC = (m_wCorner[CORNER_RT] + m_wCorner[CORNER_LT]) >> 1;
	iRC = (m_wCorner[CORNER_RB] + m_wCorner[CORNER_RT]) >> 1;
	iBC = (m_wCorner[CORNER_LB] + m_wCorner[CORNER_RB]) >> 1;

	CQuadTree*	pQuadTree = new CQuadTree;

	switch(eChild)
	{
	case CHILD_LT:
		pQuadTree->m_wCorner[CORNER_LT] = m_wCorner[CORNER_LT];
		pQuadTree->m_wCorner[CORNER_RT] = iTC;
		pQuadTree->m_wCorner[CORNER_LB] = iLC;
		pQuadTree->m_wCorner[CORNER_RB] = m_iCenter;

		break;

	case CHILD_RT:
		pQuadTree->m_wCorner[CORNER_LT] = iTC;
		pQuadTree->m_wCorner[CORNER_RT] = m_wCorner[CORNER_RT];
		pQuadTree->m_wCorner[CORNER_LB] = m_iCenter;
		pQuadTree->m_wCorner[CORNER_RB] = iRC;
		break;

	case CHILD_LB:
		pQuadTree->m_wCorner[CORNER_LT] = iLC;
		pQuadTree->m_wCorner[CORNER_RT] = m_iCenter;
		pQuadTree->m_wCorner[CORNER_LB] = m_wCorner[CORNER_LB];
		pQuadTree->m_wCorner[CORNER_RB] = iBC;
		break;

	case CHILD_RB:
		pQuadTree->m_wCorner[CORNER_LT] = m_iCenter;
		pQuadTree->m_wCorner[CORNER_RT] = iRC;
		pQuadTree->m_wCorner[CORNER_LB] = iBC;
		pQuadTree->m_wCorner[CORNER_RB] = m_wCorner[CORNER_RB];
		break;
	}

	for(int i = 0; i < CORNER_END; ++i)
		pQuadTree->m_iCenter += pQuadTree->m_wCorner[i];

	pQuadTree->m_iCenter = pQuadTree->m_iCenter >> 2;

	return pQuadTree;
}

bool Engine::CQuadTree::MakeNeighbor(NEIGHBOR eNeighborID , int& iLT, int& iRT, int& iLB, int& iRB , const WORD& wCntX, const WORD& wCntZ)
{
	int		iSizeX = m_wCorner[CORNER_RT] - m_wCorner[CORNER_LT];
	int		iSizeY = m_wCorner[CORNER_LB] - m_wCorner[CORNER_LT];

	switch(eNeighborID)
	{
	case NEIGHBOR_LEFT:
		iLT = m_wCorner[CORNER_LT] - iSizeX;
		iRT = m_wCorner[CORNER_LT];
		iLB = m_wCorner[CORNER_LB] - iSizeX;
		iRB = m_wCorner[CORNER_LB];

		if(m_wCorner[CORNER_LT] % wCntX == 0)
			return false;
		break;

	case NEIGHBOR_TOP:
		iLT = m_wCorner[CORNER_LT] - iSizeY;
		iRT = m_wCorner[CORNER_RT] - iSizeY;
		iLB = m_wCorner[CORNER_LT];
		iRB = m_wCorner[CORNER_RT];

		if(m_wCorner[CORNER_RT] < wCntX)
			return false;
		break;

	case NEIGHBOR_RIGHT:
		iLT = m_wCorner[CORNER_RT];
		iRT = m_wCorner[CORNER_RT] + iSizeX;
		iLB = m_wCorner[CORNER_RB];
		iRB = m_wCorner[CORNER_RB] + iSizeX;

		if(m_wCorner[CORNER_RT] % wCntX == wCntX - 1)
			return false;
		break;

	case NEIGHBOR_BOTTOM:
		iLT = m_wCorner[CORNER_LB];
		iRT = m_wCorner[CORNER_RB];
		iLB = m_wCorner[CORNER_LB] + iSizeY;
		iRB = m_wCorner[CORNER_RB] + iSizeY;

		if(m_wCorner[CORNER_LB] >= wCntX * (wCntZ - 1))
			return false;
		break;
	}

	return true;
}

void Engine::CQuadTree::Release(void)
{
	for(int i = 0; i < CHILD_END; ++i)
		Engine::Safe_Delete(m_pChild[i]);
}

bool Engine::CQuadTree::LevelofDetail(const VTXTEX* pTerrainVtx)
{
	D3DXMATRIX		matView;
	Engine::Get_GraphicDev()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos = D3DXVECTOR3(matView._41, matView._42, matView._43);

	float		fDistance = D3DXVec3Length(&(vCamPos - pTerrainVtx[m_iCenter].vPos));
	float		fWidth = pTerrainVtx[m_wCorner[CORNER_RT]].vPos.x -  pTerrainVtx[m_wCorner[CORNER_LT]].vPos.x;

	if(fDistance * 0.1f > fWidth)
	{
		return true;
	}
	return false;
}

Engine::CQuadTree* Engine::CQuadTree::FindNeighbor(const VTXTEX* pTerrainVtx , const int& iLT, const int& iRT, const int& iLB, const int& iRB , const int& iCenter)
{
	if(m_wCorner[CORNER_LT] == iLT
		&& m_wCorner[CORNER_RT] == iRT
		&& m_wCorner[CORNER_LB] == iLB
		&& m_wCorner[CORNER_RB] == iRB)
		return this;

	RECT		rcQuad;
	POINT		ptCenter;

	for(int i = 0; i < CHILD_END; ++i)
	{
		if(m_pChild[i] != NULL)
		{
			ZeroMemory(&rcQuad, sizeof(RECT));
			ZeroMemory(&ptCenter, sizeof(POINT));

			SetRect(&rcQuad, (long)pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_LT]].vPos.x
				, (long)pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_LT]].vPos.z
				, (long)pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_RB]].vPos.x
				, (long)pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_RB]].vPos.z);

			ptCenter.x = (long)pTerrainVtx[iCenter].vPos.x;
			ptCenter.y = (long)pTerrainVtx[iCenter].vPos.z;

			if(PtInRect(&rcQuad, ptCenter) == TRUE)
			{
				return m_pChild[i]->FindNeighbor(pTerrainVtx, iLT, iRT, iLB, iRB, iCenter);
			}
		}	
	}

	return NULL;
}

void Engine::CQuadTree::Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt)
{
	if(m_bIsIn == true)
	{
		if(LevelofDetail(pTerrainVtx))
		{
			bool	isRender[4] = {true, true, true, true};

			for(int i = 0; i < NEIGHBOR_END; ++i)
			{
				if(m_pNeighbor[i] != NULL)
				{
					isRender[i] = m_pNeighbor[i]->LevelofDetail(pTerrainVtx);
				}
			}

			if(isRender[NEIGHBOR_LEFT] && isRender[NEIGHBOR_TOP]
			&& isRender[NEIGHBOR_RIGHT] && isRender[NEIGHBOR_BOTTOM])
			{
				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RB];
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RT];
				++(*pTriCnt);

				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RT];
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_LT];
				++(*pTriCnt);

				return;
			}

			if(isRender[NEIGHBOR_LEFT] == false)
			{
				int		iLC = (m_wCorner[CORNER_LT] + m_wCorner[CORNER_LB]) >> 1;

				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = iLC;
				++(*pTriCnt);

				pIndex[*pTriCnt]._1 = iLC;
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_LT];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_LT];
				++(*pTriCnt);
			}

			if(isRender[NEIGHBOR_TOP] == false)
			{
				int		iTC = (m_wCorner[CORNER_LT] + m_wCorner[CORNER_RT]) >> 1;

				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LT];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = iTC;
				++(*pTriCnt);

				pIndex[*pTriCnt]._1 = iTC;
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RT];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LT];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RT];
				++(*pTriCnt);
			}

			if(isRender[NEIGHBOR_RIGHT] == false)
			{
				int		iRC = (m_wCorner[CORNER_RT] + m_wCorner[CORNER_RB]) >> 1;

				pIndex[*pTriCnt]._1 = m_iCenter;
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RB];
				pIndex[*pTriCnt]._3 = iRC;
				++(*pTriCnt);

				pIndex[*pTriCnt]._1 = iRC;
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RT];
				pIndex[*pTriCnt]._3 = m_iCenter;
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._1 = m_iCenter;
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_RB];
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RT];
				++(*pTriCnt);
			}

			if(isRender[NEIGHBOR_BOTTOM] == false)
			{
				int		iBC = (m_wCorner[CORNER_RB] + m_wCorner[CORNER_LB]) >> 1;

				pIndex[*pTriCnt]._1 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = iBC;
				pIndex[*pTriCnt]._3 = m_iCenter;
				++(*pTriCnt);

				pIndex[*pTriCnt]._1 = m_iCenter;
				pIndex[*pTriCnt]._2 = iBC;
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RB];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._1 = m_iCenter;
				pIndex[*pTriCnt]._2 = m_wCorner[CORNER_LB];
				pIndex[*pTriCnt]._3 = m_wCorner[CORNER_RB];
				++(*pTriCnt);
			}
			return;
		}

		for(int i = 0; i < CHILD_END; ++i)
		{
			if(m_pChild[i] != NULL)
				m_pChild[i]->Culling_LOD(pTerrainVtx, pIndex, pTriCnt);
		}
	}
}
