/*!
 * \file HierarchyLoader.h
 * \date 2016/09/19 13:35
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

#ifndef HierarchyLoader_h__
#define HierarchyLoader_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyLoader
	: public ID3DXAllocateHierarchy
{
public:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath);
	~CHierarchyLoader(void);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, 
		LPD3DXFRAME *ppNewFrame) ;
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer) ;
	 STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) ;
	 STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) ; 
public:
	vector<LPDIRECT3DTEXTURE9>*	GetColorMap();
private:
	void AllocateName(const CHAR* const pName, CHAR** ppNewName);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	const TCHAR*			m_pPath;
	vector<LPDIRECT3DTEXTURE9>	m_pColorMap;
};

END

#endif // HierarchyLoader_h__