#include "Frustum.h"

IMPLEMENT_SINGLETON(Engine::CFrustum)
Engine::CFrustum::CFrustum(void)
: m_pDevice(NULL)
{
	
}

Engine::CFrustum::~CFrustum(void)
{

}

HRESULT Engine::CFrustum::InitFrustum(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	// 앞면 

	m_vPoint[0] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_vPoint[1] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_vPoint[2] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_vPoint[3] = D3DXVECTOR3(-1.f, -1.f, 0.f);

	// 뒷면
	m_vPoint[4] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	m_vPoint[5] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vPoint[6] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_vPoint[7] = D3DXVECTOR3(-1.f, -1.f, 1.f);

	D3DXMATRIX		matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
	}

	return S_OK;
}

void Engine::CFrustum::Transform_Frustum_ToWorld(void)
{
	memcpy(m_vConPoint, m_vPoint, sizeof(D3DXVECTOR3) * 8);

	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vConPoint[i], &m_vConPoint[i], &matView);
	}
}

void Engine::CFrustum::Transform_Frustum_Local(const D3DXMATRIX* pWorld)
{
	ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);

	D3DXMATRIX		matWorldInv;
	D3DXMatrixInverse(&matWorldInv, NULL, pWorld);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vConPoint[i], &m_vConPoint[i], &matWorldInv);
	}

	MakePlane();
}

void Engine::CFrustum::MakePlane(void)
{
	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vConPoint[1], &m_vConPoint[5], &m_vConPoint[6]);

	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vConPoint[4], &m_vConPoint[0], &m_vConPoint[3]);

	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vConPoint[4], &m_vConPoint[5], &m_vConPoint[1]);

	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vConPoint[3], &m_vConPoint[2], &m_vConPoint[6]);

	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vConPoint[7], &m_vConPoint[6], &m_vConPoint[5]);

	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vConPoint[0], &m_vConPoint[1], &m_vConPoint[2]);
}

bool Engine::CFrustum::VertexInFrustum(const D3DXVECTOR3* pPoint)
{
	for(int i = 0; i < 6; ++i)
	{
		if(D3DXPlaneDotCoord(&m_Plane[i], pPoint) > 0)
			return false;
	}

	return true;
}

void Engine::CFrustum::CullingForTerrain(INDEX32* pIndex, DWORD* pTriCnt, 
										 const VTXTEX* pTerrainVtx, 
										 const WORD& wCntX, const WORD& wCntZ)
{
	*pTriCnt = 0;

	WORD	wIndex = 0;

	bool	bIsIn[4] = { false };

	for(WORD z = 0; z < wCntZ - 1; ++z)
	{
		for(WORD x = 0; x < wCntX - 1; ++x)
		{
			wIndex = z * wCntX + x;

			bIsIn[0] = VertexInFrustum(&pTerrainVtx[wIndex + wCntX].vPos);
			bIsIn[1] = VertexInFrustum(&pTerrainVtx[wIndex + wCntX + 1].vPos);
			bIsIn[2] = VertexInFrustum(&pTerrainVtx[wIndex + 1].vPos);
			bIsIn[3] = VertexInFrustum(&pTerrainVtx[wIndex].vPos);

			// 오른쪽 위 폴리곤에 대한 인덱스 재배치
			if(bIsIn[0] || bIsIn[1] || bIsIn[2])
			{
				pIndex[*pTriCnt]._1 = wIndex + wCntX;
				pIndex[*pTriCnt]._2 = wIndex + wCntX + 1;
				pIndex[*pTriCnt]._3 = wIndex + 1;

				++(*pTriCnt);
			}
			// 왼쪽 아래 폴리곤에 대한 인덱스 재배치
			if(bIsIn[0] || bIsIn[2] || bIsIn[3])
			{
				pIndex[*pTriCnt]._1 = wIndex + wCntX;
				pIndex[*pTriCnt]._2 = wIndex + 1;
				pIndex[*pTriCnt]._3 = wIndex;

				++(*pTriCnt);
			}


		}
	}
}

bool Engine::CFrustum::SphereInFrustum(const D3DXVECTOR3* pCenter, float& fRadius)
{
	for(int i = 0; i < 6; ++i)
	{
		if(D3DXPlaneDotCoord(&m_Plane[i], pCenter) > fRadius)
			return false;
	}
	return true;
}	

