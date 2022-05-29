#include "stdafx.h"

#include "Include.h"
#include "Petal.h"
#include "Export_Function.h"
#include "Stage.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"

CPetal::CPetal(LPDIRECT3DDEVICE9 pDevice)
: CParticle(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fDetail(1.f)
{
	m_fAlpha	= 1.f;
	m_PrtN		= 0;
	m_PrtD		= NULL;

	m_fTimeAvg	= 0;
	m_bAni		= FALSE;

	m_pVtx		= NULL;
	m_pTx		= NULL;
}

CPetal::~CPetal()
{

}
CPetal* CPetal::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPetal*		pPetal = new CPetal(pDevice);

	if(FAILED(pPetal->Initialize()))
		Engine::Safe_Delete(pPetal);

	return pPetal;
}

HRESULT CPetal::SetConstantTable(void)
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
	const D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo(2);
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}

HRESULT CPetal::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Petal");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Petal");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CPetal::Initialize(void)
{
	AddComponent();

	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_PrtN	= 100;
	m_PrtD	= new Tpart[m_PrtN];

	// 입자의 운동 출력용 설정
	m_iVtx	= m_PrtN * 2 * 3;
	m_pVtx	= new VtxDUV1[ m_iVtx ];

	// Texture Load
	D3DXCreateTextureFromFileEx(m_pDevice
		, L"../bin/Resources/Texture/Petal.png"
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
		, (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
		, 0x00FFFFFF
		, NULL
		, NULL
		, &m_pTx
		);


	return S_OK;
}
void CPetal::Update(void)
{
	
}

void CPetal::Render(void)
{
	if(!m_bAni)
		return;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pShader->Begin_Shader(1);
	m_pTexture->Render(0, 0);
	m_pDevice->SetFVF(VtxDUV1::FVF);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_PrtN * 2, m_pVtx, sizeof(VtxDUV1));
	m_pShader->End_Shader();
}

/*int CPetal::SortFnc(const Engine::PARTICLE* p1, const Engine::PARTICLE* p2)
{
	FLOAT	v1, v2;

	v1 = p1->PrsZ;
	v2 = p2->PrsZ;

	if(v1 < v2)
		return 1;

	else if(v1 == v2)
		return 0;

	else 
		return -1;
}*/

void CPetal::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CPetal::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}

void CPetal::SetPart(int nIdx)
{
	
}

void CPetal::Release(void)
{

}