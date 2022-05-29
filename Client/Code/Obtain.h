/*!
 * \file Obtain.h
 * \date 2016/10/04 16:16
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

#ifndef Obtain_h__
#define Obtain_h__

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
class CObtain
	: public Engine::CGameObject
{
private:
	CObtain(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CObtain(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CObtain* Create(LPDIRECT3DDEVICE9 pDevice);
	static CObtain* Create(LPDIRECT3DDEVICE9 pDevice
							, D3DXVECTOR3 _vPos
							, float	_fAngle
							, OBTAINTYPE _eType);

public:
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void SetType(OBTAINTYPE	_eType);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CCollisionMgr*			m_pCollisionMgr;

public:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh[12];
	Engine::CShader*		m_pShader;

private:
	int					m_iCollision;
	float				m_fDetail;
	OBTAINTYPE			m_eType;
	const D3DXMATRIX*		m_pmatPlayerInfo;
};

#endif // Obtain_h__