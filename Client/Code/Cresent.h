/*!
 * \file Cresent.h
 * \date 2016/10/31 0:45
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

#ifndef Cresent_h__
#define Cresent_h__


#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CCollisionMgr;
class CCresent
	: public Engine::CGameObject
{
private:
	CCresent(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCresent(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CCresent* Create(LPDIRECT3DDEVICE9 pDevice);
	static CCresent* Create(LPDIRECT3DDEVICE9 pDevice
		, D3DXVECTOR3 _vPos
		, float	_fAngle);

public:
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void SetType(OBTAINTYPE	_eType);
	void Collision_OBB(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CCollisionMgr*			m_pCollisionMgr;

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
};


#endif // Cresent_h__