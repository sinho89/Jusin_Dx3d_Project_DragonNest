/*!
 * \file LogoBack.h
 * \date 2016/10/04 15:53
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

#ifndef LogoBack_h__
#define LogoBack_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CLogoBack
	: public Engine::CGameObject
{
private:
	CLogoBack(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogoBack(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT AddComponent(void);
	HRESULT SetConstantTable(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;

/*
private:
	D3DMATERIAL9			m_MtrInfo;*/

private:
	float		m_fAlpha;
};



#endif // LogoBack_h__