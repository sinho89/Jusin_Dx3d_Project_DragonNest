/*!
 * \file ShaderMgr.h
 * \date 2016/09/26 14:12
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CShaderMgr
{
public:
	DECLARE_SINGLETON(CShaderMgr)

private:
	CShaderMgr(void);
	~CShaderMgr(void);

public:
	HRESULT AddShaderFiles(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pShaderKey
		, const TCHAR* pFilePath);
	CShader* Clone_Shader(const TCHAR* pShaderKey);

private:
	CShader* Find_Shader(const TCHAR* pShaderKey);

public:
	void Release(void);

private:
	typedef	unordered_map<const TCHAR*, CShader*>		MAPSHADER;
	MAPSHADER		m_mapShader;
};

END

#endif // ShaderMgr_h__