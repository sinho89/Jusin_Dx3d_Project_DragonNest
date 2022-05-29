#include "Texture.h"

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
, m_dwContainerSize(0)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
: CResources(rhs.m_pDevice)
, m_vecTexture(rhs.m_vecTexture)
, m_dwContainerSize(rhs.m_dwContainerSize)
{
	++m_dwRefCnt;
}

Engine::CTexture::~CTexture(void)
{

}

Engine::CResources* Engine::CTexture::CloneResource(void)
{
	return new CTexture(*this);
}

HRESULT Engine::CTexture::LoadTexture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	IDirect3DBaseTexture9*		pTexture = NULL;

	if(wCnt == 0)
		return E_FAIL;

	TCHAR		szFullPath[MAX_PATH] = L"";

	for(size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, pFilePath, i);

		HRESULT		hr = NULL;
		switch(eTextureType)
		{
		case TEXTURE_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case TEXTURE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}
		FAILED_CHECK(hr);

		m_vecTexture.push_back(pTexture);
	}

	//이미지가 몇장인지 보관
	m_dwContainerSize = m_vecTexture.size();
	return S_OK;
}

void Engine::CTexture::Render(const DWORD& dwStage, const DWORD& iIndex)
{
	if(iIndex >= m_dwContainerSize)
		return;

	m_pDevice->SetTexture(dwStage, m_vecTexture[iIndex]);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pDevice , TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	CTexture*	pTexture = new CTexture(pDevice);
	if(FAILED(pTexture->LoadTexture(eTextureType, pFilePath, wCnt)))
	{
		MSG_BOX(L"pFilePath");
		Engine::Safe_Delete(pTexture);
	}
	return pTexture;
}

DWORD Engine::CTexture::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		DWORD		dwSize = m_vecTexture.size();
		for(DWORD i = 0; i < dwSize; ++i)
		{
			Engine::Safe_Release(m_vecTexture[i]);
		}
		m_vecTexture.clear();

		return 0;
	}
	else
		--m_dwRefCnt;

	return 0;
}

void Engine::CTexture::Set_ConstantTable(LPD3DXEFFECT pEffect, const char* pConstantName, const DWORD& dwCnt /*= 0*/)
{
	pEffect->SetTexture(pConstantName, m_vecTexture[dwCnt]);
}