#include "CubeColor.h"

Engine::CCubeColor::CCubeColor(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CCubeColor::~CCubeColor(void)
{

}

HRESULT Engine::CCubeColor::CreateBuffer(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, const DWORD& dwColor)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_COL;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 12;
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXCOL*			pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
	pVertex[0].dwColor = dwColor;

	pVertex[1].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
	pVertex[1].dwColor = dwColor;

	pVertex[2].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
	pVertex[2].dwColor = dwColor;

	pVertex[3].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
	pVertex[3].dwColor = dwColor;


	pVertex[4].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
	pVertex[4].dwColor = dwColor;

	pVertex[5].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
	pVertex[5].dwColor = dwColor;

	pVertex[6].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
	pVertex[6].dwColor = dwColor;

	pVertex[7].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
	pVertex[7].dwColor = dwColor;

	m_pVB->Unlock();


	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1; pIndex[0]._2 = 5; pIndex[0]._3 = 6;
	pIndex[1]._1 = 1; pIndex[1]._2 = 6; pIndex[1]._3 = 2;

	// -x									   
	pIndex[2]._1 = 4; pIndex[2]._2 = 0; pIndex[2]._3 = 3;
	pIndex[3]._1 = 4; pIndex[3]._2 = 3; pIndex[3]._3 = 7;


	// +y									   
	pIndex[4]._1 = 4; pIndex[4]._2 = 5; pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1; pIndex[5]._3 = 0;

	// -y									   
	pIndex[6]._1 = 3; pIndex[6]._2 = 2; pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6; pIndex[7]._3 = 7;


	// +z									   
	pIndex[8]._1 = 7; pIndex[8]._2 = 6; pIndex[8]._3 = 5;
	pIndex[9]._1 = 7; pIndex[9]._2 = 5; pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0; pIndex[10]._2 = 1; pIndex[10]._3 = 2;
	pIndex[11]._1 = 0; pIndex[11]._2 = 2; pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CResources* Engine::CCubeColor::CloneResource(void)
{
	return  new CCubeColor(*this);
}

Engine::CCubeColor* Engine::CCubeColor::Create(LPDIRECT3DDEVICE9 pDevice
											   , const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax
											   , const DWORD& dwColor)
{
	CCubeColor*		pBuffer = new CCubeColor(pDevice);

	if(FAILED(pBuffer->CreateBuffer(vMin, vMax, dwColor)))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}
