#include "stdafx.h"
#include "Obtain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Stage.h"

#include "CollisionMgr.h"


CObtain::CObtain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_iCollision(0)
{
	//ZeroMemory(&m_pMesh, sizeof(Engine::CStaticMesh) * 3);
}

CObtain::~CObtain(void)
{

}

HRESULT CObtain::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_fDetail = 1.f;
	m_pCollisionMgr = CCollisionMgr::GetInstance();

	return S_OK;
}


void CObtain::Update(void)
{
	Engine::CGameObject::Update();

	for(int i = 0; i < 12; ++i)
	{
		if(i == 7)
			continue;
		LPD3DXMESH* pMesh = m_pMesh[i]->GetMesh();
		m_pCollisionMgr->Collision_Camera(pMesh, &m_pInfo->m_matWorld);
	}
	Compute_ViewZ(&m_pInfo->m_vPos);
}

void CObtain::Render(void)
{
	HRESULT		hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();

	NULL_CHECK_RETURN(pEffect, );

	for(int i = 0; i < 12; ++i)
		m_pMesh[i]->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld, Engine::OBJ_END);
}

HRESULT CObtain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain1");
	m_pMesh[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain2");
	m_pMesh[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain3");
	m_pMesh[2] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain4");
	m_pMesh[3] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain5");
	m_pMesh[4] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain6");
	m_pMesh[5] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain7");
	m_pMesh[6] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain8");
	m_pMesh[7] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain9");
	m_pMesh[8] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain10");
	m_pMesh[9] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain11");
	m_pMesh[10] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Obtain12");
	m_pMesh[11] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CObtain* CObtain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CObtain*		pGameObject = new CObtain(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CObtain* CObtain::Create(LPDIRECT3DDEVICE9 pDevice , D3DXVECTOR3 _vPos, float _fAngle, OBTAINTYPE _eType)
{
	CObtain*		pGameObject = new CObtain(pDevice);

	pGameObject->SetType(_eType);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CObtain::SetConstantTable(void)
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

void CObtain::SetInfo(D3DXVECTOR3 _vPos, float	_fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);

	if(m_eType == OBTAIN_WALL1)
		m_pInfo->m_vPos.y = -3.f;

}

void CObtain::SetType(OBTAINTYPE _eType)
{
	m_eType = _eType;
}

