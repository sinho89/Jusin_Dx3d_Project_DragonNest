/*!
 * \file TrailTex.h
 * \date 2016/10/26 23:00
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef TrailTex_h__
#define TrailTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrailTex
	: public CVIBuffer
{
private:
	explicit CTrailTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTrailTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);
	virtual HRESULT CreateBuffer(DWORD dwVertexCnt);
	void VertexUpdate(D3DXVECTOR3 vDestPoint, D3DXVECTOR3 vSourPoint);

public:
	static CTrailTex* Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwCnt);

private:
	D3DXVECTOR3 m_vBeforePoint;
	D3DXVECTOR3 m_vBeforePoint2;
};

END

#endif // TrailTex_h__