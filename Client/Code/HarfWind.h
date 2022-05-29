/*!
 * \file HarfWind.h
 * \date 2016/10/30 19:46
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

#ifndef HarfWind_h__
#define HarfWind_h__


#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CHarfWind
	: public Engine::CGameObject
{
private:
	CHarfWind(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHarfWind(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	HRESULT SetConstantTable(void);

public:
	static CHarfWind* Create(LPDIRECT3DDEVICE9 pDevice);
	static CHarfWind* Create(LPDIRECT3DDEVICE9 pDevice
		, D3DXVECTOR3 _vPos
		, float	_fAngle);

public:
	void SetInfo(D3DXVECTOR3 _vPos, float _fAngle);
	void SetType(OBTAINTYPE	_eType);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

public:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

private:
	int					m_iCollision;
	float				m_fDetail;
	OBTAINTYPE			m_eType;
	const D3DXMATRIX*		m_pmatPlayerInfo;
	float				m_fScale;
	float				m_fAlpha;
};

#endif // HarfWind_h__