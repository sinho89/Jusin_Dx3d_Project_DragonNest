/*!
 * \file TerrainTex.h
 * \date 2016/07/20 10:41
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

#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{

private:
	virtual HRESULT	CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);
	DWORD*	LoadImage(void);

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pDevice, 
								const WORD& wCntX, 
								const WORD& wCntZ, 
								const WORD& wItv);
public:
	virtual CResources* CloneResource(void);

private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9	pDevice);
public:
	~CTerrainTex();

};

END
#endif // TerrainTex_h__