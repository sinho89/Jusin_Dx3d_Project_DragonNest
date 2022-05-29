/*!
 * \file Stage.h
 * \date 2015/04/03 0:13
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 스테이지
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Engine_Include.h"
#include "Include.h"
namespace Engine
{
	class CResourceMgr;
	class CCameraMgr;
	class CShadowMgr;
	class CMesh;
}
class CShaderEx;
class CFire;
class CCollisionMgr;
class CStage
	: public Engine::CScene
{
public:
	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI};

private:
	CStage(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStage(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);
public:
	void BossStart();
	void CreateHarfMoon();
	void CreateCresent();
	void CreateGreat();
	void CreateBossPart();
	void CreateAttPart();

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Light(void);
	HRESULT Add_NaviMesh(void);

	void Release(void);
	

public:
	void WorkCamera(void);
	

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CCameraMgr*		m_pCameraMgr;
	Engine::CShadowMgr*		m_pShadowMgr;
	Engine::CMesh*			m_pMesh;
private:
	CCollisionMgr*			m_pColiisionMgr;

private:
	bool			m_bClick;
	bool			m_bMouseFix;
	bool			m_bBoss;
	bool			m_bBossBattle;
	bool			m_bMonsterReset;
	bool			m_bBossKill;

};

#endif // Stage_h__