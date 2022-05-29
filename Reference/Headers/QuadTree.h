/*!
 * \file QuadTree.h
 * \date 2016/09/07 14:04
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

#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CQuadTree
{
DECLARE_SINGLETON(CQuadTree)

public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	enum NEIGHBOR {NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	CQuadTree(void);
public:
	~CQuadTree(void);

public:
	HRESULT	InitQuadTree(const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);
	HRESULT InitNeighbor(const VTXTEX* pTerrainVtx, CQuadTree* pParentQuad
		, const WORD& wCntX, const WORD& wCntZ);
	void CullingToQuadTree(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);
	void Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);
	void IsinFrustum(const VTXTEX* pTerrainVtx);

private:
	void	SetChild(const VTXTEX* pTerrainVtx);
	CQuadTree* MakeChild(CHILD eChild);
	bool MakeNeighbor(NEIGHBOR eNeighborID
		, int& iLT, int& iRT, int& iLB, int& iRB
		, const WORD& wCntX, const WORD& wCntZ);
	CQuadTree* FindNeighbor(const VTXTEX* pTerrainVtx
		, const int& iLT, const int& iRT, const int& iLB, const int& iRB
		, const int& iCenter);

	void	Release(void);

private:
	bool LevelofDetail(const VTXTEX* pTerrainVtx);

private:
	CQuadTree*			m_pChild[CHILD_END];
	CQuadTree*			m_pNeighbor[NEIGHBOR_END];
	WORD				m_wCorner[CORNER_END];		// �� �� �������� ���ؽ� �ε����� ����
	int					m_iCenter;					// �簢���� ����
	float				m_fRadius;					// �����ϴ� ���� ������
	bool				m_bIsIn;					// ���� ���� Ʈ���� �����ϴ� ���� ����ü�� �浹�ϴ��� �Ǵ�

};

END
#endif // QuadTree_h__