#include "TerrainTex.h"

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CTerrainTex::~CTerrainTex()
{
	
}	
Engine::CResources* Engine::CTerrainTex::CloneResource(void)
{
	return new CTerrainTex(*this);
}

HRESULT Engine::CTerrainTex::CreateBuffer(const WORD& wCntX, 
										  const WORD& wCntZ, 
										  const WORD& wItv)
{
	m_dwVtxSize  = sizeof(VTXTEX);
	m_dwVtxCnt   = wCntX * wCntZ;
	m_dwVtxFVF   = VTXFVF_TEX;

	m_dwIdxSize	 = sizeof(INDEX32);
	m_IdxFmt	 = D3DFMT_INDEX32;
	m_dwTriCnt	 = (wCntX - 1) * (wCntZ - 1) * 2;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	DWORD*		pdwPixel = LoadImage();

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int iIndex = 0;

	for(int z = 0; z < wCntZ; ++z)
	{
		for(int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;

			pVtxTex[iIndex].vPos    = D3DXVECTOR3(float(x) * wItv, 
				//0.f,
				(pdwPixel[iIndex] & 0x000000ff) / 5.f,
				float(z) * wItv);
			pVtxTex[iIndex].vNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
			pVtxTex[iIndex].vTexUV	= D3DXVECTOR2(x / (wCntX - 1.f), z / (wCntZ - 1.f));
		}
	}



	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0 , (void**)&pIndex, 0);

	int iTriCnt = 0;

	for(int z = 0; z < wCntZ - 1; ++z)
	{
		for(int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			// ¿À¸¥ÂÊ À§ Æú¸®°ï
			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;

			D3DXVECTOR3	vDest, vSour, vNormal;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPos - pVtxTex[pIndex[iTriCnt]._2].vPos;
	
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;
	
			++iTriCnt;

			// ¿ÞÂÊ ÇÏ´Ü Æú¸®°ï
			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[iTriCnt]._1].vPos - pVtxTex[pIndex[iTriCnt]._3].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._3].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;
		}
	}

	for(DWORD i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);


	m_pIB->Unlock();
	m_pVB->Unlock();
		
	Engine::Safe_Delete_Array(pdwPixel);
	
	return S_OK;
}

DWORD* Engine::CTerrainTex::LoadImage(void)
{
	HANDLE	hFile = NULL;
	DWORD	dwByte;

	hFile = CreateFile(L"../bin/Resources/Texture/Stage/Terrain/Height.bmp", 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		0);

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(fh), &dwByte, NULL);
	ReadFile(hFile, &ih, sizeof(ih), &dwByte, NULL);

	DWORD*	pdwPixel = new DWORD[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pdwPixel, sizeof(DWORD) * ih.biHeight * ih.biWidth, &dwByte, NULL);

	CloseHandle(hFile);

	return pdwPixel;
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice, 
												 const WORD& wCntX, 
												 const WORD& wCntZ, 
												 const WORD& wItv)
{
	CTerrainTex*		pTerrainTex = new CTerrainTex(pDevice);

	if(FAILED(pTerrainTex->CreateBuffer(wCntX, wCntZ, wItv)))
		Engine::Safe_Delete(pTerrainTex);

	return pTerrainTex;
}

