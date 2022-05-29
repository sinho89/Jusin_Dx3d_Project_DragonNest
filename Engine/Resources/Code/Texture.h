/*!
 * \file Texture.h
 * \date 2015/04/06 21:48
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
#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture
	: public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& rhs);

public:
	virtual ~CTexture(void);

public:
	virtual CResources* CloneResource(void);

public:
	void Set_ConstantTable(LPD3DXEFFECT pEffect, const char* pConstantName, const DWORD& dwCnt = 0);

public:
	HRESULT LoadTexture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	void Render(const DWORD& dwStage, const DWORD& iIndex);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice
		, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);

public:
	virtual DWORD Release(void);

private:
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;
	VECTEXTURE		m_vecTexture;

private:
	DWORD		m_dwContainerSize;
};

END

#endif // Texture_h__