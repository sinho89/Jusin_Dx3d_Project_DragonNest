#include "LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Release();
}

HRESULT Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice, 
									D3DLIGHT9* pLightInfo, 
									const WORD& wLightIdx)
{
	NULL_CHECK_RETURN(pDevice, E_FAIL);
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	CLight*		pLight = CLight::Create(pDevice, pLightInfo, wLightIdx);

	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_Lightlist.push_back(pLight);

	return S_OK;
}

void Engine::CLightMgr::Release(void)
{
	for_each(m_Lightlist.begin(), m_Lightlist.end(), CDeleteObj());
	m_Lightlist.clear();
}

D3DLIGHT9* Engine::CLightMgr::GetLightInfo(const UINT& iIndex /*= 0*/)
{
	LIGHTLIST::iterator		iter = m_Lightlist.begin();
	for(UINT i = 0; i < iIndex; ++i)
	{
		++iter;
	}
	return (*iter)->GetLightInfo();
}