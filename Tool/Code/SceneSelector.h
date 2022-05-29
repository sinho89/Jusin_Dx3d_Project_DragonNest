/*!
 * \file SceneSelector.h
 * \date 2016/09/28 19:42
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
#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "ToolScene.h"

class CSceneSelector
{
public:
	enum SCENE {SCENE_TOOL, SCENE_END};
public:
	explicit CSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CSceneSelector(void) {}
public:
	HRESULT operator () (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice) 
	{
		switch(m_eSceneID)
		{
		case SCENE_TOOL:
			*ppScene = CToolScene::Create(pDevice);
			break;
		}

		NULL_CHECK_RETURN(*ppScene, E_FAIL);

		return S_OK;
	}
private:
	SCENE			m_eSceneID;
};


#endif // SceneSelector_h__


