/*!
 * \file SwordTrail.h
 * \date 2016/10/26 20:47
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

#ifndef SwordTrail_h__
#define SwordTrail_h__


#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CSwordTrail
	: public Engine::CGameObject
{
private:
	CSwordTrail(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSwordTrail(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CSwordTrail* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void Calculate_WeaponPos();
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

private:
	float			m_fAlpha;
	D3DXVECTOR3 	m_vOriPoint;
	D3DXVECTOR3 	m_vPoint;
};


#endif // SwordTrail_h__