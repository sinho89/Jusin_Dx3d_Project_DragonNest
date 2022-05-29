#include "Light.h"

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	ZeroMemory(&m_LightInfo, sizeof(D3DLIGHT9));
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Init_Light(const D3DLIGHT9* pLightInfo, const WORD& wLightIdx)
{
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	memcpy(&m_LightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_pDevice->SetLight(wLightIdx, &m_LightInfo);
	m_pDevice->LightEnable(wLightIdx, TRUE);

	return S_OK;
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pDevice, 
									   const D3DLIGHT9* pLightInfo, 
									   const WORD& wLightIdx)
{
	CLight*		pLight = new CLight(pDevice);

	if(FAILED(pLight->Init_Light(pLightInfo, wLightIdx)))
		Engine::Safe_Delete(pLight);

	return pLight;
}

D3DLIGHT9* Engine::CLight::GetLightInfo(void)
{
	return &m_LightInfo;
}

