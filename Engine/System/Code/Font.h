/*!
 * \file Font.h
 * \date 2016/09/02 13:28
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

#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CFont
{
private:
	explicit CFont(LPDIRECT3DDEVICE9	pDevice);
public:
	~CFont(void);

public:
	HRESULT	Init_Font(const UINT& iHeight, 
					const UINT& iWidth, 
					const UINT& iWeight, 
					const TCHAR* pFaceName);

	void Render(const TCHAR* pString, 
				D3DXCOLOR Color, 
				const D3DXVECTOR3& vPos);

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice, 
					const UINT& iHeight, 
					const UINT& iWidth, 
					const UINT& iWeight,
					const TCHAR* pFaceName);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9			m_pDevice;
	LPD3DXFONT					m_pFont;
	UINT						m_iHeight;
	UINT						m_iWidth;
};

END
#endif // Font_h__