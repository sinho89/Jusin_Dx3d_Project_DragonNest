#include "NaviMgr.h"
#include "NaviCell.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr(void)
: m_pDevice(NULL)
, m_dwReserveSize(0)
, m_dwIdxCnt(0)
{
	
}

Engine::CNaviMgr::~CNaviMgr(void)
{
	Release();
}

void Engine::CNaviMgr::Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize)
{
	m_pDevice = pDevice;
	m_dwReserveSize = dwSize;
	m_vecNaviCell.reserve(dwSize);
}

HRESULT Engine::CNaviMgr::Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
{
	CNaviCell*		pCell = CNaviCell::Create(m_pDevice, pPointA, pPointB, pPointC, m_dwIdxCnt);
	NULL_CHECK_RETURN(pCell, E_FAIL);

	++m_dwIdxCnt;

	m_vecNaviCell.push_back(pCell);
	return S_OK;
}

void Engine::CNaviMgr::Render_NaviMesh(void)
{
	UINT		iSize = m_vecNaviCell.size();
	for(UINT i = 0; i < iSize; ++i)
		m_vecNaviCell[i]->Render();
}

void Engine::CNaviMgr::Release(void)
{
	for_each(m_vecNaviCell.begin(), m_vecNaviCell.end(), CDeleteObj());
	m_vecNaviCell.clear();
}

void Engine::CNaviMgr::Link_Cell(void)
{
	VECCELL::iterator	iter =  m_vecNaviCell.begin();

	if(m_vecNaviCell.end() == iter)
		return ;

	for( ;iter != m_vecNaviCell.end() ; ++iter)
	{
		VECCELL::iterator iter_Target = m_vecNaviCell.begin();

		while(iter_Target != m_vecNaviCell.end())
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if( (*iter_Target)->Compare_Point((*iter)->Get_Point(POINT_A)
				, (*iter)->Get_Point(POINT_B)
				, (*iter)) )
			{
				(*iter)->Set_Neighbor(NEIGHBOR_AB, (*iter_Target));
			}
			else if( (*iter_Target)->Compare_Point((*iter)->Get_Point(POINT_B)
				, (*iter)->Get_Point(POINT_C)
				, (*iter)) )
			{
				(*iter)->Set_Neighbor(NEIGHBOR_BC, (*iter_Target));
			}
			else if( (*iter_Target)->Compare_Point((*iter)->Get_Point(POINT_C)
				, (*iter)->Get_Point(POINT_A)
				, (*iter)) )
			{
				(*iter)->Set_Neighbor(NEIGHBOR_CA, (*iter_Target));
			}

			++iter_Target;
		}
	}
}

DWORD Engine::CNaviMgr::MoveOnNaviMesh(D3DXVECTOR3* pPos , const D3DXVECTOR3* pDir , const DWORD& dwCurrentIdx)
{
	NEIGHBOR		eNeighborID;
	DWORD			dwNextIdx = dwCurrentIdx;

	if(m_vecNaviCell[dwCurrentIdx]->Check_Pass(pPos, pDir, &eNeighborID))
	{
		CNaviCell*		pNeighbor = m_vecNaviCell[dwCurrentIdx]->Get_Neighbor(eNeighborID);
		if(pNeighbor == NULL)
		{
			//슬라이딩 벡터
			int		a = 10;
		}
		else
		{
			dwNextIdx = pNeighbor->Get_Index();
			*pPos += *pDir;
		}
	}
	else
	{
		*pPos += *pDir;	
	}

	return dwNextIdx;
}

