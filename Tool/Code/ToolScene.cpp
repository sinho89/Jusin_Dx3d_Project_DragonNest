#include "stdafx.h"
#include "ToolScene.h"

#include "Tool_Include.h"
#include "Export_Function.h"
#include "Layer.h"
#include "DynamicCamera.h"
#include "Terrain.h"
#include "Player.h"
#include "Weapon.h"
#include "StaticObj.h"

CToolScene::CToolScene(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CToolScene::~CToolScene(void)
{
	Release();
}

HRESULT CToolScene::InitScene(void)
{
	HRESULT		hr = NULL;

	//FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	FAILED_CHECK_RETURN_MSG(Add_NaviMesh(), E_FAIL, L"Add_NaviMesh Add false");

	hr = Engine::Get_Frustum()->InitFrustum(m_pDevice);
	FAILED_CHECK(hr);

	return S_OK;
}

void CToolScene::Update(void)
{
	Engine::Get_Frustum()->Transform_Frustum_ToWorld();

	Engine::CScene::Update();
}

void CToolScene::Render(void)
{
	Engine::CScene::Render();
	Engine::Get_NaviMgr()->Render_NaviMesh();
}

HRESULT CToolScene::Add_Environment_Layer(void)
{

	return S_OK;
}

HRESULT CToolScene::Add_GameLogic_Layer(void)
{

	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Terrain", pGameObject);

	pGameObject = CStaticObj::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"StaticObject", pGameObject);

	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Player", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CToolScene::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CDynamicCamera::Create(m_pDevice, 
		&D3DXVECTOR3(0.f, 5.f, -20.f),
		&D3DXVECTOR3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"DynamicCamera", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));

	return S_OK;
}

void CToolScene::Release(void)
{

}

CToolScene* CToolScene::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CToolScene* pStage = new CToolScene(pDevice);

	if(FAILED(pStage->InitScene()))
	{
		Engine::Safe_Delete(pStage);
	}

	return pStage;
}


HRESULT CToolScene::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;

	LightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	LightInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(0.f, -1000.f, 0.f);

	HRESULT	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CToolScene::Add_NaviMesh(void)
{
	Engine::Get_NaviMgr()->Reserve_CellContainerSize(m_pDevice, 11);

	HRESULT		hr = NULL;
	D3DXVECTOR3		vPoint[3];

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, 200.f);
	vPoint[1] = D3DXVECTOR3(150.f, 0.f, 200.f);
	vPoint[2] = D3DXVECTOR3(150.f, 0.f, -250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, 200.f);
	vPoint[1] = D3DXVECTOR3(150.f, 0.f, -250.f);
	vPoint[2] = D3DXVECTOR3(-100.f, 0.f, -250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 0.f, 200.f);
	vPoint[1] = D3DXVECTOR3(-100.f, 0.f, 200.f);
	vPoint[2] = D3DXVECTOR3(-1450.f, 0.f, 1150.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, 200.f);
	vPoint[1] = D3DXVECTOR3(-1450.f, 0.f, 200.f);
	vPoint[2] = D3DXVECTOR3(-1450.f, 0.f, 1150.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 0.f, 200.f);
	vPoint[1] =D3DXVECTOR3(-1450.f, 0.f, 1150.f);
	vPoint[2] = D3DXVECTOR3(1450.f, 0.f, 1150.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 0.f, 200.f);
	vPoint[1] =D3DXVECTOR3(1450.f, 0.f, 1150.f);
	vPoint[2] = D3DXVECTOR3(1450.f, 0.f, 200.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, -250.f);
	vPoint[1] = D3DXVECTOR3(150.f, 0.f, -250.f);
	vPoint[2] = D3DXVECTOR3(1550.f, 0.f, -1650.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, -250.f);
	vPoint[1] = D3DXVECTOR3(150.f, 0.f, -250.f);
	vPoint[2] = D3DXVECTOR3(1550.f, 0.f, -1650.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(150.f, 0.f, -250.f);
	vPoint[1] = D3DXVECTOR3(1550.f, 0.f, -250.f);
	vPoint[2] = D3DXVECTOR3(1550.f, 0.f, -1650.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(1550.f, 0.f, -1650.f);
	vPoint[1] = D3DXVECTOR3(-1550.f, 0.f, -1650.f);
	vPoint[2] = D3DXVECTOR3(-100.f, 0.f, -250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, -250.f);
	vPoint[1] = D3DXVECTOR3(-1550.f, 0.f, -1650.f);
	vPoint[2] = D3DXVECTOR3(-1550.f, 0.f, -250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);


	
	/*
	vPoint[0] = D3DXVECTOR3(-1450.f, 0.f, 1150.f);
	vPoint[1] = D3DXVECTOR3(-1450.f, 0.f, 200.f);
	vPoint[2] = D3DXVECTOR3(1450.f, 0.f, 200.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);*/

	/*vPoint[0] = D3DXVECTOR3(-1450.f, 0.f, -250.f);
	vPoint[1] = D3DXVECTOR3(-1450.f, 0.f, -500.f);
	vPoint[2] = D3DXVECTOR3(1450.f, 0.f, -250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-100.f, 0.f, -250.f);
	vPoint[1] = D3DXVECTOR3(-100.f, 0.f, 200.f);
	vPoint[2] = D3DXVECTOR3(150.f, 0.f, -250.f);
	hr = Engine::Get_NaviMgr()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);
*/
	Engine::Get_NaviMgr()->Link_Cell();
	return S_OK;
}
