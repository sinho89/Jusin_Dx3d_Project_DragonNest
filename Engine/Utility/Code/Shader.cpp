#include "Shader.h"

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pEffect(NULL)
, m_pErrBuffer(NULL)
, m_dwRefCnt(1)
{

}

Engine::CShader::CShader(const CShader& rhs)
: m_pDevice(rhs.m_pDevice)
, m_pEffect(rhs.m_pEffect)
, m_pErrBuffer(rhs.m_pErrBuffer)
, m_dwRefCnt(rhs.m_dwRefCnt)
{
	++m_dwRefCnt;
}

Engine::CShader::~CShader(void)
{

}

Engine::CShader* Engine::CShader::Clone_Shader(void)
{
	return new CShader(*this);
}

HRESULT Engine::CShader::Ready_ShaderFiles(const TCHAR* pFilePath)
{
	HRESULT		hr = D3DXCreateEffectFromFile(m_pDevice, pFilePath
		, NULL
		, NULL
		, D3DXSHADER_DEBUG
		, NULL
		, &m_pEffect
		, &m_pErrBuffer);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, (char*)m_pErrBuffer->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void Engine::CShader::Begin_Shader(const int& iPassIdx /*= 0*/)
{
	NULL_CHECK_RETURN(m_pEffect, );
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(iPassIdx);
}

void Engine::CShader::End_Shader(void)
{
	m_pEffect->EndPass();
	m_pEffect->End();
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath)
{
	CShader*	pShader = new CShader(pDevice);
	if(FAILED(pShader->Ready_ShaderFiles(pFilePath)))
		Safe_Delete(pShader);

	return pShader;
}

DWORD Engine::CShader::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Safe_Release(m_pEffect);
		Safe_Release(m_pErrBuffer);
		return 0;
	}
	else
	{
		--m_dwRefCnt;
	}
	return m_dwRefCnt;
}

