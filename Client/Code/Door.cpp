#include "stdafx.h"
#include "Door.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "Stage.h"
#include "CollisionMgr.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pMesh(NULL)
, m_iAniIdx(0)
, m_bPush(false)
, m_dwIndex(0)
, m_fAniTime(0.f)
, m_bOpenReady(false)
, m_bCloseReady(false)
{
	for(int i = 0; i < DOOR_END; ++i)
		m_bState[i] = false;
}

CDoor::~CDoor(void)
{

}

HRESULT CDoor::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fDetail = 1.f;
	m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_pMesh->SetAnimationSet(m_iAniIdx, 0.1f, 0.25f);
	m_eStateType = DOOR_CLOSE;
	m_pInfo->m_bClose = true;
	m_bState[DOOR_CLOSE] = true;

	return S_OK;
}

void CDoor::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	AnimationUpdate();

	OpenReady();
	OpenDownGate();

	Engine::CGameObject::Update();

	Compute_ViewZ(&m_pInfo->m_vPos);
}

void CDoor::Render(void)
{
	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_END);
}

HRESULT CDoor::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Door");
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

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDoor*		pGameObject = new CDoor(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos, bool _Where)
{
	CDoor*		pGameObject = new CDoor(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _Where);
	return pGameObject;
}

HRESULT CDoor::SetConstantTable(void)
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

void CDoor::AnimationUpdate(void)
{
	SetAniTime();

	float	fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	if(m_pMesh->GetPeriod() - 0.4 <= m_pMesh->GetTrackPos())
	{
		if(m_eStateType == DOOR_OPENING)
		{
			m_eStateType = DOOR_OPEN;
			m_bState[DOOR_OPENING] = false;
			m_bState[DOOR_OPEN] = true;
		}
		else if(m_eStateType == DOOR_CLOSING)
		{
			m_eStateType = DOOR_CLOSE;
			m_bState[DOOR_CLOSING] = false;
			m_bState[DOOR_CLOSE] = true;
		}
		m_pInfo->m_fAniTime = 0.f;
		m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
		m_pMesh->SetTrackPos(0.0);
	}
	else
	{
		if(m_eStateType == DOOR_OPENING)
		{
			if(m_pMesh->GetPeriod() - 6.0 <= m_pMesh->GetTrackPos())
			{
				m_pInfo->m_bClose = false;
			}
		}
	}
}

void CDoor::SetAniTime(void)
{
	switch(m_eStateType)
	{
	case DOOR_CLOSE:
		m_fAniTime = 0.2f;
		break;
	case DOOR_CLOSING:
		m_fAniTime = 0.2f;
		break;
	case DOOR_OPEN:
		m_fAniTime = 0.2f;
		break;
	case DOOR_OPENING:
		m_fAniTime = 0.2f;
		break;
	}
}

void CDoor::SetInfo(D3DXVECTOR3 _vPos, bool _Where)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_bDownGate = _Where;
}

void CDoor::OpenReady()
{
	if(m_pInfo->m_bDownGate)
	{
		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		if(((Engine::CTransform*)pPlayerInfo)->m_iKillCount == 81)
		{
			++((Engine::CTransform*)pPlayerInfo)->m_iKillCount;
			m_bOpenReady = true;
		}

		if(((Engine::CTransform*)pPlayerInfo)->m_vPos.z < -2000)
		{
			m_eStateType = DOOR_CLOSE;
			m_bState[DOOR_OPEN] = false;
			m_bState[DOOR_CLOSE] = true;
			m_pInfo->m_fAniTime = 0.f;
			m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
			m_pMesh->SetTrackPos(0.0);
			m_pInfo->m_bTownCulling = true;
			m_pInfo->m_bBridgeCulling = false;
		}
	}
	else
	{
		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		if(((Engine::CTransform*)pPlayerInfo)->m_iKillCount == 82
			&& ((Engine::CTransform*)pPlayerInfo)->m_vPos.z < -3000)
		{
			++((Engine::CTransform*)pPlayerInfo)->m_iKillCount;
			m_bOpenReady = true;
		}
	}
}

void CDoor::OpenDownGate()
{
	if(m_pInfo->m_bDownGate)
	{
		bool bCheckOpen = CCollisionMgr::GetInstance()->Collision_PlayerToGate();

		if(bCheckOpen && m_bOpenReady)
		{
			m_eStateType = DOOR_OPENING;
			m_bState[DOOR_CLOSE] = false;
			m_bState[DOOR_OPENING] = true;
			m_bOpenReady = false;
			m_pInfo->m_fAniTime = 0.f;
			m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
			m_pMesh->SetTrackPos(0.0);
			m_pInfo->m_bBridgeCulling = false;
		}
	}
	else
	{
		if(m_bOpenReady)
		{
			m_eStateType = DOOR_OPENING;
			m_bState[DOOR_CLOSE] = false;
			m_bState[DOOR_OPENING] = true;
			m_bOpenReady = false;
			m_pInfo->m_fAniTime = 0.f;
			m_pMesh->SetAnimationSet(int(m_eStateType), 0.1f, 0.25f);
			m_pMesh->SetTrackPos(0.0);
		}
	}
}

