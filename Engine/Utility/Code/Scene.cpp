#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CScene::~CScene(void)
{
	Release();
}

void Engine::CScene::Update(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}

void Engine::CScene::Render(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}

void Engine::CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

const Engine::CComponent* Engine::CScene::GetComponent(WORD eLayerID,const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	MAPLAYER::iterator iter = m_mapLayer.find(eLayerID);
	if(m_mapLayer.end() == iter)
		return NULL;

	return iter->second->GetComponent(pObjKey, pComponentKey);
}

list<Engine::CGameObject*>* Engine::CScene::GetObjList(WORD eLayerID, const TCHAR* pObjKey)
{
	MAPLAYER::iterator iter = m_mapLayer.find(eLayerID);
	if(m_mapLayer.end() == iter)
		return NULL;

	return iter->second->GetObjList(pObjKey);
}

unordered_map<const TCHAR*, list<Engine::CGameObject*>>* Engine::CScene::GetMap(WORD eLayerID)
{
	MAPLAYER::iterator iter = m_mapLayer.find(eLayerID);
	if(m_mapLayer.end() == iter)
		return NULL;

	return iter->second->GetMap();
}

Engine::CGameObject* Engine::CScene::GetObject(WORD eLayerID, const TCHAR* pObjKey)
{
	MAPLAYER::iterator iter = m_mapLayer.find(eLayerID);
	if(m_mapLayer.end() == iter)
		return NULL;

	return iter->second->GetObj(pObjKey);
}

