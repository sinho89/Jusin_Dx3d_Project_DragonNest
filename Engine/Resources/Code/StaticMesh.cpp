#include "StaticMesh.h"
#include "CubeColor.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_ppTexture(NULL)
, m_pMtrl(NULL)
, m_pMesh(NULL)
, m_pSubset(NULL)
, m_dwSubsetCnt(0)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: CMesh(rhs)
, m_ppTexture(rhs.m_ppTexture)
, m_pMtrl(rhs.m_pMtrl)
, m_pMesh(rhs.m_pMesh)
, m_pSubset(rhs.m_pSubset)
, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
{
	++m_dwRefCnt;
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

Engine::CResources* Engine::CStaticMesh::CloneResource(void)
{
	return new CStaticMesh(*this);
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, OBJECTTYPE _eType)
{
	HRESULT		hr = NULL;

	TCHAR		szFullPath[MAX_PATH] = L"";

	/*Texture/Tile/
	Tile%d.png*/
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice
		, NULL
		, &m_pSubsetBuffer
		, NULL
		, &m_dwSubsetCnt
		, &m_pMesh);
	FAILED_CHECK(hr);

	m_pSubset = ((D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer());

	m_pMtrl = new D3DMATERIAL9[m_dwSubsetCnt];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		m_pMtrl[i] = m_pSubset[i].MatD3D;

		lstrcpy(szFullPath, pPath);
		MultiByteToWideChar(CP_ACP
			, 0, m_pSubset[i].pTextureFilename
			, strlen(m_pSubset[i].pTextureFilename)
			, szFileName, 128);
		lstrcat(szFullPath, szFileName);

		hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
		FAILED_CHECK(hr);
	}

	MakeBoundingBox();
	return S_OK;
}

void Engine::CStaticMesh::RenderMesh_ForSDK(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorldMatrix, OBJECTTYPE eType)
{
	pEffect->Begin(NULL, 0);

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pDevice->SetTexture(0, m_ppTexture[i]);
		m_pDevice->SetMaterial(&m_pMtrl[i]);

		bool	isAlpha = CheckAlpha(i, "A");

		int		iPassIdx = 0;
		if(true == isAlpha)
			iPassIdx = 1;
		else
			iPassIdx = 0;

		if(eType == OBJ_ALPHA)
			iPassIdx = 1;

		pEffect->BeginPass(iPassIdx);
		pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)&m_pMtrl[i].Diffuse);
		pEffect->SetVector("g_vMtrlAmbient", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
		pEffect->SetVector("g_vMtrlSpecular", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
		pEffect->SetFloat("g_fPower", m_pMtrl[i].Power);

		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);
		pEffect->EndPass();
	}

	pEffect->End();

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if(pWorldMatrix != NULL)
		m_pDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);


	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

DWORD Engine::CStaticMesh::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);

		for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
			Safe_Release(m_ppTexture[i]);

		Safe_Delete_Array(m_ppTexture);
		Safe_Release(m_pSubsetBuffer);
		Safe_Release(m_pMesh);
		
		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pFileName, OBJECTTYPE _eType)
{
	CStaticMesh*	pMesh = new CStaticMesh(pDevice);
	if(FAILED(pMesh->LoadMeshFromFile(pFilePath, pFileName, _eType)))
		Safe_Delete(pMesh);

	return pMesh;
}

void Engine::CStaticMesh::MakeBoundingBox(void)
{
	DWORD		dwFVF = m_pMesh->GetFVF();

	if(dwFVF & D3DFVF_XYZ)
	{
		D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];
		m_pMesh->GetDeclaration(VtxFVF);

		int		iIndex = 0;
		DWORD	dwOffset = 0;

		while(true)
		{
			if(VtxFVF[iIndex].Usage == D3DDECLUSAGE_POSITION)
			{
				dwOffset = VtxFVF[iIndex].Offset;
				break;
			}
			++iIndex;
		}

		void*		pVtxInfo = NULL;

		m_pMesh->LockVertexBuffer(0, &pVtxInfo);

		BYTE*		pByteVtxInfo = (BYTE*)pVtxInfo;
		pVtxInfo = &pByteVtxInfo[dwOffset];

		D3DXComputeBoundingBox((D3DXVECTOR3*)pVtxInfo, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFVF)
			, &m_vMin, &m_vMax);

		m_pMesh->UnlockVertexBuffer();
	}

	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_vMin,m_vMax, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

bool Engine::CStaticMesh::CheckAlpha(int iSubsetIdx, const char* pFindTag)
{
	int		iLength = strlen(m_pSubset[iSubsetIdx].pTextureFilename);

	for(int i = 0; i < iLength; ++i)
	{
		if('.' == m_pSubset[iSubsetIdx].pTextureFilename[i])
		{
			if(*pFindTag == m_pSubset[iSubsetIdx].pTextureFilename[i - 1])
				return true;
		}
	}
	return false;
}

LPD3DXMESH* Engine::CStaticMesh::GetMesh()
{
	return &m_pMesh;
}

float Engine::CStaticMesh::GetGaussianDistribution(float x, float y, float rho)
{
	float g = 1.0f / sqrt( 2.0f * 3.141592654f * rho * rho );
	return g * exp( -(x * x + y * y) / (2 * rho * rho) );
}

void Engine::CStaticMesh::GetGaussianOffsets(bool bHorizontal, D3DXVECTOR2 vViewportTexelSize, D3DXVECTOR2* vSampleOffsets, float* fSampleWeights)
{
	fSampleWeights[0] = 1.0f * GetGaussianDistribution( 0, 0, 2.0f );
	vSampleOffsets[0] = D3DXVECTOR2( 0.0f, 0.0f );

	if( bHorizontal ) {
		for( int i = 1; i < 15; i += 2 ) {
			vSampleOffsets[i + 0] = D3DXVECTOR2(  i * vViewportTexelSize.x, 0.0f );
			vSampleOffsets[i + 1] = D3DXVECTOR2( -i * vViewportTexelSize.x, 0.0f );

			fSampleWeights[i + 0] = 2.0f * GetGaussianDistribution( float(i + 0), 0.0f, 3.0f );
			fSampleWeights[i + 1] = 2.0f * GetGaussianDistribution( float(i + 1), 0.0f, 3.0f );
		}
	}

	else {
		for( int i = 1; i < 15; i += 2 ) {
			vSampleOffsets[i + 0] = D3DXVECTOR2( 0.0f,  i * vViewportTexelSize.y );
			vSampleOffsets[i + 1] = D3DXVECTOR2( 0.0f, -i * vViewportTexelSize.y );

			fSampleWeights[i + 0] = 2.0f * GetGaussianDistribution( 0.0f, float(i + 0), 3.0f );
			fSampleWeights[i + 1] = 2.0f * GetGaussianDistribution( 0.0f, float(i + 1), 3.0f );
		}
	}
}

HRESULT Engine::CStaticMesh::Initialize()
{
	return S_OK;
}

HRESULT Engine::CStaticMesh::Progress(const D3DXMATRIX* world)
{
	return S_OK;

}

HRESULT Engine::CStaticMesh::ScreenGrab()
{
	return S_OK;

}

void Engine::CStaticMesh::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{

}

