#include "stdafx.h"
#include "Logo.h"
#include "Layer.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"
#include "LogoBack.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pManagement(Engine::Get_Management())
, m_pLoading(NULL)
{

}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::InitScene(void)
{
	HRESULT		hr = NULL;

	hr = m_pResourceMgr->AddTexture(m_pDevice, RESOURCE_LOGO, Engine::TEXTURE_NORMAL
		, L"Texture_Logo"
		, L"../bin/Resources/Texture/LogoBack2.jpg", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

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

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice
		, L"Shader_Shadow"
		, L"../../Reference/Headers/ShaderShadow.hpp");
	FAILED_CHECK(hr);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice
		, L"Shader_Fire"
		, L"../../Reference/Headers/ShaderFire.hpp");
	FAILED_CHECK(hr);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice
		, L"Shader_Petal"
		, L"../../Reference/Headers/ShaderPetal.hpp");
	FAILED_CHECK(hr);

	// loading
	m_pLoading = CLoading::Create(CLoading::LOADID_STAGE);
	FAILED_CHECK_RETURN(m_pLoading, E_FAIL);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");

	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();

	if(m_pLoading->GetComplete() == true)
	{
		if(GetAsyncKeyState(VK_RETURN))
		{
			HRESULT hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, , L"씬 전환 실패");
			return;
		}
	}
	
}

void CLogo::Render(void)
{
	Engine::CScene::Render();

	Engine::Get_FontMgr()->Render(L"휴먼편지체", D3DXVECTOR3(150.f, WINCY - 200.f, 0.f), 
		m_pLoading->GetLoadingMessage(), D3DCOLOR_ARGB(255, 255, 255, 255));
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pLogo = new CLogo(pDevice);
	if(FAILED(pLogo->InitScene()))
		Engine::Safe_Delete(pLogo);

	return pLogo;
}

HRESULT CLogo::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CLogoBack::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CLogoBack 생성 실패");
	pLayer->AddObject(L"LogoBack" , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	
	
	return S_OK;
}

void CLogo::Release(void)
{
	Engine::Get_ResourceMgr()->ResourceReset(RESOURCE_LOGO);
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Safe_Delete(m_pLoading);
}

HRESULT CLogo::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;

	LightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(0.f, 0.f, 1.f);

	HRESULT	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	return S_OK;
}
