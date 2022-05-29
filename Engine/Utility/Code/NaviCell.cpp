#include "NaviCell.h"
#include "Line2D.h"

Engine::CNaviCell::CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
: m_pDevice(NULL)
, m_pLine(NULL)
, m_dwIndex(0)
{
	ZeroMemory(m_pLine2D, sizeof(CLine2D*) * LINE_END);
	ZeroMemory(m_pNeighbor, sizeof(CNaviCell*) * NEIGHBOR_END);
	
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;
}

Engine::CNaviCell::~CNaviCell(void)
{
	Release();
}

HRESULT Engine::CNaviCell::Init_Cell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx)
{
	m_pDevice = pDevice;
	m_dwIndex = dwIdx;

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);

	HRESULT hr = D3DXCreateLine(m_pDevice, &m_pLine);
	FAILED_CHECK(hr);
	return S_OK;
}

void Engine::CNaviCell::Render(void)
{
	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3		vPoint[4];
	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];

	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if(vPoint[i].z < 0.f)
			vPoint[i].z = 0.0f;
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	m_pLine->SetWidth(2.f);

	m_pLine->Begin();
	m_pLine->DrawTransform(vPoint, POINT_END + 1
		, &matIdentity
		, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_pLine->End();
}

Engine::CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pDevice , const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC , const DWORD& dwIdx)
{
	CNaviCell*		pCell = new CNaviCell(pPointA, pPointB, pPointC);
	if(FAILED(pCell->Init_Cell(pDevice, dwIdx)))
		Safe_Delete(pCell);

	return pCell;
}

void Engine::CNaviCell::Release(void)
{
	for(int i = 0; i < LINE_END; ++i)
		Safe_Delete(m_pLine2D[i]);

	Safe_Release(m_pLine);
}

bool Engine::CNaviCell::Compare_Point(const D3DXVECTOR3* pFirstPoint, const D3DXVECTOR3* pSecondPoint, CNaviCell* pNeighbor)
{
	if(*pFirstPoint == m_vPoint[POINT_A])
	{
		if(*pSecondPoint == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pSecondPoint == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
	}

	if(*pFirstPoint == m_vPoint[POINT_B])
	{
		if(*pSecondPoint == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pSecondPoint == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	if(*pFirstPoint == m_vPoint[POINT_C])
	{
		if(*pSecondPoint == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else if(*pSecondPoint == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}
	return false;
}

bool Engine::CNaviCell::Check_Pass(const D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, NEIGHBOR* pNeighbor)
{
	for(int i = 0; i < 3; ++i)
	{
		if(m_pLine2D[i]->Check_LinePass( &(D3DXVECTOR2(pPos->x + pDir->x, pPos->z + pDir->z) )))
		{
			*pNeighbor = NEIGHBOR(i);
			return true;
		}
	}
	return false;
}

DWORD Engine::CNaviCell::Get_Index(void)
{
	return m_dwIndex;
}
