#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "SceneSelector.h"
#include "ToolScene.h"
#include "VIBuffer.h"

CMainApp::CMainApp(void)
: m_pGraphicDev(Engine::Get_GraphicDev())
, m_pManagement(Engine::Get_Management())
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
{

}

CMainApp::~CMainApp(void)
{
	Release();
}

HRESULT CMainApp::InitApp(void)
{
	HRESULT		hr = NULL;
	hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"디바이스 생성 실패");

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	hr = Engine::Get_Input()->InitInputDevice(g_hInst, g_hWnd);
	FAILED_CHECK(hr);

	hr = m_pManagement->InitManagement(m_pDevice);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Management Init Failed");

	hr = m_pResourceMgr->ReserveContainerSize(RESOURCE_END);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"리소스 컨테이너 할당 실패");

	m_pTimeMgr->InitTime();

	///////// 쉐이더 추가 ////////

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice
		, L"Shader_Default"
		, L"../../Reference/Headers/ShaderDefault.hpp");
	FAILED_CHECK(hr);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice
		, L"Shader_Terrain"
		, L"../../Reference/Headers/ShaderTerrain.hpp");
	FAILED_CHECK(hr);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice
		, L"Shader_Mesh"
		, L"../../Reference/Headers/ShaderMesh.hpp");
	FAILED_CHECK(hr);

	///////////// 메쉬추가 ////////////////////

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Terrain"
		, L"../bin/Resources/Mesh/StaticMesh/Terrain/"
		, L"Terrain.x");

	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_StaticObject"
		, L"../bin/Resources/Mesh/StaticMesh/StaticObject/"
		, L"StaticObject.x");

	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Player"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/"
		, L"Player.x");

	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Weapon"
		, L"../bin/Resources/Mesh/StaticMesh/"
		, L"Weapon.x");

	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pScene = CToolScene::Create(m_pDevice);

	return S_OK;
}

void CMainApp::Update(void)
{
	m_pTimeMgr->SetTime();

	m_pScene->Update();
	Engine::Get_Input()->SetInputState();
	
}

void CMainApp::Render(void)
{
	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
	m_pDevice->BeginScene();

	m_pScene->Render();

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}



void CMainApp::Release(void)
{
	//Utility
	Engine::Safe_Single_Destory(m_pManagement);
	/*Engine::Get_Frustum()->DestroyInstance();
	Engine::Get_QuadTree()->DestroyInstance();*/
	Engine::Get_NaviMgr()->DestroyInstance();

	//Resource
	Engine::Safe_Single_Destory(m_pResourceMgr);

	//System
	Engine::Safe_Single_Destory(m_pTimeMgr);
	Engine::Get_FontMgr()->DestroyInstance();
	Engine::Safe_Single_Destory(m_pGraphicDev);
}


CMainApp* CMainApp::Create(void)
{
	CMainApp*		pMainApp = new CMainApp;
	if(FAILED(pMainApp->InitApp()))
	{
		delete pMainApp;
		pMainApp = NULL;
	}
	return pMainApp;
}