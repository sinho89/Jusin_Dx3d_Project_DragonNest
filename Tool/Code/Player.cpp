#include "stdafx.h"
#include "Player.h"

#include "Tool_Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_iAniIdx(0)
, m_bPush(false)
, m_dwIndex(0)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_fDetail = 1.f;
	m_pMesh->SetAnimationSet(m_iAniIdx);

	//m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 800.f);
	return S_OK;
}

void CPlayer::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	KeyCheck();
	
	if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPos())
	{
		m_iAniIdx = 28;
		m_pMesh->SetAnimationSet(m_iAniIdx);
		m_pMesh->SetTrackPos(0.0);
	}

	Engine::CGameObject::Update();

	Compute_ViewZ(&m_pInfo->m_vPos);
}

void CPlayer::Render(void)
{
	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld);
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_Player");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer*		pGameObject = new CPlayer(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CPlayer::KeyCheck(void)
{
	Engine::CInput*		pInput = Engine::Get_Input();
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	if(pInput->GetDIKeyState(DIK_RETURN) & 0x80)
	{
		if(m_bPush == true)
			return ;

		m_bPush = true;

		m_iAniIdx = 10;
		m_pMesh->SetAnimationSet(m_iAniIdx);
	}
	else
	{
		m_bPush = false;
	}

	if(pInput->GetDIKeyState(DIK_W) & 0x80)
	{
		m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * 50000.f * fTime), m_dwIndex);
		//m_pInfo->m_vPos += m_pInfo->m_vDir * 500.f * fTime;
	}

	if(pInput->GetDIKeyState(DIK_S) & 0x80)
	{
		m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh( &m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * -50000.f * fTime), m_dwIndex);
		//m_pInfo->m_vPos -= m_pInfo->m_vDir * 500.f * fTime;
	}

	if(pInput->GetDIKeyState(DIK_A) & 0x80)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f * fTime);
	}

	if(pInput->GetDIKeyState(DIK_D) & 0x80)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f * fTime);
	}
}

HRESULT CPlayer::SetConstantTable(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fDetail", m_fDetail);
	//Light
	const D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}