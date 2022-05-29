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
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CTexture;
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
	void AnimationUpdate(void);
	void SetAniTime(void);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT SetConstantTable(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTexture*		m_pTexture[7];

private:
	const D3DXMATRIX*	m_mapCenterBone;

private:
	DWORD		m_iAniIdx;
	DWORD		m_iOriAniIdx;
	DWORD		m_dwIndex;
	bool		m_bPush;
	float		m_fDetail;
	STATE_TYPE	m_eStateType;
	bool		m_bState[STATE_END];
	bool		m_bAttackCurver;
	bool		m_bDirMoveCurver[4];
	float		m_fAniTime;
	bool		m_bGetUp;
	bool		m_bSkillHarf;
	bool		m_bSkillCresent;
	bool		m_bSkillGreat;
	float		m_fAniSpeed;
};


#endif // Player_h__