/*!
 * \file Light.h
 * \date 2016/09/01 14:39
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

#ifndef Light_h__
#define Light_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLight
{
private:
	explicit CLight(LPDIRECT3DDEVICE9	pDevice);
public:
	~CLight(void);

public:
	D3DLIGHT9* GetLightInfo(void);

public:
	HRESULT	Init_Light(const D3DLIGHT9* pLightInfo, 
						   const WORD& wLightIdx);

	static CLight* Create(LPDIRECT3DDEVICE9 pDevice, 
							const D3DLIGHT9* pLightInfo, 
							const WORD& wLightIdx);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DLIGHT9				m_LightInfo;

};

END
#endif // Light_h__