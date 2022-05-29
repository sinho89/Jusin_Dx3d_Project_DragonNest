#include "stdafx.h"
#include "LogoBack.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "Shader.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fAlpha(1.f)
{
	/*ZeroMemory(&m_MtrInfo, sizeof(D3DMATERIAL9));*/
}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(2.f, 2.f, 2.f);
	return S_OK;
}

void CLogoBack::Update(void)
{
	Engine::CGameObject::Update();
}

void CLogoBack::Render(void)
{
	HRESULT		hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	m_pShader->Begin_Shader();

	m_pBuffer->Render(NULL);

	m_pShader->End_Shader();
}

HRESULT CLogoBack::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_LOGO, L"Texture_Logo");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Default");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	return S_OK;
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack*		pGameObject = new CLogoBack(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

HRESULT CLogoBack::SetConstantTable(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture");
	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	return S_OK;
}



