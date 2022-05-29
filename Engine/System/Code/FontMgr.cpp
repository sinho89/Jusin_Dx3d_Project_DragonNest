#include "FontMgr.h"
#include "Engine_Include.h"
#include "Font.h"

IMPLEMENT_SINGLETON(Engine::CFontMgr)

Engine::CFontMgr::CFontMgr(void)
{

}

Engine::CFontMgr::~CFontMgr(void)
{
	Release();
}

HRESULT Engine::CFontMgr::AddFont(LPDIRECT3DDEVICE9 pDevice, 
								  const TCHAR* pFontKey, 
								  const UINT& iHeight, 
								  const UINT& iWidth, 
								  const UINT& iWeight)
{
	MAPFONT::iterator		iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontKey));

	if(iter != m_mapFont.end())
		return E_FAIL;

	CFont*		pFont = CFont::Create(pDevice, iHeight, iWidth, iWeight, pFontKey);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.insert(MAPFONT::value_type(pFontKey, pFont));

	return S_OK;
}

void Engine::CFontMgr::Render(const TCHAR* pFontKey, 
							  const D3DXVECTOR3& vPos, 
							  const TCHAR* pString, 
							  D3DXCOLOR Color)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontKey));

	if(iter == m_mapFont.end())
		return ;

	iter->second->Render(pString, Color, vPos);
}

void Engine::CFontMgr::Release(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

