#include "stdafx.h"
#include "MainBar.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "CameraMgr.h"

CMainBar::CMainBar(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
{
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matView);
}

CMainBar::~CMainBar(void)
{

}

HRESULT CMainBar::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fX = WINCX / 2.f;
	m_fY = WINCY - 150.f;
	m_fSizeX = 800.f;
	m_fSizeY = 150.f;
	return S_OK;
}

void CMainBar::Update(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_matView._11 = m_fSizeX;
	m_matView._22 = m_fSizeY;
	m_matView._33 = 1.f;

	// -0.5 -> -400			0.5 -> 400
	m_matView._41 = m_fX - (WINCX >> 1);
	m_matView._42 = -m_fY + (WINCY >> 1);

	Engine::CGameObject::Update();
}

void CMainBar::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX		matOldView, matOldProj;

	matOldView = *Engine::CCameraMgr::GetInstance()->GetView();
	matOldProj = *Engine::CCameraMgr::GetInstance()->GetProj();

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pTexture->Render(0,0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CMainBar::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_MainBar");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CMainBar* CMainBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMainBar*		pGameObject = new CMainBar(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

