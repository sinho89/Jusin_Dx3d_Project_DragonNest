#include "stdafx.h"

#include "Include.h"
#include "BossBreath.h"
#include "Export_Function.h"
#include "Stage.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"

CBossBreath::CBossBreath(LPDIRECT3DDEVICE9 pDevice)
: CParticle(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
{
	m_fAlpha	= 1.f;
}

CBossBreath::~CBossBreath()
{

}
CBossBreath* CBossBreath::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBossBreath*		pPetal = new CBossBreath(pDevice);

	if(FAILED(pPetal->Initialize()))
		Engine::Safe_Delete(pPetal);

	return pPetal;
}

CBossBreath* CBossBreath::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 vPos)
{
	CBossBreath*		pPetal = new CBossBreath(pDevice);

	if(FAILED(pPetal->Initialize()))
		Engine::Safe_Delete(pPetal);

	pPetal->SetPos(vPos);

	return pPetal;
}

HRESULT CBossBreath::SetConstantTable(void)
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

HRESULT CBossBreath::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Skill");
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

HRESULT CBossBreath::Initialize(void)
{
	AddComponent();

	m_PrtN	= 3000;
	m_PrtD	= new Tpart[m_PrtN];

	// ������ � ��¿� ����
	m_iVtx	= m_PrtN * 2 * 3;
	m_pVtx	= new VtxDUV1[ m_iVtx ];

	D3DXCreateTextureFromFileEx(m_pDevice
		, L"../bin/Resources/Texture/cycle.png"
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

	m_dTimeCur	= timeGetTime();
	m_dTimeInt	= 60;

	return S_OK;
}
void CBossBreath::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_fAniTime += fTime;

	if(!m_bAni)
		return;
	
	int		i;

	// 1. ��� �����Ѵ�.
	FLOAT	ftime = m_fTimeAvg * 0.1f;


	for(i=0; i<m_PrtN; ++i)
	{
		Tpart* pPrt = &m_PrtD[i];

		// ���������� ���Ѵ�.
		D3DXVECTOR3	vcAirR = pPrt->m_CrnV;					// ���������� ���� ����
		FLOAT		fLenV  = D3DXVec3LengthSq(&vcAirR);		// �ӵ��� ����(Vx*Vx + Vy*Vy + Vz*Vz)ũ�� ����

		// ���������� ���� ���͸� ���Ѵ�.
		D3DXVec3Normalize(&vcAirR, &vcAirR);

		// �̵� �ӵ��� �ݴ�� ����
		vcAirR	*= -1.F;

		// �ӷ����� * ���� ���� ����� ����.
		vcAirR	*= fLenV * pPrt->m_fDamp;

		// 1. ���ӵ��� ���������� ���Ѵ�.
		pPrt->m_CrnA = pPrt->m_IntA + vcAirR;

		// 2. ���� �ӵ� ����
		pPrt->m_CrnV += pPrt->m_CrnA * ftime;

		// 3. ���� ��ġ ����
		pPrt->m_CrnP += pPrt->m_CrnV * ftime;

		// 4. ��谪 ����. ����� ���� ���·� ����.

		if(D3DXVec3Length(&(pPrt->m_CrnP - pPrt->m_IntP)) > 3000.f)
		{
			pPrt->m_bLive	= FALSE;
		}
		/*if(pPrt->m_CrnP.y<0.f)
		{
			pPrt->m_bLive	= FALSE;
		}*/
	}


	// 2. ��ƼŬ�� ������ �����Ѵ�.
	for(i=0; i<m_PrtN; ++i)
	{
		Tpart* pPrt = &m_PrtD[i];

		if(FALSE == pPrt->m_bLive)
			continue;

		D3DXCOLOR	xc = pPrt->m_dColor;

		pPrt->m_fLife -=pPrt->m_fFade*ftime;

		if(pPrt->m_fLife<=0.f)
		{
			pPrt->m_bLive	= FALSE;
			continue;
		}

		xc.a	= 1.f/*pPrt->m_fLife*/;
		pPrt->m_dColor	= xc;
	}


	// 3. �ؽ�ó �ִϸ��̼ǿ� ���� ��ƼŬ�� �����Ѵ�.
	DWORD	dTimeCur	= timeGetTime();

	if(dTimeCur>=(m_dTimeCur+m_dTimeInt))
	{
		m_dTimeCur = dTimeCur;

		for(i=0; i<m_PrtN; ++i)
		{
			Tpart* pPrt = &m_PrtD[i];

			if(FALSE == pPrt->m_bLive)
				continue;

			++pPrt->m_PrsImg;
			pPrt->m_PrsImg %=22;
		}
	}



	// 3. ���� ��ƼŬ�� ����Ѵ�.
	for(i=0; i<m_PrtN; ++i)
	{
		Tpart* pPrt = &m_PrtD[i];

		if(TRUE == pPrt->m_bLive)
			continue;

		this->SetPart(i);
	}

	// 4. ����� �����Ѵ�.

	// ī�޶��� ����
	D3DXMATRIX mtView;
	m_pDevice->GetTransform(D3DTS_VIEW, &mtView);

	D3DXVECTOR3 vcCamX(mtView._11, mtView._21, mtView._31);
	D3DXVECTOR3 vcCamY(mtView._12, mtView._22, mtView._32);
	D3DXVECTOR3 vcCamZ(mtView._13, mtView._23, mtView._33);

	for(i=0; i<m_PrtN; ++i)
	{	
		Tpart* pPrt = &m_PrtD[i];

		D3DXVECTOR3	vcP	= pPrt->m_CrnP;

		// ī�޶��� Z��� ��ƼŬ�� ��ġ�� ����
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
		xcC.a = 1.f;

		FLOAT		fW = pPrt->m_PrsW;
		FLOAT		fH = pPrt->m_PrsH;
		FLOAT		fD = min(fW, fH);

		VtxDUV1* pVtx = &m_pVtx[i*6 + 0];

		int			nIdxX	= pPrt->m_PrsImg %4;
		int			nIdxY	= pPrt->m_PrsImg /8;
		D3DXVECTOR2	uv00( (nIdxX+0)/4.f, (nIdxY+0)/8.f);
		D3DXVECTOR2	uv11( (nIdxX+1)/4.f, (nIdxY+1)/8.f);


		(pVtx+0)->p.x	= vcP.x - (vcCamX.x - vcCamY.x) * fW;
		(pVtx+0)->p.y	= vcP.y - (vcCamX.y - vcCamY.y) * fH;
		(pVtx+0)->p.z	= vcP.z - (vcCamX.z - vcCamY.z) * fD;
		(pVtx+0)->u		= uv00.x;
		(pVtx+0)->v		= uv00.y;
		(pVtx+0)->d		= xcC;

		(pVtx+1)->p.x	= vcP.x + (vcCamX.x + vcCamY.x) * fW;
		(pVtx+1)->p.y	= vcP.y + (vcCamX.y + vcCamY.y) * fH;
		(pVtx+1)->p.z	= vcP.z + (vcCamX.z + vcCamY.z) * fD;
		(pVtx+1)->u		= uv11.x;
		(pVtx+1)->v		= uv00.y;
		(pVtx+1)->d		= xcC;

		(pVtx+2)->p.x	= vcP.x - (vcCamX.x + vcCamY.x) * fW;
		(pVtx+2)->p.y	= vcP.y - (vcCamX.y + vcCamY.y) * fH;
		(pVtx+2)->p.z	= vcP.z - (vcCamX.z + vcCamY.z) * fD;
		(pVtx+2)->u		= uv00.x;
		(pVtx+2)->v		= uv11.y;
		(pVtx+2)->d		= xcC;

		(pVtx+3)->p.x	= vcP.x + (vcCamX.x - vcCamY.x) * fW;
		(pVtx+3)->p.y	= vcP.y + (vcCamX.y - vcCamY.y) * fH;
		(pVtx+3)->p.z	= vcP.z + (vcCamX.z - vcCamY.z) * fD;
		(pVtx+3)->u		= uv11.x;
		(pVtx+3)->v		= uv11.y;
		(pVtx+3)->d		= xcC;

		m_pVtx[i*6 + 4] = m_pVtx[i*6 + 2];
		m_pVtx[i*6 + 5] = m_pVtx[i*6 + 1];
	}

	SetAvgTime(0.005f / Engine::Get_TimeMgr()->GetTime());

	Engine::CGameObject::Update();

}

void CBossBreath::Render(void)
{
	if(!m_bAni)
		return;

	if(m_pInfo->m_bSkillRender)
	{
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pDevice->SetTexture(0, m_pTx);
		m_pDevice->SetFVF(VtxDUV1::FVF);

		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_PrtN * 2, m_pVtx, sizeof(VtxDUV1));

		m_pDevice->SetTexture(0, NULL);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}
	
}

void CBossBreath::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CBossBreath::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}

void CBossBreath::SetPos(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CBossBreath::SetPart(int nIdx)
{
	Tpart* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// ���� ��
	FLOAT	fPhi;		// ���� ��
	FLOAT	fSpdR;		// �ӵ� ũ��

	// �ʱ� ���ӵ�
	pPrt->m_IntA = m_pInfo->m_vDir * 1000.f;

	//�ʱ� �ӵ��� ��ġ�� �����ϱ� ���� ����
	fTheta	= float(rand()%90 - 45);
	fTheta	-= 10.f;

	fPhi	= float(rand()%360);

	fSpdR = 10000.f + rand()%1001;
	fSpdR *=0.03f;

	// �������� ����
	fTheta	= D3DXToRadian(fTheta);
	fPhi	= D3DXToRadian(fPhi);

	// �ʱ� �ӵ�

	D3DXVECTOR3 vRight, vUp, vLook;

	memcpy(&vRight, &m_pInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&vUp, &m_pInfo->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vLook, &m_pInfo->m_matWorld.m[2][0], sizeof(D3DXVECTOR3));
	/*
	pPrt->m_IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->m_IntV.y = fSpdR * cosf(fTheta);
	pPrt->m_IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);*/

	m_pInfo->m_vDir.x = m_pInfo->m_vDir.x + (rand() % 4 - 2) * 0.008;
	m_pInfo->m_vDir.y = m_pInfo->m_vDir.y + (rand() % 4 - 2) * 0.008;
	m_pInfo->m_vDir.z = m_pInfo->m_vDir.z + (rand() % 4 - 2) * 0.008;
	pPrt->m_IntV = m_pInfo->m_vDir * fSpdR * cosf(fTheta);


	// �ʱ� ��ġ		
	pPrt->m_IntP.x = m_pInfo->m_vPos.x;
	pPrt->m_IntP.y = m_pInfo->m_vPos.y;
	pPrt->m_IntP.z = m_pInfo->m_vPos.z;


	// ź�� ��� ����
	pPrt->m_fElst= (50 + rand()%51)*0.01f;

	// �������� ���
	pPrt->m_fDamp= (100 + rand()%101)*0.00005F;

	// �ʱ� ��ġ, �ӵ�, ���ӵ��� ������ ������ �ʱ� ������ ����
	pPrt->m_CrnP = m_pInfo->m_vPos;
	pPrt->m_CrnV =pPrt->m_IntV;
	pPrt->m_CrnRight = pPrt->m_IntRight;
	pPrt->m_CrnUp = pPrt->m_IntUp;
	pPrt->m_CrnLook = pPrt->m_IntLook;
	pPrt->m_CrnA = pPrt->m_IntA;



	// ������ ���� ���
	pPrt->m_bLive	= TRUE;
	pPrt->m_fLife	= 30.f + rand()%71;
	pPrt->m_fLife	= 30.f + rand()%71;
	pPrt->m_fLife	*=0.01f;

	pPrt->m_fFade	=( 100 + rand()%101  ) *0.002f;
	pPrt->m_dColor	=	D3DXCOLOR(1.f, rand()%25 * 0.01f, 0.f, 1.f);

	// ������ ǥ�� ���
	pPrt->m_PrsW = 10.f + rand()%31;
	pPrt->m_PrsW *= 0.5f;

	pPrt->m_PrsH = 10.f + rand()%31;
	pPrt->m_PrsH *= 0.5f;

	//pPrt->m_PrsW = 10.f;
	//pPrt->m_PrsH = 10.f;

	pPrt->m_PrsImg	= rand()%22;

}

void CBossBreath::Release(void)
{

}