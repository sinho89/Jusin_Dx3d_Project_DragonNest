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


#ifndef House_h__
#define House_h__


#include "GameObject.h"
#include "Tool_Include.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CStaticObj
	: public Engine::CGameObject
{
private:
	CStaticObj(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStaticObj(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CStaticObj* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

public:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

private:
	int					m_iCollision;
	float				m_fDetail;
};

#endif // House_h__