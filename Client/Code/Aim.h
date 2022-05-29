/*!
 * \file Aim.h
 * \date 2016/11/03 6:37
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

#ifndef Aim_h__
#define Aim_h__


#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CAim
	: public Engine::CGameObject
{
private:
	CAim(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAim(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CAim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Aim_h__