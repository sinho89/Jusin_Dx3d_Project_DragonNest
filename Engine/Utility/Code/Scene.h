/*!
 * \file Scene.h
 * \date 2015/04/02 23:10
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
#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;
class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CScene(void);

public:
	const CComponent* GetComponent(WORD eLayerID,const TCHAR* pObjKey, const TCHAR* pComponentKey);

public:
	virtual HRESULT InitScene(void) PURE;
	virtual void Update(void);
	virtual void Render(void);

public:
	list<CGameObject*>*	GetObjList(WORD eLayerID, const TCHAR* pObjKey);
	unordered_map<const TCHAR*, list<Engine::CGameObject*>>* GetMap(WORD eLayerID);
	CGameObject* GetObject(WORD eLayerID, const TCHAR* pObjKey);
private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef unordered_map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;
};

END

#endif // Scene_h__