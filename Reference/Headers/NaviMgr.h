/*!
 * \file NaviMgr.h
 * \date 2016/09/13 14:28
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

#ifndef NaviMgr_h__
#define NaviMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CNaviCell;
class ENGINE_DLL CNaviMgr
{
public:
	DECLARE_SINGLETON(CNaviMgr)

private:
	CNaviMgr(void);
	~CNaviMgr(void);

public:
	void Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize);
	HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);
	void Link_Cell(void);
	DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos
		, const D3DXVECTOR3* pDir
		, const DWORD& dwCurrentIdx);
	void Render_NaviMesh(void);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	DWORD					m_dwReserveSize;

private:
	typedef vector<CNaviCell*>		VECCELL;
	VECCELL			m_vecNaviCell;

private:
	DWORD		m_dwIdxCnt;
};

END

#endif // NaviMgr_h__