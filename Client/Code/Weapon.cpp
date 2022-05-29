#include "stdafx.h"
#include "Weapon.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Stage.h"
#include "Shader.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_pmatPlayerInfo(NULL)
, m_pmatWeaponRef(NULL)
, m_fDetail(1)
{

}

CWeapon::~CWeapon(void)
{

}

HRESULT CWeapon::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//m_pInfo->m_vScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	return S_OK;
}

void CWeapon::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		const Engine::CComponent*		pPlayerMesh = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"DynamicMesh");

		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(pPlayerInfo)->m_matWorld);
		m_pmatWeaponRef = ((Engine::CDynamicMesh*)pPlayerMesh)->FindFrame("_BoxBone01");
	}
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(90.f);
	m_pInfo->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(90.f);
 
	Engine::CGameObject::Update();
	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);
}

void CWeapon::Render(void)
{
	HRESULT		hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_END);
}

HRESULT CWeapon::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Weapon");
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

CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CWeapon*		pGameObject = new CWeapon(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

HRESULT CWeapon::SetConstantTable(void)
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
	D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}
