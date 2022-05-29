#include "CubeTex.h"

Engine::CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CCubeTex::~CCubeTex(void)
{

}

Engine::CResources* Engine::CCubeTex::CloneResource(void)
{
	return new CCubeTex(*this);
}

HRESULT Engine::CCubeTex::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_CUBE;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 12;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXCUBE*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vTex = pVertex[0].vPos;

	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[1].vTex = pVertex[1].vPos;

	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[2].vTex = pVertex[2].vPos;

	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vTex = pVertex[3].vPos;


	pVertex[4].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vTex = pVertex[4].vPos;

	pVertex[5].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[5].vTex = pVertex[5].vPos;

	pVertex[6].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[6].vTex = pVertex[6].vPos;

	pVertex[7].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vTex = pVertex[7].vPos;

	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//x+
	pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
	pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;

	//x-
	pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
	pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;

	//y+
	pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
	pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;

	//y-
	pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
	pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;

	//z+
	pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
	pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;

	//z-
	pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
	pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

	m_pIB->Unlock();
	return S_OK;
}

Engine::CCubeTex* Engine::CCubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTex*	pBuffer = new CCubeTex(pDevice);
	if(FAILED(pBuffer->CreateBuffer()))
		Safe_Delete(pBuffer);

	return pBuffer;
}