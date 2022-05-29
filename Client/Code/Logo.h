/*!
 * \file Logo.h
 * \date 2015/04/03 0:04
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief �ΰ�
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"

namespace Engine
{
	class CResourceMgr;
	class CManagement;
}

class CLoading;
class CLogo
	: public Engine::CScene
{
public:
	//�� ���� �ʿ��� ���̾ �����Ѵ�.
	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC};

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogo(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT	Add_Light(void);
	void Release(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CManagement*	m_pManagement;

	CLoading*				m_pLoading;
};

#endif // Logo_h__