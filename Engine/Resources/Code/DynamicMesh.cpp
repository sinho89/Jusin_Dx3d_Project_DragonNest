#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"
#include "CubeColor.h"
#include "Component.h"
#include "Shader.h"
#include "CameraMgr.h"
#include "Export_Function.h"
#include "Engine_Include.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_pRootBone(NULL)
, m_pLoader(NULL)
, m_pAniCtrl(NULL)
{
	memset(&m_tShadowInfo, 0, sizeof(ShadowInfo));

	m_tShadowInfo.m_bDisplayStats = TRUE;
	m_tShadowInfo.m_bSoftShadows = TRUE;
	m_tShadowInfo.m_bFiltered = TRUE;
	m_SetTexture = false;
}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
: CMesh(rhs)
, m_pRootBone(rhs.m_pRootBone)
, m_pLoader(rhs.m_pLoader)
{
	memset(&m_tShadowInfo, 0, sizeof(ShadowInfo));

	m_tShadowInfo.m_bDisplayStats = TRUE;
	m_tShadowInfo.m_bSoftShadows = TRUE;
	m_tShadowInfo.m_bFiltered = TRUE;
	m_SetTexture = false;
	m_pAniCtrl = new CAniCtrl(*(rhs.m_pAniCtrl));
	++m_dwRefCnt;
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{
	
}

Engine::CResources* Engine::CDynamicMesh::CloneResource(void)
{
	return new CDynamicMesh(*this);
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, OBJECTTYPE _eType)
{
	D3DVERTEXELEMENT9 dwElement[] =
	{
		{ 0,   0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

		{ 0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },

		{ 0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		D3DDECL_END()
	};

	TCHAR		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	/*ID3DXAllocateHierarchy*/
	m_pLoader = new CHierarchyLoader(m_pDevice, pPath);
	m_pAniCtrl = new CAniCtrl;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(szFullPath
		, D3DXMESH_MANAGED, m_pDevice
		, m_pLoader
		, NULL
		, &m_pRootBone
		, m_pAniCtrl->GetAniCtrl());
	FAILED_CHECK(hr);

	LPD3DXMESH pTempScene = NULL;
	D3DXLoadMeshFromX( szFullPath, D3DXMESH_32BIT, m_pDevice,
		NULL, NULL, NULL, NULL, &pTempScene );

	pTempScene->CloneMesh( D3DXMESH_32BIT, dwElement, m_pDevice, &m_pScene );

	Safe_Release(pTempScene);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((Bone*)m_pRootBone, &matIdentity);
	SetUpFrameMatrixPointer((Bone*)m_pRootBone);

	//MakeBoundingSphere((Bone*)m_pRootBone);

	return S_OK;
}

DWORD Engine::CDynamicMesh::Release(void)
{
	--m_dwRefCnt;

	if(m_dwRefCnt == 0)
	{
		m_pLoader->DestroyFrame(m_pRootBone);
		Safe_Delete(m_pLoader);
		Safe_Delete(m_pAniCtrl);

		return 0;
	}
	else
	{
		Safe_Delete(m_pAniCtrl);
		--m_dwRefCnt;
	}

	return m_dwRefCnt;
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath , const TCHAR* pFileName, OBJECTTYPE _eType)
{
	CDynamicMesh*		pMesh = new CDynamicMesh(pDevice);
	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName, _eType)))
		Safe_Delete(pMesh);

	return pMesh;
}

void Engine::CDynamicMesh::RenderMesh_ForSDK(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorldMatrix, OBJECTTYPE eType)
{
	//Progress(pWorldMatrix);
	Render_MeshContainer((Bone*)m_pRootBone, pEffect, *pWorldMatrix, eType);

	//m_pDevice->GetRenderTarget( 0, &m_tShadowInfo.m_pOldColorRT );
	//m_pDevice->GetDepthStencilSurface( &m_tShadowInfo.m_pOldDepthRT );

	//// Render the scene depth to the shadow map
	//m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pShadowSurf );
	//m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pShadowDepth );

	//// Clear the viewport
	//m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );

	//// Set the technique
	//m_pEffect->SetTechnique( "techShadow" );

	//m_pEffect->Begin( NULL, NULL );
	//m_pEffect->BeginPass( 0 );

	//Render_MeshContainer((Bone*)m_pRootBone, pEffect, *pWorldMatrix, eType);

	//m_pEffect->EndPass();
	//m_pEffect->End();

	//m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pScreenSurf );
	//m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pNewDepthRT );

	//m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

	//m_pEffect->SetTechnique( "techUnlit" );

	//m_pEffect->SetTexture( "tShadowMap", m_tShadowInfo.m_pShadowMap );

	//m_pEffect->Begin( NULL, NULL );
	//m_pEffect->BeginPass( 0 );

	//Render_MeshContainer((Bone*)m_pRootBone, pEffect, *pWorldMatrix, eType);

	//m_pEffect->EndPass();
	//m_pEffect->End();

	//m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pBlurSurf[0] );
	//m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pNewDepthRT );

	//m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

	//m_pEffect->SetTechnique( "techBlurH" );

	//GetGaussianOffsets(TRUE, D3DXVECTOR2(1.0f / (FLOAT)SHADOW_MAP_SIZE, 1.0f / (FLOAT)SHADOW_MAP_SIZE),
	//	m_tShadowInfo.m_vSampleOffsets, m_tShadowInfo.m_fSampleWeights);
	//m_pEffect->SetValue("g_vSampleOffsets", m_tShadowInfo.m_vSampleOffsets, 15 * sizeof(D3DXVECTOR2));
	//m_pEffect->SetValue("g_fSampleWeights", m_tShadowInfo.m_fSampleWeights, 15 * sizeof(FLOAT));

	//m_pEffect->SetTexture( "tScreenMap", m_tShadowInfo.m_pScreenMap );

	//m_pEffect->Begin( NULL, NULL );
	//m_pEffect->BeginPass( 0 );

	//m_pDevice->SetStreamSource( 0, m_tShadowInfo.m_pQuadVB, 0, sizeof(QuadVertex) );
	//m_pDevice->SetFVF( FVF_QUADVERTEX );
	//m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	//m_pEffect->EndPass();
	//m_pEffect->End();

	//m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pBlurSurf[1] );
	//m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pNewDepthRT );

	//m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

	//m_pEffect->SetTechnique( "techBlurV" );

	//GetGaussianOffsets(FALSE, D3DXVECTOR2(1.0f / (FLOAT)SHADOW_MAP_SIZE, 1.0f / (FLOAT)SHADOW_MAP_SIZE),
	//	m_tShadowInfo.m_vSampleOffsets, m_tShadowInfo.m_fSampleWeights);
	//m_pEffect->SetValue("g_vSampleOffsets", m_tShadowInfo.m_vSampleOffsets, 15 * sizeof(D3DXVECTOR2));
	//m_pEffect->SetValue("g_fSampleWeights", m_tShadowInfo.m_fSampleWeights, 15 * sizeof(FLOAT));

	//m_pEffect->SetTexture( "tBlurHMap", m_tShadowInfo.m_pBlurMap[0] );

	//m_pEffect->Begin( NULL, NULL );
	//m_pEffect->BeginPass( 0 );

	//m_pDevice->SetStreamSource( 0, m_tShadowInfo.m_pQuadVB, 0, sizeof(QuadVertex) );
	//m_pDevice->SetFVF( FVF_QUADVERTEX );
	//m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	//m_pEffect->EndPass();
	//m_pEffect->End();

	//m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pOldColorRT );
	//m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pOldDepthRT );
	//Safe_Release( m_tShadowInfo.m_pOldColorRT );
	//Safe_Release( m_tShadowInfo.m_pOldDepthRT );

	//m_pEffect->SetTechnique( "techScene" );

	//m_pEffect->SetTexture( "tBlurVMap", m_tShadowInfo.m_pBlurMap[1] );

	//m_pEffect->Begin( NULL, NULL );
	//m_pEffect->BeginPass( 0 );

	//m_pEffect->CommitChanges();
	//Render_MeshContainer((Bone*)m_pRootBone, pEffect, *pWorldMatrix, eType);

	//m_pEffect->EndPass();
	//m_pEffect->End();
	

	
}

void Engine::CDynamicMesh::UpdateFrameMatrix(Bone* bone, const D3DXMATRIX* pParentMatrix)
{
	if(bone == NULL)
		return;

	D3DXMatrixMultiply(&bone->CombinedTransformationMatrix,
					&bone->TransformationMatrix,
					pParentMatrix);

	if(bone->pFrameSibling)
		UpdateFrameMatrix((Bone*)bone->pFrameSibling, pParentMatrix);
	if(bone->pFrameFirstChild)
		UpdateFrameMatrix((Bone*)bone->pFrameFirstChild, &bone->CombinedTransformationMatrix);
}

void Engine::CDynamicMesh::SetUpFrameMatrixPointer(Bone* bone)
{
	if(bone->pMeshContainer != NULL)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;

		if(boneMesh->pSkinInfo != NULL)
		{
			int NumBones = boneMesh->pSkinInfo->GetNumBones();

			boneMesh->boneMatrixPtrs = new D3DXMATRIX*[NumBones];

			for(int i = 0; i < NumBones; i++)
			{
				Bone *b = (Bone*)D3DXFrameFind(m_pRootBone,
												boneMesh->pSkinInfo->GetBoneName(i));

				if(b != NULL)
				{
					boneMesh->boneMatrixPtrs[i] = &b->CombinedTransformationMatrix;
				}
				else
				{
					boneMesh->boneMatrixPtrs[i] = NULL;
				}
			}
		}
	}

	if(bone->pFrameSibling != NULL)
		SetUpFrameMatrixPointer((Bone*)bone->pFrameSibling);
	if(bone->pFrameFirstChild != NULL)
		SetUpFrameMatrixPointer((Bone*)bone->pFrameFirstChild);
}

void Engine::CDynamicMesh::Find_MeshContainer(Bone* pFrame, LPD3DXEFFECT pEffect)
{

	if( m_tShadowInfo.m_bSoftShadows )
	{
		m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pScreenSurf );
		m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pNewDepthRT );

		m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

		m_pEffect->SetTechnique( "techUnlit" );

		m_pEffect->SetTexture( "tShadowMap", m_tShadowInfo.m_pShadowMap );

		m_pEffect->Begin( NULL, NULL );
		m_pEffect->BeginPass( 0 );


		m_pEffect->EndPass();
		m_pEffect->End();

		m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pBlurSurf[0] );
		m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pNewDepthRT );

		m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

		m_pEffect->SetTechnique( "techBlurH" );

		GetGaussianOffsets(TRUE, D3DXVECTOR2(1.0f / (FLOAT)SHADOW_MAP_SIZE, 1.0f / (FLOAT)SHADOW_MAP_SIZE),
			m_tShadowInfo.m_vSampleOffsets, m_tShadowInfo.m_fSampleWeights);
		m_pEffect->SetValue("g_vSampleOffsets", m_tShadowInfo.m_vSampleOffsets, 15 * sizeof(D3DXVECTOR2));
		m_pEffect->SetValue("g_fSampleWeights", m_tShadowInfo.m_fSampleWeights, 15 * sizeof(FLOAT));

		m_pEffect->SetTexture( "tScreenMap", m_tShadowInfo.m_pScreenMap );

		m_pEffect->Begin( NULL, NULL );
		m_pEffect->BeginPass( 0 );

		m_pDevice->SetStreamSource( 0, m_tShadowInfo.m_pQuadVB, 0, sizeof(QuadVertex) );
		m_pDevice->SetFVF( FVF_QUADVERTEX );
		m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		m_pEffect->EndPass();
		m_pEffect->End();

		m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pBlurSurf[1] );
		m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pNewDepthRT );

		m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

		m_pEffect->SetTechnique( "techBlurV" );

		GetGaussianOffsets(FALSE, D3DXVECTOR2(1.0f / (FLOAT)SHADOW_MAP_SIZE, 1.0f / (FLOAT)SHADOW_MAP_SIZE),
			m_tShadowInfo.m_vSampleOffsets, m_tShadowInfo.m_fSampleWeights);
		m_pEffect->SetValue("g_vSampleOffsets", m_tShadowInfo.m_vSampleOffsets, 15 * sizeof(D3DXVECTOR2));
		m_pEffect->SetValue("g_fSampleWeights", m_tShadowInfo.m_fSampleWeights, 15 * sizeof(FLOAT));

		m_pEffect->SetTexture( "tBlurHMap", m_tShadowInfo.m_pBlurMap[0] );

		m_pEffect->Begin( NULL, NULL );
		m_pEffect->BeginPass( 0 );

		m_pDevice->SetStreamSource( 0, m_tShadowInfo.m_pQuadVB, 0, sizeof(QuadVertex) );
		m_pDevice->SetFVF( FVF_QUADVERTEX );
		m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		m_pEffect->EndPass();
		m_pEffect->End();

		m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pOldColorRT );
		m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pOldDepthRT );
		Safe_Release( m_tShadowInfo.m_pOldColorRT );
		Safe_Release( m_tShadowInfo.m_pOldDepthRT );

		m_pEffect->SetTechnique( "techScene" );

		m_pEffect->SetTexture( "tBlurVMap", m_tShadowInfo.m_pBlurMap[1] );
	
		m_pEffect->Begin( NULL, NULL );
		m_pEffect->BeginPass( 0 );

		m_pEffect->CommitChanges();

		m_pEffect->EndPass();
		m_pEffect->End();
	}
	else
	{
		m_pDevice->SetRenderTarget( 0, m_tShadowInfo.m_pOldColorRT );
		m_pDevice->SetDepthStencilSurface( m_tShadowInfo.m_pOldDepthRT );
		Safe_Release( m_tShadowInfo.m_pOldColorRT );
		Safe_Release( m_tShadowInfo.m_pOldDepthRT );

		m_pEffect->SetTechnique( "techSceneHard" );

		m_pEffect->SetTexture( "tShadowMap", m_tShadowInfo.m_pShadowMap );

		m_pEffect->Begin( 0, 0 );

		m_pEffect->BeginPass( 0 );

		m_SetTexture = true;

		m_pEffect->EndPass();

		m_pEffect->End();
	}
	
}

void Engine::CDynamicMesh::Render_MeshContainer(Bone* bone, LPD3DXEFFECT pEffect, const D3DXMATRIX world, OBJECTTYPE eType)
{
	if(bone == NULL)
		bone = (Bone*)m_pRootBone;

	if(eType == OBJ_BOSS)
	{
	}

	if(bone->pMeshContainer != NULL)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;

		/*if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_hair.tga")
			m_pEffect->SetInt("i_SampleChoice", 0);
		if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_head.tga")
			m_pEffect->SetInt("i_SampleChoice", 1);
		if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_body.tga")
			m_pEffect->SetInt("i_SampleChoice", 2);
		if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_gauntlet.tga")
			m_pEffect->SetInt("i_SampleChoice", 3);
		if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_glove.tga")
			m_pEffect->SetInt("i_SampleChoice", 4);
		if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_leg.tga")
			m_pEffect->SetInt("i_SampleChoice", 5);
		if(boneMesh->wstrTextureName.c_str() == L"npc_wacolosseum_boots.tga")
			m_pEffect->SetInt("i_SampleChoice", 6);*/

		Render_Bone(bone, boneMesh, pEffect, eType, world);
		
	}


	if(bone->pFrameSibling != NULL)
		Render_MeshContainer((Bone*)bone->pFrameSibling, pEffect, world, eType);
	if(bone->pFrameFirstChild != NULL)
		Render_MeshContainer((Bone*)bone->pFrameFirstChild, pEffect, world, eType);
	
}
void Engine::CDynamicMesh::Render_Bone(Bone* bone, BoneMesh* boneMesh, LPD3DXEFFECT pEffect, OBJECTTYPE eType, const D3DXMATRIX world)
{
	if(boneMesh->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION pBoneComb =
			reinterpret_cast<LPD3DXBONECOMBINATION>(boneMesh->pBoneCombinationBuf->GetBufferPointer());

		for(int i = 0; i < (int)boneMesh->NumAttributeGroups; ++i)
		{
			for(DWORD iPaletteEntry = 0; iPaletteEntry < boneMesh->NumPaletteEntries; ++iPaletteEntry)
			{
				int iMatrixIndex = pBoneComb[i].BoneId[iPaletteEntry];
				if(iMatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply(&boneMesh->currentBoneMatrices[iPaletteEntry],
						&boneMesh->boneOffsetMatrices[iMatrixIndex],
						boneMesh->boneMatrixPtrs[iMatrixIndex]);
				}
			}

			Render_Shadow(bone, boneMesh, pBoneComb, pEffect, eType, i, world);
		}
	}
}
void Engine::CDynamicMesh::Render_Shadow(Bone* bone, BoneMesh* boneMesh, LPD3DXBONECOMBINATION pBoneComb, LPD3DXEFFECT pEffect, OBJECTTYPE eType, int i, const D3DXMATRIX world)
{
	pEffect->SetInt("NumBoneInfluence", boneMesh->NumInfl);
	pEffect->SetMatrixArray("MatrixPalette", boneMesh->currentBoneMatrices, boneMesh->NumPaletteEntries);

	int mtrlIndex = pBoneComb[i].AttribId;

	pEffect->Begin(NULL, NULL);

	if(eType == OBJ_GOBLIN
		|| eType == OBJ_ORGE
		|| eType == OBJ_BOSS
		|| eType == OBJ_PLAYER)
		pEffect->BeginPass(3);
	else
		pEffect->BeginPass(2);

	//m_pDevice->SetMaterial(&boneMesh->pMaterials[mtrlIndex].MatD3D);
	//m_pDevice->SetTexture(0, boneMesh->ppTexture[mtrlIndex]);

	pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)(&boneMesh->pMaterials[mtrlIndex].MatD3D.Diffuse));
	pEffect->SetVector("g_vMtrlAmbient", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetVector("g_vMtrlSpecular", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("g_fPower", boneMesh->pMaterials[mtrlIndex].MatD3D.Power);

	pEffect->SetTexture("g_BaseTexture", boneMesh->ppTexture[mtrlIndex]);

	pEffect->CommitChanges();
	boneMesh->MeshData.pMesh->DrawSubset(mtrlIndex);
	//m_pSphereMesh->DrawSubset(mtrlIndex);

	/*D3DXMATRIX r, s, t;
	D3DXMatrixRotationYawPitchRoll(&r, -D3DX_PI * 0.5f, 0.0f, 0.0f);

	s = r * bone->CombinedTransformationMatrix * world;
	D3DXMatrixTranslation(&t, s(3, 0), s(3, 1), s(3, 2));
	m_pDevice->SetTransform(D3DTS_WORLD, &t);
	
	*/
	pEffect->EndPass();
	pEffect->End();
}

void Engine::CDynamicMesh::Render_Skeleton(Bone* bone, Bone* parent, const D3DXMATRIX world)
{
	if(bone == NULL) bone = (Bone*)m_pRootBone;

	D3DXMATRIX r, s, t;
	D3DXMatrixRotationYawPitchRoll(&r, -D3DX_PI * 0.5f, 0.0f, 0.0f);

	if(parent != NULL && bone->Name != NULL && parent->Name != NULL)
	{
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		s = r * bone->CombinedTransformationMatrix * world;
		D3DXMatrixTranslation(&t, s(3, 0), s(3, 1), s(3, 2));
		m_pDevice->SetTransform(D3DTS_WORLD, &t);
		m_pSphereMesh->DrawSubset(0);

		D3DXMATRIX w1 = bone->CombinedTransformationMatrix;
		D3DXMATRIX w2 = parent->CombinedTransformationMatrix;

		D3DXVECTOR3 thisBone = D3DXVECTOR3(w1(3, 0), w1(3, 1), w1(3, 2));
		D3DXVECTOR3 parentBone = D3DXVECTOR3(w2(3, 0), w2(3, 1), w2(3, 2));

		m_pDevice->SetTransform(D3DTS_WORLD, &world);
		VTXCOL vert[] = {VTXCOL(parentBone, 0xffff0000), VTXCOL(thisBone, 0xff00ff00)};
		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetFVF(VTXFVF_COL);
		m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &vert[0], sizeof(VTXCOL));
	}

	if(bone->pFrameSibling)
		Render_Skeleton((Bone*)bone->pFrameSibling, parent, world);
	if(bone->pFrameFirstChild)
		Render_Skeleton((Bone*)bone->pFrameFirstChild, bone, world);
}


void Engine::CDynamicMesh::SetAnimationSet(UINT iAniIdx, FLOAT fWeight, DOUBLE dDuration)
{
	m_pAniCtrl->SetAnimationSet(iAniIdx, fWeight, dDuration);
}

void Engine::CDynamicMesh::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->FrameMove(fTimePerSec);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	UpdateFrameMatrix((Bone*)m_pRootBone, &matIdentity);
	//SetUpFrameMatrixPointer((Bone*)m_pRootBone);
}

DOUBLE Engine::CDynamicMesh::GetTrackPos(void)
{
	return m_pAniCtrl->GetTrackPos();
}

DOUBLE Engine::CDynamicMesh::GetPeriod(void)
{
	return m_pAniCtrl->GetPeriod();
}

void Engine::CDynamicMesh::SetTrackPos(DOUBLE dPostion)
{
	m_pAniCtrl->SetTrackPos(dPostion);
}

void Engine::CDynamicMesh::MakeBoundingSphere(Bone* bone)
{
	D3DXCreateSphere(m_pDevice, 10.f, 10, 10, &m_pSphereMesh, NULL);
}

const D3DXMATRIX* Engine::CDynamicMesh::FindFrame(const char* pFrameName)
{
	Bone*		pFrame = ((Bone*)D3DXFrameFind(m_pRootBone, pFrameName));

	return &pFrame->CombinedTransformationMatrix;
}

float Engine::CDynamicMesh::GetGaussianDistribution(float x, float y, float rho)
{
	float g = 1.0f / sqrt( 2.0f * 3.141592654f * rho * rho );
	return g * exp( -(x * x + y * y) / (2 * rho * rho) );
}

void Engine::CDynamicMesh::GetGaussianOffsets(bool bHorizontal, D3DXVECTOR2 vViewportTexelSize, D3DXVECTOR2* vSampleOffsets, float* fSampleWeights)
{
	fSampleWeights[0] = 1.0f * GetGaussianDistribution( 0, 0, 2.0f );
	vSampleOffsets[0] = D3DXVECTOR2( 0.0f, 0.0f );

	if( bHorizontal ) {
		for( int i = 1; i < 15; i += 2 ) {
			vSampleOffsets[i + 0] = D3DXVECTOR2(  i * vViewportTexelSize.x, 0.0f );
			vSampleOffsets[i + 1] = D3DXVECTOR2( -i * vViewportTexelSize.x, 0.0f );

			fSampleWeights[i + 0] = 2.0f * GetGaussianDistribution( float(i + 0), 0.0f, 3.0f );
			fSampleWeights[i + 1] = 2.0f * GetGaussianDistribution( float(i + 1), 0.0f, 3.0f );
		}
	}

	else {
		for( int i = 1; i < 15; i += 2 ) {
			vSampleOffsets[i + 0] = D3DXVECTOR2( 0.0f,  i * vViewportTexelSize.y );
			vSampleOffsets[i + 1] = D3DXVECTOR2( 0.0f, -i * vViewportTexelSize.y );

			fSampleWeights[i + 0] = 2.0f * GetGaussianDistribution( 0.0f, float(i + 0), 3.0f );
			fSampleWeights[i + 1] = 2.0f * GetGaussianDistribution( 0.0f, float(i + 1), 3.0f );
		}
	}
}

HRESULT Engine::CDynamicMesh::Initialize()
{

	Engine::CComponent*		pComponent = NULL;

	pComponent = Get_ShaderMgr()->Clone_Shader(L"Shader_Shadow");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	m_pEffect = m_pShader->Get_EffectHandle();

	m_pDevice->CreateVertexBuffer(4 * sizeof(QuadVertex), D3DUSAGE_WRITEONLY, FVF_QUADVERTEX,
		D3DPOOL_DEFAULT, &m_tShadowInfo.m_pQuadVB, NULL );

	QuadVertex* pVertices;
	m_tShadowInfo.m_pQuadVB->Lock( 0, 0, (void**)&pVertices, 0 );
	pVertices[0].p = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
	pVertices[1].p = D3DXVECTOR4( 0.0f, SCREEN_HEIGHT / 2, 0.0f, 1.0f );
	pVertices[2].p = D3DXVECTOR4( SCREEN_WIDTH / 2, 0.0f, 0.0f, 1.0f );
	pVertices[3].p = D3DXVECTOR4( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, 1.0f );

	pVertices[0].t = D3DXVECTOR2( 0.0f, 0.0f );
	pVertices[1].t = D3DXVECTOR2( 0.0f, 1.0f );
	pVertices[2].t = D3DXVECTOR2( 1.0f, 0.0f );
	pVertices[3].t = D3DXVECTOR2( 1.0f, 1.0f );
	m_tShadowInfo.m_pQuadVB->Unlock();

	m_pDevice->CreateTexture( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F, D3DPOOL_DEFAULT, &m_tShadowInfo.m_pShadowMap, NULL );
	m_tShadowInfo.m_pShadowMap->GetSurfaceLevel( 0, &m_tShadowInfo.m_pShadowSurf );

	m_pDevice->CreateTexture( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_tShadowInfo.m_pScreenMap, NULL );
	m_tShadowInfo.m_pScreenMap->GetSurfaceLevel( 0, &m_tShadowInfo.m_pScreenSurf );

	for( int i = 0; i < 2; i++ )
	{
		 m_pDevice->CreateTexture( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_tShadowInfo.m_pBlurMap[i], NULL );

		m_tShadowInfo.m_pBlurMap[i]->GetSurfaceLevel( 0, & m_tShadowInfo.m_pBlurSurf[i] );
	}

	// Create the shadow depth surface
	m_pDevice->CreateDepthStencilSurface( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16,
		D3DMULTISAMPLE_NONE, 0, TRUE, &m_tShadowInfo.m_pShadowDepth, NULL );

	// Create the general depth surface
	m_pDevice->CreateDepthStencilSurface( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, D3DFMT_D16,
		D3DMULTISAMPLE_NONE, 0, TRUE, &m_tShadowInfo.m_pNewDepthRT, NULL );
	
	m_tShadowInfo.m_vEyePos = D3DXVECTOR3( -20.0f, 20.0f, -20.0f );
	m_tShadowInfo.m_vEyeAim = D3DXVECTOR3(  20.0f,  0.0f,  20.0f );
	m_tShadowInfo.m_vUp =	  D3DXVECTOR3(   0.0f,  1.0f,   0.0f );

	return S_OK;
}

HRESULT Engine::CDynamicMesh::Progress(const D3DXMATRIX* world)
{
	m_pEffect = m_pShader->Get_EffectHandle();

	// Computee the world matrix
	D3DXMATRIX matWorld = *world;

	// Compute the view matrix
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &m_tShadowInfo.m_vEyePos, &m_tShadowInfo.m_vEyeAim, &m_tShadowInfo.m_vUp );

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian(60.0f), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 1024.0f );

	D3DXMATRIX matWorldViewProj = matWorld * matView * matProj;

	D3DXMATRIX matWorldIT;
	D3DXMatrixInverse( &matWorldIT, NULL, &matWorld );
	D3DXMatrixTranspose( &matWorldIT, &matWorldIT );


	D3DXVECTOR3 vLightPos = D3DXVECTOR3(  40.0f, 60.0f, -40.0f );
	D3DXVECTOR3 vLightAim = D3DXVECTOR3(   0.0f,  0.0f,   0.0f );

	D3DXMatrixLookAtLH( &matView, &vLightPos, &vLightAim, &m_tShadowInfo.m_vUp );

	D3DXMatrixOrthoLH( &matProj, 45.0f, 45.0f, 1.0f, 1024.0f );

	D3DXMATRIX matLightViewProj = matWorld * matView * matProj;

	float fTexOffs = 0.5 + (0.5 / (float)SHADOW_MAP_SIZE);
	D3DXMATRIX matTexAdj( 0.5f,		0.0f,	0.0f,	0.0f,
		0.0f,    -0.5f,	0.0f,	0.0f,
		0.0f,		0.0f,	1.0f,	0.0f,
		fTexOffs, fTexOffs,  0.0f, 1.0f );

	D3DXMATRIX matTexture = matLightViewProj * matTexAdj;

	m_pEffect->SetMatrix( "g_matWorldViewProj", &matWorldViewProj );
	m_pEffect->SetMatrix( "g_matLightViewProj", &matLightViewProj );
	m_pEffect->SetMatrix( "g_matWorld", &matWorld );
	m_pEffect->SetMatrix( "g_matWorldIT", &matWorldIT );
	m_pEffect->SetMatrix( "g_matTexture", &matTexture );

	m_pEffect->SetVector( "g_vLightPos", (D3DXVECTOR4*)&vLightPos );
	m_pEffect->SetVector( "g_vEyePos", (D3DXVECTOR4*)&m_tShadowInfo.m_vEyePos );
	m_pEffect->SetVector( "g_vLightColor", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 0.5f ) );

	m_pEffect->SetBool("g_bFiltered", m_tShadowInfo.m_bFiltered);

	return S_OK;

}

HRESULT Engine::CDynamicMesh::ScreenGrab()
{
	return S_OK;

}

void Engine::CDynamicMesh::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

LPD3DXFRAME* Engine::CDynamicMesh::GetRootBone()
{
	return &m_pRootBone;
}

