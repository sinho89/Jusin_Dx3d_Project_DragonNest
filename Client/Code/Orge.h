/*!
 * \file Orge.h
 * \date 2016/10/06 15:10
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

#ifndef Orge_h__
#define Orge_h__


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
class COrge
	: public Engine::CGameObject
{
private:
	COrge(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~COrge(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

public:
	static COrge* Create(LPDIRECT3DDEVICE9 pDevice);
	static COrge* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, float _fAngle);
	void AnimationUpdate(void);
	void AttackSightColl(void);
	void SetAniTime(void);
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);

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
	ORGE_STATE_TYPE	m_eStateType;
	bool			m_bState[ORGE_STATE_END];
	float			m_fAniTime;
};


#endif // Orge_h__