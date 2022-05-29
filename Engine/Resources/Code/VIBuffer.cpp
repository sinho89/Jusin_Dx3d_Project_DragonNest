#include "VIBuffer.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
,m_pVB(NULL)
,m_dwVtxSize(0)
,m_dwVtxCnt(0)
,m_dwVtxFVF(0)
,m_pIB(NULL)
,m_dwIdxSize(0)
,m_dwTriCnt(0)
,m_IdxFmt(D3DFMT_UNKNOWN)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
: CResources(rhs.m_pDevice)
, m_pVB(rhs.m_pVB)
, m_dwVtxSize(rhs.m_dwVtxSize)
, m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwVtxFVF(rhs.m_dwVtxFVF)
, m_pIB(rhs.m_pIB)
, m_dwIdxSize(rhs.m_dwIdxSize)
, m_dwTriCnt(rhs.m_dwTriCnt)
, m_IdxFmt(rhs.m_IdxFmt)
{
	++m_dwRefCnt;
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void Engine::CVIBuffer::GetVtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pVtxInfo, pOriVtx, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetVtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;
	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pOriVtx, pVtxInfo, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetIdxInfo(void* pIdxInfo, const DWORD* pTriCnt)
{
	void* pOriIndex = NULL;

	m_dwTriCnt = *pTriCnt;

	m_pIB->Lock(0, 0, &pOriIndex , 0);
	memcpy(pOriIndex, pIdxInfo, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	HRESULT		hr = NULL;
	hr = m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"VertexBuffer Create Failed");

	hr = m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"IndexBuffer Create Failed");
	return S_OK;
}

HRESULT Engine::CVIBuffer::CreateBuffer(DWORD dwVertexCnt)
{
	return S_OK;
}

void Engine::CVIBuffer::Render(const D3DXMATRIX* pmatWorld)
{
	if(pmatWorld != NULL)
		m_pDevice->SetTransform(D3DTS_WORLD, pmatWorld);

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

DWORD Engine::CVIBuffer::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Engine::Safe_Release(m_pVB);
		Engine::Safe_Release(m_pIB);

		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

void Engine::CVIBuffer::Change_BoxColor(DWORD dwColor)
{
	VTXCOL*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	for(int i = 0; i < 8; ++i)
	{
		pVertex[i].dwColor = dwColor;
	}
	m_pVB->Unlock();
}
