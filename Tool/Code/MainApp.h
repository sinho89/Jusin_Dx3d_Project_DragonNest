/*!
 * \file MainApp.h
 * \date 2016/09/28 23:34
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



#ifndef MainApp_h__
#define MainApp_h__

#include "Tool_Include.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CResourceMgr;
	class CTimeMgr;
	class CScene;
}

class CStaticObj;
class CMainApp
{
private:
	CMainApp(void);

public:
	~CMainApp(void);

public:
	HRESULT InitApp(void);
	void Update(void);
	void Render(void);

public:
	static CMainApp* Create(void);

public:
	void Release(void);

private:
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::CManagement*	m_pManagement;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
public:
	Engine::CScene*							m_pScene;
	bool									m_bCreate;
	TCHAR*									m_szName;

};

#endif // MainApp_h__