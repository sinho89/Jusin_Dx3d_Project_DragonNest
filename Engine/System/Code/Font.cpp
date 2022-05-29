#include "Font.h"

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pFont(NULL)
{

}

Engine::CFont::~CFont(void)
{
	Release();
}

HRESULT Engine::CFont::Init_Font(const UINT& iHeight, 
								 const UINT& iWidth, 
								 const UINT& iWeight, 
								 const TCHAR* pFaceName)
{
	D3DXFONT_DESC		Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.Height = m_iHeight = iHeight;
	Font_Desc.Width  = m_iWidth  = iWidth;
	Font_Desc.Weight = iWeight;

	lstrcpy(Font_Desc.FaceName, pFaceName);
	Font_Desc.CharSet = HANGEUL_CHARSET;

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont));

	return S_OK;
}

void Engine::CFont::Render(const TCHAR* pString, 
						   D3DXCOLOR Color, 
						   const D3DXVECTOR3& vPos)
{
	RECT		rc = { 
		long(vPos.x), 
		long(vPos.y), 
		long(vPos.x) + m_iWidth * lstrlen(pString),
		long(vPos.y) + m_iHeight };

	m_pFont->DrawTextW(NULL, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
}

Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pDevice, 
									 const UINT& iHeight, 
									 const UINT& iWidth, 
									 const UINT& iWeight, 
									 const TCHAR* pFaceName)
{
	CFont*		pFont = new CFont(pDevice);

	if(FAILED(pFont->Init_Font(iHeight, iWidth, iWeight, pFaceName)))
		Engine::Safe_Delete(pFont);

	return pFont;
}

void Engine::CFont::Release(void)
{
	Engine::Safe_Release(m_pFont);
}

