/*!
 * \file Shader.h
 * \date 2016/09/26 13:43
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

#ifndef Shader_h__
#define Shader_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader
	: public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);

public:
	virtual ~CShader(void);

public:
	LPD3DXEFFECT Get_EffectHandle(void) {return m_pEffect;}

public:
	CShader* Clone_Shader(void);

public:
	HRESULT Ready_ShaderFiles(const TCHAR* pFilePath);
	void Begin_Shader(const int& iPassIdx = 0);
	void End_Shader(void);
	void Render(void);

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);

public:
	virtual DWORD Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXEFFECT			m_pEffect;
	LPD3DXBUFFER			m_pErrBuffer;

private:
	DWORD		m_dwRefCnt;
};

END

#endif // Shader_h__