/*!
 * \file Petal.h
 * \date 2016/10/28 22:01
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

#ifndef Petal_h__
#define Petal_h__

#include "Particle.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};
class CPetal : public CParticle
{
protected:
	explicit CPetal(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CPetal();

public:
	static CPetal* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual void Update(void);
	virtual void Render(void);

	virtual void	SetAni(BOOL bAni=TRUE);		// �ִϸ��̼� Ȱ��ȭ/ ��Ȱ��ȭ
	virtual void	SetAvgTime(FLOAT fTime);	// ��� �ð� ����
	//virtual int		SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	HRESULT SetConstantTable(void);
private:
	HRESULT AddComponent(void);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);


private:
	float		m_fAlpha;
	float		m_fDetail;
};

#endif // Petal_h__