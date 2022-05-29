/*!
 * \file CresentDust.h
 * \date 2016/10/31 3:30
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

#ifndef CresentDust_h__
#define CresentDust_h__




#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CCresentDust
	: public Engine::CGameObject
{
private:
	CCresentDust(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCresentDust(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CCresentDust* Create(LPDIRECT3DDEVICE9 pDevice);
	static CCresentDust* Create(LPDIRECT3DDEVICE9 pDevice
		, D3DXVECTOR3 _vPos
		, float	_fAngle);

public:
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void SetType(OBTAINTYPE	_eType);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

public:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

private:
	int					m_iCollision;
	float				m_fDetail;
	OBTAINTYPE			m_eType;
	const D3DXMATRIX*		m_pmatPlayerInfo;
	float				m_fScale;
	float				m_fAlpha;
	bool				m_bScale;
};

#endif // CresentDust_h__