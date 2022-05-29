/*!
 * \file Door.h
 * \date 2016/10/17 19:21
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

#ifndef Door_h__
#define Door_h__


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
class CDoor
	: public Engine::CGameObject
{
private:
	CDoor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDoor(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pDevice);
	static CDoor* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, bool _Where);
	void AnimationUpdate(void);
	void SetAniTime(void);
	void SetInfo(D3DXVECTOR3 _vPos, bool _Where);
	void OpenReady();
	void OpenDownGate();

private:
	HRESULT SetConstantTable(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CCollisionMgr*			m_pCollisionMgr;
private:
	Engine::CTransform*		m_pInfo;
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CShader*		m_pShader;
private:
	DWORD				m_iAniIdx;
	DWORD				m_dwIndex;
	bool				m_bPush;
	float				m_fDetail;
	DOOR_STATE_TYPE		m_eStateType;
	bool				m_bState[DOOR_END];
	float				m_fAniTime;
	bool				m_bOpenReady;
	bool				m_bCloseReady;
};




#endif // Door_h__