/*!
 * \file Management.h
 * \date 2015/04/02 22:59
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
#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CComponent;
class CGameObject;
class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	CManagement(void);
	~CManagement(void);

public:
	const CComponent* GetComponent(WORD eLayerID,const TCHAR* pObjKey, const TCHAR* pComponentKey);
	list<CGameObject*>* GetObjList(WORD eLayerID,const TCHAR* pObjKey);
	unordered_map<const TCHAR*, list<Engine::CGameObject*>>* GetMap(WORD eLayerID);
	CGameObject* GetObject(WORD eLayerID, const TCHAR* pObjKey);
	void AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject);


public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void Update(void);
	void Render(const float& fTime);

public:
	template <typename T>
	HRESULT SceneChange(T& Functor);

private:
	void Release(void);

private:
	CScene*		m_pScene;
	CRenderer*	m_pRenderer;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
		Engine::Safe_Delete(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pDevice), E_FAIL);

	m_pRenderer->SetCurrentScene(m_pScene);
	return S_OK;
}

END

#endif // Management_h__