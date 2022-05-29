/*!
 * \file Water.h
 * \date 2016/10/29 4:36
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Water_h__
#define Water_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CWater
	: public Engine::CGameObject
{
private:
	PDEV		m_pDev;

	PDVD		m_pFVF;
	PDEF		m_pEft;

	int			m_TileN;
	FLOAT		m_TileW;
	int			m_nVtx;
	int			m_nFce;
	VtxIdx*		m_pFce;
	Vtx*		m_pVtx;

	PDTC		m_pTxCbm;
	PDTX		m_pTxNor;
	float		m_fDetail;
	PDRE		m_pRndEnv;

private:
	CWater(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWater(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	void T_SetupCubeViewMatrix(D3DXMATRIX* pOut, DWORD dwFace );

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pDevice);
	static CWater* Create(LPDIRECT3DDEVICE9 pDevice
		, D3DXVECTOR3 _vPos
		, float	_fAngle);

public:
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void SetType(OBTAINTYPE	_eType);
	void SetTexture(PDTC pTex);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

public:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

};


#endif // Water_h__