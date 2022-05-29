/*!
 * \file Logo.h
 * \date 2015/04/03 0:04
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief ·Î°í
 *
 * TODO: long description
 *
 * \note
*/

#include "Scene.h"

#ifndef ToolScene_h__
#define ToolScene_h__

namespace Engine
{
	class CResourceMgr;
	class CMesh;
}
class CToolScene
	: public Engine::CScene
{
public:
	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI};

private:
	CToolScene(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CToolScene(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CToolScene* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Light(void);
	HRESULT Add_NaviMesh(void);
	void Release(void);
	void SetUp(void);


private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CMesh*			m_pMesh;

};


#endif // ToolScene_h__