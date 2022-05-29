/*!
 * \file Frustum.h
 * \date 2016/09/06 14:12
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

#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum
{
DECLARE_SINGLETON(CFrustum)

private:
	CFrustum(void);
	~CFrustum(void);

public:
	HRESULT	InitFrustum(LPDIRECT3DDEVICE9	pDevice);
	void	Transform_Frustum_ToWorld(void);
	void	Transform_Frustum_Local(const D3DXMATRIX* pWorld);
	void CullingForTerrain(INDEX32*	pIndex, 
		DWORD* pTriCnt, 
		const VTXTEX* pTerrainVtx, 
		const WORD& wCntX, 
		const WORD& wCntZ);

	bool	VertexInFrustum(const D3DXVECTOR3*		pPoint);
	bool	SphereInFrustum(const D3DXVECTOR3*		pCenter, float& fRadius);

private:
	void	MakePlane(void);
	
	


private:
	LPDIRECT3DDEVICE9				m_pDevice;
private:
	D3DXVECTOR3						m_vPoint[8];
	D3DXVECTOR3						m_vConPoint[8];

	D3DXPLANE						m_Plane[6];
};

END
#endif // Frustum_h__