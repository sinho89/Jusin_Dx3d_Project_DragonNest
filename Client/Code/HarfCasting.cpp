#include "stdafx.h"
#include "HarfCasting.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Stage.h"

#include "CollisionMgr.h"


CHarfCasting::CHarfCasting(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_iCollision(0)
, m_fScale(2.f)
, m_fAlpha(0.f)
{
	//ZeroMemory(&m_pMesh, sizeof(Engine::CStaticMesh) * 3);
}

CHarfCasting::~CHarfCasting(void)
{

}

HRESULT CHarfCasting::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_fDetail = 1.f;
	m_fAlpha = 0.1f;
	m_pInfo->m_vPos.y += 100.f;
	return S_OK;
}


void CHarfCasting::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);

	m_pInfo->m_vPos.x = pPlayerInfo->m_vPos.x;
	m_pInfo->m_vPos.z = pPlayerInfo->m_vPos.z;


	if(m_pInfo->m_bSkillRender)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(5.f);
		m_pInfo->m_vPos.y -= 1.f;
		m_pInfo->m_vScale = D3DXVECTOR3(m_fScale, m_fScale, m_fScale);
		m_fScale -= 0.01f;

		if(m_pInfo->m_vPos.y <= 0.f)
		{
			m_fScale = 2.f;
			m_pInfo->m_vPos.y += 100.f;
			m_pInfo->m_bSkillRender = false;
		}

		Engine::CGameObject::Update();

		Compute_ViewZ(&m_pInfo->m_vPos);
	}
}

void CHarfCasting::Render(void)
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

HRESULT CHarfCasting::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_HarfCasting");
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

CHarfCasting* CHarfCasting::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHarfCasting*		pGameObject = new CHarfCasting(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CHarfCasting* CHarfCasting::Create(LPDIRECT3DDEVICE9 pDevice , D3DXVECTOR3 _vPos, float _fAngle)
{
	CHarfCasting*		pGameObject = new CHarfCasting(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CHarfCasting::SetConstantTable(void)
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

void CHarfCasting::SetInfo(D3DXVECTOR3 _vPos, float	_fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);
}

void CHarfCasting::SetType(OBTAINTYPE _eType)
{
	m_eType = _eType;
}

