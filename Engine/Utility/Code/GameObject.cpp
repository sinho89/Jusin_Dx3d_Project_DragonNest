#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_bAlive(true)
{
}

Engine::CGameObject::~CGameObject(void)
{
	Release();
}

void Engine::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for(; iter != iter_end ; ++iter)
	{
		iter->second->Update();
	}
}

void Engine::CGameObject::Release(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	DWORD		dwRefCnt = 0;
	for( ; iter != iter_end; ++iter)
	{
		dwRefCnt = iter->second->Release();
		if(dwRefCnt == 0)
			Safe_Delete(iter->second);
	}
	m_mapComponent.clear();

	/*for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();*/
}

const Engine::CComponent* Engine::CGameObject::GetComponent(const TCHAR* pComponentKey)
{
	MAPCOMPONENT::iterator	iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(pComponentKey));
	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

void Engine::CGameObject::Compute_ViewZ(const D3DXVECTOR3* pPos)
{
	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(D3DXVECTOR3));

	float		fDistance = D3DXVec3Length(&(vCamPos - *pPos));

	m_fViewZ = fDistance;
}

bool* Engine::CGameObject::GetAlive()
{
	return &m_pInfo->m_bAlive;
}

