/*!
 * \file CresentPart.h
 * \date 2016/11/03 1:28
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

#ifndef CresentPart_h__
#define CresentPart_h__


#include "Particle.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CCresentPart : public CParticle
{
protected:
	explicit CCresentPart(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CCresentPart();

public:
	static CCresentPart* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual void Update(void);
	virtual void Render(void);

	virtual void	SetAni(BOOL bAni=TRUE);		// 애니메이션 활성화/ 비활성화
	virtual void	SetAvgTime(FLOAT fTime);	// 평균 시간 설정
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
};

#endif // CresentPart_h__