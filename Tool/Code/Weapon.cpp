#include "stdafx.h"
#include "Weapon.h"

#include "Tool_Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "ToolScene.h"
#include "MainApp.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_pmatPlayerInfo(NULL)
, m_pmatWeaponRef(NULL)
{

}

CWeapon::~CWeapon(void)
{

}

HRESULT CWeapon::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(10.f, 10.f, 10.f);
	return S_OK;
}

void CWeapon::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		const Engine::CComponent*		pPlayerInfo = Engine::Get_Management()->GetComponent(CToolScene::LAYER_GAMELOGIC, L"Player", L"Transform");
		const Engine::CComponent*		pPlayerMesh = Engine::Get_Management()->GetComponent(CToolScene::LAYER_GAMELOGIC, L"Player", L"DynamicMesh");

		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(pPlayerInfo)->m_matWorld);
		m_pmatWeaponRef = ((Engine::CDynamicMesh*)pPlayerMesh)->FindFrame("Bip01_R_Hand");
	}
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(90.f);
	m_pInfo->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(90.f);

	Engine::CGameObject::Update();
	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);
}

void CWeapon::Render(void)
{
	/*m_pMesh->RenderMesh_ForSDK(&m_pInfo->m_matWorld);*/
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
	return S_OK;
}

CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CWeapon*		pGameObject = new CWeapon(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}