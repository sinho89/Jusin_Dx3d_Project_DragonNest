/*!
 * \file Weapon.h
 * \date 2016/10/03 15:32
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

#ifndef Weapon_h__
#define Weapon_h__


#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CWeapon
	: public Engine::CGameObject
{
private:
	CWeapon(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWeapon(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT SetConstantTable(void);
private:
	HRESULT AddComponent(void);

public:
	static CWeapon* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

	const D3DXMATRIX*		m_pmatPlayerInfo;
	const D3DXMATRIX*		m_pmatWeaponRef;

	float				m_fDetail;

};

#endif // Weapon_h__