#include "stdafx.h"
#include "Water.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Stage.h"

#include "CollisionMgr.h"


CWater::CWater(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
{
	m_pDev		= pDevice;
	m_pFVF		= NULL;
	m_pEft		= NULL;

	m_pTxCbm	= NULL;
	m_pTxNor	= NULL;

	m_TileN		= 0;
	m_TileW		= 0;
	m_nVtx		= 0;
	m_nFce		= 0;

	m_pFce		= NULL;
	m_pVtx		= NULL;
}

CWater::~CWater(void)
{

}

HRESULT CWater::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_fDetail = 1.f;

	m_pInfo->m_vPos = D3DXVECTOR3(0.f, -10.f, -1500.f);

	/*D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, 100.f, 100.f, 100.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 50.f, 800.f);
	m_pInfo->m_matWorld =  matScale * matTrans;*/
	
	HRESULT	hr;

	HWND	hWnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;
	m_pDevice->GetCreationParameters(&ppm);
	hWnd	= ppm.hFocusWindow;

	DWORD dwFlags = 0;
#if defined( _DEBUG ) || defined( DEBUG )
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile( m_pDevice
		, L"../../Reference/Headers/ShaderWater.hpp"
		, NULL
		, NULL
		, dwFlags
		, NULL
		, &m_pEft
		, &pErr);

	if ( FAILED(hr) )
	{
		char sErr[2048]={0};
		if(pErr)
		{
			char* s=(char*)pErr->GetBufferPointer();
			sprintf(sErr, s);
		}
		else
			sprintf(sErr, "Cannot Compile Shader.");


		return -1;
	}

	DWORD	dFVF = Vtx::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE]={0};
	D3DXDeclaratorFromFVF(dFVF, vertex_decl);
	if( FAILED( hr = m_pDevice->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return -1;

	D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Texture/bump.bmp", &m_pTxNor);

	int x;
	int z;
	int n;
	INT	nVtxT;		// 축에 대한 버텍스 수

	VEC3	vcEps;


	m_TileN = 64;
	m_TileW = 16.f;

	vcEps = -VEC3(m_TileN * m_TileW/2, 30, m_TileN * m_TileW/2);

	nVtxT	= m_TileN+1;
	m_nVtx	= nVtxT * nVtxT;
	m_nFce	= 2* m_TileN * m_TileN;

	m_pVtx = new Vtx   [m_nVtx];
	m_pFce = new VtxIdx[m_nFce];

	// 1. 정점과 인덱스 구성

	//	1-----3
	//	.\    |
	//	.  \  |
	//	.    \|
	//	0-----2

	n=0;

	for(z=0; z<m_TileN; ++z)
	{
		for(x=0;x<m_TileN; ++x)
		{
			int _0 = nVtxT*(z+0) +x;
			int _1 = nVtxT*(z+1) +x;
			int _2 = nVtxT*(z+0) +x +1;
			int _3 = nVtxT*(z+1) +x +1;


			m_pFce[n] = VtxIdx(_0, _1, _2);
			++n;
			m_pFce[n] = VtxIdx(_3, _2, _1);
			++n;
		}
	}


	for(z=0; z<=m_TileN; ++z)
	{
		for(x=0;x<=m_TileN; ++x)
		{
			n = z * nVtxT + x;

			m_pVtx[n].p = VEC3( FLOAT(x), 0.F, FLOAT(z));
			m_pVtx[n].p *= m_TileW;
			m_pVtx[n].p +=vcEps;
		}
	}

	LPDIRECT3DSURFACE9	pSrf;
	D3DSURFACE_DESC		dscC;
	D3DSURFACE_DESC		dscD;

	if(FAILED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrf)))
		return-1;

	pSrf->GetDesc( &dscC);
	pSrf->Release();

	if(FAILED(m_pDevice->GetDepthStencilSurface(&pSrf)))
		return -1;

	pSrf->GetDesc(&dscD);
	pSrf->Release();


	hr = D3DXCreateRenderToEnvMap( m_pDevice, 512
		, 1, dscC.Format
		, TRUE, dscD.Format
		, &m_pRndEnv );

	if( FAILED( hr ) )
		return -1;

	hr = D3DXCreateCubeTexture( m_pDevice
		, 512
		, 1, D3DUSAGE_RENDERTARGET
		, dscC.Format
		, D3DPOOL_DEFAULT, &m_pTxCbm);



	if( FAILED( hr ) )
		return -1;

	//SetTexture(m_pTxCbm);

	return S_OK;
}


void CWater::T_SetupCubeViewMatrix(D3DXMATRIX* pOut, DWORD dwFace )
{
	D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vLookDir;
	D3DXVECTOR3 vUpDir;

	switch( dwFace )
	{
	case D3DCUBEMAP_FACE_POSITIVE_X:
		vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_X:
		vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	}

	// Set the view transform for this cubemap surface
	D3DXMatrixLookAtLH( pOut, &vEyePt, &vLookDir, &vUpDir );
}


void CWater::Update(void)
{
	INT		i;
	D3DXMATRIX mtViwCur;
	D3DXMATRIX mtPrjCur;

	m_pDevice->GetTransform(D3DTS_VIEW, &mtViwCur);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &mtPrjCur);


	D3DXMATRIX mtViw[6];	// the view matries for the cubemap surface
	D3DXMATRIX mtPrj;		// for Env


	D3DXMatrixPerspectiveFovLH( &mtPrj, D3DX_PI/2., 1.0F, 1.0F, 5000.0f );


	// Set the view transform for this cubemap surface
	for(i=0; i<6; ++i)
	{
		T_SetupCubeViewMatrix(&mtViw[i], (D3DCUBEMAP_FACES) i );
		mtViw[i] = mtViwCur * mtViw[i];
	}


	// Rendering to cubemap surface
	m_pRndEnv->BeginCube(m_pTxCbm);
	{
		for(i=0; i<6; ++i)
		{
			m_pRndEnv->Face( (D3DCUBEMAP_FACES) i, 0 );

			m_pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF006699, 1.0f, 0L);
			m_pDevice->SetTransform( D3DTS_VIEW, &mtViw[i] );
			m_pDevice->SetTransform( D3DTS_PROJECTION, &mtPrj );

			Render();
		}
	}
	m_pRndEnv->End(0);


	m_pDevice->SetTransform(D3DTS_VIEW, &mtViwCur);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &mtPrjCur);

	Engine::CGameObject::Update();

	//Compute_ViewZ(&m_pInfo->m_vPos);
}

void CWater::Render(void)
{

	INT		hr=0;
	MATA	mtWld = m_pInfo->m_matWorld;
	MATA	mtViw, mtPrj;
	FLOAT	fTime;

	//MATA	mtViw, mtPrj;

	fTime = Engine::Get_TimeMgr()->GetTime() * 100;

	m_pDevice->GetTransform(D3DTS_VIEW, &mtViw);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &mtPrj);

	m_pDevice->SetVertexDeclaration(m_pFVF);

	hr = m_pEft->SetMatrix( "m_mtWld", &mtWld);
	hr = m_pEft->SetMatrix( "m_mtViw", &mtViw);
	hr = m_pEft->SetMatrix( "m_mtPrj", &mtPrj);
	hr = m_pEft->SetFloat ( "m_fTime", fTime);
	hr = m_pEft->SetTexture( "m_TxCbm", m_pTxCbm);
	hr = m_pEft->SetTexture( "m_TxNor", m_pTxNor);

	m_pEft->SetTechnique("Tech0");

	m_pEft->Begin(NULL, 0);
	m_pEft->BeginPass(0);

	m_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0
		, m_nVtx, m_nFce
		, m_pFce, (D3DFORMAT)VtxIdx::FVF
		, m_pVtx, sizeof(Vtx) );

	m_pEft->EndPass();
	m_pEft->End();

	m_pDevice->SetVertexDeclaration(NULL);
	m_pDevice->SetVertexShader(NULL);
	m_pDevice->SetPixelShader(NULL);

}

HRESULT CWater::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_HarfMoon");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CWater* CWater::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CWater*		pGameObject = new CWater(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

CWater* CWater::Create(LPDIRECT3DDEVICE9 pDevice , D3DXVECTOR3 _vPos, float _fAngle)
{
	CWater*		pGameObject = new CWater(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	pGameObject->SetInfo(_vPos, _fAngle);
	return pGameObject;
}

HRESULT CWater::SetConstantTable(void)
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
	D3DLIGHT9*	pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}

void CWater::SetInfo(D3DXVECTOR3 _vPos, float	_fAngle)
{
	m_pInfo->m_vPos = _vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(_fAngle);
}

void CWater::SetType(OBTAINTYPE _eType)
{
}

void CWater::SetTexture(PDTC pTex)
{
	m_pTxCbm = pTex;
}