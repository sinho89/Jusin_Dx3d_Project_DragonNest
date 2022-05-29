/*!
 * \file Mesh.h
 * \date 2016/09/12 14:07
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

#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"
#include "Engine_Include.h"

BEGIN(Engine)
class CShader;
class CVIBuffer;
class ENGINE_DLL CMesh
	: public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& rhs);

public:
	virtual ~CMesh(void);

public:
	void GetMinMax(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax);

public:
	void SetBoundingBoxColor(DWORD dwColor);

public:
	virtual CResources* CloneResource(void) PURE;

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, OBJECTTYPE _eType) PURE;
	virtual void RenderMesh_ForSDK(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorldMatrix, OBJECTTYPE eType)PURE;
	virtual float GetGaussianDistribution( float x, float y, float rho )PURE;
	virtual void GetGaussianOffsets( bool bHorizontal, D3DXVECTOR2 vViewportTexelSize,
		D3DXVECTOR2* vSampleOffsets, float* fSampleWeights )PURE;
	virtual HRESULT Initialize()PURE;
	virtual void SetDevice(LPDIRECT3DDEVICE9 pDevice)PURE;
	virtual HRESULT Progress(const D3DXMATRIX* world)PURE;
	virtual HRESULT ScreenGrab()PURE;

	LPD3DXEFFECT* GetEffect();
protected:
	D3DXVECTOR3		m_vMin, m_vMax;
	CVIBuffer*		m_pBoundingBox;
	CShader*		m_pShader;

	LPD3DXEFFECT	m_pEffect;
	ShadowInfo		m_tShadowInfo;
	bool			m_bFrameStart;
	LPD3DXMESH		m_pScene;
};

END

#endif // Mesh_h__