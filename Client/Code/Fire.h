/*!
 * \file Fire.h
 * \date 2016/10/28 11:33
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

#ifndef Fire_h__
#define Fire_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CShader;
	class CTransform;
	class CStaticMesh;
};

class CFire
	: public Engine::CGameObject
{
private:
	CFire(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFire(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	void*	GetVertexDeclaration();

	void	SetAni(BOOL bAni=TRUE);		// �ִϸ��̼� Ȱ��ȭ/ ��Ȱ��ȭ
	void	SetAvgTime(FLOAT fTime);	// ��� �ð� ����

public:
	void	SetPart(int nIdx);

private:
	virtual HRESULT AddComponent(void);

public:
	static CFire* Create(LPDIRECT3DDEVICE9 pDevice);

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
	FLOAT				m_fTimeAvg;	// Frame ��� �ð�

	INT					m_PrtN;		// ������ ��
	Tpart*				m_PrtD;		// Particle Data

	// ȭ�� ��¿�
	INT					m_iVtx;		// Vertex Number
	VtxDUV1*			m_pVtx;		// Vertex

	LPDIRECT3DTEXTURE9	m_pTx0;		// Water
	LPDIRECT3DTEXTURE9	m_pTx1;		// Noise
	BOOL				m_bAni;

	void*				m_pFVF;
};


#endif // Fire_h__