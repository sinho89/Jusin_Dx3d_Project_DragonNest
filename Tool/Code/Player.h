/*!
 * \file Player.h
 * \date 2016/09/21 13:25
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Player_h__
#define Player_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CDynamicMesh;
	class CShader;
};

class CPlayer
	: public Engine::CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayer(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	void KeyCheck(void);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT SetConstantTable(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

private:
	DWORD		m_iAniIdx;
	DWORD		m_dwIndex;
	bool		m_bPush;
	float		m_fDetail;
};


#endif // Player_h__