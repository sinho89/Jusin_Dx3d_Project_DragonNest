#include "ShaderMgr.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(Engine::CShaderMgr)

Engine::CShaderMgr::CShaderMgr(void)
{

}

Engine::CShaderMgr::~CShaderMgr(void)
{

}

HRESULT Engine::CShaderMgr::AddShaderFiles(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pShaderKey , const TCHAR* pFilePath)
{
	CShader*		pShader = Find_Shader(pShaderKey);
	NULL_CHECK_RETURN(!pShader, E_FAIL);

	pShader = CShader::Create(pDevice, pFilePath);
	NULL_CHECK_RETURN(pShader, E_FAIL);

	m_mapShader.insert(MAPSHADER::value_type(pShaderKey, pShader));
	return S_OK;
}

Engine::CShader* Engine::CShaderMgr::Clone_Shader(const TCHAR* pShaderKey)
{
	CShader*	pShader = Find_Shader(pShaderKey);
	NULL_CHECK_RETURN(pShader, NULL);

	return pShader->Clone_Shader();
}

Engine::CShader* Engine::CShaderMgr::Find_Shader(const TCHAR* pShaderKey)
{
	MAPSHADER::iterator		iter = find_if(m_mapShader.begin(), m_mapShader.end(), CTagFinder(pShaderKey));
	if(iter == m_mapShader.end())
		return NULL;

	return iter->second;
}

void Engine::CShaderMgr::Release(void)
{
	MAPSHADER::iterator		iter = m_mapShader.begin();
	MAPSHADER::iterator		iter_end = m_mapShader.end();

	DWORD		dwRefCnt = 0;
	for( ; iter != iter_end; ++iter)
	{
		dwRefCnt = iter->second->Release();
		if(dwRefCnt == 0)
			Safe_Delete(iter->second);
	}
	m_mapShader.clear();
}

