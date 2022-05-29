#include "stdafx.h"
#include "Boss.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "Stage.h"
#include "CollisionMgr.h"
#include "Texture.h"
#include "Shader.h"
#include "SoundMgr.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_iAniIdx(0)
, m_bPush(false)
, m_dwIndex(0)
, m_fAniTime(0.f)
, m_fRageTime(0.f)
, m_fFlyTime(0.f)
, m_fBreathTime(0.f)
{
	for(int i = 0; i < BOSS_STATE_END; ++i)
		m_bState[i] = false;
}

CBoss::~CBoss(void)
{

}

HRESULT CBoss::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fDetail = 1.f;
	m_pInfo->m_fSpeed = 200.f;
	m_pInfo->m_vScale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
	m_pMesh->SetAnimationSet(m_iAniIdx, 0.1f, 0.25f);
	m_eStateType = BOSS_STATE_STAND;
	m_bState[BOSS_STATE_STAND] = true;
	m_pInfo->m_iHp = 100;
	m_pInfo->m_iBounceAtt = 500;
	m_fAniTime = 0.2f;

	return S_OK;
}

void CBoss::Update(void)
{
	
		if(!m_pInfo->m_bAlive)
			return;

		D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

		AnimationUpdate();
		AttackSightColl();

		//LookTarget();
		Turn();

		Engine::CGameObject::Update();

		Compute_ViewZ(&m_pInfo->m_vPos);

		m_pInfo->m_matBoneMouse = (*m_pMesh->FindFrame("Bone03")) * m_pInfo->m_matWorld;

	
}

void CBoss::Render(void)
{
	
		if(!m_pInfo->m_bAlive)
			return;

		if(m_pInfo->m_bSkillRender)
		{
			m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());

			HRESULT hr = SetConstantTable();
			FAILED_CHECK_RETURN(hr, );

			LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
			NULL_CHECK_RETURN(pEffect, );

			m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_BOSS);
		}
}

HRESULT CBoss::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Boss");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	return S_OK;
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss*		pGameObject = new CBoss(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, float _fAngle)
{
	CBoss*		pGameObject = new CBoss(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CBoss::SetConstantTable(void)
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

void CBoss::AnimationUpdate(void)
{
	if(m_pInfo->m_bBounceHit)
	{
		m_pInfo->m_bBounceHit = false;

		m_eStateType = BOSS_STATE_DYE_START;

		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_DYE_START] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);

		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		((Engine::CTransform*)pPlayerInfo)->m_bAttackCasting = false;
		m_pInfo->m_iHp -= ((Engine::CTransform*)pPlayerInfo)->m_iBounceAtt;
		m_pInfo->m_fAniTime = 0.f;
		return;
	}

	SetAniTime();

	float	fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;
	
	if(m_pInfo->m_bSkillRender)
	{
		m_fRageTime += fTime;
	}

	if(m_fRageTime >= 20.f)
	{
		if(!m_bState[BOSS_STATE_DYE_START]
		&& !m_bState[BOSS_STATE_DYE_LOOP])
		{
			RageSkill();

		}
	}
	if(m_pMesh->GetPeriod() - m_fAniTime <= m_pMesh->GetTrackPos())
	{
		if(m_bState[BOSS_STATE_DASH])
		{
			m_eStateType = BOSS_STATE_STAND;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_STAND] = true;
			m_pInfo->m_bBossDash = false;
			m_pInfo->m_bAttackCasting = false;
		}
		else if(m_bState[BOSS_STATE_BITE])
		{
			m_eStateType = BOSS_STATE_STAND;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_STAND] = true;
			m_pInfo->m_bBossBite = false;
			m_pInfo->m_bAttackCasting  =false;
		}
		else if(m_bState[BOSS_STATE_RIGHT_ATT])
		{
			m_eStateType = BOSS_STATE_STAND;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_STAND] = true;
			m_pInfo->m_bBossRightAtt = false;
			m_pInfo->m_bAttackCasting  =false;
		}
		else if(m_bState[BOSS_STATE_LEFT_ATT])
		{
			m_eStateType = BOSS_STATE_STAND;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_STAND] = true;
			m_pInfo->m_bBossLeftAtt = false;
			m_pInfo->m_bAttackCasting  =false;
		}
		else if(m_bState[BOSS_STATE_TURN_LEFT])
		{
			m_pInfo->m_bBossTurnLeft = false;
		}
		else if(m_bState[BOSS_STATE_TURN_RIGHT])
		{
			m_pInfo->m_bBossTurnRight = false;
		}
		else if(m_bState[BOSS_STATE_FLY_START])
		{
			m_eStateType = BOSS_STATE_FLY_LOOP;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_FLY_LOOP] = true;
			m_fFlyTime = 0;
			m_pInfo->m_bOpening = true;
		}
		else if(m_bState[BOSS_STATE_FLY_LOOP])
		{
			if(m_fFlyTime >= 2.0f)
			{
				CSoundMgr::GetInstance()->PlayerFireStartSound(L"FireStart.ogg");
				m_eStateType = BOSS_STATE_FIRE_START;
				for(int i = 0; i < BOSS_STATE_END; ++i)
					m_bState[i] = false;
				m_bState[BOSS_STATE_FIRE_START] = true;
				

				m_fFlyTime = 0.f;
			}
		}
		else if(m_bState[BOSS_STATE_FIRE_START])
		{

			CSoundMgr::GetInstance()->PlayerFireLoopSound(L"FireLoop.ogg");
			m_eStateType = BOSS_STATE_FIRE_LOOP;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_FIRE_LOOP] = true;

			const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"BossBreath", L"Transform");
			Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

			pBossInfo->m_bSkillRender = true;

			m_fBreathTime = 0.f;
		}
		else if(m_bState[BOSS_STATE_FIRE_LOOP])
		{
			if(m_fBreathTime >= 10.0f)
			{
				m_eStateType = BOSS_STATE_FIRE_END;
				for(int i = 0; i < BOSS_STATE_END; ++i)
					m_bState[i] = false;
				m_bState[BOSS_STATE_FIRE_END] = true;

				const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"BossBreath", L"Transform");
				Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

				pBossInfo->m_bSkillRender = false;
				m_fBreathTime = 0.f;
			}
		}
		else if(m_bState[BOSS_STATE_FIRE_END])
		{
			m_eStateType = BOSS_STATE_FLY_END;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_FLY_END] = true;
		}
		else if(m_bState[BOSS_STATE_FLY_END])
		{
			m_eStateType = BOSS_STATE_STAND;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_STAND] = true;
			m_pInfo->m_bBossFly = false; 
			m_fRageTime = 0.f;
		}
		else if(m_bState[BOSS_STATE_DYE_START])
		{
			m_eStateType = BOSS_STATE_DYE_LOOP;
			for(int i = 0; i < BOSS_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[BOSS_STATE_DYE_LOOP] = true;
			m_pInfo->m_bBossFly = false; 
			m_fRageTime = 0.f;
		}
		else if(m_bState[BOSS_STATE_DYE_LOOP])
		{
			m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
			m_pMesh->SetTrackPos(0.0);
			m_pInfo->m_fAniTime = 0.f;
		}
		
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_fAniTime = 0.f;
		
	}
	else
	{
		if(m_bState[BOSS_STATE_DASH])
		{
			if(m_pMesh->GetPeriod() - 5.0 > m_pMesh->GetTrackPos() &&
			m_pMesh->GetPeriod() - 6.0 <= m_pMesh->GetTrackPos())
			{
				m_pInfo->m_vPos += m_pInfo->m_vDir * 5000.f * fTime;
			}

			if(m_pInfo->m_bAttRange)
			{
				const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
				if(((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit == true)
					return;
				((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit = true;
				((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;
				D3DXVec3Normalize(&((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir, &((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir);
			}
		}
		if(m_bState[BOSS_STATE_BITE])
		{
			if(m_pMesh->GetPeriod() - 4.0 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 4.5 <= m_pMesh->GetTrackPos())
			{
				if(m_pInfo->m_bAttRange)
				{
					const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
					if(((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit == true)
						return;
					((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit = true;
					((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;
					D3DXVec3Normalize(&((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir, &((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir);
				}
			}
		}
		if(m_bState[BOSS_STATE_RIGHT_ATT])
		{
			if(m_pMesh->GetPeriod() - 2.5 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 3.0 <= m_pMesh->GetTrackPos())
			{
				if(m_pInfo->m_bAttRange)
				{
					const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
					if(((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit == true)
						return;
					((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit = true;
					((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;
					D3DXVec3Normalize(&((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir, &((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir);
				}
			}
		}
		if(m_bState[BOSS_STATE_LEFT_ATT])
		{
			if(m_pMesh->GetPeriod() - 2.5 > m_pMesh->GetTrackPos() &&
				m_pMesh->GetPeriod() - 3.0 <= m_pMesh->GetTrackPos())
			{
				if(m_pInfo->m_bAttRange)
				{
					const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
					if(((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit == true)
						return;
					((Engine::CTransform*)pPlayerInfo)->m_bKnockBackHit = true;
					((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;
					D3DXVec3Normalize(&((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir, &((Engine::CTransform*)pPlayerInfo)->m_vKnockBackDir);
				}
			}
		}

		if(m_bState[BOSS_STATE_TURN_RIGHT])
		{
			m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(0.3f);
		}
		if(m_bState[BOSS_STATE_TURN_LEFT])
		{
			m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(0.3f);
		}
		if(m_bState[BOSS_STATE_FLY_LOOP])
		{
			m_fFlyTime += fTime;
		}
		if(m_bState[BOSS_STATE_FIRE_START])
		{
			m_pInfo->m_fHarfMoonCreateTime += fTime;
			LookTarget();
		}
		if(m_bState[BOSS_STATE_FIRE_LOOP])
		{
			LookTarget();
			m_fBreathTime += fTime;
		}
		
	}

	if(m_pInfo->m_bBossBite && !m_bState[BOSS_STATE_BITE])
	{
		if(m_bState[BOSS_STATE_DYE_START]
		|| m_bState[BOSS_STATE_DYE_LOOP])
			return;

		CSoundMgr::GetInstance()->PlayerBiteSound(L"Bite.ogg");
		m_eStateType = BOSS_STATE_BITE;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_BITE] = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_bAttackCasting = true;
		m_pInfo->m_fAniTime = 0.f;
	}
	if(m_pInfo->m_bBossRightAtt && !m_bState[BOSS_STATE_RIGHT_ATT])
	{
		if(m_bState[BOSS_STATE_DYE_START]
		|| m_bState[BOSS_STATE_DYE_LOOP])
			return;

		CSoundMgr::GetInstance()->PlayerBiteSound(L"Bite.ogg");
		m_eStateType = BOSS_STATE_RIGHT_ATT;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_RIGHT_ATT] = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_bAttackCasting = true;
		m_pInfo->m_fAniTime = 0.f;
	}
	if(m_pInfo->m_bBossLeftAtt && !m_bState[BOSS_STATE_LEFT_ATT])
	{
		if(m_bState[BOSS_STATE_DYE_START]
		|| m_bState[BOSS_STATE_DYE_LOOP])
			return;

		CSoundMgr::GetInstance()->PlayerBiteSound(L"Bite.ogg");
		m_eStateType = BOSS_STATE_LEFT_ATT;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_LEFT_ATT] = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_bAttackCasting = true;
		m_pInfo->m_fAniTime = 0.f;
	}
	if(m_pInfo->m_bBossTurnRight && !m_bState[BOSS_STATE_TURN_RIGHT])
	{
		if(m_bState[BOSS_STATE_DYE_START]
		|| m_bState[BOSS_STATE_DYE_LOOP])
			return;
		m_eStateType = BOSS_STATE_TURN_RIGHT;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_TURN_RIGHT] = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_fAniTime = 0.f;
	}
	if(m_pInfo->m_bBossTurnLeft && !m_bState[BOSS_STATE_TURN_LEFT])
	{
		if(m_bState[BOSS_STATE_DYE_START]
		|| m_bState[BOSS_STATE_DYE_LOOP])
			return;
		m_eStateType = BOSS_STATE_TURN_LEFT;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_TURN_LEFT] = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_fAniTime = 0.f;
	}

	

	
}

void CBoss::AttackSightColl(void)
{
	if(m_pInfo->m_bBossDash)
	{
		if(m_bState[BOSS_STATE_DYE_START]
		|| m_bState[BOSS_STATE_DYE_LOOP])
			return;
		if(m_bState[BOSS_STATE_DASH])
			return;

		CSoundMgr::GetInstance()->PlayerBashSound(L"Bash.ogg");
		m_pInfo->m_fAniTime = 0.f;
		m_eStateType = BOSS_STATE_DASH;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_DASH] = true;
		m_pInfo->m_bAttackCasting = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		LookTarget();
	}
}

void CBoss::SetAniTime(void)
{
	switch(m_eStateType)
	{
	case BOSS_STATE_STAND:
		m_fAniTime = 0.2f;
		break;
	case BOSS_STATE_DASH:
		m_fAniTime = 0.2f;
		break;
	case BOSS_STATE_BITE:
		m_fAniTime = 0.2f;
		break;
	case BOSS_STATE_RIGHT_ATT:
		m_fAniTime = 0.2f;
		break;
	case BOSS_STATE_LEFT_ATT:
		m_fAniTime = 0.2f;
		break;
	case BOSS_STATE_FLY_START:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_FLY_LOOP:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_FLY_END:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_FIRE_START:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_FIRE_LOOP:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_FIRE_END:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_DYE_START:
		m_fAniTime = 0.5f;
		break;
	case BOSS_STATE_DYE_LOOP:
		m_fAniTime = 0.5f;
		break;
	}
}

void CBoss::SetInfo(D3DXVECTOR3 _vPos, float _fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);
}

void CBoss::LookTarget()
{/*
	if(m_bState[BOSS_STATE_DASH])
		return;*/

	const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vTarget = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;

	D3DXVec3Normalize(&vTarget, &vTarget);

	m_vTargetDir = vTarget;

	float fDestAngle = D3DXVec3Dot(&g_vLook, &vTarget);

	fDestAngle = acosf(fDestAngle);

	float fSourAngle = D3DXVec3Dot(&D3DXVECTOR3(1.f, 0.f, 0.f), &vTarget);

	if(fSourAngle >= 0)
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fDestAngle;
	else
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = fDestAngle;

	/*m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
		, &(vTarget * m_pInfo->m_fSpeed * fTime), m_dwIndex);*/
}

void CBoss::Turn()
{
	if(m_bState[BOSS_STATE_DASH]
	|| m_bState[BOSS_STATE_BITE]
	|| m_bState[BOSS_STATE_LEFT_ATT]
	|| m_bState[BOSS_STATE_RIGHT_ATT]
	|| m_bState[BOSS_STATE_FLY_START]
	|| m_bState[BOSS_STATE_FLY_LOOP]
	|| m_bState[BOSS_STATE_FLY_END]
	|| m_bState[BOSS_STATE_FIRE_START]
	|| m_bState[BOSS_STATE_FIRE_LOOP]
	|| m_bState[BOSS_STATE_FIRE_END])
		return;

	const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vTarget = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;

	D3DXVec3Normalize(&vTarget, &vTarget);

	m_vTargetDir = vTarget;

	float fDestAngle = D3DXVec3Dot(&g_vLook, &vTarget);

	fDestAngle = acosf(fDestAngle);

	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	D3DXVec3Normalize(&vRight, &vRight);

	float fSourAngle = D3DXVec3Dot(&vRight, &vTarget);

	if(fSourAngle < 0.1 && fSourAngle > -0.1)
	{
		if(m_bState[BOSS_STATE_STAND])
			return;
		m_eStateType = BOSS_STATE_STAND;
		for(int i = 0; i < BOSS_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[BOSS_STATE_STAND] = true;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);

		m_pInfo->m_bBossTurnLeft = false;
		m_pInfo->m_bBossTurnRight = false;
	}
	else
	{
		if(fSourAngle >= 0.8 && fSourAngle < 1.f)
		{
			m_pInfo->m_bBossTurnLeft = true;
		}
		else if(fSourAngle <= -0.8 && fSourAngle > -1.f)
		{
			m_pInfo->m_bBossTurnRight = true;
		}
	}
}

void CBoss::RageSkill()
{
	if(!m_bState[BOSS_STATE_STAND])
		return;

	m_fRageTime = 0;

	for(int i = 0; i < BOSS_STATE_END; ++i)
		m_bState[i] = false;
	m_eStateType = BOSS_STATE_FLY_START;
	m_bState[BOSS_STATE_FLY_START] = true;

	const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vTarget = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;

	D3DXVec3Normalize(&vTarget, &vTarget);

	m_vTargetDir = vTarget;

	float fDestAngle = D3DXVec3Dot(&g_vLook, &vTarget);

	fDestAngle = acosf(fDestAngle);

	float fSourAngle = D3DXVec3Dot(&D3DXVECTOR3(1.f, 0.f, 0.f), &vTarget);

	if(fSourAngle >= 0)
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fDestAngle;
	else
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = fDestAngle;

	m_pInfo->m_bBossFly = true;
	m_pInfo->m_bBossDash =false;
	m_pInfo->m_bBossBite =false;
	m_pInfo->m_bBossRightAtt =false;
	m_pInfo->m_bBossLeftAtt =false;
	m_pInfo->m_bBossTurnRight =false;
	m_pInfo->m_bBossTurnLeft =false;

	m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
	m_pMesh->SetTrackPos(0.0);
	m_pInfo->m_fAniTime = 0.f;
}

