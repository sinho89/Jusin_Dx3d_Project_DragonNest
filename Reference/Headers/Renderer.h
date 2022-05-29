/*!
 * \file Renderer.h
 * \date 2015/04/02 23:05
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 씬의 렌더링을 수행하는 클래스
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CScene;
class CGameObject;

class ENGINE_DLL CRenderer
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CRenderer(void);

public:
	void SetCurrentScene(CScene* pScene);
	void AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject);

public:
	HRESULT InitScene(void);
	void Render(const float& fTime);

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void Render_Priority(void);
	void Render_NoneAlpha(void);
	void Render_Alpha(void);
	void Render_UI(void);
	void Render_FPS(const float& fTime);
	void Release(void);

private:
	CScene*			m_pScene;
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	float		m_fTime;
	TCHAR		m_szFps[128];
	DWORD		m_dwCount;

private:
	typedef list<CGameObject*>		RENDERLIST;
	RENDERLIST			m_RenderGroup[TYPE_END];
};

END

#endif // Renderer_h__