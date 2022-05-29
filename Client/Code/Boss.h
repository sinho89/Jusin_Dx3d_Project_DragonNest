/*!
 * \file Boss.h
 * \date 2016/10/21 14:52
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


#ifndef Boss_h__
#define Boss_h__


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
class CBoss
	: public Engine::CGameObject
{
private:
	CBoss(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBoss(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pDevice);
	static CBoss* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, float _fAngle);
	void AnimationUpdate(void);
	void AttackSightColl(void);
	void SetAniTime(void);
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void LookTarget();
	void Turn();
	void RageSkill();

private:
	HRESULT SetConstantTable(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CCollisionMgr*			m_pCollisionMgr;

private:
	DWORD			m_iAniIdx;
	DWORD			m_dwIndex;
	bool			m_bPush;
	float			m_fDetail;
	BOSS_STATE_TYPE	m_eStateType;
	bool			m_bState[BOSS_STATE_END];
	float			m_fAniTime;
	D3DXVECTOR3		m_vTargetDir;
	float			m_fRageTime;
	float			m_fFlyTime;
	float			m_fBreathTime;
};

#endif // Boss_h__