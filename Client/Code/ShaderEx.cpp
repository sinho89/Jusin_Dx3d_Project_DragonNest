#include "stdafx.h"
#include "ShaderEx.h"


#define SHOW_SHADER_PROC


CShaderEx::CShaderEx()
{
	m_pDev	= NULL;
	m_pEft	= NULL;
	m_pFVF	= NULL;

	m_pTexS	= NULL;
	m_pTexD	= NULL;
}


CShaderEx::~CShaderEx()
{
	Destroy();
}


INT CShaderEx::Create(PDEV pDev)
{
	HRESULT	hr=0;

	m_pDev	= pDev;


	m_pVtx[0] = VtxDUV1( -1.f,  1.f,  0,	 0.f, 0.f, D3DXCOLOR(1,0,0,1));
	m_pVtx[1] = VtxDUV1(  1.f,  1.f,  0,	 1.f, 0.f, D3DXCOLOR(0,1,0,1));
	m_pVtx[2] = VtxDUV1(  1.f, -1.f,  0,	 1.f, 1.f, D3DXCOLOR(0,0,1,1));
	m_pVtx[3] = VtxDUV1( -1.f, -1.f,  0,	 0.f, 1.f, D3DXCOLOR(0,0,1,1));


	DWORD dwFlags = 0;
#if defined( _DEBUG ) || defined( DEBUG )
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pErr = NULL;

	hr = D3DXCreateEffectFromFile( m_pDev
		, L"../../Reference/Headers/ShaderFire.hpp"
		, NULL
		, NULL
		, dwFlags
		, NULL
		, &m_pEft
		, &pErr);


	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE]={0};
	D3DXDeclaratorFromFVF(VtxDUV1::FVF, vertex_decl);

	if( FAILED( hr = m_pDev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return -1;




	return 0;
}

void CShaderEx::Destroy()
{
	Engine::Safe_Release(	m_pEft	);
	Engine::Safe_Release(	m_pFVF	);
}


INT CShaderEx::FrameMove()
{
	return 0;
}


void CShaderEx::Render()
{
	HRESULT hr=0;
	//	FOR Debug
	D3DXMATRIX mtI;
	D3DXMatrixIdentity(&mtI);

	m_pDev->SetTransform(D3DTS_WORLD, &mtI);
	m_pDev->SetTransform(D3DTS_VIEW, &mtI);
	m_pDev->SetTransform(D3DTS_PROJECTION, &mtI);
	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDev->SetRenderState(D3DRS_FOGENABLE, FALSE);


	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);



	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	m_pDev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	m_pDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);


	FLOAT	hHeatHaze = .1f;
	PDTX	pTex0 = m_pTexS;
	PDTX	pTex1 = m_pTexD;

	m_pDev->SetVertexDeclaration(m_pFVF);

	m_pEft->SetTechnique("Tech");

	m_pEft->SetFloat("g_HeatHaze", hHeatHaze);

	m_pEft->Begin(NULL, 0);
	m_pEft->BeginPass(0);

	m_pDev->SetTexture(0, pTex0);
	m_pDev->SetTexture(1, pTex1);
	m_pDev->SetFVF(VtxDUV1::FVF);
	m_pDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, m_pVtx, sizeof(VtxDUV1));

	m_pEft->EndPass();
	m_pEft->End();



#ifdef SHOW_SHADER_PROC
	VtxDUV1 pVtx[4];

	pVtx[0] = VtxDUV1( -1.0f,  1.0f,  0,	 0.f, 0.f);
	pVtx[1] = VtxDUV1( -0.4f,  1.0f,  0,	 1.f, 0.f);
	pVtx[2] = VtxDUV1( -0.4f,  0.4f,  0,	 1.f, 1.f);
	pVtx[3] = VtxDUV1( -1.0f,  0.4f,  0,	 0.f, 1.f);

	hr = m_pDev->SetTexture(0, m_pTexS);
	hr = m_pDev->SetFVF(VtxDUV1::FVF);
	hr = m_pDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, pVtx, sizeof(VtxDUV1));

	pVtx[0].p = VEC3( -1.0f,  0.4f,  0);
	pVtx[1].p = VEC3( -0.4f,  0.4f,  0);
	pVtx[2].p = VEC3( -0.4f, -0.2f,  0);
	pVtx[3].p = VEC3( -1.0f, -0.2f,  0);

	hr = m_pDev->SetTexture(0, m_pTexD);
	hr = m_pDev->SetFVF(VtxDUV1::FVF);
	hr = m_pDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, pVtx, sizeof(VtxDUV1));
#endif


	m_pDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	m_pDev->SetTexture(0, NULL);
	m_pDev->SetTexture(1, NULL);

	m_pDev->SetVertexDeclaration( NULL);
	m_pDev->SetVertexShader( NULL);
	m_pDev->SetPixelShader( NULL);


	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}



INT CShaderEx::Restore()
{
	m_pEft->OnResetDevice();

	return 0;
}


void CShaderEx::Invalidate()
{
	m_pEft->OnLostDevice();
}


void CShaderEx::SetSceneTexture(PDTX pTx)
{
	m_pTexS = pTx;
}

void CShaderEx::SetDistortionTexture(PDTX pTx)
{
	m_pTexD = pTx;
}