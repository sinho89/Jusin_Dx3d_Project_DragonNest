/*!
 * \file Goblin.h
 * \date 2016/10/16 15:35
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

#ifndef Goblin_h__
#define Goblin_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CDynamicMesh;
	class CShader;
};
class CCollisionMgr;
class CGoblin
	: public Engine::CGameObject
{
private:
	CGoblin(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGoblin(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

public:
	static CGoblin* Create(LPDIRECT3DDEVICE9 pDevice);
	static CGoblin* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, float _fAngle);
	void AnimationUpdate(void);
	void AttackSightColl(void);
	void SetAniTime(void);
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void MoveSightColl(void);

private:
	HRESULT SetConstantTable(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CCollisionMgr*			m_pCollisionMgr;
private:
	DWORD				m_iAniIdx;
	DWORD				m_dwIndex;
	bool				m_bPush;
	float				m_fDetail;
	GOBLIN_STATE_TYPE	m_eStateType;
	bool				m_bState[GOBLIN_STATE_END];
	float				m_fAniTime;
};


#endif // Goblin_h__