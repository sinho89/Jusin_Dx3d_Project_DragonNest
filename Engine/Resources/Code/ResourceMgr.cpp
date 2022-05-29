#include "ResourceMgr.h"
#include "Resources.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "CubeTex.h"
#include "TrailTex.h"

IMPLEMENT_SINGLETON(Engine::CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
: m_pmapResource(NULL)
, m_wReservedSize(0)
{

}

Engine::CResourceMgr::~CResourceMgr(void)
{
	Release();
}

Engine::CResources* Engine::CResourceMgr::FindResource(const WORD& wContainerIdx
													   , const TCHAR* pResourceKey)
{
	MAPRESOURCE::iterator	iter = find_if(m_pmapResource[wContainerIdx].begin()
		, m_pmapResource[wContainerIdx].end(), CTagFinder(pResourceKey));

	if(iter == m_pmapResource[wContainerIdx].end())
		return NULL;

	return iter->second;
}

Engine::CResources* Engine::CResourceMgr::CloneResource(const WORD& wContainerIdx
														, const TCHAR* pResourceKey)
{
	CResources*		pResource = FindResource(wContainerIdx, pResourceKey);
	NULL_CHECK_RETURN(pResource, NULL);

	return pResource->CloneResource();
}

HRESULT Engine::CResourceMgr::ReserveContainerSize(const WORD& wSize)
{
	if(m_wReservedSize != 0	|| m_pmapResource != NULL)
		return E_FAIL;

	m_pmapResource = new MAPRESOURCE[wSize];

	m_wReservedSize = wSize;
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice
										, const WORD& wContainerIdx 
										, CVIBuffer::BUFFERTYPE eBufferType
										, const TCHAR* pResourceKey 
										, const WORD& wCntX /*= 0*/
										, const WORD& wCntZ /*= 0*/
										, const WORD& wItv /*= 1*/
										)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = FindResource(wContainerIdx, pResourceKey);
	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"버퍼가 이미 추가 되어 있음");
		return E_FAIL;
	}

	CVIBuffer*		pBuffer = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_RCTEX:
		pBuffer = CRcTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TERRAIN:
		pBuffer = CTerrainTex::Create(pDevice, wCntX, wCntZ, wItv);
		break;

	case CVIBuffer::BUFFER_CUBETEX:
		pBuffer = CCubeTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TRAIL:
		pBuffer = CTrailTex::Create(pDevice, wCntX);
		break;
	}

	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pResourceKey, pBuffer));
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice
										 , const WORD& wContainerIdx 
										 , TEXTURETYPE eTextureType
										 , const TCHAR* pResourceKey
										 , const TCHAR* pFilePath
										 , const WORD& wCnt /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = FindResource(wContainerIdx, pResourceKey);
	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"텍스쳐가 이미 추가 되어 있음");
		return E_FAIL;
	}

	pResource = CTexture::Create(pDevice, eTextureType, pFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, L"리소스 할당 실패");

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pResourceKey, pResource));
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddMesh(LPDIRECT3DDEVICE9 pDevice
									  , UINT iContainerIdx 
									  , MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName
									  , OBJECTTYPE _eType)
{
	CResources*		pResource = FindResource(iContainerIdx, pMeshKey);
	NULL_CHECK_RETURN(!pResource, E_FAIL);

	switch(eMeshType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pDevice, pFilePath, pFileName, _eType);
		break;

	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pDevice, pFilePath, pFileName, _eType);
		break;
	}
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_pmapResource[iContainerIdx].insert(MAPRESOURCE::value_type(pMeshKey, pResource));
	return S_OK;
}

void Engine::CResourceMgr::ResourceReset(const WORD& wContainerIdx)
{
	for_each(m_pmapResource[wContainerIdx].begin(), m_pmapResource[wContainerIdx].end()
		, CDeleteMap());
	m_pmapResource[wContainerIdx].clear();
}

void Engine::CResourceMgr::Release(void)
{
	if(0 == m_wReservedSize)
		return;

	for (size_t i = 0; i < m_wReservedSize; ++i)
	{
		MAPRESOURCE::iterator	iter = m_pmapResource[i].begin();
		MAPRESOURCE::iterator	iter_end = m_pmapResource[i].end();

		DWORD		dwRefCnt = 0;
		for( ; iter != iter_end; ++iter)
		{
			dwRefCnt = iter->second->Release();
			if(dwRefCnt == 0)
				Safe_Delete(iter->second);
		}
		m_pmapResource[i].clear();
	}	
	Engine::Safe_Delete_Array(m_pmapResource);
}