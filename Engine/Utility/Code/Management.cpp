#include "Management.h"

#include "Renderer.h"
#include "Scene.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pScene(NULL)
, m_pRenderer(NULL)
, m_pDevice(NULL)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

void Engine::CManagement::AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	m_pRenderer->AddRenderGroup(eRenderGroup, pGameObject);
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	m_pRenderer = CRenderer::Create(pDevice);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create Failed");

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
}

void Engine::CManagement::Render(const float& fTime)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render(fTime);
}

void Engine::CManagement::Release(void)
{
	Engine::Get_LightMgr()->DestroyInstance();

	Engine::Safe_Delete(m_pRenderer);  
	Engine::Safe_Delete(m_pScene);
}

const Engine::CComponent* Engine::CManagement::GetComponent(WORD eLayerID,const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	return m_pScene->GetComponent(eLayerID, pObjKey, pComponentKey);
}

list<Engine::CGameObject*>* Engine::CManagement::GetObjList(WORD eLayerID,const TCHAR* pObjKey)
{
	return m_pScene->GetObjList(eLayerID, pObjKey);
}

unordered_map<const TCHAR*, list<Engine::CGameObject*>>* Engine::CManagement::GetMap(WORD eLayerID)
{
	return m_pScene->GetMap(eLayerID);
}

Engine::CGameObject* Engine::CManagement::GetObject(WORD eLayerID, const TCHAR* pObjKey)
{
	return m_pScene->GetObject(eLayerID, pObjKey);
}


