#include "stdafx.h"
#include "Terrain.h"

#include "Tool_Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_iCollision(0)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_fDetail = 4.f;

	return S_OK;
}

void CTerrain::Update(void)
{
	Engine::CGameObject::Update();

	Compute_ViewZ(&m_pInfo->m_vPos);
}

void CTerrain::Render(void)
{
	if(m_iCollision)
		m_pMesh->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	else
		m_pMesh->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 0, 255, 0));

	HRESULT		hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld);


}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Terrain");
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

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain*		pGameObject = new CTerrain(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}


HRESULT CTerrain::SetConstantTable(void)
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

