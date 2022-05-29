/*!
 * \file Layer.h
 * \date 2015/04/03 16:06
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
#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;
class ENGINE_DLL CLayer
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLayer(void);

public:
	const CComponent* GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey);

public:
	HRESULT AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject);

public:
	void Update(void);
	void Render(void);

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	typedef list<CGameObject*>				OBJECTLIST;
	typedef unordered_map<const TCHAR*, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST		m_mapObjlist;

public:
	list<CGameObject*>*	GetObjList(const TCHAR* pObjKey);
	unordered_map<const TCHAR*, list<Engine::CGameObject*>>* GetMap();
	CGameObject* GetObj(const TCHAR* pObjKey);
};

END

#endif // Layer_h__