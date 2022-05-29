/*!
 * \file MainBar.cpp
 * \date 2016/10/14 16:11
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

#ifndef MainBar_h__
#define MainBar_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CMainBar
	: public Engine::CGameObject
{
private:
	CMainBar(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMainBar(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CMainBar* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
};

#endif // MainBar_h__