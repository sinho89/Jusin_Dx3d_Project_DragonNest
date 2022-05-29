#include "HierarchyLoader.h"

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath)
: m_pDevice(pDevice)
, m_pPath(pPath)
{

}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	Bone* newBone = new Bone;
	memset(newBone, 0, sizeof(Bone));

	if(Name != NULL)
	{
		newBone->Name = new char[strlen(Name) + 1];
		strcpy(newBone->Name, Name);
	}

	D3DXMatrixIdentity(&newBone->TransformationMatrix);
	D3DXMatrixIdentity(&newBone->CombinedTransformationMatrix);

	*ppNewFrame = (D3DXFRAME*)newBone;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name
														   , CONST D3DXMESHDATA *pMeshData
														   , CONST D3DXMATERIAL *pMaterials
														   , CONST D3DXEFFECTINSTANCE *pEffectInstances
														   , DWORD NumMaterials
														   , CONST DWORD *pAdjacency
														   , LPD3DXSKININFO pSkinInfo
														   , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	LPD3DXMESH		pMesh = pMeshData->pMesh;

	if(pMesh->GetFVF() == 0)
		return E_FAIL;

	HRESULT		hr = NULL;
	BoneMesh*		boneMesh = new BoneMesh;
	ZeroMemory(boneMesh, sizeof(BoneMesh));

	if(Name != NULL)
		AllocateName(Name, &boneMesh->Name);

	//인접정보
	DWORD		dwNumFaces = pMesh->GetNumFaces();
	boneMesh->pAdjacency = new DWORD[dwNumFaces * 3];
	memcpy(boneMesh->pAdjacency, pAdjacency, sizeof(DWORD) * dwNumFaces * 3);

	boneMesh->MeshData.Type = D3DXMESHTYPE_MESH;

	//노멀벡터
	if(!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions()
			, pMesh->GetFVF() | D3DFVF_NORMAL
			, m_pDevice
			, &boneMesh->MeshData.pMesh);
		FAILED_CHECK(hr);

		D3DXComputeNormals(boneMesh->MeshData.pMesh, boneMesh->pAdjacency);
	}
	else
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), m_pDevice
			, &boneMesh->MeshData.pMesh);
		FAILED_CHECK(hr);

		pMesh->AddRef();
	}


	//재질정보
	boneMesh->NumMaterials = max(NumMaterials, 1);

	boneMesh->pMaterials = new D3DXMATERIAL[boneMesh->NumMaterials];
	boneMesh->ppTexture = new LPDIRECT3DTEXTURE9[boneMesh->NumMaterials];
	ZeroMemory(boneMesh->pMaterials, sizeof(D3DXMATERIAL) * boneMesh->NumMaterials);
	ZeroMemory(boneMesh->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * boneMesh->NumMaterials);

	if(NumMaterials > 0)
	{
		memcpy(boneMesh->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * boneMesh->NumMaterials);

		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			if(boneMesh->pMaterials[i].pTextureFilename == NULL)
				continue;

			TCHAR		szFullPath[MAX_PATH] = L"";
			TCHAR		szFileName[128] = L"";

			lstrcpy(szFullPath, m_pPath);
			MultiByteToWideChar(CP_ACP
				, 0, boneMesh->pMaterials[i].pTextureFilename
				, strlen(boneMesh->pMaterials[i].pTextureFilename)
				, szFileName, 128);
			lstrcat(szFullPath, szFileName);

			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &boneMesh->ppTexture[i]);
			m_pColorMap.push_back(boneMesh->ppTexture[i]);

			boneMesh->wstrTextureName = szFileName;
			FAILED_CHECK(hr);
		}
	}
	else
	{
		ZeroMemory(&boneMesh->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
		boneMesh->pMaterials[0].MatD3D.Diffuse.r = 1.f;
		boneMesh->pMaterials[0].MatD3D.Diffuse.g = 1.f;
		boneMesh->pMaterials[0].MatD3D.Diffuse.b = 1.f;
		boneMesh->pMaterials[0].MatD3D.Diffuse.a = 1.f;
		boneMesh->pMaterials[0].pTextureFilename = NULL;
	}

	if(pSkinInfo != NULL)
	{
		boneMesh->pSkinInfo = pSkinInfo;

		pSkinInfo->AddRef();

		DWORD maxVertInfluences = 0;
		DWORD numBoneComboEntries = 0;
		boneMesh->NumPaletteEntries = boneMesh->pSkinInfo->GetNumBones();

		pSkinInfo->ConvertToIndexedBlendedMesh(pMeshData->pMesh,
			D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
			boneMesh->NumPaletteEntries,
			boneMesh->pAdjacency,
			NULL,
			NULL,
			NULL,
			&boneMesh->NumInfl,
			&boneMesh->NumAttributeGroups,
			&boneMesh->pBoneCombinationBuf,
			&boneMesh->MeshData.pMesh);

		boneMesh->MeshData.pMesh->GetAttributeTable(NULL, &boneMesh->NumAttributeGroups);
		boneMesh->attributeTable = new D3DXATTRIBUTERANGE[boneMesh->NumAttributeGroups];
		boneMesh->MeshData.pMesh->GetAttributeTable(boneMesh->attributeTable, NULL);

		int NumBones = pSkinInfo->GetNumBones();
		boneMesh->boneOffsetMatrices = new D3DXMATRIX[NumBones];
		boneMesh->currentBoneMatrices = new D3DXMATRIX[NumBones];

		for(int i = 0; i < NumBones; i++)
			boneMesh->boneOffsetMatrices[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));
	}

	*ppNewMeshContainer = boneMesh;

	return S_OK;

}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if(pFrameToFree == NULL)
		return E_FAIL;

	if(pFrameToFree->pMeshContainer != NULL)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if(pFrameToFree->pFrameFirstChild != NULL)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if(pFrameToFree->pFrameSibling != NULL)
		DestroyFrame(pFrameToFree->pFrameSibling);

	Safe_Delete_Array(pFrameToFree->Name);
	Safe_Delete(pFrameToFree);
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	BoneMesh*		pMeshContainer = static_cast<BoneMesh*>(pMeshContainerToFree);

	if(pMeshContainer->pBoneCombinationBuf)
		pMeshContainer->pBoneCombinationBuf->Release();

	if(pMeshContainerToFree != NULL)
		delete pMeshContainerToFree;

	return S_OK;
}

void Engine::CHierarchyLoader::AllocateName(const CHAR* const pName, CHAR** ppNewName)
{
	int			iLength = strlen(pName);

	*ppNewName = new CHAR[iLength + 1];

	strcpy_s(*ppNewName, iLength + 1, pName);
}

vector<LPDIRECT3DTEXTURE9>* Engine::CHierarchyLoader::GetColorMap()
{
	return &m_pColorMap;
}
