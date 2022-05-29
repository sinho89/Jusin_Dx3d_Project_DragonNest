#include "stdafx.h"
#include "Stage.h"

#include "Include.h"
#include "Export_Function.h"
#include "Layer.h"
#include "DynamicCamera.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "CollisionMgr.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Player.h"
#include "Weapon.h"
#include "Obtain.h"
#include "Orge.h"
#include "MainBar.h"
#include "Goblin.h"
#include "Door.h"
#include "Boss.h"
#include "SwordTrail.h"
#include "ShaderEx.h"
#include "Fire.h"
#include "HarfMoon.h"
#include "HarfCasting.h"
#include "DynamicMesh.h"
#include "Petal.h"
#include "SkillPart.h"
#include "Water.h"
#include "HarfDust.h"
#include "HarfWind.h"
#include "Hurricane.h"
#include "CresentCasting.h"
#include "Cresent.h"
#include "HarfAura.h"
#include "CresentAura.h"
#include "CresentDust.h"
#include "Great.h"
#include "GreatHit.h"
#include "GreatDust.h"
#include "GreatRing.h"
#include "GreatAura.h"
#include "Cycle.h"
#include "Wind.h"
#include "BossBreath.h"
#include "SoundMgr.h"
#include "TrailPart.h"
#include "HarfPart.h"
#include "CresentPart.h"
#include "GreatPart.h"
#include "BossPart.h"
#include "Aim.h"
#include "AttPart.h"
#include "Hit.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pCameraMgr(Engine::Get_CameraMgr())
, m_pShadowMgr(Engine::Get_ShadowMgr())
, m_pColiisionMgr(CCollisionMgr::GetInstance())
, m_bClick(false)
, m_bMouseFix(true) 
, m_bBoss(false)
, m_bBossBattle(true)
, m_bMonsterReset(false)
, m_bBossKill(false)
{
	
}

CStage::~CStage(void)
{
	Release();
}

HRESULT CStage::InitScene(void)
{
	HRESULT		hr = NULL;

	m_pShadowMgr->SetDevice(m_pDevice);
	m_pCameraMgr->SetDevice(m_pDevice);
	m_pCameraMgr->SetViewMatrix();
	m_pCameraMgr->SetProMatrix(&WINCX, &WINCY);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	FAILED_CHECK_RETURN_MSG(Add_NaviMesh(), E_FAIL, L"Add_NaviMesh Add false");

	FAILED_CHECK(hr);

	CSoundMgr::GetInstance()->LoadSoundFile();
	CSoundMgr::GetInstance()->PlayBGMSound(L"bgm.ogg");
	
	return S_OK;
}

void CStage::Update(void)
{
	WorkCamera();			// 카메라 워킹
	m_pColiisionMgr->Collision_MonsterToPlayer_MoveCheck();
	m_pColiisionMgr->Collision_MonsterToPlayer_SightCheck();
	m_pColiisionMgr->Collision_MonsterToMonster_MoveCheck();

	m_pColiisionMgr->Collision_Attack_RangeSet();
	m_pColiisionMgr->Coliision_MonsterToPlayer_AttackRangeCheck();
	m_pColiisionMgr->Collision_PlayerToGate_MoveCheck();

	m_pColiisionMgr->Collision_BossToPlayer();
	m_pColiisionMgr->Collision_BossStateCheck();
	m_pColiisionMgr->Coliision_BossToPlayerKnockBack();
	m_pColiisionMgr->Collision_BossBite();
	m_pColiisionMgr->Collision_BossRightAtt();
	m_pColiisionMgr->Collision_BossLeftAtt();
	m_pColiisionMgr->Coliision_BossToPlayerKnockBack();


	Engine::CScene::Update();

	//CreateAttPart();
	CreateHarfMoon();
	CreateCresent();
	CreateGreat();

	if(Engine::Get_Input()->GetDIKeyState(DIK_R) & 0x80)
	{
		if(m_bBossKill)
			return;
		m_bBossKill = true;
		const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");
		Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

		pBossInfo->m_bSkillHit = true;
		pBossInfo->m_bBounceHit = true;
	}
	else
		m_bBossKill = false;

	if(Engine::Get_Input()->GetDIKeyState(DIK_E) & 0x80)
	{
		if(m_bMonsterReset)
			return;
		m_pColiisionMgr->ReviveMonster();
		m_bMonsterReset = true;
	}
	else
		m_bMonsterReset = false;

	BossStart();
}

void CStage::Render(void)
{
	Engine::CScene::Render();

	//Engine::Get_NaviMgr()->Render_NaviMesh();
}

HRESULT CStage::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Terrain", pGameObject);

	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Player", pGameObject);

	pGameObject = CWeapon::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Weapon", pGameObject);
////////////////////////////////////////////////////////////////////////////////////////////////////// 마을 정면 /////////////////////////////////

	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(0.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(200.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(400.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(600.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-200.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-400.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-600.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 500.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(200.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(400.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(600.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-200.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-400.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-600.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 300.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(200.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(400.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(600.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-200.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-400.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-600.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 700.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(200.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(400.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(600.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-200.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-400.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-600.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 900.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////// 마을 좌측 /////////////////////////////////

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 1200.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 1400.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 1600.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 1800.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, 2000.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 1200.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 1400.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 1600.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 1800.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, 2000.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 1200.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 1400.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 1600.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 1800.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(800.f, 0.f, 2000.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////// 마을 우측 /////////////////////////////////

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 1200.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 1400.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 1600.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 1800.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, 2000.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 1200.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 1400.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 1600.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 1800.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, 2000.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 1200.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 1400.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 1600.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 1800.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-800.f, 0.f, 2000.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////// 설명몹 ////////////////////////////////////////////////////////////////////////////
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-600.f, 0.f, 2000.f), -90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(600.f, 0.f, 2000.f), 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////// 보스존 좌측/////////////////////////////////////////////////////////
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -5800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -5600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -6200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -6400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -5400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -6600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -6800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1200.f, 0.f, -5200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -5800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -5600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -6200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -6400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -5400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -6600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -6800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1000.f, 0.f, -5200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -5800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -5600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -6200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -6400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -5400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -6600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -6800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(1400.f, 0.f, -5200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////// 보스존 우측/////////////////////////////////////////////////////////
	pGameObject = COrge::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -5800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -5600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -6200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -6400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -5400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -6600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -6800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1200.f, 0.f, -5200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -5800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -5600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -6200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -6400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -5400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -6600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -6800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1000.f, 0.f, -5200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -5800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -5600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -6200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -6400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -5400.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -6600.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -6800.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	pGameObject = CGoblin::Create(m_pDevice, D3DXVECTOR3(-1400.f, 0.f, -5200.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Monster", pGameObject);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pGameObject = CBoss::Create(m_pDevice, D3DXVECTOR3(0.f, 0.f, -6000.f), 180.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Boss", pGameObject);

	/*pGameObject = CSwordTrail::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Trail", pGameObject);*/

	pGameObject = CSkyBox::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"SkyBox", pGameObject);

	pGameObject = CHarfMoon::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"HarfMoon", pGameObject);

	pGameObject = CHarfCasting::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"HarfCasting", pGameObject);

	pGameObject = CHarfAura::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"HarfAura", pGameObject);

	pGameObject = CHarfDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"HarfDust", pGameObject);

	pGameObject = CHarfWind::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"HarfWind", pGameObject);

	pGameObject = CHurricane::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Hurricane", pGameObject);

	pGameObject = CCresentCasting::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentCasting", pGameObject);

	pGameObject = CCresent::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Cresent", pGameObject);

	pGameObject = CCresent::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Cresent2", pGameObject);

	pGameObject = CCresent::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Cresent3", pGameObject);

	pGameObject = CCresent::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Cresent4", pGameObject);

	pGameObject = CCresent::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Cresent5", pGameObject);

	pGameObject = CCresentDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentDust", pGameObject);

	pGameObject = CCresentDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentDust2", pGameObject);

	pGameObject = CCresentDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentDust3", pGameObject);

	pGameObject = CCresentDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentDust4", pGameObject);

	pGameObject = CCresentDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentDust5", pGameObject);

	pGameObject = CCresentAura::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CresentAura", pGameObject);

	pGameObject = CGreat::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Great", pGameObject);

	pGameObject = CGreatHit::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"GreatHit", pGameObject);

	pGameObject = CGreatDust::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"GreatDust", pGameObject);

	pGameObject = CGreatAura::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"GreatAura", pGameObject);

	pGameObject = CGreatRing::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"GreatRing", pGameObject);

	pGameObject = CCycle::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Cycle", pGameObject);

	pGameObject = CWind::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Wind", pGameObject);

	pGameObject = CSkillPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"SkillPart", pGameObject);

	pGameObject = CBossBreath::Create(m_pDevice, D3DXVECTOR3(999999.f, 999999.f, 999999.f)); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"BossBreath", pGameObject);

	pGameObject = CTrailPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"TrailPart", pGameObject);

	pGameObject = CHarfPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"HarfPart", pGameObject);

	pGameObject = CHarfPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"HarfPart2", pGameObject);

	pGameObject = CHarfPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"HarfPart3", pGameObject);

	pGameObject = CHarfPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"HarfPart4", pGameObject);

	pGameObject = CHarfPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"HarfPart5", pGameObject);

	pGameObject = CCresentPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"CresentPart", pGameObject);

	pGameObject = CCresentPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"CresentPart2", pGameObject);

	pGameObject = CCresentPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"CresentPart3", pGameObject);

	pGameObject = CCresentPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"CresentPart4", pGameObject);

	pGameObject = CCresentPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"CresentPart5", pGameObject);

	pGameObject = CGreatPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"GreatPart", pGameObject);

	pGameObject = CGreatPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"GreatPart2", pGameObject);

	pGameObject = CGreatPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"GreatPart3", pGameObject);

	pGameObject = CGreatPart::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	((CPetal*)pGameObject)->SetAni();
	pLayer->AddObject(L"GreatPart4", pGameObject);

	/*pGameObject = CWater::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Water", pGameObject);*/
	
	pGameObject = CDoor::Create(m_pDevice, D3DXVECTOR3(0.f, 0.f, -500.f), true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Door", pGameObject);

	pGameObject = CDoor::Create(m_pDevice, D3DXVECTOR3(0.f, 0.f, -4000.f), false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Door", pGameObject);

	pGameObject = CObtain::Create(m_pDevice); 
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Obtain", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CStage::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	/*pGameObject = CMainBar::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"MainBar", pGameObject);*/

	/*pGameObject = CAim::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"Aim", pGameObject);*/


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));

	return S_OK;
}

void CStage::Release(void)
{

}

void CStage::CreateAttPart()
{
	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		if(((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bCreateHitPart == true)
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bCreateHitPart = false;
			
			Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);

			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CHit::Create(m_pDevice, ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos, 0); 
			Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
			pLayer->AddObject(L"Hit", pGameObject);

			m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

		}
	}
}
void CStage::CreateGreat()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pGreatComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Great", L"Transform");
	const Engine::CComponent* pGreatHitComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatHit", L"Transform");
	const Engine::CComponent* pGreatDustComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatDust", L"Transform");
	const Engine::CComponent* pGreatAuraComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatAura", L"Transform");
	const Engine::CComponent* pGreatRingComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatRing", L"Transform");
	const Engine::CComponent* pCycleComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Cycle", L"Transform");
	const Engine::CComponent* pWindComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Wind", L"Transform");
	const Engine::CComponent* pSkillPartComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"SkillPart", L"Transform");
	const Engine::CComponent* pBossBreathComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"BossBreath", L"Transform");
	const Engine::CComponent* pTrailPartComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
	const Engine::CComponent* pGreatPartComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatPart", L"Transform");
	const Engine::CComponent* pGreatPartComponent2 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatPart2", L"Transform");
	const Engine::CComponent* pGreatPartComponent3 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatPart3", L"Transform");
	const Engine::CComponent* pGreatPartComponent4 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"GreatPart4", L"Transform");
	
	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pGreatInfo = ((Engine::CTransform*)pGreatComponent);
	Engine::CTransform* pGreatHitInfo = ((Engine::CTransform*)pGreatHitComponent);
	Engine::CTransform* pGreatDustInfo = ((Engine::CTransform*)pGreatDustComponent);
	Engine::CTransform* pGreatAuraInfo = ((Engine::CTransform*)pGreatAuraComponent);
	Engine::CTransform* pGreatRingInfo = ((Engine::CTransform*)pGreatRingComponent);
	Engine::CTransform* pCycleInfo = ((Engine::CTransform*)pCycleComponent);
	Engine::CTransform* pWindInfo = ((Engine::CTransform*)pWindComponent);
	Engine::CTransform* pSkillPartInfo = ((Engine::CTransform*)pSkillPartComponent);
	Engine::CTransform* pBossBreathInfo = ((Engine::CTransform*)pBossBreathComponent);
	Engine::CTransform* pTrailPartInfo = ((Engine::CTransform*)pTrailPartComponent);
	Engine::CTransform* pGreatPartInfo = ((Engine::CTransform*)pGreatPartComponent);
	Engine::CTransform* pGreatPartInfo2 = ((Engine::CTransform*)pGreatPartComponent2);
	Engine::CTransform* pGreatPartInfo3 = ((Engine::CTransform*)pGreatPartComponent3);
	Engine::CTransform* pGreatPartInfo4 = ((Engine::CTransform*)pGreatPartComponent4);

	D3DXVECTOR3 vRight, vUp, vLook;

	memcpy(&vRight, &pPlayerInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vRight, &vRight);
	memcpy(&vUp, &pPlayerInfo->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vUp, &vUp);
	memcpy(&vLook, &pPlayerInfo->m_matWorld.m[2][0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vLook, &vLook);

	pGreatPartInfo->m_vPos = pPlayerInfo->m_vPos;
	pGreatPartInfo->m_vPos += vRight * 50.f;
	pGreatPartInfo->m_vDir = D3DXVECTOR3(0.f, 1.f, 0.f);

	pGreatPartInfo2->m_vPos = pPlayerInfo->m_vPos;
	pGreatPartInfo2->m_vPos += vRight * 50.f * -1;
	pGreatPartInfo2->m_vDir = D3DXVECTOR3(0.f, 1.f, 0.f);

	pGreatPartInfo3->m_vPos = pPlayerInfo->m_vPos;
	pGreatPartInfo3->m_vPos += vLook * 50.f;
	pGreatPartInfo3->m_vDir = D3DXVECTOR3(0.f, 1.f, 0.f);

	pGreatPartInfo4->m_vPos = pPlayerInfo->m_vPos;
	pGreatPartInfo4->m_vPos += vLook * 50.f * -1;
	pGreatPartInfo4->m_vDir = D3DXVECTOR3(0.f, 1.f, 0.f);


	if(pPlayerInfo->m_fGreatCreateTime >= 1.f
		&& pPlayerInfo->m_bCreateGreat)
	{

		CSoundMgr::GetInstance()->PlayerGreatSound(L"Great.wav");
		CSoundMgr::GetInstance()->GreatWindSound(L"Wind.ogg");
		pGreatInfo->m_vPos = pPlayerInfo->m_vPos;
		pGreatInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y];
		pGreatInfo->m_bSkillRender = true;
		
		pGreatHitInfo->m_vPos = pPlayerInfo->m_vPos;
		pGreatHitInfo->m_vPos.y = 5.f;
		pGreatHitInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y];
		pGreatHitInfo->m_bSkillRender = true;

		pWindInfo->m_vPos = pPlayerInfo->m_vPos;
		pWindInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y];
		pWindInfo->m_bSkillRender = true;
	
		pPlayerInfo->m_bCreateGreat= false;
	}


	D3DXVECTOR3 vPlayerRight, vPlayerInvLook;
	memcpy(&vPlayerRight, &pPlayerInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&vPlayerInvLook, &pPlayerInfo->m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	pSkillPartInfo->m_vPos = pPlayerInfo->m_vPos;
	pSkillPartInfo->m_vPos += vPlayerRight * 22.5f;
	pSkillPartInfo->m_vPos += vPlayerInvLook * 5.5f;


	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

	pBossBreathInfo->m_vPos = pBossInfo->m_vPos;
	pBossBreathInfo->m_vPos += pBossInfo->m_vDir * 500.f;
	pBossBreathInfo->m_vPos.y += 100.f;
	pBossBreathInfo->m_vDir = pBossInfo->m_vDir;

	const Engine::CComponent* pWeaponComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Weapon", L"Transform");
	Engine::CTransform* pWeaponInfo = ((Engine::CTransform*)pWeaponComponent);

	D3DXVECTOR3 vWeaponUp;
	memcpy(&vWeaponUp, &pWeaponInfo->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));

	D3DXVec3Normalize(&vWeaponUp, &vWeaponUp);

	pTrailPartInfo->m_vDir = vWeaponUp;

	D3DXVECTOR3 vWeapon;
	memcpy(&vWeapon, &pWeaponInfo->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	pTrailPartInfo->m_vPos = vWeapon;
	pTrailPartInfo->m_vPos += pTrailPartInfo->m_vDir * 50.f;

	D3DXVECTOR3 vDir;

	vDir = pTrailPartInfo->m_vPos - vWeapon;
	D3DXVec3Normalize(&vDir, &vDir);

	pTrailPartInfo->m_vDir = vDir;
	pTrailPartInfo->m_vPos = vWeapon;


	if(pPlayerInfo->m_fGreatCreateTime >= 0.3f && 
		pPlayerInfo->m_fGreatCreateTime < 0.4f &&
		pSkillPartInfo->m_bSkillRender == false)
	{
		pSkillPartInfo->m_bSkillRender = true;
	}

	if(pWindInfo->m_bWindArrive)
	{
		pWindInfo->m_bWindArrive = false;
	}

	if(pPlayerInfo->m_bCreateGreatCasting)
	{
		pGreatPartInfo->m_bSkillRender = true;
		pGreatPartInfo2->m_bSkillRender = true;
		pGreatPartInfo3->m_bSkillRender = true;
		pGreatPartInfo4->m_bSkillRender = true;
		pGreatAuraInfo->m_bSkillRender = true;
		pGreatRingInfo->m_bSkillRender = true;
		pCycleInfo->m_bSkillRender = true;
		pPlayerInfo->m_bCreateGreatCasting = false;
	}

}
void CStage::CreateBossPart()
{

}
void CStage::CreateHarfMoon()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pHarfMoonComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfMoon", L"Transform");
	const Engine::CComponent* pHarfCastingComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfCasting", L"Transform");
	const Engine::CComponent* pHarfDustComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfDust", L"Transform");
	const Engine::CComponent* pHarfWindComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfWind", L"Transform");
	const Engine::CComponent* pHarfHurricaneComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Hurricane", L"Transform");
	const Engine::CComponent* pHarfAuraComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfAura", L"Transform");
	const Engine::CComponent* pHarfPartComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfPart", L"Transform");
	const Engine::CComponent* pHarfPartComponent2 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfPart2", L"Transform");
	const Engine::CComponent* pHarfPartComponent3 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfPart3", L"Transform");
	const Engine::CComponent* pHarfPartComponent4 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfPart4", L"Transform");
	const Engine::CComponent* pHarfPartComponent5 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfPart5", L"Transform");


	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pHarfMoonInfo = ((Engine::CTransform*)pHarfMoonComponent);
	Engine::CTransform* pHarfCastingInfo = ((Engine::CTransform*)pHarfCastingComponent);
	Engine::CTransform* pHarfDustInfo = ((Engine::CTransform*)pHarfDustComponent);
	Engine::CTransform* pHarfWindInfo = ((Engine::CTransform*)pHarfWindComponent);
	Engine::CTransform* pHarfHurricaneInfo = ((Engine::CTransform*)pHarfHurricaneComponent);
	Engine::CTransform* pHarfAuraInfo = ((Engine::CTransform*)pHarfAuraComponent);	
	Engine::CTransform* pHarfPartInfo = ((Engine::CTransform*)pHarfPartComponent);
	Engine::CTransform* pHarfPartInfo2 = ((Engine::CTransform*)pHarfPartComponent2);
	Engine::CTransform* pHarfPartInfo3 = ((Engine::CTransform*)pHarfPartComponent3);
	Engine::CTransform* pHarfPartInfo4 = ((Engine::CTransform*)pHarfPartComponent4);
	Engine::CTransform* pHarfPartInfo5 = ((Engine::CTransform*)pHarfPartComponent5);

	D3DXVECTOR3 vRight, vLook;

	memcpy(&vLook, &pHarfMoonInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vLook, &vLook);
	memcpy(&vRight, &pHarfMoonInfo->m_matWorld.m[2][0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vRight, &vRight);

	pHarfPartInfo->m_vPos = pHarfMoonInfo->m_vPos;
	pHarfPartInfo->m_vPos += vLook * 20.f * -1;
	pHarfPartInfo->m_vDir = pHarfMoonInfo->m_vDir;

	pHarfPartInfo2->m_vPos = pHarfMoonInfo->m_vPos;
	pHarfPartInfo2->m_vPos += vLook * 50.f * -1;
	pHarfPartInfo2->m_vPos += vRight * 300.f;
	pHarfPartInfo2->m_vDir = pHarfMoonInfo->m_vDir;

	pHarfPartInfo3->m_vPos = pHarfMoonInfo->m_vPos;
	pHarfPartInfo3->m_vPos += vLook * 50.f * -1;
	pHarfPartInfo3->m_vPos += vRight * 300.f * -1.f;
	pHarfPartInfo3->m_vDir = pHarfMoonInfo->m_vDir;

	pHarfPartInfo4->m_vPos = pHarfMoonInfo->m_vPos;
	pHarfPartInfo4->m_vPos += vLook * 40.f * -1;
	pHarfPartInfo4->m_vPos += vRight * 150.f;
	pHarfPartInfo4->m_vDir = pHarfMoonInfo->m_vDir;

	pHarfPartInfo5->m_vPos = pHarfMoonInfo->m_vPos;
	pHarfPartInfo5->m_vPos += vLook * 40.f * -1;
	pHarfPartInfo5->m_vPos += vRight * 150.f * -1.f;
	pHarfPartInfo5->m_vDir = pHarfMoonInfo->m_vDir;



	if(pPlayerInfo->m_fHarfMoonCreateTime >= 1.5f
		&& pPlayerInfo->m_bCreateHarfMoon)
	{
		pHarfMoonInfo->m_vPos = pPlayerInfo->m_vPos;
		pHarfMoonInfo->m_vPos.y += 50.f; 
		pHarfMoonInfo->m_vDir = pPlayerInfo->m_vDir;
		pHarfMoonInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(270.f);
		pHarfMoonInfo->m_bSkillRender = true;

		/*D3DXVECTOR3 vRight;

		memcpy(&vRight, &pHarfMoonInfo->m_matWorld.m[2][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		pHarfDustInfo->m_vPos = pPlayerInfo->m_vPos;
		pHarfDustInfo->m_vPos += vRight * -1 * 300.f;
		pHarfDustInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f);
		pHarfDustInfo->m_bSkillRender = true;*/

		pPlayerInfo->m_bCreateHarfMoon = false;
		pHarfPartInfo->m_bSkillRender = true;
		pHarfPartInfo2->m_bSkillRender = true;
		pHarfPartInfo3->m_bSkillRender = true;
		pHarfPartInfo4->m_bSkillRender = true;
		pHarfPartInfo5->m_bSkillRender = true;
		CSoundMgr::GetInstance()->PlayerHarfSound(L"Half2.wav");
	}

	if(pPlayerInfo->m_bCreateHarfCasting)
	{
		//pHarfCastingInfo->m_bSkillRender = true;
		pHarfWindInfo->m_bSkillRender = true;
		pHarfHurricaneInfo->m_bSkillRender = true;
		pHarfAuraInfo->m_bSkillRender = true;
		pPlayerInfo->m_bCreateHarfCasting = false;

	}

}

void CStage::CreateCresent()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pCresentComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Cresent", L"Transform");
	const Engine::CComponent* pCresentComponent2 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Cresent2", L"Transform");
	const Engine::CComponent* pCresentComponent3 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Cresent3", L"Transform");
	const Engine::CComponent* pCresentComponent4 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Cresent4", L"Transform");
	const Engine::CComponent* pCresentComponent5 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Cresent5", L"Transform");
	const Engine::CComponent* pCastingComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentCasting", L"Transform");
	const Engine::CComponent* pHarfWindComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfWind", L"Transform");
	const Engine::CComponent* pHarfHurricaneComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Hurricane", L"Transform");
	const Engine::CComponent* pHarfDustComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"HarfDust", L"Transform");
	const Engine::CComponent* pCresentAuraComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentAura", L"Transform");
	const Engine::CComponent* pCresentDustComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentDust", L"Transform");
	const Engine::CComponent* pCresentDustComponent2 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentDust2", L"Transform");
	const Engine::CComponent* pCresentDustComponent3 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentDust3", L"Transform");
	const Engine::CComponent* pCresentDustComponent4 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentDust4", L"Transform");
	const Engine::CComponent* pCresentDustComponent5 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentDust5", L"Transform");
	const Engine::CComponent* pCresentPartComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentPart", L"Transform");
	const Engine::CComponent* pCresentPartComponent2 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentPart2", L"Transform");
	const Engine::CComponent* pCresentPartComponent3 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentPart3", L"Transform");
	const Engine::CComponent* pCresentPartComponent4 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentPart4", L"Transform");
	const Engine::CComponent* pCresentPartComponent5 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"CresentPart5", L"Transform");


	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pCastingInfo = ((Engine::CTransform*)pCastingComponent);
	Engine::CTransform* pCresentInfo = ((Engine::CTransform*)pCresentComponent);
	Engine::CTransform* pCresentInfo2 = ((Engine::CTransform*)pCresentComponent2);
	Engine::CTransform* pCresentInfo3 = ((Engine::CTransform*)pCresentComponent3);
	Engine::CTransform* pCresentInfo4 = ((Engine::CTransform*)pCresentComponent4);
	Engine::CTransform* pCresentInfo5 = ((Engine::CTransform*)pCresentComponent5);
	Engine::CTransform* pHarfWindInfo = ((Engine::CTransform*)pHarfWindComponent);
	Engine::CTransform* pHarfHurricaneInfo = ((Engine::CTransform*)pHarfHurricaneComponent);
	Engine::CTransform* pHarfDustInfo = ((Engine::CTransform*)pHarfDustComponent);
	Engine::CTransform* pCresentAuraInfo = ((Engine::CTransform*)pCresentAuraComponent);
	Engine::CTransform* pCresentDustInfo = ((Engine::CTransform*)pCresentDustComponent);
	Engine::CTransform* pCresentDustInfo2 = ((Engine::CTransform*)pCresentDustComponent2);
	Engine::CTransform* pCresentDustInfo3 = ((Engine::CTransform*)pCresentDustComponent3);
	Engine::CTransform* pCresentDustInfo4 = ((Engine::CTransform*)pCresentDustComponent4);
	Engine::CTransform* pCresentDustInfo5 = ((Engine::CTransform*)pCresentDustComponent5);
	Engine::CTransform* pCresentPartInfo = ((Engine::CTransform*)pCresentPartComponent);
	Engine::CTransform* pCresentPartInfo2 = ((Engine::CTransform*)pCresentPartComponent2);
	Engine::CTransform* pCresentPartInfo3 = ((Engine::CTransform*)pCresentPartComponent3);
	Engine::CTransform* pCresentPartInfo4 = ((Engine::CTransform*)pCresentPartComponent4);
	Engine::CTransform* pCresentPartInfo5 = ((Engine::CTransform*)pCresentPartComponent5);

	D3DXVECTOR3 vInvUp;
	D3DXVECTOR3 vInvUp2;
	D3DXVECTOR3 vInvUp3;
	D3DXVECTOR3 vInvUp4;
	D3DXVECTOR3 vInvUp5;

	memcpy(&vInvUp, &pCresentInfo->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vInvUp2, &pCresentInfo2->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vInvUp3, &pCresentInfo3->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vInvUp4, &pCresentInfo4->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vInvUp5, &pCresentInfo5->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));

	vInvUp *= -1.f;
	vInvUp2 *= -1.f;
	vInvUp3 *= -1.f;
	vInvUp4 *= -1.f;
	vInvUp5 *= -1.f;

	D3DXVec3Normalize(&vInvUp, &vInvUp);
	D3DXVec3Normalize(&vInvUp2, &vInvUp2);
	D3DXVec3Normalize(&vInvUp3, &vInvUp3);
	D3DXVec3Normalize(&vInvUp4, &vInvUp4);
	D3DXVec3Normalize(&vInvUp5, &vInvUp5);

	pCresentPartInfo->m_vPos = pCresentInfo->m_vPos;
	pCresentPartInfo->m_vPos += vInvUp * 30.f;
	pCresentPartInfo->m_vDir = pCresentInfo->m_vDir;

	pCresentPartInfo2->m_vPos = pCresentInfo2->m_vPos;
	pCresentPartInfo2->m_vPos += vInvUp2 * 30.f;
	pCresentPartInfo2->m_vDir = pCresentInfo2->m_vDir;

	pCresentPartInfo3->m_vPos = pCresentInfo3->m_vPos;
	pCresentPartInfo3->m_vPos += vInvUp3 * 30.f;
	pCresentPartInfo3->m_vDir = pCresentInfo3->m_vDir;

	pCresentPartInfo4->m_vPos = pCresentInfo4->m_vPos;
	pCresentPartInfo4->m_vPos += vInvUp4 * 30.f;
	pCresentPartInfo4->m_vDir = pCresentInfo4->m_vDir;

	pCresentPartInfo5->m_vPos = pCresentInfo5->m_vPos;
	pCresentPartInfo5->m_vPos += vInvUp5 * 30.f;
	pCresentPartInfo5->m_vDir = pCresentInfo5->m_vDir;

	if(pPlayerInfo->m_fCresentCreateTime >= 1.5f
		&& pPlayerInfo->m_bCreateCresent)
	{

		pCresentPartInfo->m_bSkillRender = true;
		pCresentPartInfo2->m_bSkillRender = true;
		pCresentPartInfo3->m_bSkillRender = true;
		pCresentPartInfo4->m_bSkillRender = true;
		pCresentPartInfo5->m_bSkillRender = true;
		CSoundMgr::GetInstance()->PlayerCresentSound(L"Cresent2.wav");
		/*pHarfDustInfo->m_vPos = pPlayerInfo->m_vPos;
		pHarfDustInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f);
		pHarfDustInfo->m_bSkillRender = true;*/

		pCresentDustInfo->m_vPos = pPlayerInfo->m_vPos;
		pCresentDustInfo->m_vPos += pPlayerInfo->m_vDir * -1 * 30.f;
		pCresentDustInfo->m_vDir = pPlayerInfo->m_vDir;
		pCresentDustInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y];
		pCresentDustInfo->m_bSkillRender = true;

		pCresentInfo->m_vPos = pPlayerInfo->m_vPos;
		pCresentInfo->m_vPos.y += 50.f; 
		pCresentInfo->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(180.f);
		pCresentInfo->m_vDir = pPlayerInfo->m_vDir;
		pCresentInfo->m_bSkillRender = true;

		pCresentDustInfo2->m_vPos = pPlayerInfo->m_vPos;
		pCresentDustInfo2->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(337.5f);
		pCresentDustInfo2->m_vPos += pPlayerInfo->m_vDir * -1 * 30.f;

		pCresentInfo2->m_vPos = pPlayerInfo->m_vPos;
		pCresentInfo2->m_vPos.y += 50.f; 
		pCresentInfo2->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(157.5f);

		D3DXVECTOR3 vPlayerRight;
		memcpy(&vPlayerRight, &pPlayerInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		vPlayerRight = vPlayerRight + pPlayerInfo->m_vDir;
		vPlayerRight = vPlayerRight + pPlayerInfo->m_vDir;
		D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

		pCresentDustInfo2->m_vDir = vPlayerRight;
		pCresentDustInfo2->m_bSkillRender = true;
		pCresentInfo2->m_vDir = vPlayerRight;
		pCresentInfo2->m_bSkillRender = true;


		pCresentDustInfo3->m_vPos = pPlayerInfo->m_vPos;
		pCresentDustInfo3->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(315.f);
		pCresentDustInfo3->m_vPos += pPlayerInfo->m_vDir * -1 * 30.f;

		pCresentInfo3->m_vPos = pPlayerInfo->m_vPos;
		pCresentInfo3->m_vPos.y += 50.f; 
		pCresentInfo3->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(135.f);

		vPlayerRight;
		memcpy(&vPlayerRight, &pPlayerInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		vPlayerRight = vPlayerRight + pPlayerInfo->m_vDir;
		D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

		pCresentDustInfo3->m_vDir = vPlayerRight;
		pCresentDustInfo3->m_bSkillRender = true;

		pCresentInfo3->m_vDir = vPlayerRight;
		pCresentInfo3->m_bSkillRender = true;

		pCresentDustInfo4->m_vPos = pPlayerInfo->m_vPos;
		pCresentDustInfo4->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(22.5f);
		pCresentDustInfo4->m_vPos += pPlayerInfo->m_vDir * -1 * 30.f;

		pCresentInfo4->m_vPos = pPlayerInfo->m_vPos;
		pCresentInfo4->m_vPos.y += 50.f; 
		pCresentInfo4->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(202.5f);

		vPlayerRight;
		memcpy(&vPlayerRight, &pPlayerInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		vPlayerRight = (vPlayerRight * -1) + pPlayerInfo->m_vDir;
		vPlayerRight = vPlayerRight + pPlayerInfo->m_vDir;
		D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

		pCresentDustInfo4->m_vDir = vPlayerRight;
		pCresentDustInfo4->m_bSkillRender = true;

		pCresentInfo4->m_vDir = vPlayerRight;
		pCresentInfo4->m_bSkillRender = true;


		pCresentDustInfo5->m_vPos = pPlayerInfo->m_vPos;
		pCresentDustInfo5->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(45.f);
		pCresentDustInfo5->m_vPos += pPlayerInfo->m_vDir * -1 * 30.f;

		pCresentInfo5->m_vPos = pPlayerInfo->m_vPos;
		pCresentInfo5->m_vPos.y += 50.f; 
		pCresentInfo5->m_fAngle[Engine::ANGLE_Y] = pPlayerInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(225.f);

		vPlayerRight;
		memcpy(&vPlayerRight, &pPlayerInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		vPlayerRight = (vPlayerRight * -1) + pPlayerInfo->m_vDir;
		D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

		pCresentDustInfo5->m_vDir = vPlayerRight;
		pCresentDustInfo5->m_bSkillRender = true;

		pCresentInfo5->m_vDir = vPlayerRight;
		pCresentInfo5->m_bSkillRender = true;


		pPlayerInfo->m_bCreateCresent = false;
	}

	if(pPlayerInfo->m_bCreateCresentCasting)
	{
		pHarfWindInfo->m_bSkillRender = true;
		pHarfHurricaneInfo->m_bSkillRender = true;
		//pCastingInfo->m_bSkillRender = true;
		pCresentAuraInfo->m_bSkillRender = true;
		pPlayerInfo->m_bCreateCresentCasting = false;
	}
}
CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pStage = new CStage(pDevice);

	if(FAILED(pStage->InitScene()))
	{
		Engine::Safe_Delete(pStage);
	}

	return pStage;
}


HRESULT CStage::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	// Obtain
	LightInfo.Type = D3DLIGHT_DIRECTIONAL;

	LightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient	= D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	LightInfo.Specular	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(0.f, -0.25f, -1.f);

	HRESULT	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	D3DLIGHT9		PlayerInfo;
	ZeroMemory(&PlayerInfo, sizeof(D3DLIGHT9));

	PlayerInfo.Type = D3DLIGHT_DIRECTIONAL;

	PlayerInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	PlayerInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	PlayerInfo.Specular	= D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	PlayerInfo.Direction = D3DXVECTOR3(0.f, -0.25f, 1.f);

	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &PlayerInfo, 1);
	FAILED_CHECK(hr);

	D3DLIGHT9		SkyBoxInfo;
	ZeroMemory(&SkyBoxInfo, sizeof(D3DLIGHT9));

	SkyBoxInfo.Type = D3DLIGHT_DIRECTIONAL;

	SkyBoxInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	SkyBoxInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	SkyBoxInfo.Specular	= D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	SkyBoxInfo.Direction = D3DXVECTOR3(0.f, -1.f, 0.f);

	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &SkyBoxInfo, 2);
	FAILED_CHECK(hr);

	D3DLIGHT9		TerrainInfo;
	ZeroMemory(&TerrainInfo, sizeof(D3DLIGHT9));

	TerrainInfo.Type = D3DLIGHT_DIRECTIONAL;

	TerrainInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	TerrainInfo.Ambient	= D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
	TerrainInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	TerrainInfo.Direction = D3DXVECTOR3(0.f, -1.f, 1.f);

	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &TerrainInfo, 3);
	FAILED_CHECK(hr);

	D3DLIGHT9		EffectInfo;
	ZeroMemory(&EffectInfo, sizeof(D3DLIGHT9));

	EffectInfo.Type = D3DLIGHT_DIRECTIONAL;

	EffectInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	EffectInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	EffectInfo.Specular	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	EffectInfo.Direction = D3DXVECTOR3(0.f, 0.f, 0.f);

	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &EffectInfo, 4);
	FAILED_CHECK(hr);


	/*D3DLIGHT9		SkyBoxLightInfo;
	ZeroMemory(&SkyBoxLightInfo, sizeof(D3DLIGHT9));

	SkyBoxLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	SkyBoxLightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	SkyBoxLightInfo.Ambient	= D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	SkyBoxLightInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	SkyBoxLightInfo.Direction = D3DXVECTOR3(0.f, -1.f, 0.f);

	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &SkyBoxLightInfo, 1);
	FAILED_CHECK(hr);*/


	return S_OK;
}

HRESULT CStage::Add_NaviMesh(void)
{
	Engine::Get_NaviMgr()->Reserve_CellContainerSize(m_pDevice, 11);

	HRESULT		hr = NULL;
	D3DXVECTOR3		vPoint[3];

	///// 시작공간
	vPoint[0] = D3DXVECTOR3(120.f, 3.f, -700.f);
	vPoint[1] = D3DXVECTOR3(-100.f, 3.f, -700.f);
	vPoint[2] = D3DXVECTOR3(-120.f, 3.f, -325.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(120.f, 3.f, -700.f);
	vPoint[1] = D3DXVECTOR3(-120.f, 3.f, -325.f);
	vPoint[2] = D3DXVECTOR3(120.f, 3.f, -325.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(120.f, 3.f, -325.f);
	vPoint[1] = D3DXVECTOR3(-120.f, 3.f, -325.f);
	vPoint[2] = D3DXVECTOR3(1700.f, 3.f, 2200.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-120.f, 3.f, -325.f);
	vPoint[1] = D3DXVECTOR3(-1700.f, 3.f, 2200.f);
	vPoint[2] = D3DXVECTOR3(1700.f, 3.f, 2200.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(1700.f, 3.f, -325.f);
	vPoint[1] = D3DXVECTOR3(120.f, 3.f, -325.f);
	vPoint[2] = D3DXVECTOR3(1700.f, 3.f, 2200.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-120.f, 3.f, -325.f);
	vPoint[1] = D3DXVECTOR3(-1700.f, 3.f, -325.f);
	vPoint[2] = D3DXVECTOR3(-1700.f, 3.f, 2200.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	///다리

	vPoint[0] = D3DXVECTOR3(-165.f, 3.f, -850.f);
	vPoint[1] = D3DXVECTOR3(-100.f, 3.f, -700.f);
	vPoint[2] = D3DXVECTOR3(120.f, 3.f, -700.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(50.f, 3.f, -900.f);
	vPoint[1] = D3DXVECTOR3(-165.f, 3.f, -850.f);
	vPoint[2] = D3DXVECTOR3(120.f, 3.f, -700.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(40.f, 3.f, -1080.f);
	vPoint[1] = D3DXVECTOR3(-165.f, 3.f, -850.f);
	vPoint[2] = D3DXVECTOR3(50.f, 3.f, -900.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-165.f, 3.f, -1080.f);
	vPoint[1] = D3DXVECTOR3(-165.f, 3.f, -850.f);
	vPoint[2] = D3DXVECTOR3(40.f, 3.f, -1080.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-55.f, 3.f, -1380.f);
	vPoint[1] = D3DXVECTOR3(-165.f, 3.f, -1080.f);
	vPoint[2] = D3DXVECTOR3(40.f, 3.f, -1080.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 3.f, -1450.f);
	vPoint[1] = D3DXVECTOR3(-55.f, 3.f, -1380.f);
	vPoint[2] = D3DXVECTOR3(40.f, 3.f, -1080.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-55.f, 3.f, -1650.f);
	vPoint[1] = D3DXVECTOR3(-55.f, 3.f, -1380.f);
	vPoint[2] = D3DXVECTOR3(150.f, 3.f, -1450.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 3.f, -1650.f);
	vPoint[1] = D3DXVECTOR3(-55.f, 3.f, -1650.f);
	vPoint[2] = D3DXVECTOR3(150.f, 3.f, -1450.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-170.f, 3.f, -2000.f);
	vPoint[1] = D3DXVECTOR3(-55.f, 3.f, -1650.f);
	vPoint[2] = D3DXVECTOR3(150.f, 3.f, -1650.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(50.f, 3.f, -2000.f);
	vPoint[1] = D3DXVECTOR3(-170.f, 3.f, -2000.f);
	vPoint[2] = D3DXVECTOR3(150.f, 3.f, -1650.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-190.f, 3.f, -2100.f);
	vPoint[1] = D3DXVECTOR3(-170.f, 3.f, -2000.f);
	vPoint[2] = D3DXVECTOR3(50.f, 3.f, -2000.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(40.f, 3.f, -2130.f);
	vPoint[1] = D3DXVECTOR3(-190.f, 3.f, -2100.f);
	vPoint[2] = D3DXVECTOR3(50.f, 3.f, -2000.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(170.f, 3.f, -2500.f);
	vPoint[1] = D3DXVECTOR3(-190.f, 3.f, -2100.f);
	vPoint[2] = D3DXVECTOR3(40.f, 3.f, -2130.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-55.f, 3.f, -2470.f);
	vPoint[1] = D3DXVECTOR3(-190.f, 3.f, -2100.f);
	vPoint[2] = D3DXVECTOR3(170.f, 3.f, -2500.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(170.f, 3.f, -2670.f);
	vPoint[1] = D3DXVECTOR3(-55.f, 3.f, -2470.f);
	vPoint[2] = D3DXVECTOR3(170.f, 3.f, -2500.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-65.f, 3.f, -2700.f);
	vPoint[1] = D3DXVECTOR3(-55.f, 3.f, -2470.f);
	vPoint[2] = D3DXVECTOR3(170.f, 3.f, -2670.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(100.f, 3.f, -2880.f);
	vPoint[1] = D3DXVECTOR3(-65.f, 3.f, -2700.f);
	vPoint[2] = D3DXVECTOR3(170.f, 3.f, -2670.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-160.f, 3.f, -3000.f);
	vPoint[1] = D3DXVECTOR3(-65.f, 3.f, -2700.f);
	vPoint[2] = D3DXVECTOR3(100.f, 3.f, -2880.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(50.f, 3.f, -3000.f);
	vPoint[1] = D3DXVECTOR3(-160.f, 3.f, -3000.f);
	vPoint[2] = D3DXVECTOR3(100.f, 3.f, -2880.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-170.f, 3.f, -3170.f);
	vPoint[1] = D3DXVECTOR3(-160.f, 3.f, -3000.f);
	vPoint[2] = D3DXVECTOR3(50.f, 3.f, -3000.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(40.f, 3.f, -3170.f);
	vPoint[1] = D3DXVECTOR3(-170.f, 3.f, -3170.f);
	vPoint[2] = D3DXVECTOR3(50.f, 3.f, -3000.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-65.f, 3.f, -3500.f);
	vPoint[1] = D3DXVECTOR3(-170.f, 3.f, -3170.f);
	vPoint[2] = D3DXVECTOR3(40.f, 3.f, -3170.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 3.f, -3500.f);
	vPoint[1] = D3DXVECTOR3(-65.f, 3.f, -3500.f);
	vPoint[2] = D3DXVECTOR3(40.f, 3.f, -3170.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-65.f, 3.f, -3750.f);
	vPoint[1] = D3DXVECTOR3(-65.f, 3.f, -3500.f);
	vPoint[2] =  D3DXVECTOR3(150.f, 3.f, -3500.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(160.f, 3.f, -3700.f);
	vPoint[1] = D3DXVECTOR3(-65.f, 3.f, -3750.f);
	vPoint[2] =  D3DXVECTOR3(150.f, 3.f, -3500.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-120.f, 3.f, -3900.f);
	vPoint[1] = D3DXVECTOR3(-65.f, 3.f, -3750.f);
	vPoint[2] = D3DXVECTOR3(160.f, 3.f, -3700.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(120.f, 3.f, -3900.f);
	vPoint[1] = D3DXVECTOR3(-120.f, 3.f, -3900.f);
	vPoint[2] = D3DXVECTOR3(160.f, 3.f, -3700.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	// 보스방
	vPoint[0] = D3DXVECTOR3(-120.f, 3.f, -4250.f);
	vPoint[1] = D3DXVECTOR3(-120.f, 3.f, -3900.f);
	vPoint[2] = D3DXVECTOR3(120.f, 3.f, -3900.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(120.f, 3.f, -4250.f);
	vPoint[1] = D3DXVECTOR3(-120.f, 3.f, -4250.f);
	vPoint[2] = D3DXVECTOR3(120.f, 3.f, -3900.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(1800.f, 3.f, -7300.f);
	vPoint[1] = D3DXVECTOR3(-120.f, 3.f, -4250.f);
	vPoint[2] = D3DXVECTOR3(120.f, 3.f, -4250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(1800.f, 3.f, -7300.f);
	vPoint[1] = D3DXVECTOR3(120.f, 3.f, -4250.f);
	vPoint[2] = D3DXVECTOR3(1800.f, 3.f, -4250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(1800.f, 3.f, -7300.f);
	vPoint[1] = D3DXVECTOR3(-1700.f, 3.f, -7300.f);
	vPoint[2] = D3DXVECTOR3(-120.f, 3.f, -4250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-1700.f, 3.f, -7300.f);
	vPoint[1] = D3DXVECTOR3(-1700.f, 3.f, -4250.f);
	vPoint[2] = D3DXVECTOR3(-120.f, 3.f, -4250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	Engine::Get_NaviMgr()->Link_Cell();
	return S_OK;
}

void CStage::WorkCamera(void)
{
	if(Engine::Get_Input()->GetDIKeyState(DIK_Q) & 0x80)
	{
		if(m_bClick)
			return;

		m_bClick = true;

		if(m_bMouseFix == true)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	else
		m_bClick = false;

	if(m_bMouseFix == false)
		return;

	if(m_bMouseFix)
	{
		m_pCameraMgr->FixMouse(&WINCX, &WINCY, g_hWnd);
		m_pCameraMgr->MouseMove();
	}
	m_pCameraMgr->SetCameraTransForm();
	m_pCameraMgr->SetViewMatrix();
}

void CStage::BossStart()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");

	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

	float fDistance = D3DXVec3Length(&(pPlayerInfo->m_vPos - pBossInfo->m_vPos));

	if(fDistance < 4000.f)
	{
		if(!pBossInfo->m_bSkillRender)
		{
			pBossInfo->m_bSkillRender = true;
			CSoundMgr::GetInstance()->PlayerBossApearSound(L"BossApear.ogg");
		}
		
	}

}



