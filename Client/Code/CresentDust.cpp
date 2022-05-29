#include "stdafx.h"
#include "CresentDust.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Stage.h"

#include "CollisionMgr.h"


CCresentDust::CCresentDust(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_iCollision(0)
, m_fScale(1.f)
, m_fAlpha(0.f)
{
	//ZeroMemory(&m_pMesh, sizeof(Engine::CStaticMesh) * 3);
}

CCresentDust::~CCresentDust(void)
{

}

HRESULT CCresentDust::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_fDetail = 1.f;
	m_fAlpha = 0.1f;
	m_pInfo->m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	return S_OK;
}


void CCresentDust::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);

	if(m_pInfo->m_bSkillRender)
	{
		if(m_pInfo->m_fAniTime >= 3.f)
		{
			m_pInfo->m_bSkillRender = false;
		}

		m_pInfo->m_vScale = D3DXVECTOR3(m_fScale, m_fScale, m_fScale);
		if(m_fScale >= 1.f)
			m_bScale = true;
		else if(m_fScale <= 0.f)
			m_bScale = false;

		if(m_bScale)
			m_fScale -= 0.1f;
		else
			m_fScale += 0.1f;

		
		m_pInfo->m_vPos += m_pInfo->m_vDir * 500.f * fTime;
		Engine::CGameObject::Update();

		Compute_ViewZ(&m_pInfo->m_vPos);
	}
	else
	{
		m_pInfo->m_fAniTime = 0.f;
		m_fScale = 1.f;
	}
}

void CCresentDust::Render(void)
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

HRESULT CCresentDust::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_CresentDust");
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

CCresentDust* CCresentDust::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCresentDust*		pGameObject = new CCresentDust(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CCresentDust* CCresentDust::Create(LPDIRECT3DDEVICE9 pDevice , D3DXVECTOR3 _vPos, float _fAngle)
{
	CCresentDust*		pGameObject = new CCresentDust(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CCresentDust::SetConstantTable(void)
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

void CCresentDust::SetInfo(D3DXVECTOR3 _vPos, float	_fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);
}

void CCresentDust::SetType(OBTAINTYPE _eType)
{
	m_eType = _eType;
}

