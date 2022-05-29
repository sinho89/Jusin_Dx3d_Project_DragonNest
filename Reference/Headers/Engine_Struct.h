/*!
 * \file Engine_Struct.h
 * \date 2015/04/04 23:03
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
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	struct ShadowInfo
	{
		LPDIRECT3DVERTEXBUFFER9	m_pQuadVB;

		LPDIRECT3DTEXTURE9	m_pSpotMap;	

		LPDIRECT3DTEXTURE9	m_pShadowMap;	
		LPDIRECT3DSURFACE9	m_pShadowSurf;	
		LPDIRECT3DSURFACE9	m_pShadowDepth;	

		LPDIRECT3DTEXTURE9	m_pScreenMap;	
		LPDIRECT3DSURFACE9	m_pScreenSurf;	

		LPDIRECT3DTEXTURE9	m_pBlurMap[2];	
		LPDIRECT3DSURFACE9	m_pBlurSurf[2];	

		LPDIRECT3DSURFACE9	m_pNewDepthRT;	
		LPDIRECT3DSURFACE9	m_pOldColorRT;	
		LPDIRECT3DSURFACE9	m_pOldDepthRT;	

		D3DXVECTOR2			m_vSampleOffsets[15];
		FLOAT				m_fSampleWeights[15];

		LPDIRECT3DSURFACE9	m_pScreenshot;		
		UINT				m_uNumScreenshots;	

		UINT				m_uFrameCount;		
		FLOAT				m_fStartTime;		
		FLOAT				m_fFramerate;		
		FLOAT				m_fStopTime;			
		BOOL				m_bDisplayStats;	

		D3DXVECTOR3			m_vEyePos;			
		D3DXVECTOR3			m_vEyeAim;			
		D3DXVECTOR3			m_vUp;				

		BOOL				m_bSoftShadows;
		BOOL				m_bFiltered;
	};
	struct Bone : public D3DXFRAME
	{
		D3DXMATRIX CombinedTransformationMatrix;
	};

	struct BoneMesh : public D3DXMESHCONTAINER
	{
		wstring						wstrTextureName;
		LPDIRECT3DTEXTURE9*			ppTexture;
		ID3DXMesh* OriginalMesh;

		DWORD NumAttributeGroups;
		DWORD NumInfl;
		DWORD NumPaletteEntries;

		D3DXATTRIBUTERANGE* attributeTable;
		D3DXMATRIX** boneMatrixPtrs;
		D3DXMATRIX* boneOffsetMatrices;
		D3DXMATRIX* currentBoneMatrices;

		LPD3DXBUFFER	pBoneCombinationBuf;
	};

	struct QuadVertex
	{
		D3DXVECTOR4 p;
		D3DXVECTOR2 t;
	};
	#define FVF_QUADVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)
	
	typedef struct tagVertexTexture
	{
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vNormal;
		D3DXVECTOR2				vTexUV;
	}VTXTEX;
	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexColor
	{
		tagVertexColor();
		tagVertexColor(D3DXVECTOR3 pos, D3DCOLOR col) { vPos = pos; dwColor = col; }
		D3DXVECTOR3			vPos;
		D3DCOLOR			dwColor;
	}VTXCOL;
	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vTex;
	}VTXCUBE;
	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEXCOORDSIZE3(0) | D3DFVF_TEX1;

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;
}

#endif // Engine_Struct_h__