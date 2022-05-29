#include "stdafx.h"
#include "SwordTrail.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "Shader.h"
#include "Stage.h"
#include "DynamicMesh.h"
#include "TrailTex.h"

CSwordTrail::CSwordTrail(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fAlpha(1.f)
, m_vOriPoint(0.f, 0.f, 0.f)
, m_vPoint(0.f, 0.f, 0.f)
{
	/*ZeroMemory(&m_MtrInfo, sizeof(D3DMATERIAL9));*/
}

CSwordTrail::~CSwordTrail(void)
{

}

HRESULT CSwordTrail::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}

void CSwordTrail::Update(void)
{
	Calculate_WeaponPos();
	Engine::CGameObject::Update();
}

void CSwordTrail::Render(void)
{
	HRESULT		hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pShader->Begin_Shader();

	m_pBuffer->Render(NULL);

	m_pShader->End_Shader();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CSwordTrail::Calculate_WeaponPos()
{
	const Engine::CComponent*		pWeaponInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Weapon", L"Transform");

	((Engine::CTrailTex*)m_pBuffer)->VertexUpdate(m_vPoint, m_vOriPoint);
}

HRESULT CSwordTrail::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_Trail");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Trail");
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

CSwordTrail* CSwordTrail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSwordTrail*		pGameObject = new CSwordTrail(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

HRESULT CSwordTrail::SetConstantTable(void)
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



