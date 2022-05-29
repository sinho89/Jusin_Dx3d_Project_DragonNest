/*!
 * \file SkyBox.h
 * \date 2016/09/23 14:29
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

#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CSkyBox
	: public Engine::CGameObject
{
private:
	CSkyBox(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkyBox(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	HRESULT SetConstantTable(void);
private:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

private:
	float				m_fDetail;
};

#endif // SkyBox_h__