#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "SceneSelector.h"
#include "VIBuffer.h"
#include "SoundMgr.h"

CMainApp::CMainApp(void)
: m_pGraphicDev(Engine::Get_GraphicDev())
, m_pManagement(Engine::Get_Management())
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
{
	//AllocConsole(); 
	//freopen( "CONOUT$",  "wt", stdout);
	//SetConsoleTitleA("Debug");
	CSoundMgr::GetInstance()->LoadSoundFile();
}

CMainApp::~CMainApp(void)
{
	Release();
	//FreeConsole();
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

	hr = Engine::Get_FontMgr()->AddFont(m_pDevice, L"나눔글꼴 Bold", 30, 18, FW_HEAVY);
	FAILED_CHECK(hr);

	hr = Engine::Get_FontMgr()->AddFont(m_pDevice, L"휴먼편지체", 30, 15, FW_NORMAL);
	FAILED_CHECK(hr);

	hr = m_pManagement->InitManagement(m_pDevice);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Management Init Failed");

	hr = m_pResourceMgr->ReserveContainerSize(RESOURCE_END);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"리소스 컨테이너 할당 실패");

	hr = m_pResourceMgr->AddBuffer(m_pDevice, RESOURCE_STATIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Buffer_RcTex 생성 실패");


	hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_LOGO));
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"씬 전환 실패");

	m_pTimeMgr->InitTime();

	return S_OK;
}

void CMainApp::Update(void)
{
	m_pTimeMgr->SetTime();

	ShowCursor(false);

	Engine::Get_Input()->SetInputState();

	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}

void CMainApp::Release(void)
{
	//Utility
	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Get_Frustum()->DestroyInstance();
	Engine::Get_QuadTree()->DestroyInstance();
	Engine::Get_NaviMgr()->DestroyInstance();

	//Resource
	Engine::Safe_Single_Destory(m_pResourceMgr);

	//System
	Engine::Get_Input()->DestroyInstance();
	Engine::Safe_Single_Destory(m_pTimeMgr);
	Engine::Get_FontMgr()->DestroyInstance();
	Engine::Safe_Single_Destory(m_pGraphicDev);
	CSoundMgr::GetInstance()->DestroyInstance();
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