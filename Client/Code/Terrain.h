/*!
 * \file Terrain.h
 * \date 2016/09/29 1:56
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

#ifndef Terrain_h__
#define Terrain_h__


#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CTerrain
	: public Engine::CGameObject
{
private:
	CTerrain(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTerrain(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

private:
	int					m_iCollision;
	D3DMATERIAL9		m_MtrInfo;
	float				m_fDetail;
};

#endif // Terrain_h__