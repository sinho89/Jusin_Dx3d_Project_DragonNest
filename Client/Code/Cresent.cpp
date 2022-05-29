#include "stdafx.h"
#include "Cresent.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Stage.h"

#include "CollisionMgr.h"
#include "SoundMgr.h"


CCresent::CCresent(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_iCollision(0)
, m_fScale(1.f)
, m_fAlpha(0.f)
{
	//ZeroMemory(&m_pMesh, sizeof(Engine::CStaticMesh) * 3);
}

CCresent::~CCresent(void)
{

}

HRESULT CCresent::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_fDetail = 1.f;
	m_pCollisionMgr = CCollisionMgr::GetInstance();

	m_pInfo->m_vScale = D3DXVECTOR3 (8.f, 10.f, 5.f);
	return S_OK;
}


void CCresent::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	if(m_pInfo->m_bSkillRender)
	{
		Collision_OBB();

		Engine::CGameObject::Update();

		Compute_ViewZ(&m_pInfo->m_vPos);

		if(m_pInfo->m_fAniTime > 3.f)
			m_pInfo->m_bSkillRender = false;

		m_pInfo->m_vPos += m_pInfo->m_vDir * 500.f * fTime;
	}
	else
	{
		m_pInfo->m_fAniTime = 0;
	}
}

void CCresent::Render(void)
{
	if(m_pInfo->m_bSkillRender)
	{
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		HRESULT		hr = SetConstantTable();
		FAILED_CHECK_RETURN(hr, );

		LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();

		NULL_CHECK_RETURN(pEffect, );
		m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_SKILL);

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CCresent::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Cresent");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CCresent* CCresent::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCresent*		pGameObject = new CCresent(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CCresent* CCresent::Create(LPDIRECT3DDEVICE9 pDevice , D3DXVECTOR3 _vPos, float _fAngle)
{
	CCresent*		pGameObject = new CCresent(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CCresent::SetConstantTable(void)
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
	pEffect->SetFloat("g_fAlpha" , 0.5f);

	//Light
	D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}

void CCresent::SetInfo(D3DXVECTOR3 _vPos, float	_fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);
}

void CCresent::SetType(OBTAINTYPE _eType)
{
	m_eType = _eType;
}

void CCresent::Collision_OBB(void)
{

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		const Engine::CComponent*		pMonsterInfo =(*iter)->GetComponent(L"Transform");

		D3DXVECTOR3 MonsterPos = ((Engine::CTransform*)pMonsterInfo)->m_vPos;
		
		float fDistance = D3DXVec3Length(&(MonsterPos - (m_pInfo->m_vPos)));
		if(fDistance < 100)
		{
			if(!((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bSkillHit)
			{

				CSoundMgr::GetInstance()->PlayerHitSound(L"Hit.wav");
				((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bBounceHit = true;
				((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bSkillHit = true;
			}
		}

	}
}

