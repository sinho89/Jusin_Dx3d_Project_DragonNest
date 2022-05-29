#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "CameraMgr.h"
#include "Stage.h"
#include "CollisionMgr.h"
#include "Texture.h"
#include "Layer.h"
#include "HarfMoon.h"
#include "SoundMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_mapCenterBone(NULL)
, m_iAniIdx(0)
, m_bPush(false)
, m_dwIndex(0)
, m_iOriAniIdx(0)
, m_bAttackCurver(false)
, m_fAniTime(0.f)
, m_bGetUp(false)
, m_bSkillHarf(false)
, m_bSkillCresent(false)
, m_bSkillGreat(false)
{
	for(int i = 0; i < STATE_END; ++i)
		m_bState[i] = false;
	for(int i = 0; i < 4; ++i)
		m_bDirMoveCurver[i] = false;
}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fDetail = 1.f;
	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 1400.f);
	Engine::CCameraMgr::GetInstance()->SetTargetTransform(m_pInfo);

	m_pInfo->m_iAttackCount = 0;
	m_pInfo->m_fSpeed = 300.f;
	m_eStateType = STATE_STAND;
	m_bState[STATE_STAND] = true;
	m_fAniTime = 0.2f;
	m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
	m_pInfo->m_iHp = 3000;
	m_pInfo->m_iNormalAtt = 100;
	m_pInfo->m_iBounceAtt = 400;

	m_pMesh->Initialize();

	m_pTexture[0]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerHairMap");
	m_pTexture[1]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerHeadMap");
	m_pTexture[2]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerBodyMap");
	m_pTexture[3]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerGuntleMap");
	m_pTexture[4]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerHandMap");
	m_pTexture[5]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerLegMap");
	m_pTexture[6]->Set_ConstantTable(*(m_pMesh->GetEffect()), "tPlayerBootMap");

	m_fAniSpeed = 80.f;
	return S_OK;
}

void CPlayer::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	KeyCheck();
	AnimationUpdate();
	//m_pMesh->Progress();
	Engine::CGameObject::Update();
	Compute_ViewZ(&m_pInfo->m_vPos);
}

void CPlayer::Render(void)
{
	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_PLAYER);
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Player");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_pMesh->SetDevice(m_pDevice);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	//texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Hair");
	m_pTexture[0] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Head");
	m_pTexture[1] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Body");
	m_pTexture[2] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Guntle");
	m_pTexture[3] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Hand");
	m_pTexture[4] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Leg");
	m_pTexture[5] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Player_Boots");
	m_pTexture[6] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer*		pGameObject = new CPlayer(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}


void CPlayer::AnimationUpdate(void)
{
	SetAniTime();

	float	fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	if(m_pMesh->GetPeriod() - m_fAniTime <= m_pMesh->GetTrackPos())
	{
		if(m_bState[STATE_ATT_1])
		{
			m_eStateType = STATE_STAND;
		
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			for(int i = 0; i < STATE_END; ++i)
				m_bState[i] = false;

			m_bState[STATE_ATT_1] = false;
			m_bState[STATE_STAND] = true;
			m_pInfo->m_bAttackCasting = false;
		}
		else if(m_bState[STATE_ATT_2])
		{
			m_eStateType = STATE_STAND;

			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);


			m_bState[STATE_ATT_2] = false;
			m_bState[STATE_STAND] = true;
			m_pInfo->m_bAttackCasting = false;
		}
		else if(m_bState[STATE_ATT_3])
		{
			m_eStateType = STATE_STAND;

			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);


			m_bState[STATE_ATT_3] = false;
			m_bState[STATE_STAND] = true;
			m_pInfo->m_bAttackCasting = false;
		}
		else if(m_bState[STATE_ATT_4])
		{
			m_eStateType = STATE_STAND;

			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_ATT_4] = false;
			m_bState[STATE_STAND] = true;
			m_pInfo->m_bAttackCasting = false;
		}
		else if(m_bState[STATE_KNOCKBACK_1])
		{
			m_eStateType = STATE_GETUP;
			m_pInfo->m_bKnockBackHit = false;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			for(int i = 0; i < STATE_END; ++i)
				m_bState[i] = false;

			m_bGetUp = true;
			m_bState[STATE_KNOCKBACK_1] = false;
			m_bState[STATE_GETUP] = true;//
		}
		else if(m_bState[STATE_DOWN])
		{
			m_eStateType = STATE_GETUP;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_DOWN] = false;
			m_bState[STATE_GETUP] = true;
		}
		else if(m_bState[STATE_GETUP])
		{
			m_eStateType = STATE_STAND;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_GETUP] = false;
			m_bState[STATE_STAND] = true;
			m_bGetUp = false;
		}
		else if(m_bState[STATE_SKILL_HARF])
		{
			m_eStateType = STATE_STAND;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_SKILL_HARF] = false;
			m_bState[STATE_STAND] = true;
			m_bSkillHarf = false;
			m_pInfo->m_fHarfMoonCreateTime = 0.f;
			m_pInfo->m_bCreateHarfMoon = false;
			m_pInfo->m_bCreateHarfCasting = false;
			m_fAniSpeed = 80.f;
			
			const Engine::CComponent* pTrailComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
			Engine::CTransform* pTrailInfo = ((Engine::CTransform*)pTrailComponent);
			pTrailInfo->m_bSkillRender = false;
		}
		else if(m_bState[STATE_SKILL_CRESENT])
		{
			m_eStateType = STATE_STAND;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_SKILL_CRESENT] = false;
			m_bState[STATE_STAND] = true;
			m_bSkillCresent = false;
			m_pInfo->m_fCresentCreateTime = 0.f;
			m_pInfo->m_bCreateCresent = false;
			m_pInfo->m_bCreateCresentCasting = false;

			const Engine::CComponent* pTrailComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
			Engine::CTransform* pTrailInfo = ((Engine::CTransform*)pTrailComponent);
			pTrailInfo->m_bSkillRender = false;
		}
		else if(m_bState[STATE_SKILL_GREAT])
		{
			m_eStateType = STATE_STAND;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_SKILL_GREAT] = false;
			m_bState[STATE_STAND] = true;
			m_bSkillGreat = false;
			m_pInfo->m_fGreatCreateTime = 0.f;
			m_pInfo->m_bCreateGreat= false;
			m_pInfo->m_bCreateGreatCasting = false;

			const Engine::CComponent* pTrailComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
			Engine::CTransform* pTrailInfo = ((Engine::CTransform*)pTrailComponent);
			pTrailInfo->m_bSkillRender = false;
		}
		else
		{
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);
		}
		m_pInfo->m_vKnockBackDir = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_pInfo->m_fAniTime = 0.f;
	}
	else
	{
		if(m_bState[STATE_ATT_1])
		{
			if(m_pInfo->m_bPlayerAttRange)
			{
				if(m_pInfo->m_fAniTime > 0.2f)
				{
					CCollisionMgr::GetInstance()->Collision_PlayerAttack_Normal();
				}
			}
			if(!m_pInfo->m_bAttackSight)
			{
				if(m_pMesh->GetPeriod() - 1.2 <= m_pMesh->GetTrackPos())
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 10.f * fTime * -1), m_dwIndex);
				}
				else
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 20.f * fTime), m_dwIndex);
				}
			}
		}
		else if(m_bState[STATE_ATT_2])
		{
			if(m_pInfo->m_bPlayerAttRange)
			{
				if(m_pInfo->m_fAniTime > 0.4f)
				{
					CCollisionMgr::GetInstance()->Collision_PlayerAttack_Normal();
				}
			}
			if(!m_pInfo->m_bAttackSight)

			{
				if(m_pMesh->GetPeriod() - 1.5 > m_pMesh->GetTrackPos() &&
					m_pMesh->GetPeriod() - 2.0 <= m_pMesh->GetTrackPos())
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 50.f * fTime), m_dwIndex);
				}
				else
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 10.f * fTime), m_dwIndex);
				}
			}
		}
		else if(m_bState[STATE_ATT_3])
		{
			if(m_pInfo->m_bPlayerAttRange)
			{
				if(m_pInfo->m_fAniTime > 0.4f)
				{
					CCollisionMgr::GetInstance()->Collision_PlayerAttack_Normal();
				}
			}
			if(!m_pInfo->m_bAttackSight)
			{
				if(m_pMesh->GetPeriod() - 2.0 > m_pMesh->GetTrackPos() &&
					m_pMesh->GetPeriod() - 3.0 <= m_pMesh->GetTrackPos())
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 50.f * fTime), m_dwIndex);
				}
				else
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 10.f * fTime), m_dwIndex);
				}
			}
		}
		else if(m_bState[STATE_ATT_4])
		{
			if(m_pInfo->m_bPlayerAttRange)
			{
				if(m_pInfo->m_fAniTime > 0.6f)
				{
					CCollisionMgr::GetInstance()->Collision_PlayerAttack_KnockBack();
				}
			}
			if(!m_pInfo->m_bAttackSight)
			{
				if(m_pMesh->GetPeriod() - 1.0 > m_pMesh->GetTrackPos())
				{
					m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
						, &(m_pInfo->m_vDir * 50.f * fTime), m_dwIndex);
				}
			}
		}
		else if(m_bState[STATE_KNOCKBACK_1])
		{
			if(m_pMesh->GetPeriod() - 0.6 > m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vKnockBackDir * 400.f * fTime), m_dwIndex);

				m_pInfo->m_iAttackCount = 0;
			}
		}
		else if(m_bState[STATE_DOWN])
		{
			m_bGetUp = true;
		}
		else if(m_bState[STATE_SKILL_HARF])
		{
			m_pInfo->m_fHarfMoonCreateTime += fTime;
			

			if(m_pMesh->GetPeriod() - 7.0 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 8.0 <= m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * -1 * 40.f * fTime), m_dwIndex);
			}
			if(m_pMesh->GetPeriod() - 4.0 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 5.0 <= m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * 100 * fTime), m_dwIndex);
				//m_fAniSpeed *= 0.05f;
			}
		}
		else if(m_bState[STATE_SKILL_CRESENT])
		{
			m_pInfo->m_fCresentCreateTime += fTime;

			if(m_pMesh->GetPeriod() - 7.0 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 8.0 <= m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * -1 * 40.f * fTime), m_dwIndex);
			}
			if(m_pMesh->GetPeriod() - 4.0 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 5.0 <= m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * 100 * fTime), m_dwIndex);
				//m_fAniSpeed *= 0.05f;
			}
		}
		else if(m_bState[STATE_SKILL_GREAT])
		{
			m_pInfo->m_fGreatCreateTime += fTime;

			if(m_pMesh->GetPeriod() - 10.f > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 11.f <= m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * 50 * fTime), m_dwIndex);
				//m_fAniSpeed *= 0.05f;
			}

			if(m_pMesh->GetPeriod() - 1.f > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 2.f <= m_pMesh->GetTrackPos())
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * 50 * -1 * fTime), m_dwIndex);
				//m_fAniSpeed *= 0.05f;
			}
		}
		else
			m_pInfo->m_iAttackCount = 0;


	}

	if(m_pInfo->m_bKnockBackHit && m_bState[STATE_KNOCKBACK_1] == false)
	{
		CSoundMgr::GetInstance()->PlayerKnockbackSound(L"PlayerKnockBack.wav");
		m_pInfo->m_bKnockBackHit = false;

		m_eStateType = STATE_KNOCKBACK_1;

		for(int i = 0; i < STATE_END; ++i)
			m_bState[i] = false;
		m_bState[STATE_KNOCKBACK_1] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);
	}

	if(m_bSkillHarf && m_bState[STATE_SKILL_HARF] == false)
	{
		m_eStateType = STATE_SKILL_HARF;

		for(int i = 0; i < STATE_END; ++i)
			m_bState[i] = false;
		m_bState[STATE_SKILL_HARF] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);
	}
	if(m_bSkillCresent && m_bState[STATE_SKILL_CRESENT] == false)
	{
		m_eStateType = STATE_SKILL_CRESENT;

		for(int i = 0; i < STATE_END; ++i)
			m_bState[i] = false;
		m_bState[STATE_SKILL_CRESENT] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);
	}
	if(m_bSkillGreat && m_bState[STATE_SKILL_GREAT] == false)
	{
		m_eStateType = STATE_SKILL_GREAT;

		for(int i = 0; i < STATE_END; ++i)
			m_bState[i] = false;
		m_bState[STATE_SKILL_GREAT] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);
	}
}

void CPlayer::KeyCheck(void)
{
	Engine::CInput*		pInput = Engine::Get_Input();
	float	fTime = Engine::Get_TimeMgr()->GetTime();


	///////////// 스킬 //////////////////

	if(pInput->GetDIKeyState(DIK_1) & 0x80)
	{
		if(m_bSkillHarf 
			|| m_bSkillCresent
			|| m_bSkillGreat)
			return;
		m_bSkillHarf = true;
		m_pInfo->m_fHarfMoonCreateTime = 0.f;
		m_pInfo->m_bCreateHarfMoon = true;
		m_pInfo->m_bCreateHarfCasting = true;
		CSoundMgr::GetInstance()->PlayerHarfSound(L"Half1.wav");
		CSoundMgr::GetInstance()->PlayerGreatSound(L"voice1.wav");
		/// 하프문
		const Engine::CComponent* pTrailComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
		Engine::CTransform* pTrailInfo = ((Engine::CTransform*)pTrailComponent);
		pTrailInfo->m_bSkillRender = true;
		pTrailInfo->m_eColorType = Engine::COLOR_BLUE;
	}
	if(pInput->GetDIKeyState(DIK_2) & 0x80)
	{
		if(m_bSkillHarf 
			|| m_bSkillCresent
			|| m_bSkillGreat)
			return;
		m_bSkillCresent = true;
		m_pInfo->m_fCresentCreateTime = 0.f;
		m_pInfo->m_bCreateCresent = true;
		m_pInfo->m_bCreateCresentCasting = true;
		CSoundMgr::GetInstance()->PlayerCresentSound(L"Cresent1.wav");
		CSoundMgr::GetInstance()->PlayerGreatSound(L"voice2.wav");

		const Engine::CComponent* pTrailComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
		Engine::CTransform* pTrailInfo = ((Engine::CTransform*)pTrailComponent);
		pTrailInfo->m_bSkillRender = true;
		pTrailInfo->m_eColorType = Engine::COLOR_RED;
		/// 크레센트
	}
	if(pInput->GetDIKeyState(DIK_3) & 0x80)
	{
		if(m_bSkillHarf 
			|| m_bSkillCresent
			|| m_bSkillGreat)
			return;
		m_bSkillGreat = true;
		m_pInfo->m_fGreatCreateTime = 0.f;
		m_pInfo->m_bCreateGreat = true;
		m_pInfo->m_bCreateGreatCasting = true;
		CSoundMgr::GetInstance()->PlayerGreatSound(L"Half1.wav");
		CSoundMgr::GetInstance()->PlayerCresentSound(L"voice3.wav");

		const Engine::CComponent* pTrailComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"TrailPart", L"Transform");
		Engine::CTransform* pTrailInfo = ((Engine::CTransform*)pTrailComponent);
		pTrailInfo->m_bSkillRender = true;
		pTrailInfo->m_eColorType = Engine::COLOR_PURPLE;
		/// 궁
	}

	if(m_bSkillHarf 
		|| m_bSkillCresent
		|| m_bSkillGreat)
		return;
	///////////// 공격 //////////////////
	if(pInput->GetDIMouseState(pInput->DIM_LBUTTON) & 0x80)
	{
		if(m_bAttackCurver)
			return;
		m_bAttackCurver = true;

		if(m_bGetUp)
		{
			m_eStateType = STATE_GETUP;
			m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
			m_pMesh->SetTrackPos(0.0);

			m_bState[STATE_DOWN] = false;
			m_bState[STATE_GETUP] = true;

			m_bGetUp = false;
			return;
		}
		switch(m_pInfo->m_iAttackCount)
		{
		case 0:
			for(int i = 0; i < STATE_END; ++i)
				m_bState[i] = false;
			m_bState[STATE_ATT_1] = true;
			m_eStateType = STATE_ATT_1;
			m_pInfo->m_iAttackCount = 1;
			CSoundMgr::GetInstance()->PlayerAttackSound(L"Attack.wav");
			break;

		case 1:
			for(int i = 0; i < STATE_END; ++i)
				m_bState[i] = false;
			m_bState[STATE_ATT_2] = true;
			m_eStateType = STATE_ATT_2;
			m_pInfo->m_iAttackCount = 2;
			CSoundMgr::GetInstance()->PlayerAttackSound(L"Attack3.wav");
			break;

		case 2:
			for(int i = 0; i < STATE_END; ++i)
				m_bState[i] = false;
			m_bState[STATE_ATT_3] = true;
			m_eStateType = STATE_ATT_3;
			m_pInfo->m_iAttackCount = 3;
			CSoundMgr::GetInstance()->PlayerAttackSound(L"Attack.wav");
			break;

		case 3:
			for(int i = 0; i < STATE_END; ++i)
				m_bState[i] = false;
			m_bState[STATE_ATT_4] = true;
			m_eStateType = STATE_ATT_4;
			m_pInfo->m_iAttackCount = 0;
			CSoundMgr::GetInstance()->PlayerAttackSound(L"Attack2.wav");
			break;
		}
		
		m_pInfo->m_fAniTime = 0.f;
		m_pInfo->m_bAttackCasting = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		return;
	}
	else
		m_bAttackCurver = false;

	///////////////// 이동 ///////////////////

	if(m_eStateType == STATE_DOWN)
	{
		if((pInput->GetDIKeyState(DIK_W) & 0x80)
			|| (pInput->GetDIKeyState(DIK_A) & 0x80)
			|| (pInput->GetDIKeyState(DIK_S) & 0x80)
			|| (pInput->GetDIKeyState(DIK_D) & 0x80))
		{
			if(m_bDirMoveCurver[0] == true)
				return;
			m_bDirMoveCurver[0] = true;
			if(m_bGetUp && m_bState[STATE_DOWN])
			{
				m_eStateType = STATE_GETUP;
				m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
				m_pMesh->SetTrackPos(0.0);

				m_bState[STATE_DOWN] = false;
				m_bState[STATE_GETUP] = true;

				m_bGetUp = false;
			}
		}
		else
			m_bDirMoveCurver[0] = false;
	}
	else if(m_eStateType != STATE_KNOCKBACK_1 && m_eStateType != STATE_GETUP)
	{
		if(pInput->GetDIKeyState(DIK_W) & 0x80)
		{
			if(m_eStateType == STATE_RUN_LEFT)
			{
			}
			else if(m_eStateType == STATE_RUN_RIGHT)
			{

			}
			else
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * m_pInfo->m_fSpeed * fTime), m_dwIndex);

				//m_pInfo->m_vPos += m_pInfo->m_vDir * m_pInfo->m_fSpeed * fTime;

				m_eStateType = STATE_RUN_FRONT;
				m_pInfo->m_bPlayerFrontCheck = true;
				if(m_bState[STATE_RUN_FRONT])
					return;
				for(int i = 0; i < STATE_END; ++i)
					m_bState[i] = false;

				m_bState[STATE_RUN_FRONT] = true;
				m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
				m_pMesh->SetTrackPos(0.0);

			}
		}
		else if(m_bState[STATE_RUN_FRONT])
		{
			if(m_eStateType != STATE_RUN_FRONT)
			{
				m_bState[STATE_RUN_FRONT] = false;
				m_pInfo->m_bPlayerFrontCheck = false;
				if(m_bState[STATE_STAND] == false)
				{
					m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
					m_pMesh->SetTrackPos(0.0);
				}
			}
		}
		else
			m_eStateType = STATE_STAND;


		if(pInput->GetDIKeyState(DIK_S) & 0x80)
		{
			if(m_eStateType == STATE_RUN_LEFT)
			{
			}
			else if(m_eStateType == STATE_RUN_RIGHT)
			{

			}
			else
			{
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(m_pInfo->m_vDir * -m_pInfo->m_fSpeed * fTime), m_dwIndex);

				//m_pInfo->m_vPos += m_pInfo->m_vDir * -m_pInfo->m_fSpeed * fTime;

				m_eStateType = STATE_RUN_BACK;
				m_pInfo->m_bPlayerBackCheck = true;

				if(m_bState[STATE_RUN_BACK])
					return;
				for(int i = 0; i < STATE_END; ++i)
					m_bState[i] = false;
				m_bState[STATE_RUN_BACK] = true;
				m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
				m_pMesh->SetTrackPos(0.0);
			}
		}
		else if(m_bState[STATE_RUN_BACK])
		{
			if(m_eStateType != STATE_RUN_BACK)
			{
				m_bState[STATE_RUN_BACK] = false;
				m_pInfo->m_bPlayerBackCheck = false;

				if(m_bState[STATE_STAND] == false)
				{
					m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
					m_pMesh->SetTrackPos(0.0);
				}
			}
		}
		else
			m_eStateType = STATE_STAND;

		if(pInput->GetDIKeyState(DIK_A) & 0x80)
		{
			if(m_eStateType == STATE_RUN_BACK)
			{
			}
			else
			{
				D3DXVECTOR3 vRight;
				D3DXVec3Cross(&vRight, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
				
				D3DXVec3Normalize(&vRight, &vRight);

				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(vRight * m_pInfo->m_fSpeed * fTime), m_dwIndex);
				//m_pInfo->m_vPos += vRight * m_pInfo->m_fSpeed * fTime;
				
				m_eStateType = STATE_RUN_LEFT;
				m_pInfo->m_bPlayerLeftCheck = true;

				if(m_bState[STATE_RUN_LEFT])
					return;
				for(int i = 0; i < STATE_END; ++i)
					m_bState[i] = false;
				m_bState[STATE_RUN_LEFT] = true;
				m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
				m_pMesh->SetTrackPos(0.0);
			}
		}
		else if(m_bState[STATE_RUN_LEFT])
		{
			if(m_eStateType != STATE_RUN_LEFT)
			{
				m_bState[STATE_RUN_LEFT] = false;
				m_pInfo->m_bPlayerLeftCheck = false;
				if(m_bState[STATE_STAND] == false)
				{
					m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
					m_pMesh->SetTrackPos(0.0);
				}
			}
		}
		else
			m_eStateType = STATE_STAND;

		if(pInput->GetDIKeyState(DIK_D) & 0x80)
		{
			if(pInput->GetDIKeyState(DIK_S) & 0x80)
			{
			}
			else
			{
				D3DXVECTOR3 vRight;
				D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

				D3DXVec3Normalize(&vRight, &vRight);
			
				m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
					, &(vRight * m_pInfo->m_fSpeed * fTime), m_dwIndex);

				//m_pInfo->m_vPos += vRight * m_pInfo->m_fSpeed * fTime;
				
				m_eStateType = STATE_RUN_RIGHT;
				m_pInfo->m_bPlayerRightCheck = true;

				if(m_bState[STATE_RUN_RIGHT])
					return;
				for(int i = 0; i < STATE_END; ++i)
					m_bState[i] = false;
				m_bState[STATE_RUN_RIGHT] = true;
				m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
				m_pMesh->SetTrackPos(0.0);
			}
		}
		else if(m_bState[STATE_RUN_RIGHT])
		{
			if(m_eStateType != STATE_RUN_RIGHT)
			{
				m_bState[STATE_RUN_RIGHT] = false;
				m_pInfo->m_bPlayerRightCheck = false;
				if(m_bState[STATE_STAND] == false)
				{
					m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
					m_pMesh->SetTrackPos(0.0);
				}
			}
		}
		else
			m_eStateType = STATE_STAND;
		
	}
		
	
	
	
}

HRESULT CPlayer::SetConstantTable(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fDetail", m_fDetail);
	pEffect->SetInt("g_iHit", 0);

	//Light
	const D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo(1);
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}

void CPlayer::SetAniTime(void)
{
	switch(m_eStateType)
	{
	case STATE_STAND:
		m_fAniTime = 0.5f;
		break;
	case STATE_RUN_FRONT:
		m_fAniTime = 0.2f;
		break;
	case STATE_RUN_BACK:
		m_fAniTime = 0.2f;
		break;
	case STATE_RUN_LEFT:
		m_fAniTime = 0.2f;
		break;
	case STATE_ATT_1:
		m_fAniTime = 0.3f;
		break;
	case STATE_ATT_2:
		m_fAniTime = 0.3f;
		break;
	case STATE_ATT_3:
		m_fAniTime = 0.3f;
		break;
	case STATE_ATT_4:
		m_fAniTime = 0.3f;
		break;
	case STATE_KNOCKBACK_1:
		m_fAniTime = 0.3f;
		break;
	case STATE_DOWN:
		m_bGetUp = true;
		m_fAniTime = 0.3f;
		break;
	case STATE_GETUP:
		m_fAniTime = 0.3f;
		break;
	case STATE_SKILL_HARF:
		m_fAniTime = 0.3f;
		break;/*
	case STATE_SKILL_CRESENT:
		m_fAniTime = 0.3f;
		break;
	case STATE_SKILL_GREAT:
		m_fAniTime = 0.3f;
		break;*/
	}
}

