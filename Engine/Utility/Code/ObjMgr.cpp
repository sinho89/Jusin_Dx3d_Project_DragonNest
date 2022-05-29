#include "ObjMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(Engine::CObjMgr)

Engine::CObjMgr::CObjMgr(void)
{

}

Engine::CObjMgr::~CObjMgr(void)
{
	Release();
}


void Engine::CObjMgr::Update(void)
{
	for(int i=0; i<LAYER_END; ++i)
	{
		MAPOBJLIST::iterator mapiter = m_mapObjlist[i].begin();
		MAPOBJLIST::iterator mapiter_end = m_mapObjlist[i].end();

		for(; mapiter != mapiter_end; ++mapiter)
		{
			OBJLIST::iterator list_iter = mapiter->second.begin();
			OBJLIST::iterator list_iter_end = mapiter->second.end();

			for(; list_iter != list_iter_end;)
			{
				if((*list_iter)->GetAlive() == false)
				{
					Engine::Safe_Delete(*list_iter);
					list_iter = mapiter->second.erase(list_iter);
				}
				else
				{
					(*list_iter)->Update();
					++list_iter;
				}
			}
		}
	}
}

void Engine::CObjMgr::Render(void)
{
	for(int i=0; i<LAYER_END; ++i)
	{
		/*D3DXMATRIX proj;
		if(i == LAYER_UI)
		{
			D3DXMatrixOrthoOffCenterLH(&proj, ENGINE::WINCX, ENGINE::WINCY, 1.f, 1000.0f);
		}*/
		MAPOBJLIST::iterator mapiter = m_mapObjlist[i].begin();
		MAPOBJLIST::iterator mapiter_end = m_mapObjlist[i].end();

		for(; mapiter != mapiter_end; ++mapiter)
		{
			OBJLIST::iterator list_iter = mapiter->second.begin();
			OBJLIST::iterator list_iter_end = mapiter->second.end();

			for(; list_iter != list_iter_end; ++list_iter)
			{
				(*list_iter)->Render();
			}
		}
		//if(i == LAYER_UI)
		//{
		//	D3DXMatrixPerspectiveFovLH(
		//		&proj,
		//		D3DX_PI * 0.25f, // 45 - degree
		//		ENGINE::WINCX / ENGINE::WINCY,
		//		1.0f,
		//		1000.0f);
		//}
		
	}
}

void Engine::CObjMgr::Release(void)
{
	for(int i=0; i<LAYER_END; ++i)
	{
		MAPOBJLIST::iterator mapiter = m_mapObjlist[i].begin();
		MAPOBJLIST::iterator mapiter_end = m_mapObjlist[i].end();

		for(; mapiter != mapiter_end; ++mapiter)
		{
			OBJLIST::iterator list_iter = mapiter->second.begin();
			OBJLIST::iterator list_iter_end = mapiter->second.end();

			for(; list_iter != list_iter_end; ++list_iter)
			{
				Engine::Safe_Delete(*list_iter);
			}
			mapiter->second.clear();
		}
		m_mapObjlist[i].clear();
	}
}

HRESULT Engine::CObjMgr::AddObject(const TCHAR* szObjKey, 
									 CGameObject* pGameObject, 
									 LAYERTYPE LayerType)
{
	if(pGameObject)
	{
		/*MAPOBJLIST::iterator mapiter = find_if(m_mapObjlist[LayerType].begin(),
												m_mapObjlist[LayerType].end(),
												CTagFinder(szObjKey));*/
		MAPOBJLIST::iterator mapiter = m_mapObjlist[LayerType].find(szObjKey);

		if(mapiter == m_mapObjlist[LayerType].end())
		{
			m_mapObjlist[LayerType].insert(MAPOBJLIST::value_type(szObjKey, OBJLIST()));
		}

		(m_mapObjlist[LayerType])[szObjKey].push_back(pGameObject);
	}	

	return S_OK;
}

const Engine::CComponent* Engine::CObjMgr::GetComponent(const TCHAR* szObjKey, 
														  const TCHAR* szComponentKey, 
														  LAYERTYPE LayerType)
{
	MAPOBJLIST::iterator mapiter = m_mapObjlist[LayerType].find(szObjKey);
	
	if(mapiter == m_mapObjlist[LayerType].end())
		return NULL;

	OBJLIST::iterator		iter_list		= mapiter->second.begin();
	OBJLIST::iterator		iter_list_end	= mapiter->second.end();

	for(; iter_list != iter_list_end; ++iter_list)
	{
		const CComponent*	pComponent = (*iter_list)->GetComponent(szComponentKey);

		if(pComponent != NULL)
			return pComponent;
	}

	return NULL;
}


Engine::CGameObject* Engine::CObjMgr::GetGameObject(const TCHAR* szObjKey, LAYERTYPE LayerType)
{
	MAPOBJLIST::iterator mapiter = m_mapObjlist[LayerType].find(szObjKey);

	if(mapiter == m_mapObjlist[LayerType].end())
		return NULL;

	return mapiter->second.front();
}

void Engine::CObjMgr::SetDivisionType(DivisionType _eType)
{
	m_eDivType = _eType;
}

Engine::CObjMgr::DivisionType Engine::CObjMgr::GetDivisionType()
{
	return m_eDivType;
}

Engine::CObjMgr::OBJLIST* Engine::CObjMgr::GetObjList(const TCHAR* szObjKey, LAYERTYPE LayerType)
{
	MAPOBJLIST::iterator mapiter = m_mapObjlist[LayerType].find(szObjKey);

	if(mapiter == m_mapObjlist[LayerType].end())
		return NULL;

	return &mapiter->second;
}