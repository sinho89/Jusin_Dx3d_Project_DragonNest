#include "stdafx.h"
#include "Goblin.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "Stage.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CGoblin::CGoblin(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_iAniIdx(0)
, m_bPush(false)
, m_dwIndex(0)
, m_fAniTime(0.f)
{
	for(int i = 0; i < GOBLIN_STATE_END; ++i)
		m_bState[i] = false;
}

CGoblin::~CGoblin(void)
{

}

HRESULT CGoblin::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fDetail = 1.f;
	m_pInfo->m_vScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	m_pInfo->m_fSpeed = 150.f;
	m_pMesh->SetAnimationSet(m_iAniIdx, 0.1f, 0.25f);
	m_eStateType = GOBLIN_STATE_STAND;
	m_bState[GOBLIN_STATE_STAND] = true;
	m_pInfo->m_iHp = 100;
	m_pInfo->m_iNormalAtt = 100;

	return S_OK;
}

void CGoblin::Update(void)
{
	if(!m_pInfo->m_bAlive)
		return;
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	AnimationUpdate();
	AttackSightColl();

	Engine::CGameObject::Update();

	Compute_ViewZ(&m_pInfo->m_vPos);
}

void CGoblin::Render(void)
{
	if(!m_pInfo->m_bAlive)
		return;
	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_GOBLIN);
}

HRESULT CGoblin::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Goblin");
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

CGoblin* CGoblin::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGoblin*		pGameObject = new CGoblin(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CGoblin* CGoblin::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, float _fAngle)
{
	CGoblin*		pGameObject = new CGoblin(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CGoblin::SetConstantTable(void)
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
	pEffect->SetFloat("g_fAlpha", 1.f);
	if(m_pInfo->m_bHit)
		pEffect->SetInt("g_iHit", 1);
	else
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

void CGoblin::AnimationUpdate(void)
{
	SetAniTime();

	float	fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	if(m_pMesh->GetPeriod() - m_fAniTime <= m_pMesh->GetTrackPos())
	{
		if(m_eStateType == GOBLIN_STATE_ATT)
		{
			m_pInfo->m_bNormalAtt = false;
			m_pInfo->m_bAttackCasting = false;

			m_eStateType = GOBLIN_STATE_FRONTWALK;
			for(int i = 0; i < GOBLIN_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[GOBLIN_STATE_FRONTWALK] = true;
		}
		if(m_bState[GOBLIN_STATE_HIT])
		{
			for(int i = 0; i < GOBLIN_STATE_END; ++i)
				m_bState[i] = false;
			m_eStateType = GOBLIN_STATE_STAND;
			m_bState[GOBLIN_STATE_STAND] = true;
			m_pInfo->m_bMove = true;
			m_pInfo->m_bNormalAtt = false;
		}
		if(m_bState[GOBLIN_STATE_KNOCKBACK])
		{
			for(int i = 0; i < GOBLIN_STATE_END; ++i)
				m_bState[i] = false;
			m_eStateType = GOBLIN_STATE_DOWN;
			m_bState[GOBLIN_STATE_DOWN] = true;
		}
		else if(m_bState[GOBLIN_STATE_DOWN])
		{
			if(m_pInfo->m_iHp <= 0)
			{
				//m_pInfo->m_bAlive = false;
				const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
				++((Engine::CTransform*)pPlayerInfo)->m_iKillCount;
				m_bAlive = false;
				return;
			}
			else
			{
				for(int i = 0; i < GOBLIN_STATE_END; ++i)
					m_bState[i] = false;
				m_eStateType = GOBLIN_STATE_GETUP;
				m_bState[GOBLIN_STATE_GETUP] = true;

			}
		}
		else if(m_bState[GOBLIN_STATE_GETUP])
		{
			for(int i = 0; i < GOBLIN_STATE_END; ++i)
				m_bState[i] = false;
			m_eStateType = GOBLIN_STATE_STAND;
			m_bState[GOBLIN_STATE_STAND] = true;
			m_pInfo->m_bMove = true;
			m_pInfo->m_bNormalAtt = false;
		}
		m_pInfo->m_fAniTime = 0.f;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
	}
	else
	{
		if(m_bState[GOBLIN_STATE_HIT])
		{
			if(m_pInfo->m_fAniTime < 0.6)
			{
				const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
				D3DXVECTOR3 vDir = m_pInfo->m_vPos - ((Engine::CTransform*)pPlayerInfo)->m_vPos;

				D3DXVec3Normalize(&vDir, &vDir);

				m_pInfo->m_vPos += vDir * 50.f * fTime;
			}
		}
		if(m_bState[GOBLIN_STATE_KNOCKBACK])
		{
			if(m_pInfo->m_fAniTime < 0.6)
			{
				const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
				D3DXVECTOR3 vDir = m_pInfo->m_vPos - ((Engine::CTransform*)pPlayerInfo)->m_vPos;

				D3DXVec3Normalize(&vDir, &vDir);

				m_pInfo->m_vPos += vDir * 450.f * fTime;
			}
		}
		
	}

	if(m_bState[GOBLIN_STATE_STAND] && m_pInfo->m_bSight)
	{
		m_bState[GOBLIN_STATE_STAND] = false;
		m_bState[GOBLIN_STATE_FRONTWALK] = true;
		m_eStateType = GOBLIN_STATE_FRONTWALK;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
	}

	if(m_eStateType == GOBLIN_STATE_FRONTWALK)
	{
		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

		D3DXVECTOR3 vTarget = ((Engine::CTransform*)pPlayerInfo)->m_vPos - m_pInfo->m_vPos;

		D3DXVec3Normalize(&vTarget, &vTarget);

		float fDestAngle = D3DXVec3Dot(&g_vLook, &vTarget);

		fDestAngle = acosf(fDestAngle);

		float fSourAngle = D3DXVec3Dot(&D3DXVECTOR3(1.f, 0.f, 0.f), &vTarget);

		if(fSourAngle >= 0)
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fDestAngle;
		else
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = fDestAngle;

		/*m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
		, &(  vTarget * m_pInfo->m_fSpeed * fTime), m_dwIndex);*/

		m_pInfo->m_vPos += vTarget * m_pInfo->m_fSpeed * fTime;
	}
	if(m_pInfo->m_bNormalHit)
	{
		m_pInfo->m_bNormalHit = false;

		m_pInfo->m_bHit = true;

		//CSoundMgr::GetInstance()->PlayerHitSound(L"Hit.wav");
		if(m_bState[GOBLIN_STATE_KNOCKBACK]
		|| m_bState[GOBLIN_STATE_DOWN]
		|| m_bState[GOBLIN_STATE_GETUP])
			return;

		m_eStateType = GOBLIN_STATE_HIT;

		for(int i = 0; i < GOBLIN_STATE_HIT; ++i)
			m_bState[i] = false;
		m_bState[GOBLIN_STATE_HIT] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);

		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		((Engine::CTransform*)pPlayerInfo)->m_bAttackCasting = false;
		m_pInfo->m_iHp -= ((Engine::CTransform*)pPlayerInfo)->m_iNormalAtt;
		m_pInfo->m_fAniTime = 0.f;
	}
	if(m_pInfo->m_bBounceHit)
	{
		m_pInfo->m_bBounceHit = false;

		m_pInfo->m_bHit = true;

		m_eStateType = GOBLIN_STATE_KNOCKBACK;

		for(int i = 0; i < GOBLIN_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[GOBLIN_STATE_KNOCKBACK] = true;

		m_pMesh->SetAnimationSet(int(m_eStateType),0.1f,0.25f);
		m_pMesh->SetTrackPos(0.0);

		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		((Engine::CTransform*)pPlayerInfo)->m_bAttackCasting = false;
		m_pInfo->m_iHp -= ((Engine::CTransform*)pPlayerInfo)->m_iBounceAtt;
		m_pInfo->m_fAniTime = 0.f;
	}

	if(m_pInfo->m_bHit)
	{
		m_pInfo->m_fHitTime += fTime;

		if(m_pInfo->m_fHitTime >= 0.2f)
		{
			m_pInfo->m_bHit = false;
			m_pInfo->m_fHitTime = 0.f;
		}
	}
	else
	{
		m_pInfo->m_fHitTime = 0.f;
	}

}

void CGoblin::AttackSightColl(void)
{
	if(m_pInfo->m_bAttackSight && m_bState[GOBLIN_STATE_FRONTWALK])
	{
		CSoundMgr::GetInstance()->GoblinAttackSound(L"GoblinAttack.wav");
		m_eStateType = GOBLIN_STATE_ATT;
		m_bState[GOBLIN_STATE_FRONTWALK] = false;
		m_bState[GOBLIN_STATE_ATT] = true;
		m_pInfo->m_fAniTime = 0.f;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
		m_pInfo->m_bAttackCasting = true;
		m_pInfo->m_bAttackSight = false;
	}
}

void CGoblin::SetAniTime(void)
{
	switch(m_eStateType)
	{
	case GOBLIN_STATE_STAND:
		m_fAniTime = 0.2f;
		break;
	}
}

void CGoblin::SetInfo(D3DXVECTOR3 _vPos, float _fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_vOriPos = _vPos;
	m_pInfo->m_fOriAngle = _fAngle;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);
}

void CGoblin::MoveSightColl(void)
{
	if(m_pInfo->m_bMove)
	{
		m_eStateType = GOBLIN_STATE_FRONTWALK;

		if(!m_bState[GOBLIN_STATE_FRONTWALK])
		{
			m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
			m_pMesh->SetTrackPos(0.0);
		}
		for(int i = 0; i < GOBLIN_STATE_END; ++i)
			m_bState[i] = false;
		m_bState[GOBLIN_STATE_FRONTWALK] = true;
	}
	else
	{
		if(m_pInfo->m_bNormalAtt)
		{
			if( m_bState[GOBLIN_STATE_HIT]
				|| m_bState[GOBLIN_STATE_KNOCKBACK]
				|| m_bState[GOBLIN_STATE_DOWN]
				|| m_bState[GOBLIN_STATE_GETUP])
					return;

			m_eStateType = GOBLIN_STATE_ATT;

			if(!m_bState[GOBLIN_STATE_ATT])
			{
				m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
				m_pMesh->SetTrackPos(0.0);
			}
			for(int i = 0; i < GOBLIN_STATE_END; ++i)
				m_bState[i] = false;

			m_bState[GOBLIN_STATE_ATT] = true;
			m_pInfo->m_bAttackCasting = true;
			// 공격
		}
		else
		{
			if(m_eStateType == GOBLIN_STATE_HIT)
				return;
			m_eStateType = GOBLIN_STATE_STAND;

			for(int i = 0; i < GOBLIN_STATE_END; ++i)
				m_bState[i] = false;
			m_bState[GOBLIN_STATE_STAND] = true;

			// 대기
		}
	}
}

