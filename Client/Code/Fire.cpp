#include "stdafx.h"
#include "Fire.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "CameraMgr.h"


CFire::CFire(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CFire::~CFire(void)
{

}

HRESULT CFire::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_PrtN	= 5000;
	m_PrtD	= new Tpart[m_PrtN];

	// 입자의 운동 출력용 설정
	m_iVtx	= m_PrtN * 2 * 3;
	m_pVtx	= new VtxDUV1[ m_iVtx ];

	D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Texture/alpha.png", &m_pTx0);
	D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Texture/Noise.tga", &m_pTx1);

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE]={0};
	D3DXDeclaratorFromFVF(VtxDUV1::FVF, vertex_decl);

	LPDIRECT3DVERTEXDECLARATION9 pFVF;
	if( FAILED(m_pDevice->CreateVertexDeclaration(vertex_decl, &pFVF)))
		return -1;

	m_pFVF = pFVF;

	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 800.f);
	return S_OK;
}

void CFire::Update(void)
{
	if(!m_bAni)
		return;

	int		i;

	// 1. 운동을 갱신한다.
	FLOAT	ftime = m_fTimeAvg * 0.1f;


	for(i=0; i<m_PrtN; ++i)
	{
		Tpart* pPrt = &m_PrtD[i];


		// 3. 현재 위치 갱신
		pPrt->m_CrnP += pPrt->m_CrnV * ftime;


		// 4. 경계값 설정. 벗어나면 죽은 상태로 설정.
		if(pPrt->m_CrnP.y>1000.f)
		{
			pPrt->m_bLive	= FALSE;
		}


		if(FALSE == pPrt->m_bLive)
			continue;

		D3DXCOLOR	xc = pPrt->m_dColor;

		pPrt->m_fLife -=pPrt->m_fFade*ftime;

		if(pPrt->m_fLife<=0.f)
		{
			pPrt->m_bLive	= FALSE;
			continue;
		}

		xc.a	= pPrt->m_fLife;
		pPrt->m_dColor	= xc;
	}


	// 3. 죽은 파티클을 재생한다.
	for(i=0; i<m_PrtN; ++i)
	{
		Tpart* pPrt = &m_PrtD[i];

		if(TRUE == pPrt->m_bLive)
			continue;

		this->SetPart(i);
	}





	// 4. 출력을 설정한다.

	// 카메라의 정보
	D3DXMATRIX mtView = *Engine::Get_CameraMgr()->GetView();

	D3DXVECTOR3 vcCamX(mtView._11, mtView._21, mtView._31);
	D3DXVECTOR3 vcCamY(mtView._12, mtView._22, mtView._32);
	D3DXVECTOR3 vcCamZ(mtView._13, mtView._23, mtView._33);

	for(i=0; i<m_PrtN; ++i)
	{	
		Tpart* pPrt = &m_PrtD[i];

		D3DXVECTOR3	vcP	= pPrt->m_CrnP;

		// 카메라의 Z축과 파티클의 위치와 내적
		pPrt->m_PrsZ	= D3DXVec3Dot(&vcP, &vcCamZ);
	}

	// Sorting
	qsort (m_PrtD
		, m_PrtN
		, sizeof(Tpart)
		, (int(*) (const void *, const void *)) SortFnc);


	for(i=0; i<m_PrtN; ++i)
	{	
		Tpart* pPrt = &m_PrtD[i];

		D3DXVECTOR3	vcP	= pPrt->m_CrnP;
		D3DXCOLOR	xcC	= pPrt->m_dColor;

		FLOAT		fW = pPrt->m_PrsW;

		VtxDUV1* pVtx = &m_pVtx[i*6 + 0];

		(pVtx+0)->p.x	= vcP.x - (vcCamX.x - vcCamY.x) * fW;
		(pVtx+0)->p.y	= vcP.y - (vcCamX.y - vcCamY.y) * fW;
		(pVtx+0)->p.z	= vcP.z - (vcCamX.z - vcCamY.z) * fW;
		(pVtx+0)->u		= 0;
		(pVtx+0)->v		= 0;
		(pVtx+0)->d		= xcC;

		(pVtx+1)->p.x	= vcP.x + (vcCamX.x + vcCamY.x) * fW;
		(pVtx+1)->p.y	= vcP.y + (vcCamX.y + vcCamY.y) * fW;
		(pVtx+1)->p.z	= vcP.z + (vcCamX.z + vcCamY.z) * fW;
		(pVtx+1)->u		= 1;
		(pVtx+1)->v		= 0;
		(pVtx+1)->d		= xcC;

		(pVtx+2)->p.x	= vcP.x - (vcCamX.x + vcCamY.x) * fW;
		(pVtx+2)->p.y	= vcP.y - (vcCamX.y + vcCamY.y) * fW;
		(pVtx+2)->p.z	= vcP.z - (vcCamX.z + vcCamY.z) * fW;
		(pVtx+2)->u		= 0;
		(pVtx+2)->v		= 1;
		(pVtx+2)->d		= xcC;

		(pVtx+3)->p.x	= vcP.x + (vcCamX.x - vcCamY.x) * fW;
		(pVtx+3)->p.y	= vcP.y + (vcCamX.y - vcCamY.y) * fW;
		(pVtx+3)->p.z	= vcP.z + (vcCamX.z - vcCamY.z) * fW;
		(pVtx+3)->u		= 1;
		(pVtx+3)->v		= 1;
		(pVtx+3)->d		= xcC;

		m_pVtx[i*6 + 4] = m_pVtx[i*6 + 2];
		m_pVtx[i*6 + 5] = m_pVtx[i*6 + 1];
	}

	SetAvgTime(0.005f / Engine::Get_TimeMgr()->GetTime());

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	Engine::CGameObject::Update();

}

void CFire::Render(void)
{
	if(!m_bAni)
		return;

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT	pd3Eft = m_pShader->Get_EffectHandle();;

	D3DXMATRIX mtViw = *Engine::Get_CameraMgr()->GetView();
	D3DXMATRIX mtPrj = *Engine::Get_CameraMgr()->GetProj();
	D3DXMATRIX mtVP;

	m_pDevice->GetTransform(D3DTS_VIEW, &mtViw);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &mtPrj);

	D3DXMATRIX	mtWld;
	D3DXMatrixIdentity(&mtWld);

	mtVP	= mtViw * mtPrj;
	mtWld	= mtVP;

	 pd3Eft->SetTechnique("Tech");

	m_pDevice->SetVertexDeclaration((LPDIRECT3DVERTEXDECLARATION9)m_pFVF);

	pd3Eft->Begin(NULL, NULL);
	pd3Eft->BeginPass(1);

	pd3Eft->SetMatrix("m_mtWVP", &mtWld);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


	if(!pd3Eft)
	{
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
		m_pDevice->SetTexture(0, m_pTx0);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

		m_pDevice->SetTexture(0, m_pTx1);

		pd3Eft->CommitChanges();
	}

	m_pDevice->SetFVF(VtxDUV1::FVF);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_PrtN * 2, m_pVtx, sizeof(VtxDUV1));

	m_pDevice->SetTexture(0, NULL);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	pd3Eft->EndPass();
	pd3Eft->End();
}

HRESULT CFire::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Fire");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CFire* CFire::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFire*		pGameObject = new CFire(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
HRESULT CFire::SetConstantTable(void)
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
	D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo(1);
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}

void* CFire::GetVertexDeclaration()
{
	return m_pFVF;
}

void CFire::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CFire::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}

void CFire::SetPart(int nIdx)
{
	Tpart* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// 각도 θ
	FLOAT	fPhi;		// 각도 φ
	FLOAT	fSpdR;		// 속도 크기



	//초기 속도와 위치를 설정하기 위한 변수
	fTheta	= float(rand()%61);
	fTheta	-=30.f;

	fPhi	= float(rand()%360);

	fSpdR = 10.f + rand()%81;
	fSpdR *=0.03f;

	// 라디안으로 변경
	fTheta	= D3DXToRadian(fTheta);
	fPhi	= D3DXToRadian(fPhi);

	// 초기 속도
	pPrt->m_IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->m_IntV.y = fSpdR * cosf(fTheta);
	pPrt->m_IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// 초기 위치		
	pPrt->m_IntP = D3DXVECTOR3(0.f, 3.f, 0.f) + 20 * D3DXVECTOR3( cosf(fPhi), 0, sinf(fPhi));


	// 초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->m_CrnP = pPrt->m_IntP;
	pPrt->m_CrnV = pPrt->m_IntV;


	// 입자의 생명 요소
	pPrt->m_bLive	= TRUE;
	pPrt->m_fLife	= 30.f + rand()%71;
	pPrt->m_fLife	*=0.01f;

	pPrt->m_fFade	=( 100 + rand()%101  ) *0.0001f;
	pPrt->m_dColor	=	D3DXCOLOR(1.f, 0.25f, 0.0f, pPrt->m_fLife);


	// 입자의 표현 요소
	pPrt->m_PrsW = 10000.f + rand()%101;
	pPrt->m_PrsW *= 0.2f;
}
