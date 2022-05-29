/*!
 * \file NaviCell.h
 * \date 2016/09/13 14:08
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef NaviCell_h__
#define NaviCell_h__

#include "Engine_Include.h"

BEGIN(Engine)

enum POINT {POINT_A, POINT_B, POINT_C, POINT_END};
enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END};
enum NEIGHBOR {NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};

class CLine2D;
class ENGINE_DLL CNaviCell
{
private:
	CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);

public:
	~CNaviCell(void);

public:
	const D3DXVECTOR3* Get_Point(POINT ePointID) {return &m_vPoint[ePointID];}
	CNaviCell* Get_Neighbor(NEIGHBOR eNeighborID) {return m_pNeighbor[eNeighborID];}
	DWORD Get_Index(void);

public:
	void Set_Neighbor(NEIGHBOR eNeighborID, CNaviCell* pNeighbor) {m_pNeighbor[eNeighborID] = pNeighbor;}

public:
	HRESULT Init_Cell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx);
	bool Compare_Point(const D3DXVECTOR3* pFirstPoint, const D3DXVECTOR3* pSecondPoint, CNaviCell* pNeighbor);
	bool Check_Pass(const D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, NEIGHBOR* pNeighbor);
	void Render(void);

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC
		, const DWORD& dwIdx);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CLine2D*				m_pLine2D[LINE_END];
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];
	LPD3DXLINE				m_pLine;

private:
	D3DXVECTOR3				m_vPoint[POINT_END];
	DWORD					m_dwIndex;
};

END

#endif // NaviCell_h__