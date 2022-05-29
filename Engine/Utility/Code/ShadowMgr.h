/*!
 * \file ShadowMgr.h
 * \date 2016/10/23 14:58
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ShadowMgr_h__
#define ShadowMgr_h__


#include "Engine_Include.h"

BEGIN(Engine)
class CShader;
class ENGINE_DLL CShadowMgr
{
public:
	DECLARE_SINGLETON(CShadowMgr)
private:
	CShadowMgr(void);
	~CShadowMgr(void);

public:
	void SetDevice(LPDIRECT3DDEVICE9 pDevice);
	void SetEffect();

public:
	void Create_Render_Target(const int _iW, const int _iH);
	void Start_Render_Target();
	void End_Render_Target();
	void Release();

public:
	void SetCamera();

private:
	D3DXMATRIX	m_matLightView;
	D3DXMATRIX	m_matLightProj;

	D3DXVECTOR3	m_vCamDir;
	D3DXVECTOR3	m_vCamPos;

	D3DXVECTOR3	m_vLightAt;
	D3DXVECTOR3	m_vLightPos;
	D3DXVECTOR3	m_vLightDir;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DTEXTURE9		m_pRenderTargetTex;
	LPDIRECT3DSURFACE9		m_pRenderTargetSurf;
	LPDIRECT3DSURFACE9		m_pOrigSurf;
	LPDIRECT3DSURFACE9		m_pOrigStencil;

private:
	CShader*				m_pShader;
	CShader*				m_pMappingShader;
	LPD3DXEFFECT			m_pEffect;
	LPD3DXEFFECT			m_pMappingEffect;
};
END

#endif // ShadowMgr_h__