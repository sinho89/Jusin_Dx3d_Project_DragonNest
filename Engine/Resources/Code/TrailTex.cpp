#include "TrailTex.h"

Engine::CTrailTex::CTrailTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CTrailTex::~CTrailTex(void)
{

}

Engine::CResources* Engine::CTrailTex::CloneResource(void)
{
	/*CResources* pResource = new CRcTex(*this);

	pResource->AddRef();

	return pResource;*/

	return new CTrailTex(*this);
}

HRESULT Engine::CTrailTex::CreateBuffer(DWORD dwCnt)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCnt;
	m_dwVtxFVF = VTXFVF_TEX;
	m_vBeforePoint2 = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = dwCnt / 2;
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*			pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	for(DWORD i = 0; i < m_dwVtxCnt; ++i)
	{
		pVtxTex[i].vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pVtxTex[i].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[i].vTexUV = D3DXVECTOR2(0.f, 0.f);
	}

	m_pVB->Unlock();
	
	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int iIndex = 0;
	int iTriCnt = 0;

	for(DWORD i = 0; i < m_dwVtxCnt - 1; ++i)
	{
		pIndex[iTriCnt]._1 = i + m_dwVtxCnt;
		pIndex[iTriCnt]._2 = i + m_dwVtxCnt + 1;
		pIndex[iTriCnt]._3 = i + 1;

		++iTriCnt;

		pIndex[iTriCnt]._1 = iIndex + m_dwVtxCnt;
		pIndex[iTriCnt]._2 = iIndex + 1;
		pIndex[iTriCnt]._3 = iIndex;

		++iTriCnt;
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT Engine::CTrailTex::CreateBuffer(void)
{
	return S_OK;
}

Engine::CTrailTex* Engine::CTrailTex::Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwCnt)
{
	CTrailTex*		pRcTex = new CTrailTex(pDevice);

	if(FAILED(pRcTex->CreateBuffer(dwCnt)))	
		Engine::Safe_Delete(pRcTex);

	return pRcTex;
}

void Engine::CTrailTex::VertexUpdate(D3DXVECTOR3 vDestPoint, D3DXVECTOR3 vSourPoint)
{
	VTXTEX*			pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	for(DWORD i = 0; i < m_dwVtxCnt; ++i)
	{
		pVtxTex[1].vPos = vDestPoint;
		pVtxTex[1].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

		pVtxTex[2].vPos = vSourPoint;
		pVtxTex[2].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);
	}

	m_pVB->Unlock();


//	INDEX16*			pIndex = NULL;
//
//	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
//
//	/*pIndex[0]._1 = 0;
//	pIndex[0]._2 = 1;
//	pIndex[0]._3 = 2;
//
//	pIndex[1]._1 = 0;
//	pIndex[1]._2 = 2;
//	pIndex[1]._3 = 3;*/
//
//	m_pIB->Unlock();
}
