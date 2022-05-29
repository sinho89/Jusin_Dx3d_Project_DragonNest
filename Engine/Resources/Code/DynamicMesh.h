/*!
 * \file DynamicMesh.h
 * \date 2016/09/19 13:14
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
#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CHierarchyLoader;
class CAniCtrl;
class ENGINE_DLL CDynamicMesh
	: public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CDynamicMesh(const CDynamicMesh& rhs);

public:
	virtual ~CDynamicMesh(void);

public:
	virtual CResources* CloneResource(void);
	DOUBLE GetTrackPos(void);
	DOUBLE GetPeriod(void);

public:
	void SetTrackPos(DOUBLE dPostion);
	void SetAnimationSet(UINT iAniIdx, FLOAT fWeight, DOUBLE dDuration);
	void FrameMove(const float& fTimePerSec);

	
public:
	const D3DXMATRIX* FindFrame(const char* pFrameName);
public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, OBJECTTYPE _eType);
	virtual void RenderMesh_ForSDK(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorldMatrix, OBJECTTYPE eType);
	virtual float GetGaussianDistribution( float x, float y, float rho );
	virtual void GetGaussianOffsets( bool bHorizontal, D3DXVECTOR2 vViewportTexelSize,
		D3DXVECTOR2* vSampleOffsets, float* fSampleWeights );
	virtual HRESULT Initialize();
	virtual HRESULT Progress(const D3DXMATRIX* world);
	virtual HRESULT ScreenGrab();
	virtual void SetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual DWORD Release(void);

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath
		, const TCHAR* pFileName, OBJECTTYPE _eType);

private:
	void UpdateFrameMatrix(Bone* bone, const D3DXMATRIX* pParentMatrix);
	void SetUpFrameMatrixPointer(Bone* bone);
	void Find_MeshContainer(Bone* pFrame, LPD3DXEFFECT pEffect);
	void Render_MeshContainer(Bone* bone, LPD3DXEFFECT pEffect, const D3DXMATRIX world, OBJECTTYPE eType);
	void Render_Skeleton(Bone* bone, Bone* parent, const D3DXMATRIX world);
	void Render_Shadow(Bone* bone,BoneMesh* boneMesh, LPD3DXBONECOMBINATION pBoneComb, LPD3DXEFFECT pEffect, OBJECTTYPE eType, int i, const D3DXMATRIX world);
	void Render_Bone(Bone* bone,BoneMesh* boneMesh, LPD3DXEFFECT pEffect, OBJECTTYPE eType, const D3DXMATRIX world);

public:
	LPD3DXFRAME*	GetRootBone();
private:
	void MakeBoundingSphere(Bone* bone);

private:
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
	LPD3DXFRAME				m_pRootBone;
	bool					m_SetTexture;
	LPD3DXMESH				m_pSphereMesh;
};

END

#endif // DynamicMesh_h__