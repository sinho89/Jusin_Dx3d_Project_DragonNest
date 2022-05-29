/*!
 * \file StaticMesh.h
 * \date 2016/09/12 14:17
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

#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaticMesh(const CStaticMesh& rhs);

public:
	virtual ~CStaticMesh(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, OBJECTTYPE _eType);
	virtual void RenderMesh_ForSDK(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorldMatrix,OBJECTTYPE eType);
	virtual float GetGaussianDistribution( float x, float y, float rho );
	virtual void GetGaussianOffsets( bool bHorizontal, D3DXVECTOR2 vViewportTexelSize,
		D3DXVECTOR2* vSampleOffsets, float* fSampleWeights );
	virtual HRESULT Initialize();
	virtual HRESULT Progress(const D3DXMATRIX* world);
	virtual HRESULT ScreenGrab();
	virtual void SetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual DWORD Release(void);
	
public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pFileName, OBJECTTYPE _eType);

private:
	void MakeBoundingBox(void);
	bool CheckAlpha(int iSubsetIdx, const char* pFindTag);

public:
	LPD3DXMESH*	GetMesh();
private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	D3DMATERIAL9*			m_pMtrl;
	D3DXMATERIAL*			m_pSubset;

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pSubsetBuffer;/*D3DXMATERIAL*/
	DWORD					m_dwSubsetCnt;
};

END

#endif // StaticMesh_h__